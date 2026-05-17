# Arquitectura del Sistema HexapodIA

## Visión General

```
┌─────────────────────────────────────────────────────────┐
│                    FASE 5 (Futuro)                      │
│  ┌───────────────────────────────────────────────────┐  │
│  │         Jetson Orin Nano (IA/ROS2)                │  │
│  │  • ROS2 Humble                                     │  │
│  │  • Computer Vision (YOLO, Depth)                   │  │
│  │  • SLAM (ORB-SLAM3)                                │  │
│  │  • Navigation Stack                                │  │
│  │  • Voice Commands                                  │  │
│  │  • High-level Planning                             │  │
│  └───────────────┬───────────────────────────────────┘  │
│                  │ WiFi/DDS/UART                        │
└──────────────────┼──────────────────────────────────────┘
                   │
┌──────────────────┼──────────────────────────────────────┐
│                  ▼        FASE 1-4                      │
│  ┌───────────────────────────────────────────────────┐  │
│  │              ESP32 (Control Real-Time)            │  │
│  │  • Gait Generation                                 │  │
│  │  • Inverse Kinematics                              │  │
│  │  • Servo Synchronization                           │  │
│  │  • IMU Stabilization                               │  │
│  │  • Emergency Stop                                  │  │
│  │  • Telemetry                                       │  │
│  └───────────────┬───────────────────────────────────┘  │
│                  │ Serial Bus 1Mbps                     │
│                  │                                      │
│  ┌───────────────┴───────────────────────────────────┐  │
│  │          STS3215 Smart Servos x18                 │  │
│  │                                                    │  │
│  │  [0 1 2] [3 4 5] [6 7 8] [9 10 11] [12 13 14] [15 16 17] │
│  │   RF      RM      RR       LF        LM        LR   │  │
│  └────────────────────────────────────────────────────┘  │
└─────────────────────────────────────────────────────────┘
```

## Separación de Responsabilidades

### ESP32 - Control de Locomoción (Tiempo Real)
**Prioridad:** Máxima  
**Frecuencia:** 50Hz (control loop)

**Responsabilidades:**
- ✅ Cinemática inversa (IK)
- ✅ Generación de gaits (tripod, ripple, wave)
- ✅ Sincronización multi-servo
- ✅ Estabilización con IMU
- ✅ Control de balance
- ✅ Emergency stop
- ✅ Telemetría básica
- ✅ Control remoto directo (RC)

**NO responsable de:**
- ❌ Visión computacional
- ❌ Planificación de rutas
- ❌ SLAM
- ❌ Procesamiento IA pesado
- ❌ Navegación autónoma compleja

### Jetson Orin Nano - IA y Alto Nivel (Futuro - FASE 5)
**Prioridad:** Baja (puede fallar sin detener locomoción)  
**Frecuencia:** Variable (1-30Hz según tarea)

**Responsabilidades:**
- ✅ Visión computacional
- ✅ Detección de objetos
- ✅ Tracking de personas
- ✅ SLAM y mapeo
- ✅ Planificación de rutas
- ✅ Navegación autónoma
- ✅ Comandos de voz
- ✅ ROS2 nodes
- ✅ Decisiones estratégicas

**NO responsable de:**
- ❌ Control directo de servos
- ❌ Timing crítico
- ❌ Safety systems
- ❌ Gaits básicos

## Protocolo de Comunicación ESP32 ↔ Jetson

### Modo 1: WiFi/DDS (ROS2)
**Para:** Operación normal autónoma

```
Jetson → ESP32:
  /cmd_vel (geometry_msgs/Twist)
  /cmd_pose (geometry_msgs/Pose)
  /gait_mode (std_msgs/String)

ESP32 → Jetson:
  /odom (nav_msgs/Odometry)
  /imu (sensor_msgs/Imu)
  /joint_states (sensor_msgs/JointState)
  /diagnostics (diagnostic_msgs/DiagnosticArray)
```

### Modo 2: UART Serial
**Para:** Debugging, setup, fallback

```
Formato: JSON
Baudrate: 115200

Jetson → ESP32:
  {"cmd": "move", "x": 0.2, "y": 0.0, "rot": 0.0}
  {"cmd": "gait", "type": "tripod"}
  {"cmd": "height", "z": 100}

ESP32 → Jetson:
  {"status": "ok", "x": 1.2, "y": 0.5, "heading": 45}
  {"imu": {"roll": 2.1, "pitch": -1.3, "yaw": 45.2}}
  {"error": "servo_timeout", "id": 5}
```

## Flujo de Control

### Modo Autónomo (FASE 5)
```
1. Jetson (vision) → detecta obstáculo
2. Jetson (planning) → calcula nueva ruta
3. Jetson (ROS2) → publica /cmd_vel
4. ESP32 (subscriber) → recibe velocity command
5. ESP32 (gait) → genera secuencia pasos
6. ESP32 (IK) → calcula ángulos articulares
7. ESP32 (serial) → envía comandos a servos
8. STS3215 → ejecutan movimiento
9. ESP32 (feedback) → lee posiciones reales
10. ESP32 (ROS2) → publica /odom, /joint_states
11. Jetson (SLAM) → actualiza mapa
```

### Modo Híbrido (FASE 4)
```
1. RC (piloto) → input joystick
2. ESP32 → lee PWM receiver
3. Jetson (opcional) → ajusta comando (avoid obstacles)
4. ESP32 → fusiona inputs (RC + Jetson)
5. ESP32 → genera gait
6. ... (igual que modo autónomo desde paso 6)
```

### Modo Manual (FASE 1-3)
```
1. RC (piloto) → input joystick
2. ESP32 → lee PWM receiver
3. ESP32 → genera gait directamente
4. ESP32 (IK) → calcula ángulos
5. STS3215 → ejecutan movimiento
```

## Arquitectura de Software ESP32

### Módulos Principales

```cpp
main.cpp
├── ServoController      // Abstracción STS3215
├── Kinematics          // IK/FK individual leg
├── GaitGenerator       // Tripod, ripple, wave
├── BodyController      // Balance, inclinación
├── IMUManager          // BNO085 fusion
├── RCReceiver          // HappyModel EP1
├── TelemetryManager    // Serial output
└── ROSBridge           // WiFi/DDS (FASE 5)
```

### Jerarquía de Clases

```
Robot
├── Body (posición, orientación)
│   └── BodyController
├── Leg[6] (RF, RM, RR, LF, LM, LR)
│   ├── Kinematics
│   └── Servo[3] (coxa, femur, tibia)
├── GaitEngine
│   ├── TripodGait
│   ├── RippleGait
│   └── WaveGait
├── Sensors
│   ├── IMU (BNO085)
│   └── RC (EP1)
└── Communication
    ├── SerialTelemetry
    └── ROSBridge
```

## Estado y Máquina de Estados

### Estados del Robot
```
BOOT → INIT → STANDBY → WALKING → STANDBY
         ↓        ↓         ↓         ↑
       ERROR ← ─ ─ ─ ─ ─ ─ ─ ┘
         ↓
    EMERGENCY_STOP
```

**BOOT:** Initializing hardware  
**INIT:** Homing servos, calibrating sensors  
**STANDBY:** Ready, maintaining posture  
**WALKING:** Executing gait  
**ERROR:** Recoverable error (retry)  
**EMERGENCY_STOP:** Critical failure (manual reset)

### Transiciones
```cpp
BOOT → INIT: 
  - Hardware initialized OK
  
INIT → STANDBY:
  - All servos responding
  - IMU calibrated
  - Home position reached
  
STANDBY → WALKING:
  - Valid movement command received
  
WALKING → STANDBY:
  - Movement command stopped
  - Gait cycle completed
  
ANY → ERROR:
  - Servo timeout
  - IMU error
  - Battery low warning
  
ANY → EMERGENCY_STOP:
  - Physical button pressed
  - Battery critical
  - Servo overheat
  - IMU tilt > 30°
```

## Threading Model ESP32

### Task Priorities (FreeRTOS)
```
Priority 3 (Highest):  Servo Communication (50Hz)
Priority 2:            IMU Reading (100Hz)
Priority 1:            Gait Generation (50Hz)
Priority 0 (Default):  Telemetry, ROS Bridge (10Hz)
```

### Core Assignment
```
Core 0 (Protocol CPU):
  - WiFi/BT stack
  - ROS Bridge
  - Telemetry

Core 1 (Application CPU):
  - Servo control
  - IK calculations
  - Gait generation
  - IMU fusion
```

## Memory Layout

### Flash (4MB)
```
0x000000  Bootloader (64KB)
0x010000  Partition Table
0x020000  App (1.5MB)
0x200000  OTA Update (1.5MB)
0x380000  NVS (24KB)
0x390000  SPIFFS (512KB) - config, calibration
```

### RAM (520KB)
```
Heap:       ~200KB (dynamic allocation)
Stack:      ~40KB (tasks)
Static:     ~80KB (globals)
DMA:        ~20KB (servo buffers)
Reserved:   ~180KB (system)
```

## Scalability

### FASE 1: Single Leg
- 3 servos
- Simple IK
- ~10KB RAM usage

### FASE 3: Full Hexapod
- 18 servos
- Multi-leg coordination
- ~60KB RAM usage

### FASE 5: With Jetson
- 18 servos + sensors
- ROS2 bridge
- ~120KB RAM usage
- **Still within ESP32 limits**

## Power Budget

### FASE 1 (Single Leg)
```
ESP32:          ~200mA @ 5V  = 1W
Servos idle:    ~300mA @ 12V = 3.6W
Servos moving:  ~1.5A @ 12V  = 18W
Total:                        ~23W
```

### FASE 3 (Full Hexapod)
```
ESP32:          ~200mA @ 5V   = 1W
IMU:            ~10mA @ 3.3V  = 0.03W
Servos idle:    ~1.8A @ 12V   = 21.6W
Servos walking: ~6A @ 12V     = 72W
Peak (all):     ~12A @ 12V    = 144W
Total average:                ~95W
```

### FASE 5 (With Jetson)
```
Jetson Orin:    ~15W (10W mode)
ESP32+Servos:   ~95W
Cameras:        ~5W
Total:          ~115W

Battery: 3S 11.1V 3000mAh
Runtime: ~20Wh / 115W ≈ 10min full load
         ~20Wh / 50W  ≈ 24min idle/slow
```

## Fail-Safe Mechanisms

### Hardware
- Physical emergency stop button
- Voltage monitor (brownout)
- Thermal sensors per servo
- IMU tilt detection
- Watchdog timer

### Software
- Servo timeout detection (50ms)
- Communication retry logic
- Graceful degradation (ROS fails → RC mode)
- Position limit enforcement
- Current limiting

### Recovery Actions
```
Servo timeout → disable torque, alert
Battery low   → reduce speed, return home
Overheat      → cool-down pause
Tilt excess   → emergency sit
RC signal lost→ gentle stop
```

## Development Roadmap

### FASE 1-2 (Current)
- [x] ESP32 firmware base
- [x] STS3215 library
- [x] Single leg IK
- [ ] Multi-leg sync
- [ ] Basic gait (tripod)

### FASE 3-4
- [ ] IMU integration
- [ ] Body controller
- [ ] RC receiver
- [ ] Advanced gaits
- [ ] Telemetry dashboard

### FASE 5
- [ ] ROS2 bridge
- [ ] Jetson integration
- [ ] Vision pipeline
- [ ] SLAM
- [ ] Autonomous navigation

## Referencias
- ROS2 Control: <https://control.ros.org/>
- ESP32 Technical Reference: <https://www.espressif.com/sites/default/files/documentation/esp32_technical_reference_manual_en.pdf>
- Hexapod Gaits Theory: Lokki et al. (2003)
