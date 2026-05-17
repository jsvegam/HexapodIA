# ESP32-WROOM-32 Pinout - HexapodIA

## Módulo Específico
**ESP32-WROOM-32 DevKit v1** (también compatible con NodeMCU-32S)

## Asignación de Pines por Fase

### FASE 1-3: Locomoción Base

| Pin GPIO | Función | Conexión | Notas |
|----------|---------|----------|-------|
| **17** | TX2 | STS3215 Data | Serial bus servos |
| **16** | RX2 | STS3215 Data | Serial bus servos |
| **2** | LED Status | Onboard LED | Indicador estado |
| **0** | Emergency Stop | BOOT button | Pullup interno |
| **34** | Battery Monitor | Voltage divider | Input only, ADC |

**Total pines usados:** 5

### FASE 4: Sensores + RC

| Pin GPIO | Función | Conexión | Notas |
|----------|---------|----------|-------|
| **21** | SDA | IMU BNO085 | I2C por defecto |
| **22** | SCL | IMU BNO085 | I2C por defecto |
| **25** | RC CH1 | HappyModel EP1 | PWM input (roll) |
| **26** | RC CH2 | HappyModel EP1 | PWM input (pitch) |
| **27** | RC CH3 | HappyModel EP1 | PWM input (throttle) |
| **14** | RC CH4 | HappyModel EP1 | PWM input (yaw) |

**Total pines adicionales:** 6
**Total acumulado:** 11

### FASE 5: Comunicación Jetson

| Pin GPIO | Función | Conexión | Notas |
|----------|---------|----------|-------|
| WiFi | DDS/ROS2 | Jetson Orin Nano | Preferido (inalámbrico) |
| **1** | TX0 | Jetson UART (alt) | Si necesario |
| **3** | RX0 | Jetson UART (alt) | Si necesario |

**Total pines adicionales:** 0-2 (WiFi preferido)
**Total acumulado:** 11-13

## Diagrama de Conexiones

### FASE 1: Configuración Mínima

```
┌─────────────────────────────────────────┐
│         ESP32-WROOM-32 DevKit           │
│                                         │
│  [GND]────────┬─────────────── GND      │
│               │                         │
│  [3V3]        │                         │
│               │                         │
│  [GPIO 34]────┼───[R1:100K]───┬─ Bat+  │
│               │                │        │
│  [GPIO 2]─────┼────────────────┼─ LED   │
│               │                │        │
│  [GPIO 0]─────┼────────────────┼─ BTN   │
│               │           [R2:33K]      │
│  [GPIO 16]────┼────────────────┴─ GND   │
│  [GPIO 17]────┼─────┬                   │
│               │     │                   │
│  [5V]─────────┼─────┼────────── 5V      │
└───────────────┼─────┼───────────────────┘
                │     │
                │     └──→ STS3215 Data Line
                │         (All 3 servos daisy-chained)
                │
                └──→ Common GND
```

### FASE 4: Con Sensores

```
┌─────────────────────────────────────────┐
│         ESP32-WROOM-32 DevKit           │
│                                         │
│  [GPIO 21]─────────────────── SDA       │───→ BNO085
│  [GPIO 22]─────────────────── SCL       │───→ BNO085
│                                         │
│  [GPIO 25]─────────────────── RC CH1    │───→ EP1
│  [GPIO 26]─────────────────── RC CH2    │───→ EP1
│  [GPIO 27]─────────────────── RC CH3    │───→ EP1
│  [GPIO 14]─────────────────── RC CH4    │───→ EP1
│                                         │
│  [3V3]─────────────────────── VCC       │───→ BNO085
│  [GND]─────────────────────── GND       │───→ BNO085, EP1
└─────────────────────────────────────────┘
```

## Voltage Divider para Battery Monitor

```
LiPo 3S (12.6V max)
       │
       ├──[R1: 100kΩ]──┬── GPIO34 (ADC)
       │               │
       └──[R2: 33kΩ]───┴── GND

Cálculo:
V_adc = V_bat × (R2 / (R1 + R2))
V_adc = 12.6V × (33k / 133k) = 3.12V ✅ (dentro 3.3V max)

Código:
float vADC = (analogRead(34) / 4095.0) * 3.3;
float vBat = vADC * (100 + 33) / 33;
```

## Serial Ports ESP32

ESP32 tiene 3 UART hardware:

| UART | Default Pins | Uso HexapodIA |
|------|--------------|---------------|
| UART0 | TX=1, RX=3 | USB/Debug (reservado) |
| UART1 | TX=10, RX=9 | Flash interna (NO usar) |
| **UART2** | **TX=17, RX=16** | **STS3215 Servos** ✅ |

**Importante:** Usamos UART2 (Serial2) para servos, deja UART0 libre para debugging.

## Pines a Evitar

### ⛔ NO USAR (Conectados a Flash)
- GPIO 6, 7, 8, 9, 10, 11

### ⚠️ Usar con Precaución
- **GPIO 12:** Strapping pin (boot voltage)
  - Debe estar LOW al boot para flash 3.3V
  - OK usar después de boot como I/O normal

- **GPIO 15:** Strapping pin (silent boot)
  - Debe estar HIGH al boot para silent boot
  - OK usar después como I/O normal

### ℹ️ Input Only (Sin Output, Sin Pullup Interno)
- **GPIO 34, 35, 36, 39:** ADC1 channels
  - Usar solo para analog input
  - Perfecto para battery monitor (GPIO 34)

### ✅ Strapping Pins que SÍ Usamos (Sin Problema)
- **GPIO 0:** BOOT button
  - LOW al boot = bootloader mode
  - Pullup interno presente
  - Perfecto para emergency stop

- **GPIO 2:** Onboard LED
  - Strapping pin pero safe para LED
  - Ya usado en devkit

## Configuración en Código

### Definiciones (ya en código)

```cpp
// firmware/esp32_locomotion/include/pins.h

#ifndef PINS_H
#define PINS_H

// ═══════════════════════════════════════
// SERVOS (UART2)
// ═══════════════════════════════════════
#define SERVO_SERIAL    Serial2
#define SERVO_TX_PIN    17
#define SERVO_RX_PIN    16
#define SERVO_BAUDRATE  1000000

// ═══════════════════════════════════════
// IMU (I2C)
// ═══════════════════════════════════════
#define IMU_SDA         21
#define IMU_SCL         22
#define IMU_I2C_ADDR    0x4A

// ═══════════════════════════════════════
// RC RECEIVER (PWM Input)
// ═══════════════════════════════════════
#define RC_PIN_CH1      25  // Roll
#define RC_PIN_CH2      26  // Pitch
#define RC_PIN_CH3      27  // Throttle
#define RC_PIN_CH4      14  // Yaw

// ═══════════════════════════════════════
// MONITORING
// ═══════════════════════════════════════
#define LED_STATUS      2
#define BATTERY_PIN     34
#define EMERGENCY_STOP  0

// Voltage divider calibration
#define VDIV_R1         100000  // 100kΩ
#define VDIV_R2         33000   // 33kΩ

#endif
```

## Características Especiales ESP32-WROOM-32

### WiFi/Bluetooth
- **2.4GHz WiFi b/g/n**
  - Usado FASE 5 para ROS2/DDS
  - Telemetría remota
  - OTA updates

- **Bluetooth Classic + BLE**
  - Opcional para debugging remoto
  - No prioritario para HexapodIA

### ADC (Analog to Digital)
- **2× 12-bit SAR ADCs**
  - ADC1: GPIO 32-39 (8 channels)
  - ADC2: GPIO 0, 2, 4, 12-15, 25-27 (10 channels)
  
  **Nota:** ADC2 no funciona cuando WiFi activo
  **Por eso usamos GPIO 34 (ADC1) para batería** ✅

### DAC (Digital to Analog)
- **2× 8-bit DACs**
  - GPIO 25, GPIO 26
  - No usados en HexapodIA

### Touch Sensor
- **10× capacitive touch GPIOs**
  - No usados en HexapodIA

### PWM
- **16× canales PWM**
  - Usados para leer RC (input)
  - NO para servos (tenemos smart servos serial)

## Consumo Eléctrico ESP32

```
Modo                  Corriente    Notas
─────────────────────────────────────────────────────
Active (WiFi on)      160-260mA    Máximo consumo
Active (WiFi off)     80-120mA     Locomoción normal
Modem-sleep           20-40mA      WiFi periódico
Light-sleep           0.8mA        No usado
Deep-sleep            10µA         No usado (debe caminar)
```

**Consumo HexapodIA típico:** ~120mA @ 5V = 0.6W ✅

## Referencias

- [ESP32 Datasheet](https://www.espressif.com/sites/default/files/documentation/esp32_datasheet_en.pdf)
- [ESP32-WROOM-32 Datasheet](https://www.espressif.com/sites/default/files/documentation/esp32-wroom-32_datasheet_en.pdf)
- [ESP32 Technical Reference](https://www.espressif.com/sites/default/files/documentation/esp32_technical_reference_manual_en.pdf)

## Notas Importantes

### ✅ Ventajas ESP32-WROOM-32 para HexapodIA
1. 3 UART hardware (uno libre para servos)
2. Dual-core (uno para locomoción, otro para WiFi)
3. 520KB RAM (más que suficiente)
4. WiFi integrado (FASE 5 Jetson communication)
5. ADC para battery monitor
6. Bajo costo (~€8)
7. Ecosystem maduro Arduino/PlatformIO

### ⚠️ Limitaciones (y cómo las manejamos)
1. ADC2 no funciona con WiFi
   → **Solución:** Usamos GPIO 34 (ADC1) ✅
   
2. Solo 34 GPIO disponibles
   → **Solución:** Solo necesitamos 11-13 ✅
   
3. Algunos pines son strapping
   → **Solución:** Evitados o usados correctamente ✅
   
4. 3.3V logic (servos son TTL)
   → **Solución:** STS3215 son 3.3V/5V compatible ✅

## Checklist Pre-Conexión

Antes de conectar ESP32-WROOM-32:

- [ ] Verificar voltaje alimentación: 5V (NO 12V directo)
- [ ] Voltage divider calculado para batería (R1=100K, R2=33K)
- [ ] Servos conectados a GPIO 16/17 (UART2)
- [ ] GND común ESP32 + Servos + Buck converters
- [ ] NO conectar GPIO 6-11 (flash pins)
- [ ] Capacitor 100nF cerca de EN pin (estabilidad)

## Troubleshooting Común

### "ESP32 no responde después de conectar servos"
→ Probable: GPIO 12 tiene pullup, interferencia boot
→ Solución: No usar GPIO 12, nuestro pinout ya evita

### "ADC lee valores erráticos"
→ Probable: WiFi activo en ADC2
→ Solución: Usamos GPIO 34 (ADC1), independiente de WiFi

### "Serial2 no funciona"
→ Probable: GPIO 16/17 mal asignados
→ Solución: Verificar definición SERVO_TX_PIN=17, RX=16

### "ESP32 reinicia aleatorio"
→ Probable: Caída voltaje por spike servos
→ Solución: Capacitor 3300µF en línea servos
