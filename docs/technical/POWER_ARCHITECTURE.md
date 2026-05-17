# Arquitectura de Energización - HexapodIA

## Overview

Sistema de alimentación diseñado para soportar:
- **FASE 1-4:** ESP32 + 18× STS3215 servos
- **FASE 5 (v2):** + Jetson Orin Nano + cámara + audio + sensores

La arquitectura debe permitir evolución sin rediseñar sistema completo.

## Análisis de Consumo por Fase

### FASE 1: Pierna Única (Prototipo)

```
Componente            Voltaje  Corriente    Potencia
────────────────────────────────────────────────────
ESP32                 5V       200mA        1W
STS3215 x3 (idle)     12V      300mA        3.6W
STS3215 x3 (moving)   12V      1.5A         18W
STS3215 x3 (peak)     12V      4.5A         54W (stall)
────────────────────────────────────────────────────
Total idle:                                 ~5W
Total moving:                               ~20W
Total peak:                                 ~55W
```

**Batería recomendada FASE 1:**
- LiPo 3S 11.1V 2200mAh
- Autonomía: ~25min movimiento continuo

### FASE 3: Hexapod Completo (6 Piernas)

```
Componente            Voltaje  Corriente    Potencia
────────────────────────────────────────────────────
ESP32                 5V       200mA        1W
IMU BNO085            3.3V     10mA         0.03W
STS3215 x18 (idle)    12V      1.8A         21.6W
STS3215 x18 (moving)  12V      6A           72W
STS3215 x18 (peak)    12V      18A          216W (stall)
RC Receiver           5V       50mA         0.25W
────────────────────────────────────────────────────
Total idle:                                 ~23W
Total walking:                              ~73W
Total peak:                                 ~217W
```

**Estimaciones realistas:**
- **Idle/Standing:** ~23W
- **Walking normal:** ~50-60W (no todos los servos a full load)
- **Walking rápido:** ~80-90W
- **Subir escalera:** ~120W
- **Peak (stall):** 217W (máximo teórico, evitar)

**Batería recomendada FASE 3:**
- LiPo 3S 11.1V 5000-6000mAh
- Autonomía estimada:
  - Idle: ~2 horas
  - Walking normal: ~45-60min
  - Walking intenso: ~30min

### FASE 5: Con Jetson Orin Nano (v2)

```
Componente                Voltaje  Corriente    Potencia
──────────────────────────────────────────────────────
ESP32                     5V       200mA        1W
Jetson Orin Nano          5V       3A (15W)     15W
  - 10W mode (efficiency)                       10W
  - 15W mode (performance)                      15W
  - 25W mode (max)                              25W
IMU BNO085                3.3V     10mA         0.03W
Camera (USB/CSI)          5V       500mA        2.5W
  - IMX219 (8MP)                                1.5W
  - RealSense D435i                             3W
Audio (USB mic + speaker) 5V       300mA        1.5W
STS3215 x18 (walking)     12V      6A           72W
WiFi (included Jetson)    -        -            (incluido)
──────────────────────────────────────────────────────
Total idle:                                     ~38W (10W Jetson)
Total walking + AI:                             ~92W (15W Jetson)
Total intensive (SLAM+Nav):                     ~105W (25W Jetson)
Total peak:                                     ~241W
```

**Escenarios de uso:**

| Modo | Descripción | Potencia | Duración (6000mAh) |
|------|-------------|----------|---------------------|
| Idle + monitoring | Parado, cámaras activas | ~40W | 100min |
| Patrol slow | Walking lento, SLAM ligero | ~70W | 57min |
| Navigation | Walking normal, SLAM+Nav | ~95W | 42min |
| Intensive | Walking rápido, object detection, voz | ~110W | 36min |
| Peak | Todos los sistemas @ max | ~240W | 16min |

**Batería recomendada FASE 5:**
- LiPo 3S 11.1V 6000-8000mAh (alta descarga, 30C+)
- Autonomía realista: **30-45min** operación autónoma normal
- Autonomía intensiva: **20-30min**

## Arquitectura de Distribución de Energía

### Topología Recomendada

```
┌─────────────────────────────────────────────────────────┐
│                    LiPo 3S 11.1V                        │
│                    (6000-8000mAh)                       │
└────────────┬──────────────────────────────┬─────────────┘
             │                              │
             │                              │
        XT60 Connector              Emergency Switch
             │                              │
             ├──────────────────────────────┤
             │                              │
   ┌─────────┴─────────┐         ┌─────────┴──────────┐
   │  Buck 12V → 12V   │         │  Buck 11.1V → 5V   │
   │  (Servos)         │         │  (Logic/Jetson)    │
   │  10A continuous   │         │  8A continuous     │
   │  15A peak         │         │  (40W capable)     │
   └─────────┬─────────┘         └─────────┬──────────┘
             │                              │
             │                              │
     ┌───────┴───────┐              ┌───────┴────────────┐
     │  STS3215 x18  │              │  ESP32 (5V)        │
     │  Serial Bus   │              │  Jetson (5V)       │
     │  Capacitor    │              │  Camera (5V)       │
     │  3300µF/16V   │              │  Audio (5V)        │
     └───────────────┘              │  IMU (3.3V via ESP)│
                                    └────────────────────┘
```

### Componentes Clave

#### 1. Buck Converter Servos (12V)

**Especificaciones:**
- Input: 10-13V (LiPo 3S)
- Output: 12V @ 10A continuous, 15A peak
- Eficiencia: >90%

**Opciones:**
- **DROK LM2596** (barato, ~€8, 10A)
- **Pololu D36V50F12** (profesional, ~€25, 5A) ×2 paralelo
- **UBEC 12V 10A** (hobby RC, ~€15)

**Características requeridas:**
- Bajo ripple (<50mV)
- Over-current protection
- Thermal shutdown
- Montaje con disipador

#### 2. Buck Converter Logic/Jetson (5V)

**Especificaciones:**
- Input: 10-13V (LiPo 3S)
- Output: 5V @ 8A continuous (40W)
- Eficiencia: >92%

**Opciones:**
- **Pololu D36V50F5** (~€25, 5A) - insuficiente solo
- **TPS54620 custom board** (mejor eficiencia)
- **LM2596HV 5V 8A** (~€12)
- **UBEC 5V 10A** (hobby RC, ~€18)

**Para Jetson específicamente:**
- USB-C PD capable (Jetson prefiere USB-C)
- O usar barrel jack 5V con adaptador oficial

#### 3. Capacitor Bank (Servos)

**Problema:** Servos generan spikes de corriente al arrancar (~2-3A cada uno)

**Solución:**
- **3300µF 16V electrolytic** cerca del bus de servos
- O múltiples 1000µF en paralelo
- Low ESR (<50mΩ)

**Cálculo:**
```
Energy spike: E = 0.5 × C × V²
3300µF @ 12V: E = 0.5 × 3300e-6 × 12² = 0.24J

Para 6 servos arrancando simultáneo (~12A spike):
ΔV = I × Δt / C
ΔV = 12A × 0.01s / 3300µF = 36mV drop

✅ Aceptable (< 50mV)
```

#### 4. Batería LiPo 3S

**Especificaciones mínimas:**
- **Capacidad:** 6000-8000mAh
- **Discharge rate:** 30C (permite 180-240A burst)
- **Configuración:** 3S (11.1V nominal, 12.6V full, 9V cutoff)
- **Conector:** XT60 o XT90
- **Balance connector:** JST-XH 4pin

**Opciones recomendadas:**

| Modelo | Capacidad | C-rating | Peso | Precio |
|--------|-----------|----------|------|--------|
| Turnigy 3S 5000mAh 30C | 5000mAh | 30C | ~420g | ~€35 |
| Gens Ace 3S 6000mAh 40C | 6000mAh | 40C | ~480g | ~€50 |
| Tattu 3S 8000mAh 25C | 8000mAh | 25C | ~630g | ~€70 |

**Recomendación:** Gens Ace 6000mAh 40C (balance peso/autonomía)

#### 5. Battery Management

**Componentes necesarios:**

**Voltage monitor:**
- Buzzer alarm 3S (10.5V warning, 9.5V critical)
- ~€3

**Balance charger:**
- iMAX B6 o similar
- 3S compatible
- ~€25-40

**Low-voltage cutoff (LVC):**
- HW automático: módulo LVC 3S (~€8)
- SW en ESP32: monitoreo via voltage divider

```cpp
// ESP32 voltage monitor
#define VBAT_PIN 34  // ADC1_CH6
#define VDIV_R1 100000  // 100kΩ
#define VDIV_R2 33000   // 33kΩ (divider para 12.6V → 3.3V)

float readBatteryVoltage() {
    int raw = analogRead(VBAT_PIN);
    float vADC = (raw / 4095.0) * 3.3;
    float vBat = vADC * (VDIV_R1 + VDIV_R2) / VDIV_R2;
    return vBat;
}

void checkBattery() {
    float v = readBatteryVoltage();
    if (v < 9.5) {
        // CRITICAL: emergency shutdown
        emergencyStop();
    } else if (v < 10.0) {
        // WARNING: return to base
        returnHome();
    }
}
```

## Impacto en Diseño Mecánico

### Peso Total Estimado

```
Componente                 Peso
────────────────────────────────────
Estructura PETG (body)     ~300g
Piernas x6 (PETG)          ~180g
STS3215 x18                ~1120g (62g × 18)
ESP32 + PCB                ~30g
IMU + RC receiver          ~20g
Batería 6000mAh            ~480g
Buck converters x2         ~60g
Cables + conectores        ~80g
────────────────────────────────────
Subtotal FASE 3:           ~2270g

Jetson Orin Nano           ~100g
Camera + mount             ~50g
Audio                      ~30g
────────────────────────────────────
Total FASE 5:              ~2450g
```

**Implicaciones:**
- Carga por pierna: ~410g (2450g / 6)
- Torque requerido por servo:
  - Tibia (mayor brazo): ~410g × 13cm / 2 = **2.7kg·cm** (nominal)
  - Femur: ~400g × 8cm / 2 = **1.6kg·cm**
  - Coxa: ~400g × 5.5cm / 2 = **1.1kg·cm**

**Margen de seguridad STS3215 (20kg·cm):**
- Tibia: 20 / 2.7 = **7.4× safety margin** ✅
- Femur: 20 / 1.6 = **12.5×** ✅
- Coxa: 20 / 1.1 = **18×** ✅

**Excelente margen**, permite:
- Cargas adicionales (hasta +1kg)
- Aceleración rápida
- Subir pendientes
- Terreno irregular

### Ubicación de Componentes

```
Vista superior body:

    ┌────────────────────────────────┐
    │         [Jetson]               │
    │      (centro superior)         │
    │                                │
    │  [ESP32]          [Camera]     │
    │  (delantero)      (frontal)    │
    │                                │
    │                                │
    │      [Batería 6000mAh]         │
    │    (centro bajo, CG óptimo)    │
    │                                │
    │  [Buck 12V]      [Buck 5V]     │
    │  (lateral izq)   (lateral der) │
    └────────────────────────────────┘

Vista lateral:
                [Jetson]
               ┌──────┐
    [Camera]   │ ESP  │
         │     └──────┘
         │   ┌──────────┐
         └───┤ Batería  │ ← CG bajo
             └──────────┘
            /            \
          Legs           Legs
```

**Principios:**
1. **Batería en centro bajo** → CG óptimo, estabilidad
2. **Jetson arriba** → acceso fácil, disipación calor
3. **ESP32 frontal** → cerca de servos (cables cortos)
4. **Bucks laterales** → distribución peso, cooling
5. **Cámara frontal** → campo de visión despejado

## Consideraciones Térmicas

### Heat Dissipation

**Fuentes de calor:**

| Componente | Potencia | Heat | Cooling |
|------------|----------|------|---------|
| Jetson (25W) | 25W | ~8W | Heatsink + fan |
| Buck 12V (10A) | ~7W loss | 7W | Heatsink |
| Buck 5V (8A) | ~3W loss | 3W | Heatsink |
| Servos x18 | variable | ~10W | Passive (ventilación) |

**Soluciones:**

1. **Jetson Orin Nano:**
   - Usar heatsink oficial + fan (5V, ~0.2A)
   - Thermal paste
   - Ventilación natural (aberturas en body)

2. **Buck Converters:**
   - Heatsinks con thermal pad
   - Ubicación con flujo aire
   - Considerar fan 40mm shared (FASE 5)

3. **Servos:**
   - Monitoring vía STS3215 `readTemperature()`
   - Thermal throttling en firmware:
     ```cpp
     if (temp > 60°C) {
         reduceSpeed(50%);
     }
     if (temp > 70°C) {
         emergencyStop();
     }
     ```
   - Pies TPU (aislamiento térmico del suelo)

## Cableado y Conectores

### Standards

**Power:**
- XT60 (batería principal)
- Anderson Powerpole (alternativa profesional)

**Servos:**
- JST-3pin o cable nativo STS3215
- Bus daisy-chain

**Logic:**
- JST-XH (2-4 pin) para sensores
- Dupont (prototipado)
- Molex (producción)

### Cable Gauges

| Conexión | Corriente | AWG | Longitud max |
|----------|-----------|-----|--------------|
| Batería → Bucks | 12A | 14 AWG | 10cm |
| Buck → Servos bus | 10A | 16 AWG | 20cm |
| Buck → Jetson | 3A | 20 AWG | 15cm |
| Buck → ESP32 | 0.5A | 22 AWG | 10cm |
| Servo data bus | signal | 26 AWG | <30cm total |

**Cable routing:**
- Interna al body (sin loops externos)
- Amarrado con zip ties
- Separar power de signal (ruido)
- Shielded para serial servo bus (opcional pero recomendado)

## Bill of Materials - Energización

### FASE 3 (Hexapod Completo)

| Item | Spec | Cantidad | Precio | Total |
|------|------|----------|--------|-------|
| LiPo 3S 6000mAh 40C | Gens Ace | 1 | €50 | €50 |
| Buck 12V 10A | LM2596HV | 1 | €12 | €12 |
| Buck 5V 5A | Pololu D36V50F5 | 1 | €25 | €25 |
| Capacitor 3300µF 16V | Low ESR | 2 | €2 | €4 |
| XT60 connector pair | Male/Female | 2 | €2 | €4 |
| Voltage alarm 3S | Buzzer | 1 | €3 | €3 |
| Switch 10A | Toggle | 1 | €3 | €3 |
| Cables 14 AWG | Silicone | 1m | €5 | €5 |
| Heat shrink | Assorted | - | €3 | €3 |
| **Subtotal FASE 3** | | | | **€109** |

### FASE 5 (+ Jetson IA)

| Item | Spec | Cantidad | Precio | Total |
|------|------|----------|--------|-------|
| *Carry over FASE 3* | (reutilizar) | - | - | €0 |
| LiPo 3S 8000mAh | Upgrade (opcional) | 1 | €70 | €70 |
| Buck 5V 8A upgrade | UBEC 10A | 1 | €18 | €18 |
| Jetson heatsink+fan | Official | 1 | €15 | €15 |
| Voltage monitor PCB | ADC + display | 1 | €8 | €8 |
| **Subtotal FASE 5 upgrades** | | | | **€111** |

**Total sistema energía:** ~€220 (FASE 3) + ~€110 (upgrade FASE 5) = **€330**

## Monitoreo y Telemetría

### Datos a Monitorear (ESP32)

```cpp
struct PowerTelemetry {
    float batteryVoltage;       // Via voltage divider
    float batteryCurrent;       // Via INA219 sensor
    float batteryPercentage;    // Estimado
    float powerConsumption;     // V × I
    
    // Por servo (via STS3215)
    uint8_t servoTemps[18];     // °C
    uint16_t servoLoads[18];    // 0-1023
    uint8_t servoVoltages[18];  // 0.1V units
    
    // Buck outputs
    float voltage12V;
    float voltage5V;
    
    // Warnings
    bool batteryLow;            // <10V
    bool batteryCritical;       // <9.5V
    bool servoOverheat;         // any >60°C
    bool overcurrent;           // >12A
};
```

### Display (Opcional)

**OLED 0.96" I2C** en body exterior:
- Battery %
- Voltage
- Current draw
- Runtime remaining
- Warnings

**Costo:** ~€5

## Recomendaciones Finales

### FASE 1 (Pierna Única)

**Mínimo viable:**
- LiPo 3S 2200mAh (~€20)
- Buck 12V 5A (~€8)
- Buck 5V 2A (~€5)
- Capacitor 1000µF (~€1)
- **Total:** ~€34

### FASE 3 (Hexapod Completo)

**Recomendado:**
- LiPo 3S 6000mAh 40C (~€50)
- Buck 12V 10A (~€12)
- Buck 5V 5A (~€25)
- Capacitor 3300µF ×2 (~€4)
- Voltage alarm (~€3)
- Safety switch (~€3)
- **Total:** ~€97

### FASE 5 (Con Jetson)

**Upgrades necesarios:**
- Buck 5V 8A (~€18, reemplazar anterior)
- LiPo 3S 8000mAh (€70, opcional upgrade)
- Heatsink+fan Jetson (~€15)
- **Total adicional:** ~€33-103

**Inversión total energización:** €97 (FASE 3) + €33 (FASE 5 mínimo) = **€130**

## Safety Features Críticos

### Hardware
1. ✅ Emergency stop button (físico)
2. ✅ Low-voltage cutoff (HW o SW)
3. ✅ Voltage alarm (buzzer)
4. ✅ Over-current protection (bucks)
5. ✅ Thermal shutdown (monitoring SW)

### Software
```cpp
void safetyCheck() {
    // Battery
    if (batteryVoltage < 9.5) emergencyShutdown();
    if (batteryVoltage < 10.0) returnHome();
    
    // Servos
    for (int i = 0; i < 18; i++) {
        if (servoTemp[i] > 70) emergencyShutdown();
        if (servoTemp[i] > 60) throttleSpeed(0.5);
    }
    
    // Current
    if (totalCurrent > 15.0) {
        reduceLoad();  // Sit down or stop
    }
}
```

### Fuse Protection

**Recomendado:**
- Fuse 15A inline batería → XT60
- Protección contra cortocircuito
- Reemplazable (~€1 cada uno)

---

**Documento vivo:** Actualizar según pruebas reales de consumo en FASE 3.
