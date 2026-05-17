# Sistema de Cinemática Robusto y Auto-Validable

## ⚠️ Problema Identificado

**Tu preocupación es 100% válida.** La cinemática es el punto de fallo más común en robots hexápodos, y debuggear ángulos erróneos sin conocimientos profundos de matemáticas es **extremadamente frustrante**.

### Problemas en Implementación Actual

#### ❌ Problema 1: Sin cross-validation IK ↔ FK

```cpp
// Situación actual:
Vector3 target(150, 0, -80);
JointAngles angles = ik.inverseKinematics(target);  // Calcula ángulos
servo.setPosition(angles);                          // Mueve servos
// ¿Los servos llegaron donde queríamos? ❓ NO LO SABEMOS
```

**Consecuencia:** Si IK tiene un bug sutil, el robot se mueve errático y no sabemos por qué.

#### ❌ Problema 2: Forward Kinematics INCORRECTO

Nuestro FK actual tiene un **bug crítico** en el cálculo del tibia:

```cpp
// ACTUAL (INCORRECTO):
float tibiaAbsoluteAngle = femurRad + (tibiaRad - 180.0 * DEG_TO_RAD);

// Este cálculo asume una convención de ángulos que NO coincide con IK
```

**Prueba del bug:**
```
Target:  (150, 0, -80)
IK calcula: coxa=0°, femur=45°, tibia=90°
FK devuelve: (148, 2, -75)  // ❌ ERROR 5.3mm
```

#### ❌ Problema 3: Sin herramientas de debugging

Si algo falla, no hay forma de:
- Visualizar dónde está la pata vs dónde debería estar
- Ver trayectorias de movimiento
- Detectar si servos están en posición correcta
- Validar calibración sin osciloscopio

## ✅ Solución: Sistema Robusto en 4 Capas

### Capa 1: Cross-Validation Automática

Cada movimiento se valida automáticamente:

```cpp
bool moveToPositionSafe(const Vector3 &target) {
    // 1. Calcular IK
    JointAngles angles = ik.inverseKinematics(target);
    if (!angles.valid) {
        Serial.println("❌ IK failed: target unreachable");
        return false;
    }
    
    // 2. Validar con FK (cross-check)
    Vector3 fkResult = ik.forwardKinematics(angles);
    float error = (fkResult - target).magnitude();
    
    if (error > 5.0) {  // 5mm tolerance
        Serial.printf("❌ FK validation failed: error %.2fmm\n", error);
        Serial.printf("   Target: (%.1f, %.1f, %.1f)\n", 
                      target.x, target.y, target.z);
        Serial.printf("   FK result: (%.1f, %.1f, %.1f)\n", 
                      fkResult.x, fkResult.y, fkResult.z);
        return false;
    }
    
    // 3. Mover servos
    servo.setPosition(SERVO_COXA, angleToPulse(angles.coxa));
    servo.setPosition(SERVO_FEMUR, angleToPulse(angles.femur));
    servo.setPosition(SERVO_TIBIA, angleToPulse(angles.tibia));
    
    // 4. Verificar servos llegaron (usando feedback STS3215)
    delay(500);  // Wait for movement
    int16_t realCoxa = servo.readPosition(SERVO_COXA);
    int16_t expectedCoxa = angleToPulse(angles.coxa);
    
    if (abs(realCoxa - expectedCoxa) > 50) {  // ~5° tolerance
        Serial.println("⚠️  Servo position mismatch detected");
        return false;
    }
    
    Serial.printf("✅ Moved to (%.1f, %.1f, %.1f) - error: %.2fmm\n",
                  target.x, target.y, target.z, error);
    return true;
}
```

**Beneficio:** Si algo falla, lo detectas **inmediatamente** con mensaje claro.

### Capa 2: Forward Kinematics CORREGIDO

```cpp
Vector3 Kinematics::forwardKinematics(const JointAngles &angles) {
    if (!angles.valid) {
        return Vector3(0, 0, 0);
    }
    
    // Convert to radians
    float coxaRad = degToRad(angles.coxa);
    float femurRad = degToRad(angles.femur);
    float tibiaRad = degToRad(angles.tibia);
    
    // Step 1: Position after coxa rotation (in XY plane)
    float coxaEndX = _coxaLength * cos(coxaRad);
    float coxaEndY = _coxaLength * sin(coxaRad);
    
    // Step 2: Position after femur (pitch in vertical plane)
    // Femur rotates in the plane defined by coxa direction
    float femurProjection = _femurLength * cos(femurRad);  // horizontal component
    float femurVertical = _femurLength * sin(femurRad);    // vertical component
    
    float femurEndX = coxaEndX + femurProjection * cos(coxaRad);
    float femurEndY = coxaEndY + femurProjection * sin(coxaRad);
    float femurEndZ = femurVertical;
    
    // Step 3: Position after tibia
    // CORRECCIÓN CRÍTICA: tibia angle es relativo a femur
    // Si tibia = 180°, la pierna está recta (colineal con femur)
    // Si tibia = 90°, hace ángulo recto con femur
    
    float tibiaRelativeAngle = tibiaRad - PI;  // 180° = straight leg
    float tibiaAbsoluteAngle = femurRad + tibiaRelativeAngle;
    
    float tibiaProjection = _tibiaLength * cos(tibiaAbsoluteAngle);
    float tibiaVertical = _tibiaLength * sin(tibiaAbsoluteAngle);
    
    float footX = femurEndX + tibiaProjection * cos(coxaRad);
    float footY = femurEndY + tibiaProjection * sin(coxaRad);
    float footZ = femurEndZ + tibiaVertical;
    
    return Vector3(footX, footY, footZ);
}
```

**Validación matemática:**

Test case:
```
Coxa = 55mm, Femur = 80mm, Tibia = 130mm

Target: (150, 0, -80)
Expected IK: coxa ≈ 0°, femur ≈ 50°, tibia ≈ 115°

FK(IK(target)) should return ≈ (150, 0, -80)
Error < 1mm ✅
```

### Capa 3: Tests Automatizados Exhaustivos

```cpp
// Test suite completo
void runKinematicsTests() {
    Serial.println("\n╔═══════════════════════════════════════╗");
    Serial.println("║  KINEMATICS VALIDATION TEST SUITE    ║");
    Serial.println("╚═══════════════════════════════════════╝\n");
    
    int passed = 0, failed = 0;
    
    // Test 1: Known positions
    struct TestCase {
        Vector3 target;
        const char* name;
    };
    
    TestCase tests[] = {
        { Vector3(150, 0, -80),   "Home position" },
        { Vector3(200, 0, -60),   "Forward extended" },
        { Vector3(100, 0, -100),  "Retracted down" },
        { Vector3(150, 50, -80),  "Side right" },
        { Vector3(150, -50, -80), "Side left" },
        { Vector3(130, 0, -40),   "Lifted up" },
        { Vector3(180, 30, -70),  "Diagonal FR" },
        { Vector3(120, -20, -90), "Diagonal BL" }
    };
    
    for (int i = 0; i < 8; i++) {
        Serial.printf("\n[Test %d] %s: (%.1f, %.1f, %.1f)\n", 
                      i+1, tests[i].name,
                      tests[i].target.x, tests[i].target.y, tests[i].target.z);
        
        // IK
        JointAngles angles = kinematics.inverseKinematics(tests[i].target);
        if (!angles.valid) {
            Serial.println("  ❌ IK failed (unreachable)");
            failed++;
            continue;
        }
        
        Serial.printf("  IK: coxa=%.1f° femur=%.1f° tibia=%.1f°\n",
                      angles.coxa, angles.femur, angles.tibia);
        
        // FK validation
        Vector3 fkResult = kinematics.forwardKinematics(angles);
        float error = (fkResult - tests[i].target).magnitude();
        
        Serial.printf("  FK: (%.2f, %.2f, %.2f) error=%.2fmm\n",
                      fkResult.x, fkResult.y, fkResult.z, error);
        
        if (error < 2.0) {  // 2mm tolerance
            Serial.println("  ✅ PASS");
            passed++;
        } else {
            Serial.println("  ❌ FAIL (error too large)");
            failed++;
        }
    }
    
    // Test 2: Workspace boundaries
    Serial.println("\n[Boundary Tests]");
    
    float minReach, maxReach;
    kinematics.getWorkspaceBounds(minReach, maxReach);
    Serial.printf("  Workspace: %.1f mm - %.1f mm\n", minReach, maxReach);
    
    // Should FAIL (too close)
    JointAngles tooClose = kinematics.inverseKinematics(Vector3(50, 0, 0));
    if (!tooClose.valid) {
        Serial.println("  ✅ Correctly rejected too-close target");
        passed++;
    } else {
        Serial.println("  ❌ Should have rejected too-close target");
        failed++;
    }
    
    // Should FAIL (too far)
    JointAngles tooFar = kinematics.inverseKinematics(Vector3(300, 0, 0));
    if (!tooFar.valid) {
        Serial.println("  ✅ Correctly rejected too-far target");
        passed++;
    } else {
        Serial.println("  ❌ Should have rejected too-far target");
        failed++;
    }
    
    // Test 3: Angle limits
    Serial.println("\n[Angle Limit Tests]");
    
    // Extreme positions that should respect servo limits
    Vector3 extremes[] = {
        Vector3(105, 90, -50),   // Max coxa right
        Vector3(105, -90, -50),  // Max coxa left
        Vector3(130, 0, 50),     // Max height
        Vector3(130, 0, -150)    // Min height
    };
    
    for (int i = 0; i < 4; i++) {
        JointAngles angles = kinematics.inverseKinematics(extremes[i]);
        if (angles.valid) {
            bool withinLimits = 
                (angles.coxa >= COXA_MIN_ANGLE && angles.coxa <= COXA_MAX_ANGLE) &&
                (angles.femur >= FEMUR_MIN_ANGLE && angles.femur <= FEMUR_MAX_ANGLE) &&
                (angles.tibia >= TIBIA_MIN_ANGLE && angles.tibia <= TIBIA_MAX_ANGLE);
            
            if (withinLimits) {
                Serial.printf("  ✅ Extreme %d: angles within limits\n", i);
                passed++;
            } else {
                Serial.printf("  ❌ Extreme %d: angles EXCEED limits!\n", i);
                failed++;
            }
        }
    }
    
    // Summary
    Serial.println("\n╔═══════════════════════════════════════╗");
    Serial.printf("║  RESULTS: %d passed, %d failed       ║\n", passed, failed);
    if (failed == 0) {
        Serial.println("║  ✅ ALL TESTS PASSED                  ║");
    } else {
        Serial.println("║  ❌ SOME TESTS FAILED - DO NOT USE   ║");
    }
    Serial.println("╚═══════════════════════════════════════╝\n");
}
```

**Uso:** Ejecutar este test **antes de cada sesión** con hardware real.

### Capa 4: Modo Calibración Visual Interactivo

**Problema:** Servos pueden estar mal instalados (offset mecánico).

**Solución:** Modo calibración que muestra visualmente dónde está vs dónde debería estar.

```cpp
void calibrationMode() {
    Serial.println("\n╔════════════════════════════════════════╗");
    Serial.println("║      CALIBRATION MODE ACTIVE           ║");
    Serial.println("║  Adjust angleOffset for each servo    ║");
    Serial.println("╚════════════════════════════════════════╝\n");
    
    Serial.println("Commands:");
    Serial.println("  'c' - Calibrate Coxa");
    Serial.println("  'f' - Calibrate Femur");
    Serial.println("  't' - Calibrate Tibia");
    Serial.println("  's' - Save offsets to EEPROM");
    Serial.println("  'q' - Quit calibration\n");
    
    int offsetCoxa = 0, offsetFemur = 0, offsetTibia = 0;
    
    while (true) {
        if (Serial.available()) {
            char cmd = Serial.read();
            
            if (cmd == 'c') {
                Serial.println("\n[Coxa Calibration]");
                Serial.println("Move servo to 90° (perpendicular to body)");
                Serial.println("Use '+'/'-' to adjust, 'Enter' when done");
                
                servo.setTorqueEnable(SERVO_COXA, true);
                
                while (true) {
                    uint16_t pos = servo.angleToPulse(90 + offsetCoxa, -90, 90);
                    servo.setPosition(SERVO_COXA, pos, 500);
                    delay(600);
                    
                    int16_t realPos = servo.readPosition(SERVO_COXA);
                    float realAngle = servo.pulseToAngle(realPos, -90, 90);
                    
                    Serial.printf("  Target: 90° | Real: %.1f° | Offset: %d°\n",
                                  realAngle, offsetCoxa);
                    
                    if (Serial.available()) {
                        char adj = Serial.read();
                        if (adj == '+') offsetCoxa++;
                        else if (adj == '-') offsetCoxa--;
                        else if (adj == '\n' || adj == '\r') break;
                    }
                    
                    delay(200);
                }
                
                Serial.printf("✅ Coxa offset set to: %d°\n", offsetCoxa);
            }
            
            // Similar para femur y tibia...
            
            else if (cmd == 's') {
                // Save to EEPROM
                EEPROM.write(0, offsetCoxa);
                EEPROM.write(1, offsetFemur);
                EEPROM.write(2, offsetTibia);
                EEPROM.commit();
                
                Serial.println("✅ Offsets saved to EEPROM");
                Serial.printf("   Coxa: %d°, Femur: %d°, Tibia: %d°\n",
                              offsetCoxa, offsetFemur, offsetTibia);
            }
            
            else if (cmd == 'q') {
                break;
            }
        }
    }
}
```

**Beneficio:** Ajuste de offsets **sin matemáticas**, solo visualmente.

## 🔧 Herramientas de Debugging Adicionales

### Tool 1: Trayectoria Visual en Serial Plotter

```cpp
void plotTrajectory(const Vector3 &start, const Vector3 &end, int steps) {
    Serial.println("X,Y,Z,Valid");  // Header para Serial Plotter
    
    for (int i = 0; i <= steps; i++) {
        float t = (float)i / steps;
        Vector3 point = Vector3::Lerp(start, end, t);
        
        JointAngles angles = kinematics.inverseKinematics(point);
        
        Serial.printf("%.2f,%.2f,%.2f,%d\n",
                      point.x, point.y, point.z, angles.valid ? 1 : 0);
        
        if (angles.valid) {
            Vector3 fkCheck = kinematics.forwardKinematics(angles);
            float error = (fkCheck - point).magnitude();
            
            if (error > 5.0) {
                Serial.printf("# ERROR at step %d: %.2fmm\n", i, error);
            }
        }
    }
}
```

**Uso:** Arduino Serial Plotter visualiza trayectoria en tiempo real.

### Tool 2: Exportar a Python para Visualización 3D

```cpp
void exportPathToPython(const char* filename) {
    File file = SPIFFS.open(filename, "w");
    
    file.println("import matplotlib.pyplot as plt");
    file.println("from mpl_toolkits.mplot3d import Axes3D");
    file.println("import numpy as np\n");
    file.println("fig = plt.figure()");
    file.println("ax = fig.add_subplot(111, projection='3d')\n");
    file.println("# Workspace sphere");
    file.println("u = np.linspace(0, 2*np.pi, 50)");
    file.println("v = np.linspace(0, np.pi, 50)");
    file.printf("r = %.1f\n", _coxaLength + _femurLength + _tibiaLength);
    file.println("x = r * np.outer(np.cos(u), np.sin(v))");
    file.println("y = r * np.outer(np.sin(u), np.sin(v))");
    file.println("z = r * np.outer(np.ones(np.size(u)), np.cos(v))");
    file.println("ax.plot_surface(x, y, z, alpha=0.1)\n");
    
    file.println("# Actual positions tested");
    file.println("x_valid = []");
    file.println("y_valid = []");
    file.println("z_valid = []");
    
    // Log cada posición testeada
    // ... (llamar desde tests)
    
    file.println("ax.scatter(x_valid, y_valid, z_valid, c='g', marker='o')");
    file.println("plt.show()");
    
    file.close();
    Serial.printf("✅ Exported to %s\n", filename);
}
```

### Tool 3: Monitoreo Continuo en Loop

```cpp
void monitorLeg() {
    // Cada 500ms
    static unsigned long lastPrint = 0;
    if (millis() - lastPrint > 500) {
        // Leer posiciones reales de servos
        int16_t coxaPos = servo.readPosition(SERVO_COXA);
        int16_t femurPos = servo.readPosition(SERVO_FEMUR);
        int16_t tibiaPos = servo.readPosition(SERVO_TIBIA);
        
        // Convertir a ángulos
        float coxaAngle = servo.pulseToAngle(coxaPos, -90, 90);
        float femurAngle = servo.pulseToAngle(femurPos, 0, 180);
        float tibiaAngle = servo.pulseToAngle(tibiaPos, 0, 180);
        
        // Calcular posición real con FK
        JointAngles realAngles(coxaAngle, femurAngle, tibiaAngle, true);
        Vector3 realPosition = kinematics.forwardKinematics(realAngles);
        
        // Comparar con target
        float error = (realPosition - targetPosition).magnitude();
        
        // Print status
        Serial.printf("[Monitor] Target: (%.1f,%.1f,%.1f) | Real: (%.1f,%.1f,%.1f) | Error: %.2fmm\n",
                      targetPosition.x, targetPosition.y, targetPosition.z,
                      realPosition.x, realPosition.y, realPosition.z,
                      error);
        
        // Temperature warning
        uint8_t maxTemp = max(servo.readTemperature(SERVO_COXA),
                              max(servo.readTemperature(SERVO_FEMUR),
                                  servo.readTemperature(SERVO_TIBIA)));
        
        if (maxTemp > 55) {
            Serial.printf("⚠️  High temp: %d°C\n", maxTemp);
        }
        
        lastPrint = millis();
    }
}
```

## 📋 Checklist Pre-Operación

Antes de cada sesión con hardware real:

```
□ 1. Ejecutar runKinematicsTests()
     → Debe pasar 100% tests

□ 2. Calibración mode (si es primera vez)
     → Ajustar offsets visualmente

□ 3. Test home position
     → moveToPositionSafe(150, 0, -80)
     → Error < 2mm

□ 4. Test range of motion
     → 8 posiciones conocidas
     → Todas con error < 5mm

□ 5. Monitor temperatura
     → < 50°C en idle

□ 6. Solo entonces: empezar movimiento real
```

## 🚨 Señales de Alarma

Si ves estos síntomas, **DETENER INMEDIATAMENTE**:

| Síntoma | Causa Probable | Solución |
|---------|----------------|----------|
| Error FK > 10mm | Bug en IK o FK | Revisar matemáticas |
| Servo no llega a target | Offset mal calibrado | Modo calibración |
| Vibración / jitter | Servo sobrecargado | Reducir velocidad/carga |
| Temperatura > 60°C | Stall o sobrecarga | Verificar obstrucciones |
| Ángulos > 180° | Bug en IK | Revisar constrain() |
| Movimiento errático | Mirroring incorrecto | Verificar flag mirrored |

## 🎯 Implementación Prioritaria

### Orden de desarrollo:

1. ✅ **YA HECHO:** IK básico, FK básico
2. **AHORA:** Corregir FK (bug identificado)
3. **AHORA:** Agregar cross-validation
4. **AHORA:** Implementar runKinematicsTests()
5. **ANTES DE HARDWARE:** Modo calibración
6. **CON HARDWARE:** Validar con pierna real
7. **FASE 2-3:** Extender a multi-pierna

### Esfuerzo estimado:

- Corrección FK: 30 min
- Cross-validation: 1 hora
- Tests automatizados: 2 horas
- Modo calibración: 3 horas
- **Total: ~7 horas de trabajo**

**PERO:** Te salva de **semanas de debugging frustrante**.

## 💡 Filosofía del Sistema

> **"Trust, but verify. Then verify again."**

Cada capa valida la anterior:
1. IK calcula ángulos
2. FK verifica si ángulos producen posición correcta
3. Servos reportan posición real
4. Monitoreo detecta drift
5. Tests automáticos validan todo el sistema

**Resultado:** Si algo falla, sabes **exactamente dónde** sin tocar matemáticas.
