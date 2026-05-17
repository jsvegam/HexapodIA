# Configuración STS3215 - Setup Guide

## Especificaciones STS3215

### Características Principales
- **Torque:** 20kg·cm @ 12V
- **Velocidad:** 0.11s/60° @ 12V  
- **Protocolo:** Serial half-duplex TTL
- **Resolución:** 4096 posiciones (12-bit)
- **Feedback:** Posición, velocidad, carga, temperatura, voltaje
- **Rango:** 0-4095 (0-240°)

### Especificaciones Eléctricas
- **Voltaje operación:** 10-12V (nominal 11.1V)
- **Consumo idle:** ~100mA
- **Consumo carga:** 500-800mA
- **Peak:** ~1.5A (stall)
- **Lógica:** TTL 3.3V/5V compatible

### Dimensiones
- **Tamaño:** 40.5 × 20.5 × 38mm
- **Peso:** ~62g
- **Montaje:** Standard servo horn (25T spline)

## Wiring Setup

### Conexión Serial Bus

```
ESP32          STS3215 Bus
GPIO17 (TX) ──→ Data (yellow)
GPIO16 (RX) ──→ Data (yellow)
GND        ──→ GND (black)
```

### Alimentación Separada

```
11.1V LiPo 3S
    │
    ├─→ Servos VCC (red wire)
    │
    └─→ Step-down 5V → ESP32 VIN
```

**⚠️ IMPORTANTE:**
- GND común entre ESP32 y servos
- Alimentación servos NO desde ESP32
- Capacitor 1000µF cerca de bus servos
- Cable bus < 30cm, twisted pair recomendado

### Daisy Chain Configuration

```
         ESP32 Serial
              │
         ┌────┴────┐
         │ Servo 0 │ (Coxa)
         └────┬────┘
         ┌────┴────┐
         │ Servo 1 │ (Femur)
         └────┬────┘
         ┌────┴────┐
         │ Servo 2 │ (Tibia)
         └─────────┘
```

## Configuración Inicial

### 1. Configurar ID de Servos

**Con FD Debug Tool (recomendado):**
- Conectar servo individualmente
- Escanear ID actual
- Cambiar a ID deseado
- Verificar nuevo ID

**Con código ESP32:**
```cpp
// Cambiar ID 1 a ID 0
servo.setID(1, 0);
delay(100);
servo.ping(0); // Verificar
```

### 2. Configurar Baudrate

**Recomendado: 1Mbps**
```cpp
servo.setBaudRate(0, 1000000);
```

**Opciones disponibles:**
- 1000000 (recomendado)
- 500000
- 250000
- 115200

### 3. Configurar Límites de Ángulo

**Coxa (yaw):**
```cpp
uint16_t minPos = servo.angleToPulse(-90, -90, 90);
uint16_t maxPos = servo.angleToPulse(90, -90, 90);
servo.setAngleLimits(SERVO_COXA, minPos, maxPos);
```

**Femur/Tibia (pitch):**
```cpp
uint16_t minPos = servo.angleToPulse(0, 0, 180);
uint16_t maxPos = servo.angleToPulse(180, 0, 180);
servo.setAngleLimits(SERVO_FEMUR, minPos, maxPos);
```

### 4. Configurar Torque Máximo

```cpp
// 1023 = 100% torque
servo.setMaxTorque(id, 1023); // Full torque
servo.setMaxTorque(id, 512);  // 50% torque (testing)
```

## ID Assignment Scheme

### Pierna Única (FASE 1)
```
Articulación | ID | Notas
-------------|----|---------
Coxa         | 0  | Yaw
Femur        | 1  | Pitch
Tibia        | 2  | Pitch
```

### Hexapod Completo (FASE 3)
```
Pierna | Coxa | Femur | Tibia | Posición
-------|------|-------|-------|----------
  RF   |  0   |   1   |   2   | Right Front
  RM   |  3   |   4   |   5   | Right Middle
  RR   |  6   |   7   |   8   | Right Rear
  LF   |  9   |  10   |  11   | Left Front
  LM   |  12  |  13   |  14   | Left Middle
  LR   |  15  |  16   |  17   | Left Rear
```

## Control Table Reference

### Lectura Común
| Address | Función | Rango | Unidad |
|---------|---------|-------|--------|
| 0x38-39 | Present Position | 0-4095 | pulsos |
| 0x3A-3B | Present Speed | 0-4095 | - |
| 0x3C-3D | Present Load | 0-1023 | - |
| 0x3E | Present Voltage | 0-250 | 0.1V |
| 0x3F | Present Temperature | 0-100 | °C |
| 0x42 | Moving | 0/1 | bool |

### Escritura Común
| Address | Función | Rango | Notas |
|---------|---------|-------|-------|
| 0x28 | Torque Enable | 0/1 | ON/OFF |
| 0x29 | LED | 0/1 | ON/OFF |
| 0x2A-2B | Goal Position | 0-4095 | target |
| 0x2C-2D | Goal Time | 0-65535 | ms |
| 0x2E-2F | Goal Speed | 0-4095 | velocidad |

## Testing Procedure

### 1. Test Individual Servo
```cpp
// Ping test
if (servo.ping(0)) {
    Serial.println("Servo 0 OK");
}

// LED blink test
servo.setLED(0, true);
delay(500);
servo.setLED(0, false);

// Position test
servo.setTorqueEnable(0, true);
servo.setPosition(0, 2048, 1000); // Center, 1s
delay(1500);
```

### 2. Test Range of Motion
```cpp
// Min position
servo.setPosition(0, 0, 1000);
delay(1500);

// Max position
servo.setPosition(0, 4095, 1000);
delay(1500);

// Center
servo.setPosition(0, 2048, 1000);
```

### 3. Test Load Feedback
```cpp
int16_t load = servo.readLoad(0);
Serial.printf("Load: %d\n", load);

// Load > 500 = alta carga
// Load < 100 = sin carga
```

### 4. Test Temperature
```cpp
uint8_t temp = servo.readTemperature(0);
Serial.printf("Temp: %d°C\n", temp);

// Temp < 50°C = OK
// Temp 50-60°C = Warning
// Temp > 60°C = STOP
```

## Troubleshooting

### Servo No Responde
1. Verificar conexión física (VCC, GND, Data)
2. Verificar voltaje alimentación (10-12V)
3. Verificar baudrate coincide
4. Probar con otro ID (scan 0-253)
5. Revisar cable data (continuidad)

### Comunicación Inestable
1. Acortar cable data
2. Agregar pull-up 10kΩ en data line
3. Reducir baudrate a 500kbps
4. Verificar GND común
5. Alejar de fuentes ruido (motores)

### Movimiento Errático
1. Verificar alimentación estable (voltaje)
2. Agregar capacitor 1000µF
3. Reducir velocidad movimiento
4. Verificar torque no en límite
5. Revisar temperatura < 60°C

### Sobrecalentamiento
1. Reducir max torque
2. Reducir velocidad
3. Verificar no hay obstrucciones mecánicas
4. Agregar disipador térmico
5. Reducir duty cycle (pausas)

## Mantenimiento

### Inspección Regular
- [ ] Verificar temperatura después de uso
- [ ] Revisar conectores (corrosión, tensión)
- [ ] Verificar montaje mecánico (tornillos)
- [ ] Limpiar engranajes (cada 3 meses)
- [ ] Lubricar (grasa silicona, cada 6 meses)

### Calibración Periódica
- [ ] Verificar center position (2048)
- [ ] Verificar límites ángulo
- [ ] Test range of motion completo
- [ ] Medir backlash (< 1°)

## Safety Limits

### Eléctricos
- Voltaje: 10-12V (no exceder 12.5V)
- Corriente: < 1.5A continuo por servo
- Temperatura: < 60°C operación

### Mecánicos
- Torque: < 20kg·cm
- Velocidad: no usar speed=0 (full speed) con carga
- Duty cycle: pausas cada 5min operación continua

### Software
- Timeout lectura: 50ms
- Reintentos: max 3
- Watchdog: resetear si no responde 5s
- Emergency stop: button físico
