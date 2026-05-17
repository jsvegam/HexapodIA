#include <Arduino.h>
#include "STS3215.h"
#include "Kinematics.h"

// Serial port for servos (hardware serial 2)
#define SERVO_SERIAL Serial2
#define SERVO_TX_PIN 17
#define SERVO_RX_PIN 16
#define SERVO_DIR_PIN -1  // Use -1 for auto-direction or specify pin

// Servo IDs for single leg (FASE 1)
#define SERVO_COXA  0
#define SERVO_FEMUR 1
#define SERVO_TIBIA 2

// Status LED
#define LED_PIN 2

// Create objects
STS3215 servo(SERVO_SERIAL, SERVO_DIR_PIN);
Kinematics kinematics;

// State variables
bool systemReady = false;
unsigned long lastUpdate = 0;
const unsigned long updateInterval = 20; // 50Hz control loop

// Test sequence state
enum TestMode {
    MODE_IDLE,
    MODE_INDIVIDUAL_TEST,
    MODE_IK_TEST,
    MODE_CIRCLE_TEST
};

TestMode currentMode = MODE_IDLE;
unsigned long testStartTime = 0;

// Function declarations
void setupServos();
void testServoConnection();
void testIndividualMovement();
void testIKMovement();
void testCircularMotion();
void moveToPosition(const Vector3 &target, uint16_t moveTime = 1000);
bool moveToPositionSafe(const Vector3 &target, uint16_t moveTime = 1000);
void printServoStatus();
void printJointAngles(const JointAngles &angles);
void runKinematicsTests();  // From kinematics_tests.cpp

void setup() {
    Serial.begin(115200);
    delay(1000);

    Serial.println("\n=================================");
    Serial.println("HexapodIA - FASE 1: Single Leg");
    Serial.println("=================================\n");

    pinMode(LED_PIN, OUTPUT);
    digitalWrite(LED_PIN, HIGH);

    // Initialize servo bus
    Serial.println("Initializing STS3215 servo bus...");
    if (!servo.begin(SERVO_BAUDRATE)) {
        Serial.println("ERROR: Failed to initialize servo bus");
        while(1) {
            digitalWrite(LED_PIN, !digitalRead(LED_PIN));
            delay(200);
        }
    }

    delay(500);

    // Test servo connections
    Serial.println("\nTesting servo connections...");
    testServoConnection();

    // Setup servos
    Serial.println("\nConfiguring servos...");
    setupServos();

    // Display kinematics info
    float minReach, maxReach;
    kinematics.getWorkspaceBounds(minReach, maxReach);
    Serial.printf("\nKinematics workspace:\n");
    Serial.printf("  Min reach: %.1f mm\n", minReach);
    Serial.printf("  Max reach: %.1f mm\n", maxReach);

    systemReady = true;
    digitalWrite(LED_PIN, LOW);

    // Run kinematics validation tests
    Serial.println("\n🔍 Running automatic validation...\n");
    runKinematicsTests();

    Serial.println("\n=================================");
    Serial.println("System ready!");
    Serial.println("Commands:");
    Serial.println("  '1' - Individual joint test");
    Serial.println("  '2' - IK position test");
    Serial.println("  '3' - Circular motion test");
    Serial.println("  'v' - Run validation tests");
    Serial.println("  's' - Servo status");
    Serial.println("  'h' - Home position");
    Serial.println("  'S' - Safe move (with validation)");
    Serial.println("=================================\n");
}

void loop() {
    unsigned long currentTime = millis();

    // Handle serial commands
    if (Serial.available()) {
        char cmd = Serial.read();

        switch(cmd) {
            case '1':
                currentMode = MODE_INDIVIDUAL_TEST;
                testStartTime = currentTime;
                Serial.println("\n>>> Starting individual joint test");
                break;

            case '2':
                currentMode = MODE_IK_TEST;
                testStartTime = currentTime;
                Serial.println("\n>>> Starting IK position test");
                break;

            case '3':
                currentMode = MODE_CIRCLE_TEST;
                testStartTime = currentTime;
                Serial.println("\n>>> Starting circular motion test");
                break;

            case 's':
                printServoStatus();
                break;

            case 'h':
                Serial.println("\n>>> Moving to home position");
                moveToPosition(Vector3(150, 0, -80), 1500);
                currentMode = MODE_IDLE;
                break;

            case 'v':
                Serial.println("\n>>> Running validation tests");
                runKinematicsTests();
                break;

            case 'S':
                Serial.println("\n>>> Safe move test (with cross-validation)");
                moveToPositionSafe(Vector3(180, 30, -70), 1500);
                currentMode = MODE_IDLE;
                break;

            case ' ':
                currentMode = MODE_IDLE;
                Serial.println("\n>>> Test stopped");
                break;
        }
    }

    // Control loop at fixed rate
    if (currentTime - lastUpdate >= updateInterval) {
        lastUpdate = currentTime;

        switch(currentMode) {
            case MODE_INDIVIDUAL_TEST:
                testIndividualMovement();
                break;

            case MODE_IK_TEST:
                testIKMovement();
                break;

            case MODE_CIRCLE_TEST:
                testCircularMotion();
                break;

            case MODE_IDLE:
            default:
                // Idle - do nothing
                break;
        }
    }

    delay(1);
}

void setupServos() {
    // Enable torque for all servos
    servo.setTorqueEnable(SERVO_COXA, true);
    servo.setTorqueEnable(SERVO_FEMUR, true);
    servo.setTorqueEnable(SERVO_TIBIA, true);

    delay(100);

    // Set angle limits for safety
    servo.setAngleLimits(SERVO_COXA,
        servo.angleToPulse(COXA_MIN_ANGLE, -90, 90),
        servo.angleToPulse(COXA_MAX_ANGLE, -90, 90));

    servo.setAngleLimits(SERVO_FEMUR,
        servo.angleToPulse(FEMUR_MIN_ANGLE, 0, 180),
        servo.angleToPulse(FEMUR_MAX_ANGLE, 0, 180));

    servo.setAngleLimits(SERVO_TIBIA,
        servo.angleToPulse(TIBIA_MIN_ANGLE, 0, 180),
        servo.angleToPulse(TIBIA_MAX_ANGLE, 0, 180));

    delay(100);

    // Move to home position
    Serial.println("Moving to home position...");
    moveToPosition(Vector3(150, 0, -80), 2000);
    delay(2500);

    Serial.println("Servos configured successfully");
}

void testServoConnection() {
    uint8_t servoIds[] = {SERVO_COXA, SERVO_FEMUR, SERVO_TIBIA};
    const char* servoNames[] = {"Coxa", "Femur", "Tibia"};

    for (int i = 0; i < 3; i++) {
        Serial.printf("  Testing %s (ID %d)... ", servoNames[i], servoIds[i]);

        if (servo.ping(servoIds[i])) {
            Serial.println("OK");
            servo.setLED(servoIds[i], true);
            delay(200);
            servo.setLED(servoIds[i], false);
        } else {
            Serial.println("FAILED!");
        }
        delay(100);
    }
}

void moveToPosition(const Vector3 &target, uint16_t moveTime) {
    // Calculate IK
    JointAngles angles = kinematics.inverseKinematics(target);

    if (!angles.valid) {
        Serial.println("ERROR: IK solution invalid");
        return;
    }

    // Convert angles to servo positions
    uint16_t coxaPos = servo.angleToPulse(angles.coxa + 90, 0, 180);  // Offset for center
    uint16_t femurPos = servo.angleToPulse(angles.femur, 0, 180);
    uint16_t tibiaPos = servo.angleToPulse(angles.tibia, 0, 180);

    // Send positions to servos
    servo.setPosition(SERVO_COXA, coxaPos, moveTime);
    servo.setPosition(SERVO_FEMUR, femurPos, moveTime);
    servo.setPosition(SERVO_TIBIA, tibiaPos, moveTime);
}

void testIndividualMovement() {
    unsigned long elapsed = millis() - testStartTime;
    unsigned long cycle = elapsed % 6000; // 6 second cycle

    if (cycle < 2000) {
        // Test coxa
        float angle = map(cycle, 0, 2000, -45, 45);
        servo.setPosition(SERVO_COXA, servo.angleToPulse(angle + 90, 0, 180), 0);
    } else if (cycle < 4000) {
        // Test femur
        float angle = map(cycle - 2000, 0, 2000, 30, 90);
        servo.setPosition(SERVO_FEMUR, servo.angleToPulse(angle, 0, 180), 0);
    } else {
        // Test tibia
        float angle = map(cycle - 4000, 0, 2000, 45, 135);
        servo.setPosition(SERVO_TIBIA, servo.angleToPulse(angle, 0, 180), 0);
    }
}

void testIKMovement() {
    unsigned long elapsed = millis() - testStartTime;
    unsigned long cycle = elapsed % 4000; // 4 second cycle

    Vector3 target;

    if (cycle < 1000) {
        // Forward
        target = Vector3(200, 0, -60);
    } else if (cycle < 2000) {
        // Up
        target = Vector3(150, 0, -40);
    } else if (cycle < 3000) {
        // Side
        target = Vector3(150, 50, -60);
    } else {
        // Home
        target = Vector3(150, 0, -80);
    }

    moveToPosition(target, 800);
}

void testCircularMotion() {
    unsigned long elapsed = millis() - testStartTime;
    float t = (elapsed % 5000) / 5000.0 * 2.0 * PI;

    // Circle in XZ plane
    float radius = 30;
    float centerX = 150;
    float centerZ = -60;

    Vector3 target;
    target.x = centerX + radius * cos(t);
    target.y = 0;
    target.z = centerZ + radius * sin(t);

    moveToPosition(target, 0);
}

void printServoStatus() {
    Serial.println("\n=== Servo Status ===");

    uint8_t servoIds[] = {SERVO_COXA, SERVO_FEMUR, SERVO_TIBIA};
    const char* servoNames[] = {"Coxa ", "Femur", "Tibia"};

    for (int i = 0; i < 3; i++) {
        int16_t pos = servo.readPosition(servoIds[i]);
        uint8_t temp = servo.readTemperature(servoIds[i]);
        uint8_t volt = servo.readVoltage(servoIds[i]);
        int16_t load = servo.readLoad(servoIds[i]);

        Serial.printf("%s (ID %d): Pos=%d  Temp=%d°C  Volt=%.1fV  Load=%d\n",
            servoNames[i], servoIds[i], pos, temp, volt/10.0, load);
    }

    Serial.println("===================\n");
}

void printJointAngles(const JointAngles &angles) {
    Serial.printf("Angles: Coxa=%.1f°  Femur=%.1f°  Tibia=%.1f°  Valid=%s\n",
        angles.coxa, angles.femur, angles.tibia, angles.valid ? "YES" : "NO");
}

// Safe move with cross-validation
bool moveToPositionSafe(const Vector3 &target, uint16_t moveTime) {
    Serial.printf("\n[Safe Move] Target: (%.1f, %.1f, %.1f)\n",
                  target.x, target.y, target.z);

    // Step 1: Calculate IK
    JointAngles angles = kinematics.inverseKinematics(target);

    if (!angles.valid) {
        Serial.println("❌ IK failed: target unreachable");
        return false;
    }

    Serial.printf("  IK angles: coxa=%.1f° femur=%.1f° tibia=%.1f°\n",
                  angles.coxa, angles.femur, angles.tibia);

    // Step 2: Cross-validate with FK
    Vector3 fkResult = kinematics.forwardKinematics(angles);
    float error = (fkResult - target).magnitude();

    Serial.printf("  FK validation: (%.2f, %.2f, %.2f)\n",
                  fkResult.x, fkResult.y, fkResult.z);
    Serial.printf("  Error: %.3f mm\n", error);

    if (error > 5.0) {
        Serial.printf("❌ FK validation failed: error %.3fmm > 5mm tolerance\n", error);
        Serial.println("   ABORTING MOVE - Mathematical error detected!");
        return false;
    }

    // Step 3: Move servos
    uint16_t coxaPos = servo.angleToPulse(angles.coxa + 90, 0, 180);
    uint16_t femurPos = servo.angleToPulse(angles.femur, 0, 180);
    uint16_t tibiaPos = servo.angleToPulse(angles.tibia, 0, 180);

    servo.setPosition(SERVO_COXA, coxaPos, moveTime);
    servo.setPosition(SERVO_FEMUR, femurPos, moveTime);
    servo.setPosition(SERVO_TIBIA, tibiaPos, moveTime);

    // Step 4: Wait and verify servo positions
    delay(moveTime + 200);

    int16_t realCoxaPos = servo.readPosition(SERVO_COXA);
    int16_t realFemurPos = servo.readPosition(SERVO_FEMUR);
    int16_t realTibiaPos = servo.readPosition(SERVO_TIBIA);

    Serial.printf("  Servo positions: Coxa=%d Femur=%d Tibia=%d\n",
                  realCoxaPos, realFemurPos, realTibiaPos);

    // Check for significant mismatch
    bool positionOK =
        (abs(realCoxaPos - coxaPos) < 50) &&
        (abs(realFemurPos - femurPos) < 50) &&
        (abs(realTibiaPos - tibiaPos) < 50);

    if (!positionOK) {
        Serial.println("⚠️  Warning: Servo position mismatch detected");
        Serial.println("   Check for mechanical obstructions or calibration issues");
    }

    Serial.printf("✅ Move completed successfully (error: %.2fmm)\n", error);
    return true;
}
