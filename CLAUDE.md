# CLAUDE.md - Guía del Proyecto para Claude Code

## Contexto del Proyecto
Este es **HexapodIA**, un robot hexápodo modular de 18DOF diseñado con arquitectura profesional, control tiempo real y preparado para integración IA/ROS2.

## Filosofía Central
🎯 **Locomoción primero, IA después**

- Usamos smart servos (STS3215) desde el inicio
- Arquitectura modular ESP32 + Jetson (futuro)
- Evitamos rehacer locomoción
- Geometría basada en RookiDroid probada
- Estética mecha técnico funcional

## Fase Actual
📍 **FASE 1 - Pierna Única**

Enfoque: Validar mecánica, electrónica, IK y control básico ANTES de escalar a 6 piernas.

## Estructura de Archivos

```
HexapodIA/
├── firmware/esp32_locomotion/   # Código ESP32 (PlatformIO)
│   ├── src/main.cpp             # Firmware principal
│   ├── lib/STS3215/             # Librería servos
│   └── lib/Kinematics/          # IK/FK
├── hardware/                    # Diseños mecánicos, esquemas
├── docs/                        # Documentación técnica
│   ├── technical/               # Specs, arquitectura
│   └── phase_reports/           # Objetivos por fase
├── simulation/                  # Scripts Python IK testing
└── tests/                       # Tests unitarios
```

## Decisiones de Diseño Clave

### Hardware
- **Servos:** Feetech STS3215 (smart servo, serial bus, 20kg·cm)
- **MCU:** ESP32 (control locomoción tiempo real)
- **IA (futuro):** Jetson Orin Nano (ROS2, visión, SLAM)
- **IMU:** BNO085 (estabilización)
- **Material:** PETG (estructura), TPU (pies)

### Software
- **Framework:** Arduino + PlatformIO
- **IK:** Analítico (2-link + coxa offset)
- **Gaits:** Tripod (FASE 3), Ripple (FASE 3)
- **Control Loop:** 50Hz
- **Baudrate Servos:** 1Mbps

### Dimensiones (RookiDroid-inspired)
```
Coxa:  55mm
Femur: 80mm
Tibia: 130mm
Body:  180×160mm
```

## Reglas de Desarrollo

### Firmware
1. **Nunca bloquear el control loop principal**
   - IK debe completar en < 5ms
   - Lecturas servo non-blocking preferidas
   
2. **Validar siempre rangos de ángulos**
   - Coxa: -90° a 90°
   - Femur: 0° a 180°
   - Tibia: 0° a 180°

3. **Safety first**
   - Emergency stop en GPIO0
   - Thermal monitoring de servos
   - Timeout de comunicación (50ms)
   - Watchdog timer

4. **Logging informativo**
   - Estados de máquina
   - Errores de IK
   - Timeouts de servo
   - Métricas de performance

### Mecánica
1. **Tolerancias ajustadas** pero imprimibles
2. **Insertos térmicos M3** para mantenibilidad
3. **Relleno mínimo 40%** (estructuras cargadas)
4. **Evitar soportes complejos** en diseño

## Testing Protocol

### Pre-merge Checklist
- [ ] Compila sin warnings
- [ ] IK test cases pasan
- [ ] No hay magic numbers (usar defines)
- [ ] Comentarios en lógica no-obvia
- [ ] Validación de rangos implementada

### Tests Disponibles
```bash
# En main.cpp hay tests interactivos:
'1' - Individual joint test
'2' - IK position test  
'3' - Circular motion test
's' - Servo status
'h' - Home position
```

## Problemas Conocidos

### Actuales
- [ ] IK aún no validado con hardware real
- [ ] Thermal throttling no implementado
- [ ] ROS2 bridge pendiente (FASE 5)

### Resueltos
- ✅ Librería STS3215 funcional
- ✅ Cinemática analítica implementada
- ✅ Control loop estructurado

## Próximos Pasos

### Inmediato (FASE 1)
1. Imprimir piezas pierna en PETG
2. Ensamblar con servos STS3215
3. Validar firmware con hardware real
4. Medir torque, temperatura, precisión
5. Ajustar IK según resultados

### Futuro (FASE 2)
- Diseñar estructura central (body)
- Integrar 2-3 piernas
- Implementar sincronización multi-pierna
- Gait básico (2 piernas alternadas)

## Referencias Rápidas

### Documentos Importantes
- [SPECIFICATIONS.md](docs/technical/SPECIFICATIONS.md) - Todas las specs técnicas
- [FASE_1_OBJECTIVES.md](docs/phase_reports/FASE_1_OBJECTIVES.md) - Objetivos fase actual
- [STS3215_SETUP.md](hardware/servos_config/STS3215_SETUP.md) - Setup servos

### Control Table STS3215
```cpp
// Lectura común
STS_PRESENT_POSITION_L  // 0x38-39
STS_PRESENT_TEMPERATURE // 0x3F
STS_PRESENT_VOLTAGE     // 0x3E

// Escritura común  
STS_TORQUE_ENABLE       // 0x28
STS_GOAL_POSITION_L     // 0x2A-2B
```

### Fórmulas IK (2-link)
```cpp
// Coxa
theta1 = atan2(y, x)

// 2D projection
d = sqrt(x^2 + y^2) - coxa_length
r = sqrt(d^2 + z^2)

// Law of cosines
theta3 = acos((femur^2 + tibia^2 - r^2) / (2*femur*tibia))
theta2 = atan2(z, d) + acos((femur^2 + r^2 - tibia^2) / (2*femur*r))
```

## Estilo de Código

### Naming
```cpp
// Classes: PascalCase
class ServoController {};

// Functions: camelCase
void moveToPosition() {}

// Constants: UPPER_SNAKE_CASE
#define SERVO_BAUDRATE 1000000

// Variables: camelCase
int servoPosition;
```

### Comments
```cpp
// Good: WHY, not WHAT
// Offset by 90° because servo center is 2048 (not 0)
float angle = servoAngle + 90.0;

// Bad: obvious
// Add 90 to angle
float angle = servoAngle + 90.0;
```

## Build Commands

```bash
# Build firmware
cd firmware/esp32_locomotion
pio run

# Upload to ESP32
pio run -t upload

# Serial monitor
pio device monitor -b 115200

# Clean build
pio run -t clean
```

## Contacto con Usuario
- Para dudas de hardware: preguntar antes de asumir
- Para cambios arquitectónicos: validar con usuario
- Para fase siguiente: confirmar FASE 1 completa

## Archivos que NO Deben Modificarse sin Razón
- `platformio.ini` (config validada)
- `Kinematics.cpp` (IK analítico funcional)
- `STS3215.cpp` (protocolo validado)

## Prompt de Emergencia
Si pierdes contexto, el usuario puede escribir:
```
"retoma" o "contexto maestro"
```
Y debes leer este archivo completo.
