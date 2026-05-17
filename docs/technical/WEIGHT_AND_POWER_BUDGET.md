# Weight & Power Budget - HexapodIA

## Executive Summary

| Fase | Peso Total | Consumo Típico | Autonomía (6000mAh) | Status |
|------|------------|----------------|---------------------|--------|
| **FASE 1** | ~600g | 20W | 25min | Prototipo |
| **FASE 3** | ~2270g | 75W | 50min | Walking |
| **FASE 5** | ~2450g | 94W | 42min | Autonomous |

**Recomendación batería:** LiPo 3S 11.1V 6000mAh 40C (~480g, €50)

---

## Análisis de Peso Detallado

### FASE 1: Pierna Única

```
Categoría              Item                        Cantidad  Peso Unit.  Subtotal
─────────────────────────────────────────────────────────────────────────────────
MECÁNICA
  Estructura           Coxa PETG                   1         25g         25g
                       Femur PETG                  1         15g         15g
                       Tibia PETG                  1         12g         12g
                       Pies TPU                    1         8g          8g
                       Subtotal mecánica                                 60g

SERVOS
  Actuadores           STS3215 (62g c/u)           3         62g         186g
                       Subtotal servos                                   186g

ELECTRÓNICA
  Control              ESP32-WROOM-32 DevKit       1         10g         10g
                       PCB/Protoboard              1         15g         15g
                       Cables servos               3m        1g/m        3g
                       Subtotal electrónica                              28g

POWER
  Batería              LiPo 3S 2200mAh             1         180g        180g
  Conversión           Buck 12V 5A                 1         25g         25g
                       Buck 5V 2A                  1         15g         15g
  Protección           Capacitor 1000µF            1         5g          5g
                       Switch + XT60               1         12g         12g
                       Cables power 14AWG          0.3m      8g/m        2g
                       Subtotal power                                    239g

HARDWARE
  Fijación             Tornillos M3×8              12        1g          12g
                       Tuercas M3                  8         0.5g        4g
                       Insertos térmicos M3        8         1g          8g
                       Arandelas M3                4         0.3g        1g
                       Subtotal hardware                                 25g

MISCELÁNEA
  Varios               Zip ties                    10        0.5g        5g
                       Heat shrink                 5cm       0.2g/cm     1g
                       Cinta aislante              10cm      0.3g/cm     3g
                       Cable management            -         5g          5g
                       Subtotal misc.                                    14g

─────────────────────────────────────────────────────────────────────────────────
TOTAL FASE 1:                                                           552g
Margen +10%:                                                            607g
```

**Estimación conservadora FASE 1:** **~600g**

---

### FASE 3: Hexapod Completo (6 Piernas)

```
Categoría              Item                        Cantidad  Peso Unit.  Subtotal
─────────────────────────────────────────────────────────────────────────────────
MECÁNICA
  Body                 Placa superior PETG         1         120g        120g
                       Placa inferior PETG         1         100g        100g
                       Refuerzos laterales         4         15g         60g
                       Soportes servos body        6         5g          30g
  
  Piernas              Coxa PETG (×6)              6         25g         150g
                       Femur PETG (×6)             6         15g         90g
                       Tibia PETG (×6)             6         12g         72g
                       Pies TPU (×6)               6         8g          48g
                       Subtotal mecánica                                 770g

SERVOS
  Actuadores           STS3215 (×18)               18        62g         1116g
                       Subtotal servos                                   1116g

ELECTRÓNICA
  Control              ESP32-WROOM-32              1         10g         10g
                       PCB custom/proto            1         30g         30g
                       IMU BNO085                  1         3g          3g
                       RC Receiver (ELRS)          1         5g          5g
                       Cables servos               20m       1g/m        20g
                       Cables señales              5m        0.5g/m      3g
                       Subtotal electrónica                              71g

POWER
  Batería              LiPo 3S 6000mAh 40C         1         480g        480g
  Conversión           Buck 12V 10A                1         35g         35g
                       Buck 5V 5A                  1         25g         25g
  Protección           Capacitor 3300µF (×2)       2         8g          16g
                       Voltage alarm               1         5g          5g
                       Switch 10A                  1         15g         15g
                       XT60 connectors             2         8g          16g
                       Fuse 15A                    1         3g          3g
  Cableado             Cable 14AWG (power)         1m        8g/m        8g
                       Cable 16AWG (servos)        2m        5g/m        10g
                       Subtotal power                                    613g

HARDWARE
  Fijación Body        Tornillos M3×12 (body)      24        1.5g        36g
                       Tornillos M3×8 (piernas)    48        1g          48g
                       Tuercas M3                  36        0.5g        18g
                       Insertos térmicos M3        40        1g          40g
                       Arandelas M3                20        0.3g        6g
                       Espaciadores nylon          12        2g          24g
                       Subtotal hardware                                 172g

MISCELÁNEA
  Varios               Zip ties                    40        0.5g        20g
                       Heat shrink                 20cm      0.2g/cm     4g
                       Velcro batería              10cm      2g/cm       20g
                       Cable management            -         15g         15g
                       Adhesivo/foam               -         10g         10g
                       Subtotal misc.                                    69g

─────────────────────────────────────────────────────────────────────────────────
SUBTOTAL FASE 3:                                                        2811g
Optimización (PETG infill 30-40%):                                      -350g
TOTAL REAL ESTIMADO:                                                    2461g
```

**Estimación realista FASE 3:** **~2270-2460g** (promedio **2365g**)

---

### FASE 5: Con Jetson Orin Nano + IA + Interacción + Cámara 360°

```
Categoría              Item                        Cantidad  Peso Unit.  Subtotal
─────────────────────────────────────────────────────────────────────────────────
BASE FASE 3            (carry over)                -         -           2365g

JETSON + IA
  Compute              Jetson Orin Nano 8GB        1         100g        100g
                       Heatsink + fan              1         25g         25g
                       MicroSD 128GB               1         2g          2g
  
  Vision               IMX219 8MP camera           1         10g         10g
                       Camera mount PETG           1         15g         15g
                       FPV camera analog 700TVL    1         8g          8g
                       Video TX 5.8GHz 200mW       1         12g         12g
                       VTX antenna                 1         3g          3g
  
  Audio                USB microphone mini         1         15g         15g
                       I2S speaker 3W              1         20g         20g
                       Audio amplifier PAM8403     1         5g          5g
                       Speaker mount               1         8g          8g
  
  Display              OLED 0.96" I2C              1         5g          5g
                       OLED mount                  1         3g          3g
  
  Comunicación         WiFi module 5GHz (video)    1         8g          8g
                       Radio module (voice cmds)   1         6g          6g
                       Antennas                    2         4g          8g
  
  Sensores Extra       Ultrasonic HC-SR04 (×2)     2         8g          16g
                       Sensor mounts               2         3g          6g
  
  Cableado             USB-C cable Jetson          1         15g         15g
                       CSI ribbon camera           1         5g          5g
                       Cables audio                1m        2g/m        2g
                       Cables video/antenna        1m        3g/m        3g
                       Cables extras               2m        1g/m        2g
                       Subtotal Jetson+IA                                298g

CÁMARA 360° PAYLOAD
  Cámara               Insta360 Ace Pro 2          1         182g        182g
  Montaje              Adaptador trípode 1/4"      1         8g          8g
                       Soporte PETG (top mount)    1         25g         25g
                       Quick release plate         1         12g         12g
                       Subtotal cámara 360°                              227g

POWER UPGRADE (para 1h autonomía + payload)
  Batería              Upgrade a 10000mAh 25C      1         800g        800g
                       (reemplaza 6000mAh)         -1        -480g       -480g
  Buck upgrade         Buck 5V 10A (reemplaza 5A)  1         35g         35g
                                                    -1        -25g        -25g
  Capacitor extra      2200µF 6.3V (5V rail)       1         6g          6g
                       Subtotal power upgrade                            336g

HARDWARE EXTRA
  Fijación             Tornillos M2.5×6 (Jetson)   8         0.5g        4g
                       Espaciadores M2.5 10mm      4         2g          8g
                       Tornillos M3×8 (cámaras)    8         1g          8g
                       Tornillos M2×6 (antenas)    4         0.3g        1g
                       Subtotal hardware                                 21g

─────────────────────────────────────────────────────────────────────────────────
TOTAL FASE 5 (con Insta360):                                            3240g
```

**Estimación FASE 5 con Insta360 Ace Pro 2:** **~3000-3240g** (promedio **3120g**)

### Configuraciones Opcionales

**Config A - Sin cámara 360°:**
- Peso: ~2775g
- Para operaciones autónomas sin grabación

**Config B - Con cámara 360° (Insta360 Ace Pro 2):**
- Peso: ~3120g (+345g vs Config A)
- Para grabar travesías y aventuras

---

## Análisis de Consumo Energético

### FASE 1: Pierna Única

```
Componente                Voltaje   Corriente   Potencia   Duty Cycle   Promedio
──────────────────────────────────────────────────────────────────────────────────
ESP32-WROOM-32            5V        200mA       1.0W       100%         1.0W
STS3215 ×3 (idle)         12V       300mA       3.6W       40%          1.4W
STS3215 ×3 (moving slow)  12V       900mA       10.8W      30%          3.2W
STS3215 ×3 (moving fast)  12V       1500mA      18.0W      20%          3.6W
STS3215 ×3 (peak stall)   12V       4500mA      54.0W      <1%          0.5W
──────────────────────────────────────────────────────────────────────────────────
TOTAL Típico Testing:                                                   ~9.7W
TOTAL Continuous Moving:                                                ~20W
TOTAL Peak (evitar):                                                    ~55W
```

**Batería:** LiPo 3S 2200mAh (24.4Wh @ 11.1V)  
**Autonomía estimada:**
- Testing intermitente: ~150min (2.5h)
- Movimiento continuo: ~73min (1.2h)
- Movimiento agresivo: ~40min

---

### FASE 3: Hexapod Completo Walking

```
Componente                Voltaje   Corriente   Potencia   Duty Cycle   Promedio
──────────────────────────────────────────────────────────────────────────────────
ESP32-WROOM-32            5V        200mA       1.0W       100%         1.0W
IMU BNO085                3.3V      10mA        0.03W      100%         0.03W
RC Receiver (ELRS)        5V        50mA        0.25W      100%         0.25W

STS3215 ×18 (idle)        12V       1800mA      21.6W      30%          6.5W
STS3215 ×18 (tripod slow) 12V       4500mA      54.0W      40%          21.6W
STS3215 ×18 (tripod fast) 12V       6000mA      72.0W      25%          18.0W
STS3215 ×18 (ripple)      12V       3600mA      43.2W      35%          15.1W
STS3215 ×18 (stairs/load) 12V       9000mA      108.0W     5%           5.4W
STS3215 ×18 (peak stall)  12V       18000mA     216.0W     <1%          2.2W
──────────────────────────────────────────────────────────────────────────────────
Standing Idle:                                                          ~8W
Walking Slow (Ripple):                                                  ~43W
Walking Normal (Tripod):                                                ~62W
Walking Fast:                                                           ~83W
Walking + Stairs:                                                       ~108W
Peak (emergency only):                                                  ~217W
```

**Escenarios reales:**

| Actividad | Consumo | % Tiempo | Contribución |
|-----------|---------|----------|--------------|
| Idle/Standing | 8W | 20% | 1.6W |
| Walking slow explore | 43W | 40% | 17.2W |
| Walking normal | 62W | 30% | 18.6W |
| Walking fast | 83W | 8% | 6.6W |
| Obstacle/stairs | 108W | 2% | 2.2W |
| **PROMEDIO MISIÓN** | **~46W** | 100% | **46W** |

**Batería:** LiPo 3S 6000mAh (66.6Wh @ 11.1V)  
**Autonomía estimada:**
- Idle monitoring: ~500min (8.3h)
- Walking slow patrol: ~93min (1.5h)
- **Walking normal mission**: ~**52min**
- Walking fast terrain: ~48min (0.8h)
- Continuous intensive: ~37min

---

### FASE 5: Autonomous con Jetson Orin Nano + Interacción IA

```
Componente                    Voltaje   Corriente   Potencia   Duty Cycle   Promedio
────────────────────────────────────────────────────────────────────────────────────
BASE (ESP32 + IMU + RC)       -         -           1.28W      100%         1.28W

Jetson Orin Nano (10W mode)   5V        2000mA      10.0W      40%          4.0W
Jetson Orin Nano (15W mode)   5V        3000mA      15.0W      50%          7.5W
Jetson Orin Nano (25W max)    5V        5000mA      25.0W      10%          2.5W

IMX219 Camera (8MP)           5V        300mA       1.5W       100%         1.5W
FPV Camera + TX 200mW         5V        600mA       3.0W       100%         3.0W
Video Transmitter (5.8GHz)    5V        800mA       4.0W       100%         4.0W

USB Microphone                5V        100mA       0.5W       100%         0.5W
I2S Speaker 3W (voice out)    5V        600mA       3.0W       30%          0.9W
Audio Amplifier               5V        200mA       1.0W       30%          0.3W

OLED Display 0.96"            5V        40mA        0.2W       100%         0.2W
WiFi Module (video stream)    5V        300mA       1.5W       80%          1.2W
Radio module (voice commands) 5V        150mA       0.75W      60%          0.45W

Ultrasonic ×2                 5V        30mA        0.15W      50%          0.08W
Fan (Jetson cooling)          5V        200mA       1.0W       80%          0.8W

STS3215 ×18 (walking mix)     12V       -           -          -            ~42W
────────────────────────────────────────────────────────────────────────────────────
Idle + Monitoring:                                                        ~25W
Patrol Light (10W Jetson):                                                ~63W
Interactive Mode (15W Jetson):                                            ~80W
Navigation + Voice (15W):                                                 ~88W
Full Interactive + SLAM (15W):                                            ~95W
Intensive (25W + video stream):                                           ~108W
Peak (walking fast + max AI):                                             ~125W
```

**Modos de operación autónoma con interacción IA:**

| Modo | Descripción | Jetson | Periféricos | Walking<br>(sin 360°) | Walking<br>(CON 360°) | Total<br>(sin 360°) | Total<br>(con 360°) | Autonomía<br>(10000mAh) |
|------|-------------|--------|-------------|---------|---------|-------|-------|-------|
| **Idle** | Parado, monitoring | 10W | 6W | 8W | 9W | 25W | 26W | 4.3h (257min) |
| **Patrol Light** | Walk slow, object detect | 10W | 8W | 43W | 48W | 63W | 68W | 1.6h (98min) |
| **Interactive** | Walk normal, voice | 15W | 10W | 53W | 59W | 80W | 86W | 1.3h (77min) |
| **Explore + Voice** | Walk normal, SLAM | 15W | 11W | 62W | 69W | 88W | 95W | 1.2h (70min) |
| **Full Interactive** | SLAM+Nav+Voice+Video | 15W | 13W | 62W | 69W | 95W | 102W | 1.1h (65min) |
| **Recording Mode** | Walk slow + Insta360 | 15W | 10W | 45W | 51W | 70W | 76W | 1.5h (87min) ⭐ |

**Notas importantes:**
- **Walking con Insta360:** +12% consumo por peso adicional (345g)
- **Recording Mode:** Modo optimizado para grabar travesías
  - Walking slow (1.2× factor dinámico)
  - Insta360 graba con batería interna (no consume del hexapod)
  - Autonomía: ~87min (1.5h) ✅

**Desglose "Periféricos" incluye:**
- Video transmission (FPV + digital stream): 4-7W
- Audio (mic + speaker + amp): 1-2W
- WiFi/Radio (commands + telemetry): 1-2W
- Display + sensors + fan: 1-2W

**Batería REQUERIDA con Insta360:**
- ✅ **LiPo 3S 10000mAh 25C** (111Wh @ 11.1V, ~800g, €95) ⭐
- Permite **1.5h grabando travesías** en Recording Mode
- Permite **1.1h** en modo "Full Interactive"
- Alternativa: 8000mAh (~52min con Insta360, insuficiente) ⚠️

**Autonomía realista (10000mAh) con Insta360:**
- **Recording Mode (walking slow):** ~**87min (1.5h)** ✅
- Full interactive: ~65min (1.1h)
- Explore + Voice: ~70min
- Idle monitoring: ~257min (4.3h)

---

## Análisis de Torque vs Peso

### Carga por Pierna

```
Peso total FASE 3: 2365g
Peso por pierna: 2365 / 6 = 394g

Peso total FASE 5 (sin 360°): 2775g
Peso por pierna: 2775 / 6 = 463g

Peso total FASE 5 (con Insta360): 3120g
Peso por pierna: 3120 / 6 = 520g
```

### Torque Requerido por Articulación

**Modelo simplificado:**

```
Torque = (Carga × Brazo de palanca) / 2    (en standing, pierna vertical)

FASE 3 (394g por pierna):
─────────────────────────────────────────────────────────────────
Articulación    Brazo       Carga       Torque      STS3215    Margen
                (mm)        (g)         Requerido   (20kg·cm)  
─────────────────────────────────────────────────────────────────
Coxa (yaw)      55          394         1.08 kg·cm  20 kg·cm   18.5×
Femur (pitch)   80          394         1.58 kg·cm  20 kg·cm   12.7×
Tibia (pitch)   130         394         2.56 kg·cm  20 kg·cm   7.8×
─────────────────────────────────────────────────────────────────

FASE 5 sin 360° (463g por pierna):
─────────────────────────────────────────────────────────────────
Articulación    Brazo       Carga       Torque      STS3215    Margen
                (mm)        (g)         Requerido   (20kg·cm)  
─────────────────────────────────────────────────────────────────
Coxa (yaw)      55          463         1.27 kg·cm  20 kg·cm   15.7×
Femur (pitch)   80          463         1.85 kg·cm  20 kg·cm   10.8×
Tibia (pitch)   130         463         3.01 kg·cm  20 kg·cm   6.6× ✅
─────────────────────────────────────────────────────────────────

FASE 5 CON Insta360 (520g por pierna):
─────────────────────────────────────────────────────────────────
Articulación    Brazo       Carga       Torque      STS3215    Margen
                (mm)        (g)         Requerido   (20kg·cm)  
─────────────────────────────────────────────────────────────────
Coxa (yaw)      55          520         1.43 kg·cm  20 kg·cm   14.0×
Femur (pitch)   80          520         2.08 kg·cm  20 kg·cm   9.6×
Tibia (pitch)   130         520         3.38 kg·cm  20 kg·cm   5.9× ✅
─────────────────────────────────────────────────────────────────
```

**Escenarios dinámicos (FASE 5 con Insta360):**

```
Caso                    Factor      Torque Tibia    Margen vs 20kg·cm
─────────────────────────────────────────────────────────────────────
Standing (estático)     1.0×        3.38 kg·cm      5.9× ✅
Walking normal          1.5×        5.07 kg·cm      3.9× ✅
Walking fast            2.0×        6.76 kg·cm      3.0× ✅
Stairs/Obstacles        3.0×        10.14 kg·cm     2.0× ✅ (límite)
Jump/Impact             4.0×        13.52 kg·cm     1.5× ⚠️ EVITAR
Walking slow (seguro)   1.2×        4.06 kg·cm      4.9× ✅
─────────────────────────────────────────────────────────────────────
```

**Conclusión CON Insta360 Ace Pro 2 (3120g total):**
- ✅ **Buen margen** para walking normal (3.9× en tibia)
- ✅ Soporta **walking fast** con margen aceptable (3.0×)
- ⚠️ Stairs/obstacles al límite (2.0×) - **caminar despacio en terreno difícil**
- ❌ NO saltos/impactos (1.5×) - insuficiente
- ✅ **RECOMENDACIÓN:** Walking normal/slow para grabar (4.9× margin)

**Safety margin guideline:**
- Mínimo aceptable: **2.0×**
- Operación normal: **3.0-5.0×** ← Con Insta360 en walking normal ✅
- Operación conservadora: **>5.0×**

**Impacto en consumo energético:**
- La Insta360 graba de forma independiente (batería interna)
- ✅ NO aumenta consumo del hexapod
- ⚠️ Peso adicional aumenta carga en servos → +8-12% consumo estimado

---

## Presupuesto de Energía por Fase

### FASE 1: Pierna Única

| Item | Modelo | Cantidad | Precio | Peso |
|------|--------|----------|--------|------|
| LiPo 3S | 2200mAh 30C | 1 | €20 | 180g |
| Buck 12V | 5A LM2596 | 1 | €8 | 25g |
| Buck 5V | 2A LM2596 | 1 | €5 | 15g |
| Capacitor | 1000µF 16V | 1 | €1 | 5g |
| XT60 | Male+Female | 1 | €2 | 8g |
| Switch | 10A toggle | 1 | €3 | 12g |
| Cables | 14AWG 1m | 1 | €3 | 8g |
| **TOTAL FASE 1** | | | **€42** | **253g** |

### FASE 3: Hexapod Completo

| Item | Modelo | Cantidad | Precio | Peso |
|------|--------|----------|--------|------|
| LiPo 3S | 6000mAh 40C (Gens Ace) | 1 | €50 | 480g |
| Buck 12V | 10A LM2596HV | 1 | €12 | 35g |
| Buck 5V | 5A Pololu D36V50F5 | 1 | €25 | 25g |
| Capacitor | 3300µF 16V (×2) | 2 | €4 | 16g |
| XT60 | Male+Female (×2) | 2 | €4 | 16g |
| Voltage alarm | 3S buzzer | 1 | €3 | 5g |
| Switch | 10A toggle | 1 | €3 | 15g |
| Fuse | 15A blade | 1 | €2 | 3g |
| Cables 14AWG | Silicone 2m | 1 | €6 | 16g |
| Heat shrink | Assorted | 1 | €2 | 2g |
| **TOTAL FASE 3** | | | **€111** | **613g** |

### FASE 5: Jetson + IA (Upgrades)

| Item | Modelo | Cantidad | Precio | Peso |
|------|--------|----------|--------|------|
| LiPo 3S upgrade | 8000mAh 25C (opcional) | 1 | €70 | +150g |
| Buck 5V upgrade | 8A UBEC | 1 | €18 | +5g |
| Jetson heatsink | Official + fan | 1 | €15 | 25g |
| Current sensor | INA219 module | 1 | €5 | 3g |
| Voltage display | OLED voltmeter | 1 | €8 | 5g |
| **TOTAL FASE 5** | | | **€116** | **+188g** |

**Inversión total energización:**
- FASE 1: €42
- FASE 3: €111 (o €69 upgrade desde FASE 1)
- FASE 5: €116 (upgrades)
- **TOTAL:** €227 (inversión completa)

---

## Distribución de Peso en Body

### Centro de Gravedad (CG)

**Target CG:** Centro geométrico del hexágono, altura baja para estabilidad

```
Vista superior (distribución óptima):

    Front
      ↑
      │
  ┌───┴───────────────┐
  │   [Camera 15g]    │
  │   [ESP32 10g]     │
  │                   │
  │ [Buck][Batería]   │ ← Centro (CG objetivo)
  │ [12V] [480g]      │
  │ [35g]             │
  │       [Buck 5V]   │
  │       [25g]       │
  │                   │
  │   [Jetson 100g]   │
  │   [Heatsink 25g]  │
  └───────────────────┘

Vista lateral:

    Top:    [Jetson+Heatsink]  125g @ +40mm
    Mid:    [ESP32+Electrónica] 60g @ +25mm
    Low:    [Batería]          480g @ +10mm  ← CG principal
    Bottom: [Bucks+Caps]        76g @ +5mm
            ─────────────────────────────────
            [6 piernas]        structural support
```

**Cálculo CG altura (FASE 5):**

```
CG_z = Σ(masa_i × altura_i) / masa_total

CG_z = (125×40 + 60×25 + 480×10 + 76×5) / 741
CG_z = (5000 + 1500 + 4800 + 380) / 741
CG_z = 11680 / 741
CG_z ≈ 15.8mm desde bottom plate

Con piernas extended (z=-80mm):
CG_total ≈ -64mm (debajo del body plane) ✅ Muy estable
```

**Estabilidad:**
- ✅ CG bajo → alta estabilidad
- ✅ Batería (mayor masa) en nivel inferior
- ✅ Componentes livianos (Jetson) arriba → aceptable
- ✅ Distribución simétrica en x/y

---

## Thermal Budget

### Generación de Calor

```
Componente              Potencia   Eficiencia   Heat       Cooling
                        Consumida               Disipated  
─────────────────────────────────────────────────────────────────
ESP32                   1.0W       ~50%         0.5W       Passive
Jetson (15W mode)       15.0W      ~65%         5.3W       Heatsink+fan
Buck 12V (10A)          120W       91%          10.8W      Heatsink
Buck 5V (8A)            40W        92%          3.2W       Heatsink
Servos ×18 (walking)    72W        ~75%         18.0W      Ventilation
─────────────────────────────────────────────────────────────────
TOTAL Heat Generation:                          37.8W
```

### Cooling Strategy

**Passive Cooling:**
- Body con ventilation slots (40% open area laterales)
- Convección natural durante walking
- Pies elevados → airflow inferior

**Active Cooling (FASE 5):**
- Fan 40mm 5V 0.2A en Jetson heatsink
- Optional: fan 60mm 5V 0.3A para body ventilation

**Thermal Monitoring:**
```cpp
void thermalManagement() {
    // Jetson
    float jetsonTemp = readJetsonTemp();
    if (jetsonTemp > 80) throttleJetson(15W);  // From 25W
    if (jetsonTemp > 70) setFanSpeed(100);
    else if (jetsonTemp > 60) setFanSpeed(70);
    else setFanSpeed(40);
    
    // Servos (STS3215 feedback)
    for (int i = 0; i < 18; i++) {
        uint8_t temp = servo.readTemperature(i);
        if (temp > 70) emergencyStop();
        if (temp > 65) reduceWalkingSpeed(0.7);
        if (temp > 60) reduceWalkingSpeed(0.85);
    }
    
    // Bucks (via ADC thermistor, opcional)
    float buckTemp = readBuckTemperature();
    if (buckTemp > 80) reduceLoad();
}
```

**Ambient operating range:**
- Mínimo: 0°C (LiPo reduced performance)
- Óptimo: 15-30°C
- Máximo: 40°C (thermal throttling activo)

---

## Cables y Conectores

### Cable Sizing

| Conexión | Corriente Max | AWG | Longitud | Caída V | Peso/m |
|----------|---------------|-----|----------|---------|--------|
| Batería → Switch | 15A | 14 | 10cm | 0.08V | 8g |
| Switch → Bucks | 12A | 14 | 15cm | 0.12V | 8g |
| Buck 12V → Servo bus | 10A | 16 | 30cm | 0.24V | 5g |
| Buck 5V → Jetson | 5A | 18 | 15cm | 0.10V | 3g |
| Buck 5V → ESP32 | 0.5A | 22 | 10cm | 0.01V | 1g |
| Servo data bus | Signal | 26 | 25cm | - | 0.5g |
| I2C (IMU/OLED) | Signal | 26 | 15cm | - | 0.5g |

**Total cable weight:** ~92g (incluido en análisis previo)

### Connector Standard

| Uso | Tipo | Rating | Peso | Precio |
|-----|------|--------|------|--------|
| Batería main | XT60 | 60A continuous | 8g | €2 |
| Power distribution | XT30 | 30A continuous | 4g | €1.5 |
| Servos | JST-3pin o nativo | 3A | 1g | €0.5 |
| Logic signals | Dupont/JST-XH | 1A | 0.3g | €0.2 |
| USB (Jetson) | USB-C | 5A | 5g | €3 |

---

## Bill of Materials - Completo

### Mecánica (PETG + TPU)

| Item | Cantidad | Peso | Precio | Total |
|------|----------|------|--------|-------|
| Filamento PETG | 1kg | 1000g | €25 | €25 |
| Filamento TPU | 0.25kg | 250g | €15 | €15 |
| **Consumo estimado:** | | 770g | | **€20** |

### Servos

| Item | Cantidad | Peso Unit | Precio Unit | Total |
|------|----------|-----------|-------------|-------|
| Feetech STS3215 | 18 | 62g | €18 | €324 |

### Electrónica

| Item | Cantidad | Peso | Precio | Total |
|------|----------|------|--------|-------|
| ESP32-WROOM-32 DevKit | 1 | 10g | €8 | €8 |
| IMU BNO085 | 1 | 3g | €25 | €25 |
| RC Receiver (ELRS EP1) | 1 | 5g | €12 | €12 |
| Jetson Orin Nano 8GB | 1 | 100g | €499 | €499 |
| Camera IMX219 8MP | 1 | 10g | €20 | €20 |
| FPV Camera + TX | 1 | 13g | €25 | €25 |
| USB Microphone | 1 | 15g | €10 | €10 |
| I2S Speaker 3W | 1 | 20g | €8 | €8 |
| OLED 0.96" I2C | 1 | 5g | €5 | €5 |
| Ultrasonic HC-SR04 (×2) | 2 | 8g | €3 | €6 |
| **Subtotal electrónica** | | | | **€618** |

### Power (según tabla anterior)

| Fase | Inversión | Peso |
|------|-----------|------|
| FASE 1 | €42 | 253g |
| FASE 3 | €111 | 613g |
| FASE 5 | €116 | 801g |

### Hardware (Tornillos, Tuercas, Insertos)

| Item | Cantidad | Peso | Precio | Total |
|------|----------|------|--------|-------|
| Kit M3 (tornillos + tuercas) | 1 | 150g | €12 | €12 |
| Insertos térmicos M3×5 | 50 | 50g | €8 | €8 |
| Espaciadores nylon | 20 | 40g | €5 | €5 |
| **Subtotal hardware** | | | | **€25** |

### Miscelánea

| Item | Precio |
|------|--------|
| Zip ties, heat shrink, velcro | €8 |
| Thermal paste | €3 |
| Cables varios | €10 |
| **Subtotal misc.** | **€21** |

---

## Presupuesto Total del Proyecto

```
Categoría                        FASE 1    FASE 3    FASE 5    Total
─────────────────────────────────────────────────────────────────────
Mecánica (filamento)             €5        €15       €20       €20
Servos (3→18)                    €54       €324      €324      €324
Electrónica (ESP32+IMU+RC)       €20       €45       €45       €45
Electrónica IA (Jetson+cámaras)  -         -         €567      €567
Power (baterías+bucks)           €42       €111      €227      €227
Hardware (tornillos+insertos)    €5        €15       €25       €25
Miscelánea (cables, etc.)        €5        €12       €21       €21
─────────────────────────────────────────────────────────────────────
SUBTOTAL                         €131      €522      €1229     €1229
Margen (+10% imprevistos)        €13       €52       €123      €123
─────────────────────────────────────────────────────────────────────
TOTAL ESTIMADO                   €144      €574      €1352     €1352
─────────────────────────────────────────────────────────────────────
```

**Desglose inversión incremental:**
- **FASE 1 (prototipo):** €144
- **FASE 3 (upgrade):** +€430 = €574 total
- **FASE 5 (IA completa):** +€778 = €1352 total

**Peso final:**
- FASE 1: ~600g
- FASE 3: ~2365g
- FASE 5 sin 360°: ~2775g (con video, voz, display)
- FASE 5 CON Insta360: ~3120g (+345g para cámara 360° montada)

---

## Recomendaciones Finales

### Batería

**FASE 1:**
- LiPo 3S 2200mAh 30C (~€20, 180g)
- Autonomía: ~70min testing

**FASE 3:**
- ✅ **LiPo 3S 6000mAh 40C Gens Ace** (~€50, 480g)
- Autonomía: ~50min walking normal
- Balance óptimo peso/autonomía

**FASE 5 sin Insta360:**
- ✅ **LiPo 3S 8000mAh 25C** (~€70, 630g)
- Autonomía: **56-67min** con IA, video streaming, comandos voz
- Cumple objetivo de 45min-1h ✅

**FASE 5 CON Insta360 Ace Pro 2:**
- ✅ **LiPo 3S 10000mAh 25C** (~€95, 800g) ⭐ **REQUERIDO**
- Autonomía: **65-87min** (modo Recording: 87min)
- Cumple objetivo de 1h+ grabando travesías ✅
- Peso total: 3120g (torque margin: 3.9× en walking normal)

### Optimizaciones de Peso

**Si necesitas reducir peso FASE 5:**

1. **Batería:** 6000mAh en lugar de 8000mAh (-150g, -€20)
2. **Estructura:** Infill 30% en lugar de 40% en partes no críticas (-80g)
3. **Audio:** Buzzer en lugar de speaker I2S (-18g, -€5)
4. **Sensores:** Omit ultrasonic sensors (-16g, -€6)
5. **Cámara:** FPV only, omit digital camera (-25g, -€20)

**Peso reducido:** ~2330g (vs 2625g baseline)  
**Ahorro:** €51

### Margen de Seguridad

**Torque servos:**
- ✅ Baseline: 7.0× en tibia (muy bueno)
- ✅ Con payload +500g: 4.1× (seguro)
- ✅ Con payload +1000g: 3.2× (aceptable)

**Capacidad batería:**
- Sin Insta360:
  - ⚠️ 6000mAh: ~42min (insuficiente)
  - ✅ 8000mAh: 56-67min (cumple objetivo) ⭐
- Con Insta360 Ace Pro 2:
  - ⚠️ 8000mAh: ~52min (límite)
  - ✅ **10000mAh: 65-87min (recomendado)** ⭐⭐

**Consumo eléctrico:**
- Peak teórico: 240W (batería 6000mAh puede entregar 960W @ 40C) ✅
- Operación normal: 94W (40% de capacidad peak) ✅

---

## Conclusión

| Métrica | FASE 1 | FASE 3 | FASE 5 | Status |
|---------|--------|--------|--------|--------|
| Métrica | FASE 1 | FASE 3 | FASE 5<br>(sin 360°) | FASE 5<br>(CON Insta360) | Status |
|---------|--------|--------|--------|--------|--------|
| **Peso Total** | 600g | 2365g | 2775g | 3120g | ✅ Manejable |
| **Torque Margin** | 7.8× | 7.8× | 6.6× | 5.9× (3.9× walking) | ✅ Suficiente |
| **Consumo Interactive** | 20W | 62W | 95W | 102W | ✅ Razonable |
| **Batería Requerida** | 2200mAh | 6000mAh | 8000mAh | 10000mAh | ✅ Disponible |
| **Autonomía** | 73min | 50min | 56min | 65min (87min slow) | ✅ Cumple objetivo |
| **Presupuesto** | €144 | €574 | €1385 | €1438 | ✅ Competitivo |

**Veredicto:**
- ✅ STS3215 servos **funcionan con Insta360** (5.9× static, 3.9× walking normal)
- ⚠️ **Walking slow recomendado** cuando grabas (4.9× margin)
- ✅ **LiPo 10000mAh REQUERIDO** con Insta360 para 1h+ grabación
- ✅ Arquitectura es **escalable** sin rediseño
- ✅ Presupuesto es **razonable** para proyecto DIY avanzado (~€1438 total)
- ✅ Interacción completa + grabación 360° funciona dentro del budget

**Componentes críticos CON Insta360 Ace Pro 2:**
- 🔋 **Batería 10000mAh** (800g, €95) - REQUERIDO para 1h+ ⭐
- 🔌 Buck 5V 10A (upgrade necesario)
- 📷 **Soporte trípode top-mount** (quick release 1/4")
- 🎥 Video transmitter 200mW (FPV piloting)
- 🎤 Audio system (interacción IA)
- 📡 WiFi 5GHz module (telemetría)
- 🖥️ Display OLED (status)

**Impacto Insta360 en diseño:**
- Peso adicional: **+345g** (182g cámara + 163g batería upgrade)
- CG elevado: Montar lo más centrado posible para estabilidad
- Torque suficiente: ✅ Pero evitar terreno extremo
- **Modo grabación:** Walking slow (50% speed) para mejor footage y menor consumo

**Comparación configuraciones:**

| Config | Peso | Batería | Autonomía | Uso |
|--------|------|---------|-----------|-----|
| **A - Ligero** | 2775g | 8000mAh | 56min | Operación autónoma rápida |
| **B - Con Insta360** | 3120g | 10000mAh | 87min | Grabar travesías/aventuras ⭐ |

**Próximo paso:** Validar FASE 1 con hardware real, medir consumos reales, ajustar modelo.
