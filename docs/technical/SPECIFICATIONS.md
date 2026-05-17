# Especificaciones Técnicas - HexapodIA

## Configuración Mecánica

### Dimensiones Base (RookiDroid-inspired)
```
Coxa length:  55mm
Femur length: 80mm
Tibia length: 130mm
Body width:   180mm
Body length:  160mm
```

### Configuración por Pierna (3DOF)

| Articulación | Servo ID | Movimiento | Rango    |
|--------------|----------|------------|----------|
| Coxa         | x0       | Yaw        | ±90°     |
| Femur        | x1       | Pitch      | 0-180°   |
| Tibia        | x2       | Pitch      | 0-180°   |

### Layout Completo (18 servos)

```
Pierna | Coxa | Femur | Tibia
-------|------|-------|-------
  RF   |  0   |   1   |   2
  RM   |  3   |   4   |   5
  RR   |  6   |   7   |   8
  LF   |  9   |  10   |  11
  LM   |  12  |  13   |  14
  LR   |  15  |  16   |  17
```

## Servos STS3215

### Especificaciones
- **Torque:** 20kg·cm @ 12V
- **Velocidad:** 0.11s/60° @ 12V
- **Protocolo:** Serial half-duplex
- **Baudrate:** 1Mbps (recomendado)
- **Voltage:** 10-12V (nominal 11.1V LiPo 3S)
- **Feedback:** Posición, velocidad, carga, temperatura, voltaje

### Rangos de Operación
- **Posición:** 0-4095 (12-bit)
- **Velocidad:** 0-4095
- **Corriente:** Lectura en tiempo real
- **Temperatura:** Lectura en tiempo real

## Sistema Eléctrico

### Alimentación
- **Batería:** LiPo 3S 11.1V 2200-3000mAh
- **Consumo estimado:** 
  - Idle: ~2A
  - Walking: ~6-8A
  - Peak: ~12A
- **Regulador ESP32:** Step-down 5V 2A

### Conexiones ESP32
```
Serial Bus Servos: GPIO16 (TX), GPIO17 (RX)
IMU I2C:          GPIO21 (SDA), GPIO22 (SCL)
RC Receiver:      GPIO25-28
Status LED:       GPIO2
Emergency Stop:   GPIO0 (boot button)
```

## Parámetros Cinemáticos

### Sistema de Coordenadas
- **X:** Forward (adelante)
- **Y:** Left (izquierda)
- **Z:** Up (arriba)
- **Origen:** Centro del cuerpo

### Posiciones de Montaje de Piernas
```
Right Front  (RF): [ 80,  -90, 0]
Right Middle (RM): [  0,  -90, 0]
Right Rear   (RR): [-80,  -90, 0]
Left Front   (LF): [ 80,   90, 0]
Left Middle  (LM): [  0,   90, 0]
Left Rear    (LR): [-80,   90, 0]
```

### Posición de Reposo (Home)
```
Coxa:  0° (neutral)
Femur: 45° (levantada)
Tibia: 90° (doblada)
Altura del cuerpo: ~100mm
```

## Gaits Implementados

### FASE 1: Movimiento Básico
- Secuencia lineal individual

### FASE 3: Gaits Completos
- **Tripod Gait:** 3 piernas simultáneas, estable, rápido
- **Ripple Gait:** 1 pierna por vez, muy estable, lento
- **Wave Gait:** Secuencial, máxima estabilidad

## Performance Objetivo

### Velocidad
- **Tripod:** 0.2-0.3 m/s
- **Ripple:** 0.1-0.15 m/s

### Estabilidad
- **Inclinación máxima:** ±15° (con IMU)
- **Margen de estabilidad:** >20mm

### Precisión
- **Posicionamiento pie:** ±5mm
- **Repetibilidad:** ±2mm

## Limitaciones Conocidas

### Mecánicas
- Backlash < 1° (diseño optimizado)
- Flexión patas < 3mm @ max load
- Peso objetivo: ~1.5kg (sin batería)

### Eléctricas
- Autonomía: 30-60min (walking normal)
- Thermal throttling: >60°C servos

### Software
- Loop rate: 50Hz (control)
- IK calculation: <5ms
- Latencia comando: <20ms
