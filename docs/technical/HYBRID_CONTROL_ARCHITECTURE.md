# Arquitectura de Control Híbrido - HexapodIA

## Visión General

Sistema de control **dual-mode** con cambio fluido entre:
1. **Modo Manual:** Control ELRS con RadioMaster Boxer
2. **Modo Autónomo:** IA con Jetson Orin Nano + FPV

## Filosofía de Diseño

> **"Siempre manual-ready. IA cuando funciona."**

El robot SIEMPRE responde al control manual (failsafe).
La IA es un "copiloto" que puede tomar control, pero el piloto humano tiene override.

## Hardware Requerido por Modo

### FASE 3: Control Manual (Base)

```
✅ ESP32-WROOM-32
✅ 18× STS3215 servos
✅ ELRS Receiver (EP2 o similar)
✅ RadioMaster Boxer
✅ BNO085 IMU
✅ Batería 3S 6000mAh
✅ Pantalla OLED 0.96" I2C
```

**Funcionalidad:**
- Walking completo (tripod, ripple)
- Control joystick (velocidad, dirección, rotación)
- Estabilización IMU
- Telemetría en pantalla
- Audio feedback (beeps, alertas)

### FASE 5: Control Autónomo (IA)

```
✅ Todo de FASE 3 +
✅ Jetson Orin Nano
✅ Cámara USB/CSI (FPV + visión)
✅ Speaker I2S (audio avanzado)
✅ Upgrade batería 8000mAh (mayor autonomía)
```

**Funcionalidad adicional:**
- Navegación autónoma
- Obstacle avoidance
- Object tracking
- SLAM mapping
- Voice commands
- Streaming FPV a ground station

## Sistema de Control ELRS

### ¿Por qué ELRS?

| Característica | ELRS | HappyModel EP1 | WiFi RC |
|----------------|------|----------------|---------|
| Latencia | 4-16ms ✅ | ~20ms | ~50-100ms ❌ |
| Rango | 1-30km ✅ | 500m | 100m |
| Telemetría | Bidireccional ✅ | ❌ | ✅ |
| Failsafe | Hardware ✅ | Básico | Software |
| Precio RX | ~€15 | ~€12 | Gratis |

**Veredicto:** ELRS es **MUCHO mejor** para control crítico.

### RadioMaster Boxer

```
Especificaciones:
├── Protocolo: ELRS nativo
├── Canales: 16 disponibles
├── Gimbal: Hall sensor (alta precisión)
├── Pantalla: Color TFT
├── Batería: 18650 integrada
├── Rango: Hasta 30km (módulo apropiado)
└── Precio: ~€150
```

**Perfecto para HexapodIA** ✅

### ELRS Receiver Recomendado

**BetaFPV ELRS Nano RX** (~€15-18):
- 2.4GHz
- Peso: 0.6g
- Tamaño: 11×10mm
- UART output (compatible ESP32)
- Telemetría bidireccional
- Failsafe programable

**Alternativa:** Happymodel EP2 (~€12)

## Asignación de Canales ELRS

### Canales RC (16 disponibles)

| Canal | Función | Tipo | Uso |
|-------|---------|------|-----|
| **CH1** | Roll / Strafe | Analog | Movimiento lateral |
| **CH2** | Pitch / Forward | Analog | Movimiento adelante/atrás |
| **CH3** | Throttle / Speed | Analog | Velocidad general |
| **CH4** | Yaw / Rotation | Analog | Rotación en Z |
| **CH5** | Mode Switch | 3-pos | Manual / Semi-Auto / Full-Auto |
| **CH6** | Gait Select | 3-pos | Tripod / Ripple / Wave |
| **CH7** | Height Adjust | Analog | Altura body |
| **CH8** | Emergency Stop | Button | Kill switch |
| **CH9** | LED Mode | Button | Cambiar LEDs (futuro) |
| **CH10** | Reserved | - | Jetson override |
| CH11-16 | Future expansion | - | - |

### Protocolo UART ESP32 ← ELRS RX

ELRS RX puede usar varios protocolos:
1. **CRSF (CrossFire)** - Recomendado ✅
2. SBUS (inverted)
3. PPM (legacy)

**Usaremos CRSF:** Telemetría bidireccional + baja latencia

### Conexión Hardware

```
ELRS RX (BetaFPV Nano)
├── VCC → ESP32 3.3V
├── GND → Common GND
├── TX  → ESP32 GPIO 25 (RX CRSF)
└── RX  → ESP32 GPIO 26 (TX CRSF - telemetry)
```

**Nuevo pinout actualizado:**

```cpp
// RC ELRS (UART custom)
#define ELRS_RX_PIN    25    // CRSF data from receiver
#define ELRS_TX_PIN    26    // CRSF telemetry to receiver
```

## Arquitectura Switching Manual ↔ Auto

### Estado de Control

```
┌─────────────────────────────────────────────────────┐
│              Control State Machine                  │
│                                                     │
│  MANUAL ←→ SEMI_AUTO ←→ FULL_AUTO                  │
│    ↑           ↑             ↑                      │
│    └───────────┴─────────────┴─→ EMERGENCY_STOP    │
└─────────────────────────────────────────────────────┘

MANUAL:
  - ESP32 lee ELRS directamente
  - Jetson NO controla movimiento
  - IA puede mostrar info en FPV overlay

SEMI_AUTO:
  - Jetson sugiere movimientos
  - ESP32 puede override con RC
  - Útil para obstacle avoidance assist

FULL_AUTO:
  - Jetson controla navegación
  - RC solo para emergency override
  - FPV con IA annotations

EMERGENCY_STOP:
  - Kill switch (CH8)
  - Sit down immediately
  - Solo salida: power cycle o reset
```

### Lógica de Arbitración

```cpp
enum ControlMode {
    MANUAL,      // RC tiene 100% control
    SEMI_AUTO,   // RC + IA blending
    FULL_AUTO    // IA control, RC override
};

ControlMode currentMode = MANUAL;

void updateControlLoop() {
    // Leer RC SIEMPRE (failsafe)
    RCInput rc = readELRS();
    
    // Check emergency stop (máxima prioridad)
    if (rc.ch8 > 1800) {
        emergencyStop();
        return;
    }
    
    // Check mode switch (CH5)
    currentMode = getModeFromRC(rc.ch5);
    
    Vector2 commandVelocity;
    float commandRotation;
    
    switch(currentMode) {
        case MANUAL:
            // RC directo
            commandVelocity.x = mapRC(rc.ch2, -1.0, 1.0);
            commandVelocity.y = mapRC(rc.ch1, -1.0, 1.0);
            commandRotation = mapRC(rc.ch4, -1.0, 1.0);
            break;
            
        case SEMI_AUTO:
            // Blending RC + Jetson
            Vector2 rcVel = getRCVelocity(rc);
            Vector2 aiVel = getJetsonVelocity();
            
            // Si RC activo (deadzone excedido), úsalo
            if (rcVel.magnitude() > 0.1) {
                commandVelocity = rcVel;
            } else {
                // RC inactivo, usar IA
                commandVelocity = aiVel;
            }
            break;
            
        case FULL_AUTO:
            // Jetson control primario
            commandVelocity = getJetsonVelocity();
            commandRotation = getJetsonRotation();
            
            // RC override si > 50% stick
            if (abs(rc.ch2) > 1500 || abs(rc.ch1) > 1500) {
                // Piloto quiere control
                currentMode = MANUAL;  // Downgrade automático
                commandVelocity = getRCVelocity(rc);
            }
            break;
    }
    
    // Enviar comandos a gait engine
    gaitEngine.setVelocity(commandVelocity, commandRotation);
}
```

### Safety Override

**Regla de oro:** RC SIEMPRE puede tomar control

```cpp
// RC Watchdog
unsigned long lastRCUpdate = 0;
#define RC_TIMEOUT 500  // 500ms

void checkRCWatchdog() {
    if (millis() - lastRCUpdate > RC_TIMEOUT) {
        // RC signal lost
        Serial.println("⚠️ RC SIGNAL LOST");
        
        if (currentMode == MANUAL) {
            // Estábamos en manual, RC era crítico
            emergencyStop();
        } else {
            // En auto, continuar con IA pero alerta
            // (depende de caso de uso)
        }
    }
}
```

## Sistema FPV

### Opciones de Cámara

#### Opción 1: ESP32-CAM (Económica)

```
Pros:
  ✅ Bajo costo (~€8)
  ✅ WiFi streaming integrado
  ✅ Ya en ecosystem ESP32
  ✅ Fácil integración

Contras:
  ❌ Baja resolución (2MP max)
  ❌ Frame rate bajo (~15fps)
  ❌ Latencia ~100-200ms
  ❌ WiFi compite con ELRS
```

**Uso recomendado:** Telemetría, no FPV crítico

#### Opción 2: USB Webcam → Jetson (Balanceada)

```
Ejemplos:
  - Logitech C270 (~€25)
  - Razer Kiyo (~€60)
  
Pros:
  ✅ Buena calidad (720p-1080p)
  ✅ Frame rate decente (30fps)
  ✅ Plug & play con Jetson
  ✅ Bajo consumo

Contras:
  ⚠️ Latencia ~50-100ms (encoding)
  ⚠️ Procesado en Jetson (CPU load)
```

**Uso recomendado:** FPV + visión IA

#### Opción 3: CSI Camera → Jetson (Profesional)

```
Ejemplos:
  - Raspberry Pi Camera v2 (~€25)
  - IMX219 sensor (~€20)
  
Pros:
  ✅ Baja latencia (~30ms)
  ✅ Interface directo (CSI)
  ✅ Alta calidad
  ✅ Bajo CPU overhead

Contras:
  ⚠️ Requiere Jetson Orin (tiene CSI)
  ⚠️ Más complejo integrar
```

**Uso recomendado:** FPV profesional + SLAM

#### Opción 4: Analog FPV Camera (Racing Drone)

```
Ejemplos:
  - Caddx Ratel 2 (~€30)
  - RunCam Phoenix 2 (~€40)
  
Pros:
  ✅ LATENCIA MÍNIMA (5-10ms) ✅✅
  ✅ Standard FPV (plug & play goggles)
  ✅ No consume ESP32/Jetson
  ✅ Probado en drones

Contras:
  ❌ Solo FPV, no para IA
  ❌ Requiere VTX + goggles/RX
  ❌ Costo adicional (~€80 total)
```

**Uso recomendado:** FPV ultra-low latency

### Recomendación por Fase

**FASE 3 (Manual):** Analog FPV Camera ✅
- Caddx Ratel 2 (~€30)
- VTX 5.8GHz 25mW (~€15)
- FPV goggles (~€80) o monitor (~€40)
- **Latencia: 5-10ms** - perfecto para pilotaje

**FASE 5 (Autónomo):** Dual Camera ✅
- Analog FPV (pilotaje manual)
- USB/CSI Camera (IA + SLAM)

### Arquitectura Dual Camera

```
                  HexapodIA
                      │
        ┌─────────────┴─────────────┐
        │                           │
   Analog FPV                  Digital Camera
   (Caddx Ratel)              (USB/CSI)
        │                           │
        ↓                           ↓
   VTX 5.8GHz ──→ Goggles     Jetson Orin
   (piloto)                   (IA + recording)
```

**Beneficios:**
- FPV ultra-low latency para pilotaje
- Digital high-quality para IA
- Sistemas independientes (no conflictos)

## Pantalla de Información

### Especificación

**OLED 0.96" I2C** (Recomendado)

```
Características:
├── Tamaño: 0.96" (128×64 px)
├── Interface: I2C
├── Voltaje: 3.3V
├── Consumo: ~20mA
├── Librerías: Adafruit SSD1306
└── Precio: ~€5
```

### Ubicación

Montada en **top del body**, visible desde arriba/atrás.

### Información Mostrada

```
╔══════════════════════════════╗
║  HexapodIA  [MANUAL]    12.2V║
║                               ║
║  Speed:  0.2m/s    ┌─────┐   ║
║  Heading: 045°     │  ^  │   ║
║  Gait: TRIPOD      │ ←o→ │   ║
║                    │  v  │   ║
║  Temp: 45°C        └─────┘   ║
║  Time: 15:32       Compass   ║
╚══════════════════════════════╝
```

**Información dinámica:**
- Modo (MANUAL / SEMI / AUTO)
- Batería (V + %)
- Velocidad actual
- Heading / compass
- Gait activo
- Temperatura max servos
- Runtime
- Iconos status

### Conexión

```
OLED 0.96" I2C
├── VCC → ESP32 3.3V
├── GND → Common GND
├── SCL → ESP32 GPIO 22 (I2C SCL - compartido IMU)
├── SDA → ESP32 GPIO 21 (I2C SDA - compartido IMU)
```

**I2C addresses:**
- IMU BNO085: 0x4A
- OLED SSD1306: 0x3C
- **Sin conflicto** ✅

## Sistema de Audio

### Opción 1: Buzzer Pasivo PWM (Simple)

```
Hardware:
├── Buzzer pasivo 5V
├── Transistor NPN (2N2222)
├── Resistor 1kΩ
└── Conexión: ESP32 GPIO 13

Capacidad:
✅ Beeps, alerts
✅ Melodías simples
✅ Bajo consumo (~50mA)
❌ Sin speech
❌ Calidad baja

Precio: ~€2
```

**Uso:** Alerts, warnings, boot sound

### Opción 2: I2S DAC + Speaker (Avanzado)

```
Hardware:
├── MAX98357A I2S amplifier (~€5)
├── Speaker 3W 4Ω (~€3)
├── Conexión I2S:
│   ├── BCLK → ESP32 GPIO 27
│   ├── LRC  → ESP32 GPIO 14
│   └── DIN  → ESP32 GPIO 12

Capacidad:
✅ Audio alta calidad
✅ Speech/TTS posible
✅ MP3 playback
✅ Sound effects
⚠️ Mayor consumo (~500mA peak)

Precio: ~€8
```

**Uso:** Voice alerts, status messages, sound effects

### Opción 3: Jetson TTS (FASE 5)

```
Software:
├── Piper TTS (local, fast)
├── Festival TTS
└── gTTS (require internet)

Capacidad:
✅ Full speech synthesis
✅ Multiple languages
✅ Natural sounding
⚠️ Requiere Jetson active

Hardware:
└── Mismo speaker I2S via Jetson
```

**Uso:** Full voice commands feedback

### Recomendación Escalonada

**FASE 3:** Buzzer PWM
- Beeps básicos
- Alert sounds
- Bajo costo/complejidad

**FASE 5:** I2S Speaker + TTS
- Voice feedback
- Status messages
- Sound effects

### Sounds Propuestos

```cpp
// Startup
void playBootSound() {
    tone(BUZZER_PIN, 1000, 100);
    delay(150);
    tone(BUZZER_PIN, 1500, 100);
}

// Mode change
void playModeChange() {
    tone(BUZZER_PIN, 2000, 50);
}

// Warning
void playWarning() {
    for (int i = 0; i < 3; i++) {
        tone(BUZZER_PIN, 800, 100);
        delay(150);
    }
}

// Emergency
void playEmergency() {
    while(true) {  // Loop until reset
        tone(BUZZER_PIN, 1500, 200);
        delay(200);
        tone(BUZZER_PIN, 1000, 200);
        delay(200);
    }
}

// Battery low
void playBatteryLow() {
    tone(BUZZER_PIN, 500, 500);
}
```

## Pinout Actualizado Completo

### ESP32-WROOM-32 (FASE 3-5)

```cpp
// ═══════════════════════════════════════
// SERVOS (UART2)
// ═══════════════════════════════════════
#define SERVO_TX_PIN    17
#define SERVO_RX_PIN    16

// ═══════════════════════════════════════
// RC ELRS (CRSF Protocol)
// ═══════════════════════════════════════
#define ELRS_RX_PIN     25   // CRSF data in
#define ELRS_TX_PIN     26   // CRSF telemetry out

// ═══════════════════════════════════════
// IMU (I2C - shared with OLED)
// ═══════════════════════════════════════
#define IMU_SDA         21   // I2C addr 0x4A
#define IMU_SCL         22

// ═══════════════════════════════════════
// OLED Display (I2C - shared with IMU)
// ═══════════════════════════════════════
#define OLED_SDA        21   // I2C addr 0x3C
#define OLED_SCL        22

// ═══════════════════════════════════════
// AUDIO (I2S o PWM)
// ═══════════════════════════════════════
// Opción A: Buzzer PWM
#define BUZZER_PIN      13

// Opción B: I2S Speaker
#define I2S_BCLK        27
#define I2S_LRC         14
#define I2S_DIN         12

// ═══════════════════════════════════════
// MONITORING
// ═══════════════════════════════════════
#define LED_STATUS      2
#define BATTERY_PIN     34   // ADC1
#define EMERGENCY_HW    0    // BOOT button

// ═══════════════════════════════════════
// FPV ANALOG (power control)
// ═══════════════════════════════════════
#define VTX_POWER_PIN   33   // Enable VTX

// ═══════════════════════════════════════
// JETSON COMMUNICATION (FASE 5)
// ═══════════════════════════════════════
// Preferido: WiFi (ROS2/DDS)
// Alternativo: UART0 (GPIO 1/3)
```

### Jetson Orin Nano (FASE 5)

```
Connections:
├── USB Camera (USB 3.0)
├── WiFi (ROS2 ↔ ESP32)
├── CSI Camera (opcional, better latency)
├── I2S Audio out (speaker)
└── Power: 5V 8A (from buck converter)
```

## Consumo Eléctrico Actualizado

### FASE 3 (Manual + FPV)

```
Componente              Corriente    Potencia
─────────────────────────────────────────────
ESP32                   120mA @ 5V   0.6W
IMU BNO085              10mA @ 3.3V  0.03W
OLED Display            20mA @ 3.3V  0.07W
ELRS RX                 50mA @ 3.3V  0.15W
Buzzer (peak)           50mA @ 5V    0.25W
Analog FPV Camera       150mA @ 5V   0.75W
VTX 5.8GHz 25mW         200mA @ 5V   1.0W
Servos (walking)        6A @ 12V     72W
─────────────────────────────────────────────
Total walking:                       ~75W

Autonomía: 6000mAh 11.1V = 66Wh
Runtime: 66Wh / 75W ≈ 52min
```

### FASE 5 (Auto + IA)

```
Componente              Corriente    Potencia
─────────────────────────────────────────────
Todos de FASE 3                      ~75W
Jetson Orin (15W mode)  3A @ 5V      15W
USB Camera              500mA @ 5V   2.5W
I2S Speaker (avg)       300mA @ 5V   1.5W
─────────────────────────────────────────────
Total autonomous:                    ~94W

Autonomía: 8000mAh 11.1V = 89Wh
Runtime: 89Wh / 94W ≈ 56min
```

**Muy razonable** ✅

## Comunicación ESP32 ↔ Jetson

### Protocolo: ROS2 DDS sobre WiFi

```
ESP32 (micro-ROS):
  Publishers:
    - /odom (nav_msgs/Odometry)
    - /imu (sensor_msgs/Imu)
    - /battery (sensor_msgs/BatteryState)
    - /rc_input (geometry_msgs/Twist)
    
  Subscribers:
    - /cmd_vel (geometry_msgs/Twist)
    - /mode_request (std_msgs/String)

Jetson (ROS2 Humble):
  Publishers:
    - /cmd_vel (navegación autónoma)
    - /camera/image (FPV stream)
    - /detections (vision objects)
    
  Subscribers:
    - /odom (para SLAM)
    - /imu (para fusion)
```

### Fallback: Serial UART

Si WiFi falla, comunicación básica por UART0 (GPIO 1/3).

## BOM Actualizado por Fase

### FASE 3: Control Manual + FPV

| Item | Spec | Cant | Precio | Total |
|------|------|------|--------|-------|
| **RC System** |
| RadioMaster Boxer | ELRS | 1 | €150 | €150 |
| ELRS RX | BetaFPV Nano | 1 | €15 | €15 |
| **FPV System** |
| Analog FPV Cam | Caddx Ratel 2 | 1 | €30 | €30 |
| VTX 5.8GHz | 25mW | 1 | €15 | €15 |
| FPV Goggles | EV800D o similar | 1 | €80 | €80 |
| **Info Display** |
| OLED 0.96" | I2C SSD1306 | 1 | €5 | €5 |
| **Audio** |
| Buzzer pasivo | 5V | 1 | €2 | €2 |
| **Subtotal FASE 3 additions** | | | | **€297** |

### FASE 5: Autónomo + IA

| Item | Spec | Cant | Precio | Total |
|------|------|------|--------|-------|
| Jetson Orin Nano | 8GB | 1 | €500 | €500 |
| USB Camera | Logitech C270 | 1 | €25 | €25 |
| I2S Amp | MAX98357A | 1 | €5 | €5 |
| Speaker | 3W 4Ω | 1 | €3 | €3 |
| Battery upgrade | 8000mAh 3S | 1 | €70 | €70 |
| Buck 5V upgrade | 8A | 1 | €18 | €18 |
| **Subtotal FASE 5 additions** | | | | **€621** |

### Inversión Total Acumulada

```
FASE 1: Pierna única           €97
FASE 3: Hexapod + RC + FPV    €297
FASE 5: IA + Jetson           €621
─────────────────────────────────
Total proyecto completo:     €1015

Comparación:
- Kit hobby hexapod: €120 (sin IA, sin RC pro, sin FPV)
- Boston Dynamics Spot: €75,000
- Unitree Go1: €2,700

HexapodIA: €1015 con capacidades similares ✅
```

## Roadmap de Implementación

### FASE 3A: Control Manual Base
```
□ Integrar ELRS receiver
□ Implementar CRSF parser
□ Control joystick → walking
□ Gait selection por RC
□ Emergency stop funcional
```

### FASE 3B: FPV System
```
□ Montar Analog FPV camera
□ Instalar VTX 5.8GHz
□ Setup FPV goggles
□ Test latencia < 20ms
```

### FASE 3C: Info Display + Audio
```
□ Integrar OLED I2C
□ Display status info
□ Buzzer PWM alerts
□ Telemetría ELRS
```

### FASE 4: Sensores + Estabilización
```
□ Integrar IMU BNO085
□ Body leveling auto
□ Terrain adaptation
□ Telemetría avanzada
```

### FASE 5A: Jetson Integration
```
□ Install Jetson Orin Nano
□ ROS2 Humble setup
□ micro-ROS en ESP32
□ WiFi communication
```

### FASE 5B: Visión IA
```
□ USB Camera integration
□ Object detection (YOLO)
□ Obstacle avoidance
□ FPV overlay annotations
```

### FASE 5C: Navegación Autónoma
```
□ SLAM (ORB-SLAM3 o RTAB-Map)
□ Path planning
□ Waypoint navigation
□ Full autonomy
```

### FASE 5D: Audio Avanzado
```
□ I2S Speaker
□ TTS (Piper)
□ Voice commands
□ Sound effects
```

## Testing por Modo

### Test Manual Mode

```
□ RC range test (50m, 100m, 500m)
□ Latency measurement (<20ms)
□ Failsafe behavior (kill RC)
□ Emergency stop (<500ms)
□ FPV latency (<50ms total)
□ All gaits selectable
□ Smooth control response
```

### Test Semi-Auto Mode

```
□ RC override immediate
□ IA suggestions reasonable
□ Blending smooth
□ No fighting between RC/IA
□ Fallback to manual OK
```

### Test Full-Auto Mode

```
□ Obstacle avoidance works
□ Path planning functional
□ RC override instant
□ Autonomous for >5min
□ SLAM mapping accurate
□ Return-to-home works
```

## Safety Features por Modo

### Hardware Failsafes

```
✅ ELRS failsafe (configurable)
✅ Emergency stop button (CH8)
✅ Battery low voltage cutoff
✅ IMU tilt detection (>30°)
✅ Servo thermal shutdown
✅ Watchdog timer ESP32
```

### Software Failsafes

```
✅ RC signal timeout → stop
✅ Jetson comm timeout → manual mode
✅ WiFi loss → continue last command
✅ Camera loss → stop auto mode
✅ IMU failure → reduced speed
✅ Servo feedback mismatch → alert
```

## Próximos Pasos Inmediatos

### Hardware a Adquirir (Prioridad)

**FASE 1 (ya definido):**
- 3× STS3215
- ESP32-WROOM-32
- LiPo 3S 2200mAh
- Bucks

**FASE 3 (nuevo):**
- RadioMaster Boxer (~€150)
- ELRS RX BetaFPV Nano (~€15)
- Caddx Ratel 2 FPV cam (~€30)
- VTX 5.8GHz (~€15)
- FPV goggles EV800D (~€80)
- OLED 0.96" (~€5)
- Buzzer (~€2)

**Total adicional FASE 3:** ~€297

---

¿Te parece bien esta arquitectura? ¿Quieres que profundice en algún aspecto específico (ELRS setup, FPV, audio, etc.)?
