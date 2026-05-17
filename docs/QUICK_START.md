# Quick Start - HexapodIA FASE 1

## ¿Qué es esto?
Robot hexápodo modular 18DOF con smart servos, control tiempo real ESP32, y arquitectura preparada para IA.

**Fase actual:** FASE 1 - Pierna Única (validación)

## Hardware Necesario

### FASE 1 (Mínimo Viable)
- [x] 3× Feetech STS3215 servos
- [x] 1× ESP32 Dev Board
- [x] 1× LiPo 3S 11.1V (2200mAh mínimo)
- [ ] Piezas PETG impresas (coxa, femur, tibia)
- [ ] Tornillos M3 + insertos térmicos
- [ ] Cable servo bus (<30cm)

### Opcional
- [ ] BNO085 IMU (FASE 4)
- [ ] HappyModel EP1 RC (FASE 4)
- [ ] Voltímetro/amperímetro

## Setup Rápido

### 1. Hardware
```
1. Configurar IDs de servos (ver STS3215_SETUP.md)
   - Servo 0: Coxa
   - Servo 1: Femur
   - Servo 2: Tibia

2. Conectar ESP32:
   GPIO17 (TX) → Servo Data
   GPIO16 (RX) → Servo Data
   GND        → Servo GND
   
3. Alimentación:
   LiPo 3S → Servos VCC
   LiPo 3S → Step-down 5V → ESP32 VIN
   
   ⚠️ GND común entre ESP32 y servos
```

### 2. Software
```bash
# Instalar PlatformIO CLI
pip install platformio

# Clonar/navegar al proyecto
cd firmware/esp32_locomotion

# Build
pio run

# Upload
pio run -t upload

# Monitor serial
pio device monitor -b 115200
```

### 3. Primera Prueba
```
1. Abrir serial monitor (115200 baud)
2. Esperar "System ready!"
3. Presionar 'h' → Home position
4. Presionar '1' → Individual joint test
5. Presionar '2' → IK position test
6. Presionar 's' → Servo status
```

## Comandos Disponibles

| Tecla | Función |
|-------|---------|
| `1`   | Test individual articulaciones |
| `2`   | Test IK posiciones estáticas |
| `3`   | Test movimiento circular |
| `s`   | Status servos (temp, volt, load) |
| `h`   | Home position |
| `␣`   | Stop test actual |

## Validaciones FASE 1

### ✅ Software (Completo)
- [x] Librería STS3215
- [x] Cinemática inversa
- [x] Control loop 50Hz
- [x] Tests interactivos

### 🔄 Hardware (Pendiente)
- [ ] Imprimir piezas PETG
- [ ] Ensamblar pierna
- [ ] Validar torque suficiente
- [ ] Validar rango movimiento
- [ ] Validar precisión (±5mm)
- [ ] Validar temperatura (<60°C)

## Troubleshooting

### Servo no responde
```
1. Verificar voltaje 10-12V
2. Verificar GND común
3. Verificar ID correcto (scan con FD Debug Tool)
4. Probar baudrate más bajo (500kbps)
```

### IK "unreachable"
```
1. Verificar target dentro workspace (55-265mm)
2. Ajustar dimensiones en Kinematics.h si necesario
3. Validar con FK (forward kinematics)
```

### Movimiento errático
```
1. Verificar alimentación estable (voltaje, capacitor)
2. Reducir velocidad movimiento
3. Revisar temperatura servos
4. Verificar cable bus corto y sin ruido
```

## Estructura del Código

```
main.cpp
├── setup()
│   ├── Inicializar serial
│   ├── Inicializar servos (STS3215)
│   ├── Test conexión (ping)
│   ├── Configurar límites ángulo
│   └── Mover a home position
│
└── loop()
    ├── Leer comandos serial
    ├── Ejecutar tests
    └── Control loop @ 50Hz
```

## Métricas Objetivo

| Métrica | Target | Validar |
|---------|--------|---------|
| Precisión IK | ±5mm | Test 2 |
| Temp servos | <60°C | Status 's' |
| Control loop | 50Hz (20ms) | Monitor |
| Consumo idle | <1A | Amperímetro |
| Consumo moving | <2A | Amperímetro |

## Próximos Pasos

### Al Completar FASE 1
1. Documentar resultados en `FASE_1_RESULTS.md`
2. Fotos/videos de movimiento
3. Métricas medidas vs objetivo
4. Issues encontrados + soluciones
5. Aprobación para avanzar FASE 2

### FASE 2 Preview
- Diseñar body central (estructura)
- Integrar 2-3 piernas
- Sincronización multi-pierna
- Gait básico (alternado)

## Archivos Importantes

| Archivo | Descripción |
|---------|-------------|
| [README.md](../README.md) | Overview del proyecto |
| [SPECIFICATIONS.md](technical/SPECIFICATIONS.md) | Todas las specs técnicas |
| [STS3215_SETUP.md](../hardware/servos_config/STS3215_SETUP.md) | Configuración servos |
| [FASE_1_OBJECTIVES.md](phase_reports/FASE_1_OBJECTIVES.md) | Objetivos fase actual |
| [CLAUDE.md](../CLAUDE.md) | Guía para Claude Code |

## Soporte

### Recursos
- Datasheet STS3215: [Feetech](http://www.feetechrc.com/public/upload/files/20220114/STS3215%20Servo%20User%20Manual.pdf)
- ESP32 Reference: [Espressif](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/)
- RookiDroid Reference: [GitHub](https://github.com/michaelkoch/rookidroid_hexapod)

### Debug Tips
```cpp
// Enable verbose logging
Serial.setDebugOutput(true);

// Print IK details
JointAngles angles = kinematics.inverseKinematics(target);
printJointAngles(angles);

// Check servo response time
unsigned long t1 = millis();
servo.setPosition(0, pos);
while(servo.isMoving(0));
Serial.printf("Move time: %lums\n", millis() - t1);
```

## Filosofía del Proyecto

> **"Locomoción primero, IA después"**

Este proyecto prioriza:
1. ✅ Movimiento fluido y estable
2. ✅ Arquitectura modular escalable
3. ✅ Smart servos (evitar rehacer locomoción)
4. ✅ Diseño mantenible
5. 🔄 IA avanzada (FASE 5)

No es un kit hobby. Es una plataforma robótica profesional evolutiva.
