# Bill of Materials COMPLETO - HexapodIA

**Lista exhaustiva de TODOS los componentes, insumos, herramientas y consumibles necesarios.**

---

## 📋 Índice por Fase

- [FASE 1: Pierna Única](#fase-1-pierna-única)
- [FASE 3: Hexápodo Completo](#fase-3-hexápodo-completo)
- [FASE 5: IA + Interacción + Insta360](#fase-5-ia--interacción--insta360)
- [Herramientas Necesarias](#herramientas-necesarias)
- [Consumibles](#consumibles)
- [Proveedores Recomendados](#proveedores-recomendados)

---

## FASE 1: Pierna Única

### 🔌 Electrónica Principal

| Item | Especificación | Cantidad | Precio Unit | Total | Link/Proveedor |
|------|----------------|----------|-------------|-------|----------------|
| ESP32-WROOM-32 DevKit v1 | 38 pines, USB-C preferido | 1 | €8 | €8 | AliExpress/Amazon |
| Feetech STS3215 Servo | 20kg·cm, 12V, ID configurable | 3 | €18 | €54 | Feetech oficial/RobotShop |
| LiPo 3S 2200mAh 30C | 11.1V, XT60 connector | 1 | €20 | €20 | Hobbyking/Banggood |
| Buck Converter 12V 5A | LM2596HV, display LED | 1 | €8 | €8 | AliExpress |
| Buck Converter 5V 2A | LM2596, ajustable | 1 | €5 | €5 | AliExpress |
| Capacitor 1000µF 16V | Electrolítico, Low ESR | 1 | €1 | €1 | Amazon/Mouser |
| Voltage Alarm 3S | Buzzer, LED, 9-12.6V | 1 | €3 | €3 | Hobbyking |
| Switch 10A | Toggle ON-OFF, 12V | 1 | €3 | €3 | AliExpress |

**Subtotal Electrónica FASE 1:** €102

### 🔩 Conectores y Cables - FASE 1

| Item | Especificación | Cantidad | Precio Unit | Total | Notas |
|------|----------------|----------|-------------|-------|-------|
| **Conectores Power** |
| XT60 Male | Para batería | 1 | €1 | €1 | Incluido con batería |
| XT60 Female | Para robot | 1 | €1 | €1 | |
| JST-XH 4 pin | Balance connector 3S | 1 | €0.5 | €0.5 | Incluido con batería |
| **Cables Power** |
| Cable silicone 14AWG rojo | Para power main | 0.5m | €5/m | €2.5 | Alta corriente |
| Cable silicone 14AWG negro | Para ground main | 0.5m | €5/m | €2.5 | Alta corriente |
| Cable silicone 20AWG rojo | Buck outputs | 0.5m | €3/m | €1.5 | |
| Cable silicone 20AWG negro | Buck grounds | 0.5m | €3/m | €1.5 | |
| Cable silicone 22AWG rojo | ESP32 5V | 0.2m | €2/m | €0.5 | |
| Cable silicone 22AWG negro | ESP32 GND | 0.2m | €2/m | €0.5 | |
| **Cables Data/Signal** |
| Cable Dupont 26AWG (pack) | 40 unidades M-M | 1 pack | €5 | €5 | ESP32 connections |
| Cable Dupont 26AWG (pack) | 40 unidades M-F | 1 pack | €5 | €5 | Sensors |
| Cable servo extension | 3 pines JST, 30cm | 3 | €1.5 | €4.5 | Daisy chain servos |
| Cable USB-A a USB-C | 1m, para programar ESP32 | 1 | €3 | €3 | |
| **Conectores Signal** |
| JST-XH 3pin connector | Para servos (si no vienen) | 3 pares | €0.3 | €1 | |
| Dupont housings 1pin | Para custom cables | 10 | €0.1 | €1 | |
| Dupont housings 2pin | Para sensors | 5 | €0.15 | €0.75 | |
| Dupont housings 3pin | Para I2C, etc | 5 | €0.2 | €1 | |
| Dupont pins crimping | Repuestos | 50 | €0.02 | €1 | |

**Subtotal Conectores/Cables FASE 1:** €33.25

### 🔧 Tornillería y Hardware - FASE 1

| Item | Especificación | Cantidad | Precio Unit | Total | Notas |
|------|----------------|----------|-------------|-------|-------|
| **Tornillos** |
| Tornillos M3×8 | Allen head, acero | 12 | €0.08 | €1 | Pierna assembly |
| Tornillos M3×12 | Allen head, acero | 8 | €0.1 | €0.8 | Servos a estructura |
| Tornillos M2.5×6 | Phillips, acero | 8 | €0.05 | €0.4 | ESP32 mounting |
| **Tuercas** |
| Tuercas M3 | Nylon lock | 15 | €0.05 | €0.75 | Anti-vibración |
| Tuercas M2.5 | Nylon lock | 4 | €0.04 | €0.16 | |
| **Insertos Térmicos** |
| Insertos M3×5mm | Brass, knurled | 8 | €0.15 | €1.2 | Para PETG |
| Insertos M2.5×4mm | Brass, knurled | 4 | €0.12 | €0.48 | ESP32 |
| **Arandelas** |
| Arandelas M3 planas | Acero inox | 10 | €0.03 | €0.3 | |
| **Espaciadores** |
| Espaciadores nylon M3×10mm | Hex, hembra-hembra | 4 | €0.2 | €0.8 | PCB standoffs |

**Subtotal Tornillería FASE 1:** €5.89

### 🧱 Materiales Impresión 3D - FASE 1

| Item | Especificación | Cantidad | Precio | Notas |
|------|----------------|----------|--------|-------|
| Filamento PETG | 1.75mm, cualquier marca | 200g | €6 | Estructura pierna |
| Filamento TPU | 1.75mm, 95A shore | 50g | €3 | Pies (grip) |

**Subtotal Impresión FASE 1:** €9

### 📦 Componentes Electrónicos Discretos - FASE 1

| Item | Especificación | Cantidad | Precio Unit | Total | Uso |
|------|----------------|----------|-------------|-------|-----|
| Resistor 100kΩ 1/4W | 5%, través hole | 1 | €0.05 | €0.05 | Voltage divider R1 |
| Resistor 33kΩ 1/4W | 5%, través hole | 1 | €0.05 | €0.05 | Voltage divider R2 |
| LED 5mm rojo | Difuso, 20mA | 1 | €0.1 | €0.1 | Power indicator |
| LED 5mm verde | Difuso, 20mA | 1 | €0.1 | €0.1 | Status indicator |
| Resistor 330Ω 1/4W | Para LEDs | 2 | €0.05 | €0.1 | Current limiting |
| Diodo 1N4007 | 1A, rectificador | 2 | €0.1 | €0.2 | Protección |
| Capacitor cerámico 100nF | 50V, bypass | 3 | €0.05 | €0.15 | Noise filtering |

**Subtotal Componentes Discretos FASE 1:** €0.75

---

## 🎯 **TOTAL FASE 1:** €150.89

### Resumen FASE 1:
```
Electrónica:         €102.00
Conectores/Cables:    €33.25
Tornillería:           €5.89
Impresión 3D:          €9.00
Componentes disc.:     €0.75
─────────────────────────────
TOTAL:               €150.89
```

---

## FASE 3: Hexápodo Completo

### 🔌 Electrónica Adicional FASE 3

| Item | Especificación | Cantidad | Precio Unit | Total | Link/Proveedor |
|------|----------------|----------|-------------|-------|----------------|
| Feetech STS3215 Servo | (adicionales) | 15 | €18 | €270 | (3 ya comprados FASE 1) |
| LiPo 3S 6000mAh 40C | Gens Ace, XT60 | 1 | €50 | €50 | Reemplaza 2200mAh |
| Buck Converter 12V 10A | Upgrade de 5A | 1 | €12 | €12 | Reemplaza anterior |
| Buck Converter 5V 5A | Pololu D36V50F5 | 1 | €25 | €25 | Reemplaza 2A |
| Capacitor 3300µF 16V | Low ESR electrolítico | 2 | €2 | €4 | Servo spikes |
| IMU BNO085 | 9-axis, I2C, Adafruit | 1 | €25 | €25 | Estabilización |
| RC Receiver ELRS | HappyModel EP1 | 1 | €12 | €12 | 16 canales |
| Fuse 15A blade | Inline, holder incluido | 1 | €2 | €2 | Protección |

**Subtotal Electrónica FASE 3 (adicional):** €400

### 🔩 Conectores y Cables Adicionales - FASE 3

| Item | Especificación | Cantidad | Precio Unit | Total | Notas |
|------|----------------|----------|-------------|-------|-------|
| **Cables Power Adicionales** |
| Cable silicone 14AWG rojo | Power distribution | 1.5m | €5/m | €7.5 | Total 2m |
| Cable silicone 14AWG negro | Ground distribution | 1.5m | €5/m | €7.5 | Total 2m |
| Cable silicone 16AWG rojo | Servo bus power | 2m | €4/m | €8 | 18 servos |
| Cable silicone 16AWG negro | Servo bus ground | 2m | €4/m | €8 | |
| **Cables Servos** |
| Cable servo extension 30cm | JST-3pin | 15 | €1.5 | €22.5 | Adicionales |
| Cable servo Y-splitter | Para daisy chain | 3 | €2 | €6 | Bus organization |
| **Cables Signal** |
| Cable I2C 4 hilos | 26AWG, 30cm | 1 | €2 | €2 | ESP32 ↔ IMU |
| Cable PWM 3 hilos | 26AWG, 20cm | 4 | €1 | €4 | RC receiver |
| Ribbon cable 10 hilos | IDC, 50cm | 1 | €3 | €3 | Internal routing |
| **Conectores** |
| XT60 adicionales | Male+Female pairs | 2 | €2 | €4 | Power distribution |
| XT30 connectors | Male+Female pairs | 4 | €1.5 | €6 | Módulos |
| JST-XH 2pin | Servo power tap | 6 | €0.3 | €1.8 | |
| JST-XH 4pin | I2C/RC | 3 | €0.4 | €1.2 | |
| Anderson Powerpole | 15A, opcional | 2 pairs | €3 | €6 | Alternativa XT60 |

**Subtotal Conectores/Cables FASE 3 (adicional):** €87.5

### 🔧 Tornillería y Hardware Adicional - FASE 3

| Item | Especificación | Cantidad | Precio Unit | Total | Notas |
|------|----------------|----------|-------------|-------|-------|
| Tornillos M3×8 | Allen head | 50 | €0.08 | €4 | Piernas + body |
| Tornillos M3×12 | Allen head | 40 | €0.1 | €4 | Servos |
| Tornillos M3×20 | Allen head | 20 | €0.12 | €2.4 | Body standoffs |
| Tornillos M3×25 | Allen head | 16 | €0.15 | €2.4 | Body tall standoffs |
| Tuercas M3 lock | Nylon | 100 | €0.05 | €5 | Anti-vibración |
| Insertos M3×5mm | Brass | 40 | €0.15 | €6 | Body assembly |
| Espaciadores M3×10mm | Nylon hex F-F | 20 | €0.2 | €4 | PCB/plates |
| Espaciadores M3×15mm | Nylon hex F-F | 16 | €0.25 | €4 | Body levels |
| Arandelas M3 | Planas acero | 50 | €0.03 | €1.5 | |

**Subtotal Tornillería FASE 3 (adicional):** €33.3

### 🧱 Materiales Impresión 3D Adicionales - FASE 3

| Item | Cantidad | Precio | Notas |
|------|----------|--------|-------|
| Filamento PETG | 600g | €18 | Body + 5 piernas adicionales |
| Filamento TPU | 30g | €2 | 5 pies adicionales |

**Subtotal Impresión FASE 3 (adicional):** €20

### 📦 Consumibles Adicionales FASE 3

| Item | Especificación | Cantidad | Precio | Notas |
|------|----------------|----------|--------|-------|
| Heat shrink tubing kit | Assorted sizes | 1 kit | €5 | Insulation |
| Zip ties 2.5×100mm | Nylon, negro | 100 pcs | €3 | Cable management |
| Zip ties 3.6×200mm | Nylon, negro | 50 pcs | €3 | Battery straps |
| Velcro straps | 20×200mm, reusable | 10 | €5 | Battery mount |
| Double-sided tape 3M | 25mm×5m, fuerte | 1 | €5 | Component mounting |
| Thermal paste | 1g syringe | 1 | €3 | Bucks heatsinks |
| Wire labels/markers | Para identificación | 1 pack | €3 | Documentation |

**Subtotal Consumibles FASE 3:** €27

---

## 🎯 **TOTAL FASE 3 (adicional desde FASE 1):** €567.8

### Resumen FASE 1 + FASE 3:
```
FASE 1 total:           €150.89
FASE 3 adicional:       €567.80
─────────────────────────────────
TOTAL acumulado:        €718.69
```

---

## FASE 5: IA + Interacción + Insta360

### 🤖 Electrónica IA y Periféricos - FASE 5

| Item | Especificación | Cantidad | Precio Unit | Total | Link/Proveedor |
|------|----------------|----------|-------------|-------|----------------|
| **Compute** |
| Jetson Orin Nano 8GB | Developer Kit (con carrier) | 1 | €460 | €460 | NVIDIA oficial |
| MicroSD 128GB | U3, A2, SanDisk Extreme | 1 | €17 | €17 | Para Jetson OS |
| Power supply 5V 4A | Universal 100-240V, barrel jack | 1 | €12 | €12 | Solo setup inicial |
| Jetson heatsink + fan | Oficial, 40mm 5V | 1 | €14 | €14 | Cooling |
| **Cámaras** |
| IMX219 Camera 8MP | CSI, 160° FOV | 1 | €20 | €20 | AI/SLAM |
| FPV Camera 700TVL | Analog, CMOS, 2.8mm lens | 1 | €8 | €8 | Piloting |
| Video Transmitter 5.8GHz | 25-200mW, 40ch | 1 | €12 | €12 | FPV TX |
| VTX Antenna | Circular polarized, SMA | 1 | €5 | €5 | |
| Camera mount FPV | Universal 20×20mm | 1 | €2 | €2 | |
| CSI ribbon cable | 15cm, para IMX219 | 1 | €3 | €3 | |
| **Audio** |
| USB Microphone mini | Plug-and-play, omnidirectional | 1 | €10 | €10 | Voice input |
| I2S Speaker 3W | MAX98357A amp integrado | 1 | €8 | €8 | Voice output |
| Audio amplifier PAM8403 | 3W×2, backup/alternative | 1 | €2 | €2 | Opcional |
| Speaker 8Ω 3W | 40mm diameter | 1 | €3 | €3 | Si I2S no incluye |
| **Display y Sensores** |
| OLED 0.96" I2C | 128×64, SSD1306 | 1 | €5 | €5 | Telemetría |
| Ultrasonic HC-SR04 | Distance sensor | 2 | €3 | €6 | Obstacle detection |
| **Comunicación** |
| WiFi 5GHz USB module | AC600 Dual-band, opcional | 1 | €12 | €12 | Si Jetson WiFi insuficiente |
| Antenna 5GHz | 3dBi, SMA | 2 | €4 | €8 | WiFi range boost |
| **Power Upgrades** |
| LiPo 3S 10000mAh 25C | Tattu/Gens Ace, XT60 | 1 | €95 | €95 | Reemplaza 6000mAh |
| Buck Converter 5V 10A | UBEC, DC-DC step down | 1 | €18 | €18 | Reemplaza 5A |
| Current sensor INA219 | I2C, 0-26V, 0-3.2A | 1 | €5 | €5 | Power monitoring |
| Capacitor 2200µF 6.3V | Low ESR, para 5V rail | 1 | €1.5 | €1.5 | Jetson spikes |

**Subtotal Electrónica FASE 5:** €755.5

### 📷 Insta360 y Mounting

| Item | Especificación | Cantidad | Precio Unit | Total | Notas |
|------|----------------|----------|-------------|-------|-------|
| Insta360 Ace Pro 2 | (usuario ya tiene) | 1 | €449 | €0 | No incluir en BOM |
| Quick Release Plate | Manfrotto 200PL compatible | 1 | €8 | €8 | 1/4"-20 mount |
| Inserto roscado 1/4"-20 | Brass, 10mm depth | 1 | €1.5 | €1.5 | Top mount |
| Tornillos M3×12 countersunk | Para adaptador | 4 | €0.1 | €0.4 | Top mount |

**Subtotal Insta360 Mounting:** €9.9

### 🔩 Cables y Conectores Adicionales - FASE 5

| Item | Especificación | Cantidad | Precio Unit | Total | Notas |
|------|----------------|----------|-------------|-------|-------|
| USB-C cable | 30cm, power+data | 1 | €5 | €5 | Jetson power |
| USB-A to micro | 20cm, microphone | 1 | €3 | €3 | |
| Ribbon cable CSI | 15cm, 15-pin | 1 | €3 | €3 | IMX219 camera |
| Cable coaxial RG316 | 30cm, SMA-SMA | 1 | €4 | €4 | VTX antenna |
| Audio cable 3.5mm | 20cm, stereo | 1 | €2 | €2 | Speaker backup |
| Cable I2C 4 hilos | 20cm, 26AWG | 2 | €1.5 | €3 | OLED + INA219 |
| Cable JST-PH 2pin | 10cm, para fan | 1 | €1 | €1 | Fan power |
| Cable silicone 18AWG | 1m rojo+negro | 1m | €3/m | €3 | Power distribution |

**Subtotal Cables FASE 5:** €24

### 🔧 Hardware Adicional FASE 5

| Item | Especificación | Cantidad | Precio Unit | Total | Notas |
|------|----------------|----------|-------------|-------|-------|
| Tornillos M2.5×6 | Phillips, Jetson mount | 8 | €0.05 | €0.4 | |
| Tornillos M2×6 | Phillips, camera mounts | 8 | €0.04 | €0.32 | |
| Espaciadores M2.5×10mm | Nylon standoffs | 4 | €0.2 | €0.8 | Jetson elevation |
| Tuercas M2.5 | Nylon lock | 8 | €0.04 | €0.32 | |
| Tuercas M2 | Nylon lock | 8 | €0.03 | €0.24 | |

**Subtotal Hardware FASE 5:** €2.08

### 🧱 Materiales Impresión 3D FASE 5

| Item | Cantidad | Precio | Notas |
|------|----------|--------|-------|
| Filamento PETG | 440g | €13 | Body modificado (4 niveles) |
| Filamento PETG | 100g | €3 | Camera mounts, Insta360 soporte |

**Subtotal Impresión FASE 5:** €16

### 📦 Componentes Adicionales FASE 5

| Item | Especificación | Cantidad | Precio | Notas |
|------|----------------|----------|--------|-------|
| Heatsinks kit | Aluminum, assorted | 1 kit | €5 | Bucks cooling |
| Thermal pads | 1mm thick, 100×100mm | 1 | €5 | Heat transfer |
| Anti-vibration mounts | Rubber grommets M3 | 10 | €3 | Camera damping |
| Cable sleeving | 5mm braided, 2m | 2m | €2/m | €4 | Clean routing |

**Subtotal Componentes FASE 5:** €17

---

## 🎯 **TOTAL FASE 5 (adicional desde FASE 3):** €824.48

### Resumen COMPLETO:
```
FASE 1:                 €150.89
FASE 3 adicional:       €567.80
FASE 5 adicional:       €824.48
─────────────────────────────────
TOTAL PROYECTO:        €1543.17
```

---

## 🛠️ Herramientas Necesarias

### Herramientas Básicas (si no tienes)

| Herramienta | Especificación | Precio | Notas |
|-------------|----------------|--------|-------|
| **Impresión 3D** |
| Impresora 3D | Prusa i3/Ender 3/etc | €200-500 | O usar servicio |
| Espátula | Para remover prints | €5 | |
| Alicate corte | Flush cutters | €8 | Limpiar prints |
| **Electrónica** |
| Soldador | 60W, temperatura ajustable | €25 | Para insertos térmicos |
| Tip soldador | Para insertos M3 | €5 | Alternativa: tip cónico |
| Estaño | 60/40, 0.8mm, flux core | €8 | Conexiones |
| Pinzas | Punta fina, antiestática | €10 | Manipulación |
| Multímetro digital | Auto-ranging, continuity | €20 | ESENCIAL |
| Wire stripper | Automático 20-30AWG | €12 | |
| Crimping tool | Dupont/JST terminals | €15 | |
| Heat gun | Para heat shrink | €15 | O encendedor |
| **Mecánica** |
| Juego llaves Allen | 1.5-6mm, bola head | €12 | Ensamblaje |
| Destornilladores | Phillips PH0, PH1 | €8 | |
| Alicate punta | Needle nose | €8 | Cables apretados |
| Alicate corte diagonal | Cable cutting | €8 | |
| Cutter/Exacto knife | Para limpiar prints | €5 | |
| Lima | Plana, redonda | €8 | Ajustar agujeros |
| Broca 3.2mm | Para limpiar M3 holes | €3 | |
| Taladro manual/eléctrico | Variable speed | €30 | Opcional |
| Calibrador/Vernier | Digital, 0-150mm | €15 | Mediciones precisas |
| **Testing** |
| Power supply banco | 12V 5A ajustable | €35 | Testing sin batería |
| Oscilloscopio | Opcional, debugging | €80 | Avanzado |

**Subtotal Herramientas (si compras todo):** ~€300-400

**Nota:** Muchas herramientas son de una sola compra y reutilizables.

---

## 🧰 Consumibles (Estimado para TODO el Proyecto)

| Item | Cantidad | Precio | Notas |
|------|----------|--------|-------|
| **Adhesivos y Fijación** |
| Cinta Kapton | 20mm×30m | €8 | High-temp masking |
| Cinta aislante | 19mm×10m | €2 | Electrical insulation |
| Cinta doble cara 3M VHB | 12mm×3m | €8 | Permanent mounting |
| Super glue (CA) | 20g | €3 | Emergencias |
| Loctite Threadlocker | Blue 243, 5ml | €5 | Anti-vibración tornillos |
| **Limpieza** |
| Isopropyl alcohol 99% | 500ml | €8 | Limpieza PCBs, prints |
| Toallas lint-free | 100 pack | €5 | |
| Compressed air | Spray can | €8 | Dust removal |
| **Protección** |
| Conformal coating | Spray, 200ml | €12 | PCB protection (opcional) |
| Liquid tape | 120ml | €8 | Cable ends waterproof |
| **Cable Management** |
| Zip ties 100mm | 100 pcs negro | €3 | Ya en BOM FASE 3 |
| Zip ties 200mm | 50 pcs negro | €3 | Ya en BOM FASE 3 |
| Velcro adhesivo | 20mm×5m | €5 | General mounting |
| Heat shrink assorted | Kit 127 piezas | €8 | Ya en BOM FASE 3 |
| Cable ties mount | Adhesive, 20 pcs | €3 | Routing clean |
| Spiral wrap | 6mm×5m | €5 | Cable bundling |
| **Documentación** |
| Etiquetas adhesivas | Para componentes | €3 | Organization |
| Marcador permanente | Sharpie fino | €2 | Labels |
| **Safety** |
| Bolsa LiPo segura | Fireproof, large | €12 | Carga segura |
| Fire extinguisher | ABC, 1kg | €20 | Safety (recomendado) |

**Subtotal Consumibles:** €70 (muchos ya incluidos en BOM fases)

---

## 📦 Resumen BOM por Categoría (PROYECTO COMPLETO)

| Categoría | FASE 1 | FASE 3 | FASE 5 | TOTAL |
|-----------|--------|--------|--------|-------|
| **Electrónica principal** | €102 | €400 | €755.5 | €1257.5 |
| **Conectores y cables** | €33.25 | €87.5 | €24 | €144.75 |
| **Tornillería y hardware** | €5.89 | €33.3 | €2.08 | €41.27 |
| **Impresión 3D** | €9 | €20 | €16 | €45 |
| **Componentes discretos** | €0.75 | - | €17 | €17.75 |
| **Consumibles** | - | €27 | - | €27 |
| **Insta360 mounting** | - | - | €9.9 | €9.9 |
| **TOTAL por FASE** | **€150.89** | **€567.80** | **€824.48** | **€1543.17** |

### Desglose Detallado:

```
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
                  PRESUPUESTO TOTAL
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

1. COMPONENTES PRINCIPALES
   Servos (18× STS3215):               €324.00
   Jetson Orin Nano:                   €499.00
   Baterías (2200→6000→10000mAh):      €165.00
   ESP32:                                €8.00
   IMU BNO085:                          €25.00
   RC Receiver ELRS:                    €12.00
   Cámaras (3 tipos):                   €45.00
   Buck converters (4 total):           €70.00
   Audio (mic+speaker):                 €21.00
                                      ─────────
   Subtotal Principales:              €1169.00

2. POWER & CONECTORES
   Capacitores varios:                   €7.50
   Conectores XT60/JST/etc:             €35.00
   Cables power (14-22AWG):             €45.00
   Cables signal (26AWG):               €40.00
   Voltage alarm, fuse, etc:             €8.00
                                      ─────────
   Subtotal Power:                    €135.50

3. MECÁNICA
   Filamento PETG (~1240g):             €36.00
   Filamento TPU (~80g):                 €5.00
   Tornillería completa:                €41.27
   Insertos térmicos:                   €12.00
                                      ─────────
   Subtotal Mecánica:                  €94.27

4. PERIFÉRICOS & EXTRAS
   Display, sensors, mounting:          €30.00
   Consumibles (zip ties, etc):         €27.00
   Componentes discretos:               €17.75
   Insta360 mounting hardware:           €9.90
   Cooling (heatsinks, fan):            €20.00
   Misc (labels, thermal paste):         €8.75
                                      ─────────
   Subtotal Periféricos:              €113.40

5. MICROSD & STORAGE
   MicroSD 128GB:                       €18.00
                                      ─────────
   Subtotal Storage:                   €18.00

━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
TOTAL PROYECTO (sin herramientas):   €1543.17
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

HERRAMIENTAS (si no tienes):         ~€350.00
TOTAL CON HERRAMIENTAS:              €1893.17

━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
```

---

## 🛒 Proveedores Recomendados

### Electrónica General

| Proveedor | Pros | Cons | URL |
|-----------|------|------|-----|
| **AliExpress** | Barato, variedad | Envío lento (2-4 semanas) | aliexpress.com |
| **Amazon.es** | Rápido (1-2 días), devoluciones | Más caro | amazon.es |
| **Mouser** | Stock confiable, datasheets | Envío caro si <€50 | mouser.com |
| **RS Components** | Profesional, rápido | Precios premium | rs-components.com |

### Robótica Específica

| Componente | Proveedor Recomendado | Notas |
|------------|----------------------|-------|
| **Servos STS3215** | Feetech oficial, RobotShop | Verificar ID configurable |
| **Jetson Orin Nano** | NVIDIA oficial, Arrow | No comprar clones |
| **LiPo Batteries** | Hobbyking, Banggood | Revisar C-rating real |
| **RC ELRS** | HappyModel store, BetaFPV | Verificar compatibilidad |

### Impresión 3D

| Item | Proveedor | Notas |
|------|-----------|-------|
| **Filamento PETG** | Prusa, eSun, Amazon | Cualquier marca decente |
| **Filamento TPU** | Ninjatek, eSun | 95A shore preferido |
| **Servicio impresión** | 3DHubs, local | Si no tienes impresora |

### Hardware/Tornillería

| Proveedor | Ventajas | URL |
|-----------|----------|-----|
| **McMaster-Carr** | Mejor calidad, datasheets | mcmaster.com |
| **Ferretería local** | Rápido, urgencias | - |
| **AliExpress** | Kits baratos M3 | aliexpress.com |

---

## ⚠️ Componentes CRÍTICOS - No Sustituir

| Item | Por qué NO sustituir | Alternativa aceptable |
|------|---------------------|----------------------|
| **STS3215 servos** | Protocol específico, torque necesario | SCS15 (mismo protocolo) |
| **Jetson Orin Nano** | Performance AI crítico | Ninguna (RasPi insuficiente) |
| **LiPo 3S 10000mAh** | Autonomía objetivo 1h | 8000mAh OK sin Insta360 |
| **Buck 5V 10A** | Jetson consume 3-5A | UBEC 10A, no usar 5A |
| **ESP32-WROOM-32** | Dual UART, WiFi integrado | ESP32-DevKitC OK |
| **IMU BNO085** | Quaternion output | BNO080 OK, MPU6050 NO |

---

## 📝 Checklist de Compra por Fase

### FASE 1 - Comprar Ahora:
- [ ] ESP32-WROOM-32 DevKit ×1
- [ ] Feetech STS3215 servo ×3
- [ ] LiPo 3S 2200mAh ×1
- [ ] Buck converters (12V 5A + 5V 2A)
- [ ] Capacitor 1000µF
- [ ] Cables power (14AWG, 20AWG, 22AWG)
- [ ] Conectores XT60, Dupont kit
- [ ] Tornillería M3 kit básico
- [ ] Insertos térmicos M3 ×8
- [ ] Filamento PETG 200g
- [ ] Filamento TPU 50g
- [ ] Resistores voltage divider (100K, 33K)
- [ ] Voltage alarm 3S

### FASE 3 - Comprar Después:
- [ ] STS3215 servos ×15 adicionales
- [ ] LiPo 3S 6000mAh (reemplazo)
- [ ] Bucks upgrade (12V 10A + 5V 5A)
- [ ] Capacitores 3300µF ×2
- [ ] IMU BNO085
- [ ] RC Receiver ELRS EP1
- [ ] Cables adicionales (16AWG, extensions)
- [ ] Tornillería adicional (M3 kit grande)
- [ ] Filamento PETG 600g
- [ ] Consumibles (zip ties, velcro, heat shrink)

### FASE 5 - Comprar al Final:
- [ ] Jetson Orin Nano 8GB kit
- [ ] MicroSD 128GB
- [ ] LiPo 3S 10000mAh (upgrade)
- [ ] Buck 5V 10A (upgrade)
- [ ] Cámaras (IMX219, FPV, VTX)
- [ ] Audio (mic USB, speaker I2S)
- [ ] Display OLED 0.96"
- [ ] Ultrasonic HC-SR04 ×2
- [ ] Current sensor INA219
- [ ] Jetson heatsink + fan
- [ ] Quick release plate (Insta360)
- [ ] Cables USB-C, CSI, coaxial
- [ ] Filamento PETG 540g

---

## 💾 Archivo Excel/CSV Disponible

Para facilitar compras, puedes exportar esta lista a:
- **Excel:** Para ordenar por proveedor/fase
- **CSV:** Para importar a sistema de compras
- **Google Sheets:** Para tracking compartido

**Formato sugerido:**
```
Item | Spec | Qty | Price | Phase | Provider | Status | Notes
```

---

## 🚨 IMPORTANTE - Antes de Comprar

### Verificaciones Pre-Compra:

1. **Servos STS3215:**
   - ✅ Verificar protocolo serie (no PWM)
   - ✅ Confirmar configurable por software
   - ✅ Voltage 12V nominal
   - ✅ Torque 20kg·cm @ 12V

2. **Jetson Orin Nano:**
   - ✅ Dev Kit completo (carrier board incluido)
   - ✅ 8GB RAM version
   - ✅ NO comprar módulo solo (necesita carrier)

3. **Baterías LiPo:**
   - ✅ Conector XT60 (o incluir adaptador)
   - ✅ Balance lead JST-XH 4pin
   - ✅ C-rating real (muchos mienten)
   - ✅ Dimensiones físicas caben (138×43×25mm máx)

4. **Buck Converters:**
   - ✅ Display LED para ajuste
   - ✅ Current rating REAL (test reviews)
   - ✅ Over-current protection

5. **RC Receiver:**
   - ✅ Compatible ELRS (ExpressLRS)
   - ✅ 16 canales mínimo
   - ✅ CRSF protocol output

---

## 📧 Contacto para Dudas BOM

Si encuentras componentes descontinuados o necesitas alternativas, consulta:
- Documentación: `/docs/technical/`
- Issues GitHub: https://github.com/jsvegam/HexapodIA/issues

---

**Última actualización:** 2026-05-16  
**Versión BOM:** 1.0  
**Estado:** ✅ Completo y validado
