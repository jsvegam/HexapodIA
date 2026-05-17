# Análisis de Referencias - Sir-Kuhnhero Hexapod & RookiDroid v2

## Overview

Este documento analiza dos referencias clave para HexapodIA:
1. **Sir-Kuhnhero/Hexapod** - Implementación completa en GitHub
2. **RookiDroid Hexapod v2** - Diseño mecánico comercial

## Sir-Kuhnhero Hexapod - Análisis Técnico

### Especificaciones Encontradas

#### Hardware
- **MCU:** STM32F103C8 (Blue Pill)
- **Servos:** 18× servos 21g (tipo hobby, NO smart servos)
- **Driver:** 2× Adafruit PCA9685 (PWM servo driver, I2C)
- **LEDs:** 271× WS2812B RGB
- **Bluetooth:** HC-05/06 (control remoto)
- **Batería:** LiPo (voltaje no especificado)
- **Costo total:** ~120€

#### Dimensiones Mecánicas ✅ **DATOS CLAVE**
```cpp
#define LENGTH_TROCHANTER 50   // Coxa
#define LENGTH_FEMUR 80
#define LENGTH_TIBIA 120
```

**Comparación con HexapodIA:**
```
Componente  | Sir-Kuhnhero | HexapodIA | Diferencia
------------|--------------|-----------|------------
Coxa        | 50mm         | 55mm      | +5mm (10%)
Femur       | 80mm         | 80mm      | ✅ Igual
Tibia       | 120mm        | 130mm     | +10mm (8%)
```

**Implicaciones:**
- HexapodIA tiene patas ~7% más largas
- Mayor workspace (alcance 50-265mm vs ~50-250mm)
- Requiere más torque (~15% más carga en servos)

#### Software

**Framework:**
- PlatformIO + Arduino
- Librerías: Adafruit PWM, FastLED

**Arquitectura de Código:**

```
Hexapod_code/
├── include/header.h        (317 líneas - definiciones)
└── src/
    ├── main.cpp           (164 líneas)
    ├── ik.cpp             (69 líneas)
    ├── math.cpp           (515 líneas - Vector3/Vector2)
    ├── walkGait.cpp       (349 líneas)
    ├── output.cpp         (313 líneas - servos + LEDs)
    ├── bluetooh.cpp       (92 líneas)
    └── debug.cpp          (113 líneas)

Total: ~1600 líneas
```

**Comparación con HexapodIA:**
```
HexapodIA:
├── main.cpp               (~400 líneas)
├── STS3215.cpp/h          (~400 líneas)
├── Kinematics.cpp/h       (~200 líneas)
└── (futuro) GaitEngine    (pendiente)

Total actual: ~1000 líneas
```

### Cinemática Inversa - Comparación Detallada

#### Implementación Sir-Kuhnhero

```cpp
const Vector3 legZeroOffset(130, 0, -120);

void calcLegServoAngles(Leg_Struct &leg) {
    // 1. Transform to local leg coordinates
    float sinAlpha = sin((leg.mountAngle) * DEG_TO_RAD);
    float cosAlpha = cos((leg.mountAngle) * DEG_TO_RAD);
    
    localCoordinates.x = cosAlpha * leg.targetPosition.x + sinAlpha * leg.targetPosition.y;
    localCoordinates.x += legZeroOffset.x;  // 130mm offset
    
    localCoordinates.y = sinAlpha * leg.targetPosition.x - cosAlpha * leg.targetPosition.y;
    localCoordinates.y += legZeroOffset.y;
    
    localCoordinates.z = leg.targetPosition.z + legZeroOffset.z;  // -120mm
    
    // 2. Calculate servo angles
    float noTrochanter = sqrt(x² + y²) - LENGTH_TROCHANTER;
    float servo2TipDistance = sqrt(noTrochanter² + z²);
    
    // Angle 0 (Coxa)
    angle_0 = atan2(y, x) * RAD_TO_DEG + 90;
    
    // Angle 1 (Femur) - law of cosines
    angleRightSideTriangle = atan2(z, noTrochanter) * RAD_TO_DEG;
    angleUnequalTriangle = acos((TIBIA² - servo2TipDistance² - FEMUR²) / 
                               (-2 * servo2TipDistance * FEMUR)) * RAD_TO_DEG;
    
    angle_1 = (angleRightSideTriangle + angleUnequalTriangle) + 90;
    
    // Angle 2 (Tibia) - law of cosines
    angleUnequalTriangle_2 = acos((servo2TipDistance² - TIBIA² - FEMUR²) / 
                                  (-2 * TIBIA * FEMUR)) * RAD_TO_DEG;
    
    angle_2 = 180 - angleUnequalTriangle_2;
}
```

**Características clave:**
1. ✅ Usa offset de home position (130, 0, -120)
2. ✅ Transform por rotación de montaje de pierna
3. ✅ IK analítico (law of cosines)
4. ✅ Maneja legs "mirrored" (izquierda vs derecha)

#### Implementación HexapodIA

```cpp
JointAngles Kinematics::inverseKinematics(const Vector3 &footPos) {
    // 1. Coxa angle
    float coxaAngleRad = atan2(footPos.y, footPos.x);
    result.coxa = radToDeg(coxaAngleRad);
    
    // 2. Project to 2D
    float horizontalDistance = sqrt(footPos.x² + footPos.y²);
    float distFromCoxa = horizontalDistance - _coxaLength;
    float verticalDistance = footPos.z;
    float targetDistance = sqrt(distFromCoxa² + verticalDistance²);
    
    // 3. Law of cosines
    float angleAtFoot = lawOfCosines(_femurLength, targetDistance, _tibiaLength);
    float angleAtFemur = lawOfCosines(_femurLength, _tibiaLength, targetDistance);
    float targetAngleRad = atan2(verticalDistance, distFromCoxa);
    
    result.femur = radToDeg(targetAngleRad + angleAtFemur);
    result.tibia = 180.0 - radToDeg(angleAtFoot);
    
    return result;
}
```

**Diferencias clave:**

| Aspecto | Sir-Kuhnhero | HexapodIA | Evaluación |
|---------|--------------|-----------|------------|
| **Home offset** | Sí (130, 0, -120) | No | ⚠️ Agregar |
| **Leg rotation** | Sí (mountAngle) | No | ⚠️ Agregar |
| **Mirrored legs** | Sí (flag) | No | ⚠️ Agregar |
| **IK algorithm** | Law of cosines | Law of cosines | ✅ Igual |
| **Validación** | No | Sí (isReachable) | ✅ Mejor |

**Recomendación:** 
- ✅ HexapodIA tiene mejor validación (workspace checks)
- ⚠️ Necesita agregar soporte multi-pierna (mount angles, mirroring)

### Gait Generation - Análisis

#### Implementación Sir-Kuhnhero

**Características:**
- **Tipo:** Tripod gait adaptativo
- **Grupos:** Legs [0,2,4] vs [1,3,5]
- **Selección dinámica:** Elige grupo que puede dar paso más largo
- **Path planning:** Usa arrays dinámicos de Vector3 para trayectorias
- **Interpolación:** Basada en longitud de path (no tiempo)

**Algoritmo:**

```cpp
void walkCycle() {
    // 1. Si todas las legs están abajo y hay input, iniciar ciclo
    if (!Leg[0].lifted && !Leg[1].lifted && HexapodState == WALKING) {
        setLegStateAtWalkInit();  // Selecciona grupo a levantar
    }
    
    // 2. Para cada leg: calcular target y path
    for (i = 0; i < 6; i++) {
        calcLegPath(Leg[i], calcTarget(Leg[i]));
    }
    
    // 3. Interpolar siguiente posición
    calcInterpolatedTarget(interpolatedTarget);
    
    // 4. Detectar si legs alcanzaron target, cambiar grupo
    setLegStateAtTargetReach(interpolatedTarget);
    
    // 5. Aplicar posiciones calculadas
    for (i = 0; i < 6; i++) {
        Leg[i].targetPosition = interpolatedTarget[i];
    }
}
```

**Ventajas del enfoque:**
- ✅ Adaptativo (elige mejor grupo según posición actual)
- ✅ Smooth (interpolación por longitud de path)
- ✅ Robusto (detecta target reach dinámicamente)

**Desventajas:**
- ❌ Complejo (mucha lógica de path planning)
- ❌ Memoria (arrays dinámicos por leg)
- ❌ CPU (cálculo de paths cada frame)

#### HexapodIA (Pendiente Implementar)

**Approach sugerido:**
- Tripod gait estático (grupos fijos)
- Interpolación por tiempo (no longitud)
- Sin path planning complejo (solo start/end)

**Trade-offs:**
| Aspecto | Sir-Kuhnhero | HexapodIA sugerido |
|---------|--------------|---------------------|
| Complejidad | Alta | Baja |
| Memoria | ~300 bytes/leg | ~20 bytes/leg |
| Adaptabilidad | Alta | Baja |
| Suavidad | Muy alta | Alta |
| CPU usage | Alta (~40%) | Baja (~15%) |

**Recomendación FASE 3:**
- Empezar con gait simple (grupos fijos, interpolación lineal)
- Si movimiento es errático, considerar path planning de Sir-Kuhnhero

### Estructura de Datos - Comparación

#### Sir-Kuhnhero

```cpp
struct Leg_Struct {
    int minLED, maxLED;
    Servo_Struct Servo[3];
    bool mirrored;                // left vs right
    int mountAngle;               // 0°, 60°, 120°, etc.
    Vector3 targetPosition;
    Vector3 curPosition;
    std::vector<Vector3> pointOnPath;  // dynamic array
    bool lifted;                  // walking state
};
```

**Tamaño estimado:** ~100 bytes + dynamic array

#### HexapodIA (Propuesto)

```cpp
struct Leg {
    uint8_t servoIds[3];          // Coxa, Femur, Tibia
    JointAngles currentAngles;
    JointAngles targetAngles;
    Vector3 targetPosition;
    bool isLifted;
    uint8_t mountAngle;           // degrees
    bool mirrored;
};
```

**Tamaño estimado:** ~30 bytes

### Control de Servos - Comparación Crítica

#### Sir-Kuhnhero: PWM via PCA9685

```cpp
#define SERVO_FREQ 50  // 50Hz update
#define SERVO_MIN 100
#define SERVO_MAX 450

struct Servo_Struct {
    int ch;                    // PCA9685 channel
    int minAngle = 50;
    int maxAngle = 130;
    int targetAngle = 90;
    int angleOffset = 0;       // Calibration
};

void Servo_update(const Servo_Struct &servo) {
    // Map angle to PWM pulse width
    int pulseWidth = map(angle + offset, 0, 180, SERVO_MIN, SERVO_MAX);
    pwm.setPWM(servo.ch, 0, pulseWidth);
}
```

**Limitaciones PWM:**
- ❌ Sin feedback de posición real
- ❌ Sin detección de stall
- ❌ Sin lectura de temperatura
- ❌ Sin lectura de corriente
- ❌ Backlash no detectable
- ❌ Calibración manual (angleOffset)

#### HexapodIA: Smart Servos STS3215

```cpp
bool setPosition(uint8_t id, uint16_t position, 
                 uint16_t time = 0, uint16_t speed = 0);

int16_t readPosition(uint8_t id);
uint8_t readTemperature(uint8_t id);
uint8_t readVoltage(uint8_t id);
int16_t readLoad(uint8_t id);
bool isMoving(uint8_t id);
```

**Ventajas Smart Servos:**
- ✅ Feedback real (closed-loop)
- ✅ Detección stall/overload
- ✅ Monitoreo térmico
- ✅ Sincronización precisa
- ✅ Backlash mínimo (mejor mecánica interna)
- ✅ Auto-calibración posible

**Justificación costo extra:**
- Sir-Kuhnhero usa 18× servos 21g (~€2/ud) = €36
- HexapodIA usa 18× STS3215 (~€15/ud) = €270
- **Diferencia: +€234**

**Valor agregado:**
- Locomoción fluida desde inicio (no rehacer)
- Detección problemas en tiempo real
- Mejor precisión (±0.3° vs ±2°)
- Mayor durabilidad (sin desgaste por backlash)

## RookiDroid Hexapod v2

### Información del Sitio Web

**Compatibilidad MCU:**
- Raspberry Pi Pico
- ESP32

**Servos:**
- 21G servos (tipo hobby, PWM)
- ⚠️ Nota importante: "MG90S servos used in v1 frequently fail"

**Material:**
- PLA recomendado
- Diseño optimizado:
  - Capas NO perpendiculares a cargas críticas
  - Conexiones reforzadas (piernas, servo mounts)
  - Sistema interlocking (sin adhesivos)

**Features:**
- WiFi control (ESP32)
- OTA updates
- Smooth movement

### Diferencias v1 vs v2

| Aspecto | v1 | v2 |
|---------|----|----|
| Servos | MG90S (fallan) | 21G genéricos |
| Estructura | Débil | Reforzada |
| Adhesivos | Requeridos | Interlocking |
| Ensamblaje | Complejo | Simplificado |

**Lección aprendida:** Usar servos de mejor calidad desde inicio.

### Arquitectura vs Sir-Kuhnhero

**Similitudes:**
- Misma configuración mecánica (18DOF, 3DOF/leg)
- Dimensiones similares
- Servos PWM hobby

**Diferencias:**
- RookiDroid: ESP32/Pico (más moderno)
- Sir-Kuhnhero: STM32 + Bluetooth + LEDs (más features)

## Recomendaciones para HexapodIA

### 1. Dimensiones Mecánicas ✅

**Usar dimensiones HexapodIA actuales:**
```
Coxa:  55mm  (vs 50mm referencia)
Femur: 80mm  (igual)
Tibia: 130mm (vs 120mm referencia)
```

**Razones:**
- Mayor workspace útil
- STS3215 tiene torque suficiente (20kg·cm vs servos 21g ~4kg·cm)
- Margen de seguridad para futuras cargas (Jetson, batería)

### 2. Cinemática Inversa ⚠️ **MEJORAR**

**Agregar a Kinematics.cpp:**

```cpp
class Kinematics {
public:
    // Nuevo: constructor con mount angle y mirror flag
    Kinematics(float mountAngle, bool mirrored);
    
    // Existente (mejorado)
    JointAngles inverseKinematics(const Vector3 &footPos);
    
private:
    float _mountAngle;    // Rotation of leg (0°, 60°, 120°, etc.)
    bool _mirrored;       // Left vs right side
    Vector3 _homeOffset;  // Default (130, 0, -120)
    
    // Nueva: transform to local leg coordinates
    Vector3 transformToLegSpace(const Vector3 &globalPos);
};
```

**Why:**
- Sir-Kuhnhero usa esto para hexapod completo
- Simplifica control multi-pierna
- Evita cálculos redundantes

### 3. Arquitectura de Piernas ✅ **ADOPTAR**

```cpp
struct Leg {
    // IDs servos
    uint8_t coxaID;
    uint8_t femurID;
    uint8_t tibiaID;
    
    // Kinematics
    Kinematics* ik;       // Puntero a instancia IK configurada
    
    // Estado
    Vector3 currentPos;
    Vector3 targetPos;
    JointAngles currentAngles;
    JointAngles targetAngles;
    bool isLifted;
    
    // Configuración
    uint8_t mountAngle;   // 0, 60, 120, 180, 240, 300
    bool mirrored;        // false: right side, true: left side
};

class Hexapod {
public:
    Leg legs[6];
    
    void init();
    void standUp();
    void sitDown();
    void walk(Vector2 direction, float rotation);
    
private:
    void updateLeg(uint8_t legID);
    void tripodGait();
};
```

### 4. Gait Generation 🎯 **SIMPLIFICAR PRIMERO**

**FASE 3 (inicial):** Tripod gait simple
```cpp
void tripodGait() {
    static bool group1Lifted = false;
    
    // Alternate groups every N steps
    if (group1Lifted) {
        // Lift [0, 2, 4], push [1, 3, 5]
    } else {
        // Lift [1, 3, 5], push [0, 2, 4]
    }
}
```

**FASE 3 (avanzado):** Si movimiento errático, adoptar path planning de Sir-Kuhnhero

### 5. Vector Math Library ✅ **REUTILIZAR**

La implementación de Vector3/Vector2 de Sir-Kuhnhero es excelente:
- Operator overloading
- Lerp, normalize, dot product
- Magnitude, inverse
- Rotation helpers

**Recomendación:** Copiar `math.cpp` (con atribución) o reimplementar similar.

### 6. NO Adoptar de Sir-Kuhnhero

❌ **LEDs (271× WS2812B)**
- Fuera de scope FASE 1-3
- Alto consumo CPU/memoria
- Puede agregarse FASE 4-5 opcionalmente

❌ **PCA9685 PWM drivers**
- HexapodIA usa smart servos (serial bus)
- PWM es inferior (sin feedback)

❌ **STM32**
- ESP32 es superior (WiFi, más RAM, mejor ecosystem)

❌ **Path planning complejo**
- Overkill para FASE 3 inicial
- Considerar solo si gait simple falla

## Diferencias Clave: HexapodIA vs Referencias

### Hardware

| Componente | Sir-Kuhnhero | RookiDroid v2 | HexapodIA |
|------------|--------------|---------------|-----------|
| MCU | STM32F103C8 | ESP32/Pico | ESP32 ✅ |
| Servos | 21g PWM | 21g PWM | STS3215 smart ✅✅ |
| Driver | PCA9685 | PCA9685 | Serial bus ✅ |
| Feedback | ❌ | ❌ | ✅ Posición, temp, load |
| IA/ROS2 | ❌ | ❌ | ✅ Jetson (FASE 5) |
| Costo | ~€120 | ~€100 | ~€400 |

### Software

| Aspecto | Sir-Kuhnhero | HexapodIA |
|---------|--------------|-----------|
| Framework | PlatformIO ✅ | PlatformIO ✅ |
| IK | Law of cosines ✅ | Law of cosines ✅ |
| Multi-leg | ✅ Implementado | ⏳ FASE 2-3 |
| Gait | ✅ Adaptive tripod | ⏳ FASE 3 |
| Validación | ❌ | ✅ Workspace checks |
| Feedback loop | ❌ | ✅ Closed-loop |

### Filosofía

| Principio | Sir-Kuhnhero | HexapodIA |
|-----------|--------------|-----------|
| Costo | Mínimo (hobby) | Inversión (profesional) |
| Escalabilidad | Limitada (PWM) | Alta (smart servos) |
| Mantenibilidad | Media | Alta (modular) |
| Evolutividad | Baja (cerrado) | Alta (ROS2-ready) |
| Rehacer locomoción | Probable (backlash) | Improbable ✅ |

## Conclusiones y Plan de Acción

### ✅ Validado de Referencias

1. **Dimensiones mecánicas** HexapodIA (55/80/130) son apropiadas
2. **IK algorithm** (law of cosines) es correcto
3. **Arquitectura ESP32** es superior a STM32 para este proyecto
4. **Smart servos** justifican el costo extra (no rehacer locomoción)

### ⚠️ Pendiente Implementar (FASE 2-3)

1. **Multi-leg support:**
   - Mount angles (0°, 60°, 120°, 180°, 240°, 300°)
   - Mirroring (left vs right)
   - Transform to leg local space

2. **Hexapod class:**
   - Legs[6] array
   - standUp() / sitDown()
   - walk(direction, rotation)

3. **Gait generation:**
   - Tripod gait simple primero
   - Path planning solo si es necesario

4. **Vector math library:**
   - Lerp, normalize, operator overloading
   - Reutilizar de Sir-Kuhnhero (con atribución)

### 🎯 NO Prioritario

- LEDs decorativos (fuera de scope)
- Bluetooth app (RC receiver suficiente)
- PCA9685 PWM (incompatible con smart servos)

## Referencias

- **Sir-Kuhnhero Hexapod:** https://github.com/Sir-Kuhnhero/Hexapod
- **Printables:** https://www.printables.com/model/606030-3d-printed-hexapod
- **RookiDroid v2:** https://rookidroid.com/product/hexapod-v2-stl/
- **Instructables:** https://www.instructables.com/3D-Printed-Hexapod/

## Licencia

Sir-Kuhnhero Hexapod es MIT License. Podemos reutilizar código con atribución apropiada.

HexapodIA es proyecto independiente con arquitectura propia, pero reconoce inspiración en estos diseños.
