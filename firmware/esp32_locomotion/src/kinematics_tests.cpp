#include <Arduino.h>
#include "Kinematics.h"

// Kinematics test suite
// Run this BEFORE every session with hardware

extern Kinematics kinematics;

struct TestCase {
    Vector3 target;
    const char* name;
    float maxError;  // mm tolerance
};

void runKinematicsTests() {
    Serial.println("\n╔════════════════════════════════════════════════════════╗");
    Serial.println("║     KINEMATICS VALIDATION TEST SUITE                  ║");
    Serial.println("╚════════════════════════════════════════════════════════╝\n");

    int passed = 0, failed = 0;

    // ========================================
    // Test 1: Known Positions
    // ========================================
    Serial.println("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━");
    Serial.println("TEST 1: Known Position Validation");
    Serial.println("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");

    TestCase tests[] = {
        { Vector3(150, 0, -80),   "Home position", 2.0 },
        { Vector3(200, 0, -60),   "Forward extended", 2.0 },
        { Vector3(100, 0, -100),  "Retracted down", 2.0 },
        { Vector3(150, 50, -80),  "Side right", 2.0 },
        { Vector3(150, -50, -80), "Side left", 2.0 },
        { Vector3(130, 0, -40),   "Lifted up", 2.0 },
        { Vector3(180, 30, -70),  "Diagonal FR", 2.0 },
        { Vector3(120, -20, -90), "Diagonal BL", 2.0 }
    };

    for (int i = 0; i < 8; i++) {
        Serial.printf("[Test %d] %s\n", i+1, tests[i].name);
        Serial.printf("  Target: (%.1f, %.1f, %.1f)\n",
                      tests[i].target.x, tests[i].target.y, tests[i].target.z);

        // IK
        JointAngles angles = kinematics.inverseKinematics(tests[i].target);

        if (!angles.valid) {
            Serial.println("  ❌ FAIL: IK returned invalid (target unreachable)");
            failed++;
            continue;
        }

        Serial.printf("  IK angles: coxa=%.2f° femur=%.2f° tibia=%.2f°\n",
                      angles.coxa, angles.femur, angles.tibia);

        // FK cross-validation
        Vector3 fkResult = kinematics.forwardKinematics(angles);
        float error = (fkResult - tests[i].target).magnitude();

        Serial.printf("  FK result: (%.2f, %.2f, %.2f)\n",
                      fkResult.x, fkResult.y, fkResult.z);
        Serial.printf("  Error: %.3f mm\n", error);

        if (error <= tests[i].maxError) {
            Serial.println("  ✅ PASS\n");
            passed++;
        } else {
            Serial.printf("  ❌ FAIL: Error %.3fmm > tolerance %.1fmm\n\n",
                          error, tests[i].maxError);
            failed++;
        }
    }

    // ========================================
    // Test 2: Workspace Boundaries
    // ========================================
    Serial.println("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━");
    Serial.println("TEST 2: Workspace Boundary Validation");
    Serial.println("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");

    float minReach, maxReach;
    kinematics.getWorkspaceBounds(minReach, maxReach);
    Serial.printf("Calculated workspace: %.1fmm - %.1fmm\n\n", minReach, maxReach);

    // Test: Too close (should reject)
    Serial.println("[Boundary Test 1] Target too close");
    Vector3 tooClose(50, 0, 0);
    Serial.printf("  Target: (%.1f, %.1f, %.1f)\n", tooClose.x, tooClose.y, tooClose.z);
    JointAngles anglesClose = kinematics.inverseKinematics(tooClose);

    if (!anglesClose.valid) {
        Serial.println("  ✅ PASS: Correctly rejected\n");
        passed++;
    } else {
        Serial.println("  ❌ FAIL: Should have rejected this target\n");
        failed++;
    }

    // Test: Too far (should reject)
    Serial.println("[Boundary Test 2] Target too far");
    Vector3 tooFar(300, 0, 0);
    Serial.printf("  Target: (%.1f, %.1f, %.1f)\n", tooFar.x, tooFar.y, tooFar.z);
    JointAngles anglesFar = kinematics.inverseKinematics(tooFar);

    if (!anglesFar.valid) {
        Serial.println("  ✅ PASS: Correctly rejected\n");
        passed++;
    } else {
        Serial.println("  ❌ FAIL: Should have rejected this target\n");
        failed++;
    }

    // Test: At minimum boundary (should accept)
    Serial.println("[Boundary Test 3] At minimum reach");
    Vector3 atMin(minReach + 5, 0, 0);
    Serial.printf("  Target: (%.1f, %.1f, %.1f)\n", atMin.x, atMin.y, atMin.z);
    JointAngles anglesMin = kinematics.inverseKinematics(atMin);

    if (anglesMin.valid) {
        Serial.println("  ✅ PASS: Accepted near-boundary target\n");
        passed++;
    } else {
        Serial.println("  ❌ FAIL: Should have accepted this target\n");
        failed++;
    }

    // Test: At maximum boundary (should accept)
    Serial.println("[Boundary Test 4] At maximum reach");
    Vector3 atMax(maxReach - 5, 0, 0);
    Serial.printf("  Target: (%.1f, %.1f, %.1f)\n", atMax.x, atMax.y, atMax.z);
    JointAngles anglesMax = kinematics.inverseKinematics(atMax);

    if (anglesMax.valid) {
        Serial.println("  ✅ PASS: Accepted near-boundary target\n");
        passed++;
    } else {
        Serial.println("  ❌ FAIL: Should have accepted this target\n");
        failed++;
    }

    // ========================================
    // Test 3: Angle Limits
    // ========================================
    Serial.println("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━");
    Serial.println("TEST 3: Servo Angle Limit Validation");
    Serial.println("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");

    struct LimitTest {
        Vector3 target;
        const char* name;
    };

    LimitTest limitTests[] = {
        { Vector3(105, 90, -50),   "Max coxa right" },
        { Vector3(105, -90, -50),  "Max coxa left" },
        { Vector3(130, 0, 50),     "Max height" },
        { Vector3(130, 0, -150),   "Min height" }
    };

    for (int i = 0; i < 4; i++) {
        Serial.printf("[Limit Test %d] %s\n", i+1, limitTests[i].name);
        Serial.printf("  Target: (%.1f, %.1f, %.1f)\n",
                      limitTests[i].target.x, limitTests[i].target.y, limitTests[i].target.z);

        JointAngles angles = kinematics.inverseKinematics(limitTests[i].target);

        if (!angles.valid) {
            Serial.println("  ⚠️  Target unreachable (may be OK)\n");
            continue;
        }

        Serial.printf("  Angles: coxa=%.1f° femur=%.1f° tibia=%.1f°\n",
                      angles.coxa, angles.femur, angles.tibia);

        bool withinLimits =
            (angles.coxa >= COXA_MIN_ANGLE && angles.coxa <= COXA_MAX_ANGLE) &&
            (angles.femur >= FEMUR_MIN_ANGLE && angles.femur <= FEMUR_MAX_ANGLE) &&
            (angles.tibia >= TIBIA_MIN_ANGLE && angles.tibia <= TIBIA_MAX_ANGLE);

        if (withinLimits) {
            Serial.println("  ✅ PASS: All angles within limits\n");
            passed++;
        } else {
            Serial.println("  ❌ FAIL: Angles EXCEED servo limits!\n");
            Serial.printf("     Coxa limits: [%.1f, %.1f], actual: %.1f\n",
                          COXA_MIN_ANGLE, COXA_MAX_ANGLE, angles.coxa);
            Serial.printf("     Femur limits: [%.1f, %.1f], actual: %.1f\n",
                          FEMUR_MIN_ANGLE, FEMUR_MAX_ANGLE, angles.femur);
            Serial.printf("     Tibia limits: [%.1f, %.1f], actual: %.1f\n\n",
                          TIBIA_MIN_ANGLE, TIBIA_MAX_ANGLE, angles.tibia);
            failed++;
        }
    }

    // ========================================
    // Test 4: Symmetry
    // ========================================
    Serial.println("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━");
    Serial.println("TEST 4: Symmetry Validation");
    Serial.println("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");

    Vector3 centerTarget(150, 0, -80);
    Vector3 rightTarget(150, 50, -80);
    Vector3 leftTarget(150, -50, -80);

    JointAngles centerAngles = kinematics.inverseKinematics(centerTarget);
    JointAngles rightAngles = kinematics.inverseKinematics(rightTarget);
    JointAngles leftAngles = kinematics.inverseKinematics(leftTarget);

    Serial.println("[Symmetry Test] Left vs Right");
    Serial.printf("  Center coxa: %.2f°\n", centerAngles.coxa);
    Serial.printf("  Right coxa:  %.2f° (should be positive)\n", rightAngles.coxa);
    Serial.printf("  Left coxa:   %.2f° (should be negative)\n", leftAngles.coxa);

    bool symmetryOK = (abs(rightAngles.coxa - (-leftAngles.coxa)) < 2.0) &&
                      (abs(rightAngles.femur - leftAngles.femur) < 2.0) &&
                      (abs(rightAngles.tibia - leftAngles.tibia) < 2.0);

    if (symmetryOK) {
        Serial.println("  ✅ PASS: Symmetric behavior\n");
        passed++;
    } else {
        Serial.println("  ❌ FAIL: Asymmetric behavior detected\n");
        failed++;
    }

    // ========================================
    // SUMMARY
    // ========================================
    Serial.println("╔════════════════════════════════════════════════════════╗");
    Serial.printf("║  RESULTS: %2d passed, %2d failed                          ║\n", passed, failed);

    if (failed == 0) {
        Serial.println("║                                                        ║");
        Serial.println("║  ✅ ALL TESTS PASSED                                   ║");
        Serial.println("║  System is ready for hardware operation               ║");
    } else {
        Serial.println("║                                                        ║");
        Serial.println("║  ❌ SOME TESTS FAILED                                  ║");
        Serial.println("║  DO NOT PROCEED WITH HARDWARE                          ║");
        Serial.println("║  Review implementation before continuing               ║");
    }

    Serial.println("╚════════════════════════════════════════════════════════╝\n");

    if (failed > 0) {
        Serial.println("⚠️  Recommendations:");
        Serial.println("   1. Review Kinematics.cpp implementation");
        Serial.println("   2. Check segment lengths in Kinematics.h");
        Serial.println("   3. Verify angle limit definitions");
        Serial.println("   4. Run tests again after fixes\n");
    }
}

// Helper: Move to position with cross-validation
bool moveToPositionSafe(const Vector3 &target, Kinematics &ik,
                        int coxaID, int femurID, int tibiaID,
                        void (*setServoAngle)(int, float)) {

    Serial.printf("\n[Safe Move] Target: (%.1f, %.1f, %.1f)\n",
                  target.x, target.y, target.z);

    // Step 1: Calculate IK
    JointAngles angles = ik.inverseKinematics(target);

    if (!angles.valid) {
        Serial.println("❌ IK failed: target unreachable");
        return false;
    }

    Serial.printf("  IK: coxa=%.1f° femur=%.1f° tibia=%.1f°\n",
                  angles.coxa, angles.femur, angles.tibia);

    // Step 2: Cross-validate with FK
    Vector3 fkResult = ik.forwardKinematics(angles);
    float error = (fkResult - target).magnitude();

    Serial.printf("  FK validation: (%.2f, %.2f, %.2f), error=%.2fmm\n",
                  fkResult.x, fkResult.y, fkResult.z, error);

    if (error > 5.0) {
        Serial.printf("❌ FK validation failed: error %.2fmm > 5mm tolerance\n", error);
        return false;
    }

    // Step 3: Move servos
    setServoAngle(coxaID, angles.coxa);
    setServoAngle(femurID, angles.femur);
    setServoAngle(tibiaID, angles.tibia);

    Serial.printf("✅ Move successful, error: %.2fmm\n", error);
    return true;
}
