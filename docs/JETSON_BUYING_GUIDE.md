# Guía de Compra: Jetson Orin Nano

## ⚠️ IMPORTANTE: Diferencias de Producto

Existen **2 versiones** de Jetson Orin Nano con precios MUY diferentes:

### Opción 1: Jetson Orin Nano **MODULE** (SOM) ❌
```
Precio: ~$200-250 USD
NO COMPRAR para HexapodIA
```

**Qué es:**
- Solo el módulo computacional (System on Module)
- Chip montado en PCB pequeño (~70×45mm)
- Conectores SO-DIMM para carrier board

**Qué NO incluye:**
- ❌ Carrier board (placa base)
- ❌ Conectores USB, Ethernet, HDMI
- ❌ Power supply
- ❌ Headers GPIO
- ❌ Slots M.2, CSI camera

**Para quién es:**
- Producción en masa (fabricar 1000+ unidades)
- Empresas que diseñan carrier board custom
- Producto final integrado (no desarrollo)

**Por qué NO sirve para nosotros:**
- Requiere diseñar PCB carrier (>$5000 + meses)
- O comprar carrier aparte (~$300 adicional)
- Complejidad innecesaria

---

### Opción 2: Jetson Orin Nano **DEVELOPER KIT** ✅⭐
```
Precio: ~$499 USD (8GB) / ~$399 USD (4GB)
ESTE ES EL QUE NECESITAS
```

**Qué es:**
- Kit completo listo para usar
- Módulo + Carrier Board pre-integrado
- Todo lo necesario para desarrollo

**Qué SÍ incluye:**
- ✅ Módulo Jetson Orin Nano (soldado)
- ✅ Carrier board con todos los conectores
- ✅ 4× USB 3.0 ports
- ✅ Gigabit Ethernet
- ✅ HDMI output
- ✅ 40-pin GPIO header (Raspberry Pi compatible)
- ✅ 2× CSI camera connectors
- ✅ M.2 Key M slot (NVMe SSD)
- ✅ Power supply barrel jack
- ✅ Fan header (PWM control)

**NO incluye (comprar aparte):**
- MicroSD card (128GB recomendado: ~$18)
- Power supply 5V 4A (~$15)
- Heatsink + fan (~$15, opcional pero recomendado)

**Dimensiones:**
- 100×80×35mm (con heatsink)
- Peso: ~100g

---

## Comparación: Module vs Developer Kit

| Feature | Module (SOM) | Developer Kit |
|---------|--------------|---------------|
| **Precio** | ~$250 | ~$499 (8GB) |
| **Carrier board** | ❌ NO incluido | ✅ Incluido |
| **USB ports** | ❌ | ✅ 4× USB 3.0 |
| **GPIO** | ❌ | ✅ 40-pin header |
| **Camera** | ❌ | ✅ 2× CSI |
| **Ethernet** | ❌ | ✅ GbE |
| **HDMI** | ❌ | ✅ Sí |
| **Listo para usar** | ❌ | ✅ Plug & play |
| **Para producción** | ✅ | Para desarrollo |
| **Para prototipo** | ❌ | ✅ IDEAL |

---

## Versiones de RAM: 4GB vs 8GB

Ambos Developer Kits vienen en 2 versiones:

### Jetson Orin Nano 4GB Developer Kit
```
Precio: ~$399 USD
CUDA Cores: 512
Tensor Cores: 16
Performance: 20 TOPS AI
```

**Limitaciones:**
- ⚠️ 4GB RAM compartida (CPU + GPU)
- ⚠️ YOLOv8 + RTAB-Map puede saturar RAM
- ⚠️ Múltiples modelos IA simultáneos limitado

### Jetson Orin Nano 8GB Developer Kit ⭐
```
Precio: ~$499 USD
CUDA Cores: 1024 (doble)
Tensor Cores: 32 (doble)
Performance: 40 TOPS AI (doble)
```

**Ventajas:**
- ✅ 8GB RAM (suficiente para SLAM + vision)
- ✅ 2× CUDA cores (mejor performance)
- ✅ Múltiples modelos IA simultáneos
- ✅ Futuro-proof (más margen)

**Recomendación:** ✅ **8GB version** (+$100 vale la pena)

---

## ¿Por Qué Veo Precios de $800+ USD?

Posibles razones:

### 1. Jetson Orin **NX** (Más Potente)
```
Precio: ~$799 USD
Performance: 70-100 TOPS
```
- Versión más potente de la familia Orin
- Overkill para HexapodIA
- NO necesario

### 2. Jetson Orin Nano + Accesorios Bundle
```
Precio: ~$650-800 USD
Incluye: Dev Kit + Cámara + Case + SSD + etc
```
- Algunos sellers venden kits completos
- Incluye extras que podemos comprar aparte más barato

### 3. Resellers con Markup
```
Precio: Variable
```
- Algunos distribuidores añaden 20-50% markup
- Comprar de fuentes oficiales

### 4. Modelos Descontinuados (Xavier)
```
Precio: Variable
```
- Jetson Xavier NX (generación anterior)
- Menos potente que Orin Nano
- NO comprar (old gen)

---

## Dónde Comprar (Precio Oficial)

### Fuentes Oficiales NVIDIA

| Distribuidor | Región | Precio 8GB | Link |
|--------------|--------|------------|------|
| **NVIDIA Store** | USA | $499 USD | nvidia.com/buy-jetson |
| **Arrow** | Global | $499 USD | arrow.com |
| **Seeed Studio** | Global | $499 USD | seeedstudio.com |
| **SparkFun** | USA/EU | $499 USD | sparkfun.com |
| **Mouser** | Global | $499 USD | mouser.com |

### Alternativas (Verificar Autenticidad)

| Distribuidor | Precio Típico | Notas |
|--------------|---------------|-------|
| **Amazon** | $520-550 USD | Markup ligero, envío rápido |
| **AliExpress** | $450-480 USD | ⚠️ Verificar seller oficial |
| **eBay** | Variable | ⚠️ Riesgo clones/usados |

**⚠️ ADVERTENCIA:** NO comprar en AliExpress/eBay a menos que sea seller verificado oficial. Existen clones falsos.

---

## Producto Exacto a Comprar

### ✅ Jetson Orin Nano Developer Kit 8GB

**Especificaciones:**
```
Part Number: 945-13766-0000-000
Model: Jetson Orin Nano 8GB Developer Kit
Precio oficial: $499 USD
```

**Contenido de la caja:**
- Jetson Orin Nano module (8GB) montado en carrier board
- Reference carrier board completa
- Quick start guide
- Antena WiFi (interna, integrada)
- ❌ **NO incluye power supply** (comprar aparte)

**Comprar APARTE (para setup inicial):**
- MicroSD 128GB U3 A2 (~$18)
- Power supply 5V 4A (~$15) - Ver sección voltaje abajo
- (Opcional) Heatsink + fan (~$15)

**Total real:** ~$547 USD (€505)

---

## ¿Vale la Pena el Developer Kit vs Module?

### Costo Real Comparación

**Opción A: Comprar Module (SOM)**
```
Module solo:              $250
Carrier board (comprar):  $300
Cables/conectores:         $30
Tiempo diseño PCB:      Meses
────────────────────────────────
TOTAL:                    $580+
Complejidad:              ALTA
```

**Opción B: Developer Kit (Recomendado) ⭐**
```
Dev Kit 8GB:              $499
MicroSD + Power:           $33
────────────────────────────────
TOTAL:                    $532
Complejidad:              BAJA
```

**Conclusión:** Developer Kit es **MÁS BARATO** y 10× más fácil.

---

## Verificación Pre-Compra

### Checklist Antes de Comprar:

- [ ] ¿Dice "**Developer Kit**"? (SI → correcto)
- [ ] ¿Dice solo "**Module**" o "**SOM**"? (NO → equivocado)
- [ ] ¿Es versión **8GB**? (recomendado para IA)
- [ ] ¿Precio cerca de **$499 USD**? (oficial)
- [ ] ¿Seller verificado/oficial? (NVIDIA, Arrow, etc)
- [ ] ¿Incluye carrier board? (debe decir "with carrier")

### Red Flags 🚩

- ❌ Precio < $400 USD (probablemente Module solo o scam)
- ❌ Dice "requires custom carrier board"
- ❌ Seller desconocido en AliExpress/eBay
- ❌ Foto muestra solo chip pequeño (es Module)
- ❌ No menciona puertos USB/Ethernet

---

## Alternativa: Jetson Orin Nano 4GB

Si presupuesto es crítico:

### Pros 4GB Version:
- ✅ $100 USD más barato ($399)
- ✅ Suficiente para YOLOv8 básico
- ✅ Nav2 + SLAM ligero funciona

### Cons 4GB Version:
- ⚠️ RAM limitada para múltiples modelos IA
- ⚠️ RTAB-Map + YOLOv8 simultáneo apretado
- ⚠️ Menos margen futuro (no upgrade RAM)

**Recomendación:** Si puedes, invierte los $100 extra en **8GB**. Será útil a largo plazo.

---

## Qué Incluye el Developer Kit (Fotos Ref)

### Carrier Board Connectors:

```
Vista Superior:
┌─────────────────────────────────┐
│ [GPIO 40-pin]  [Fan Header]     │
│                                 │
│         [Module SOM]            │
│       (soldado aquí)            │
│                                 │
│ [USB-C Debug] [Barrel Power]   │
│ [4× USB 3.0]  [Ethernet]       │
│ [HDMI]        [2× CSI Camera]  │
│ [M.2 Key M slot]                │
└─────────────────────────────────┘
```

**Todo está listo para conectar:**
- USB keyboard/mouse para setup inicial
- HDMI monitor (config inicial)
- Ethernet (downloads)
- Power 5V 4A

**Para HexapodIA necesitamos:**
- CSI camera port → IMX219
- USB port → Microphone
- GPIO header → I2S speaker, fan control
- Power → Buck 5V 10A desde batería

---

## Precio BOM Actualizado

### En BOM Original (€499):
```
Jetson Orin Nano 8GB: €499
```

### Precio Real (más preciso):
```
Developer Kit 8GB:     $499 USD → €460 EUR
MicroSD 128GB:          $18 USD → €17 EUR
Power supply 5V 4A:     $15 USD → €14 EUR
Heatsink+fan (oficial): $15 USD → €14 EUR
─────────────────────────────────────────
TOTAL:                 $547 USD → €505 EUR
```

**Diferencia BOM:** +€6 (despreciable)

---

## ⚡ Voltaje y Alimentación (IMPORTANTE)

### Jetson Orin Nano NO Incluye Power Supply

**El Developer Kit NO viene con adaptador de corriente.** Solo viene con:
- La placa con todos los conectores
- **Sin adaptador AC/DC**

### Alimentación en el Robot (FASE 5)

**Buenas noticias:** ✅ En HexapodIA NO usamos el power supply original.

**Cómo alimentamos Jetson:**
```
Batería LiPo 11.1V
       ↓
Buck Converter 5V 10A
       ↓
USB-C cable → Jetson Orin Nano (5V 3-5A)
```

**Especificaciones Jetson:**
- Input: **5V DC** (universal, no importa país)
- Corriente: 3-5A según carga
- Conector: Barrel jack 5.5×2.5mm O USB-C (ambos aceptan 5V)

**NO hay problema de voltaje 110V/220V** porque:
- Jetson opera a **5V DC** internamente (universal)
- Nuestro buck converter genera 5V desde batería 11.1V
- NO usamos AC power (solo para setup inicial)

### Setup Inicial (Primera Configuración)

**Para configurar Jetson por primera vez necesitas:**
1. Monitor HDMI
2. Teclado USB
3. Mouse USB
4. **Power supply 5V 4A** ← Solo para setup

**Opciones power supply setup:**

#### Opción A: Comprar Compatible Universal ✅
```
Precio: ~$12-15 USD
Buscar: "5V 4A power supply barrel jack 5.5×2.5mm"
```

**Características necesarias:**
- Output: **5V DC 4A** (20W)
- Conector: Barrel jack 5.5×2.5mm (center positive)
- Input: 100-240V AC (automático, funciona en CUALQUIER país)
- ✅ **NO importa si compras versión USA/EU/UK** (solo cambia el enchufe AC)

**Ejemplos compatibles:**
- Mean Well GST25E05-P1J
- CUI SPE-053-050
- Cualquier 5V 4A barrel jack universal

#### Opción B: USB-C PD Alternativo ✅
```
Precio: ~$20-25 USD
Buscar: "USB-C PD charger 20W"
```

**Jetson Orin Nano acepta power via:**
- Barrel jack 5V (preferido, más estable)
- **O USB-C PD** (Power Delivery)

**Ventaja USB-C:**
- Mismo cargador que celular moderno
- Probablemente ya tienes uno (iPhone 12+, Android flagship)
- Universal (funciona en todo el mundo)

**Verificar:**
- Mínimo 20W (5V 4A)
- USB-C PD compatible
- Ejemplo: Apple 20W USB-C charger funciona ✅

#### Opción C: Usar Buck 5V del Robot 🤔
```
Precio: $0 (ya lo tienes)
Viable: SÍ, pero poco práctico
```

**Proceso:**
1. Conectar batería LiPo al buck 5V
2. Cable desde buck a Jetson barrel jack/USB-C
3. Configurar Jetson con batería prendida

**Desventajas:**
- Robot debe estar ensamblado
- Batería LiPo descargándose durante setup (30-60min)
- Menos cómodo que AC power

**Recomendación:** Comprar power supply barato ($12) para setup, luego ya no lo usas más.

### Tabla Voltajes por País

| País/Región | AC Voltage | Jetson Input | Problema? |
|-------------|------------|--------------|-----------|
| **USA/México** | 110V 60Hz | 5V DC | ❌ Ninguno |
| **Europa/Chile** | 220V 50Hz | 5V DC | ❌ Ninguno |
| **UK** | 230V 50Hz | 5V DC | ❌ Ninguno |
| **Japón** | 100V 50/60Hz | 5V DC | ❌ Ninguno |

**Conclusión:** ✅ **NO importa el voltaje de tu país**
- Jetson siempre opera a 5V DC interno
- Power supply universal (100-240V input)
- En robot: Buck genera 5V desde batería

### Conector Barrel Jack Specs

**Jetson Orin Nano acepta:**
```
Barrel Jack:
- Outer diameter: 5.5mm
- Inner diameter: 2.5mm
- Center: POSITIVE (+)
- Sleeve: NEGATIVE (-)
- Voltage: 5V DC
- Current: 4A (20W)
```

**Al comprar power supply verificar:**
- [ ] 5V DC output
- [ ] 4A mínimo (mejor 5A)
- [ ] Barrel 5.5×2.5mm
- [ ] Center positive (estándar)
- [ ] Input 100-240V AC (universal)

### En el Robot: Alimentación desde Batería

**Diagrama alimentación HexapodIA:**
```
LiPo 3S 11.1V (10000mAh)
       ↓
   Switch 10A
       ↓
       ├──→ Buck 12V 10A → Servos (18×)
       │
       └──→ Buck 5V 10A → ┬─→ ESP32 (1W)
                          ├─→ Jetson (15W)
                          ├─→ Cameras (3W)
                          ├─→ Audio (2W)
                          └─→ Misc (2W)
```

**Conexión Jetson en robot:**

Opción 1: USB-C (recomendado) ✅
```
Buck 5V output (+/-) → USB-C cable → Jetson USB-C port
```
- Cable: USB-C macho a cables pelados (o adapter)
- Rojo → 5V buck output
- Negro → GND común
- Ventaja: Plug & play, fácil desconectar

Opción 2: Barrel jack
```
Buck 5V output (+/-) → Barrel connector → Jetson barrel port
```
- Connector: Barrel jack 5.5×2.5mm hembra a cables
- Centro → 5V buck output
- Sleeve → GND común

**BOM ya incluye:** Cable USB-C en lista FASE 5 (€5)

### Power Supply Recomendaciones de Compra

**Para setup inicial (si no tienes cargador USB-C):**

| Opción | Tipo | Precio | Dónde |
|--------|------|--------|-------|
| **A: Generic 5V 4A** | Barrel jack | $12 | Amazon/AliExpress |
| **B: Mean Well GST25E05** | Barrel, industrial | $18 | Mouser/Digikey |
| **C: USB-C PD 20W** | USB-C universal | $15-25 | Amazon/Apple Store |

**Búsqueda Amazon:**
- "5V 4A power supply 5.5mm 2.5mm"
- Verificar reviews (algunos mienten el amperaje)
- Preferir marcas conocidas (Mean Well, CUI, Triad)

**Si ya tienes:**
- ✅ Cargador iPhone 12+ (20W USB-C PD) → Funciona
- ✅ Cargador iPad Pro (30W USB-C PD) → Funciona
- ✅ Cargador laptop USB-C (45W+) → Funciona
- ⚠️ Cargador celular viejo 5V 2A → NO suficiente

### Testing Power Supply

**Verificar antes de conectar a Jetson:**

```bash
# Con multímetro:
1. Medir voltage sin carga: debe ser 5.0-5.2V ✅
2. Si es >5.5V: NO usar (quemará Jetson)
3. Si es <4.8V: Insuficiente

# Verificar polaridad barrel jack:
Multímetro en DC voltage:
- Probe rojo en centro → debe leer +5V
- Probe negro en sleeve → debe leer 0V (GND)
Si al revés: ¡PELIGRO! No conectar
```

**Jetson tiene protección, pero mejor prevenir.**

---

## Resumen Ejecutivo

### ❓ Qué Comprar:
✅ **Jetson Orin Nano Developer Kit 8GB**

### 💰 Precio Real:
✅ **$499 USD (€460) + accesorios ~€45 = €505 total**

### ⚠️ Qué NO Comprar:
❌ Jetson Orin Nano Module (SOM) solo (~$250)
❌ Jetson Orin NX ($799, overkill)
❌ Jetson Xavier (old gen)
❌ Sellers no-oficiales baratos (<$400)

### 🛒 Dónde Comprar:
1. **NVIDIA Store oficial** (si envía a tu país)
2. **Arrow Electronics** (distribuidor global oficial)
3. **Mouser/Seeed Studio** (alternativas oficiales)
4. **Amazon** (si es seller "NVIDIA Official Store")

### 📦 Qué Viene en la Caja:
✅ Module + Carrier board integrado
✅ WiFi/Bluetooth integrado
✅ 40-pin GPIO, 4× USB, Ethernet, HDMI, CSI
✅ Listo para usar (plug & play)

### 📦 Qué Comprar Aparte:
- MicroSD 128GB (~€17)
- Power supply 5V 4A (~€14) - Solo para setup inicial, luego no se usa
- (Opcional) Heatsink+fan oficial (~€14)

### ⚡ Voltaje:
✅ **NO hay problema** - Jetson opera a 5V DC (universal)
- Power supply: 100-240V input (funciona en cualquier país)
- En robot: Buck 5V desde batería (no usa AC power)

---

## Preguntas Frecuentes

### ¿Puedo usar Raspberry Pi en lugar de Jetson?
❌ **NO** para FASE 5:
- Raspberry Pi 5: ~2 TOPS AI (20× más lento)
- No tiene CUDA (YOLOv8 sería muy lento)
- RAM insuficiente para SLAM+Vision
- **Jetson es NECESARIO** para autonomía IA

### ¿4GB suficiente si no uso Insta360?
⚠️ **Ajustado:**
- YOLOv8 tiny: ~1.5GB
- RTAB-Map: ~2GB
- ROS2 + Nav2: ~1GB
- Sistema: ~0.5GB
- **Total:** ~5GB (sobre 4GB disponibles) → TIGHT

Recomendación: **8GB** para no tener problemas.

### ¿Existe versión más barata?
✅ Jetson Nano (old gen, 2019):
- Precio: ~$99 USD (descontinuado)
- Performance: 0.5 TOPS (80× más lento que Orin)
- ❌ **NO recomendado** (muy lento para YOLOv8)

**Conclusión:** Orin Nano 8GB es el mínimo viable para IA real.

### ¿Cuándo comprar Jetson?
**FASE 5** (no FASE 1):
- FASE 1-3: Solo ESP32 + servos
- FASE 4: + RC control
- **FASE 5:** + Jetson (cuando locomotion validada)

**Ahorra primero para:** Servos (€324), batería (€95), electrónica (€200)
**Jetson al final:** Cuando todo lo demás funciona

---

## Link Oficial NVIDIA

**Página producto oficial:**
https://www.nvidia.com/en-us/autonomous-machines/embedded-systems/jetson-orin/

**Developer Kit 8GB:**
https://developer.nvidia.com/embedded/learn/get-started-jetson-orin-nano-devkit

**Dónde comprar (NVIDIA):**
https://www.nvidia.com/en-us/autonomous-machines/embedded-systems/buy-jetson/

---

**Última actualización:** 2026-05-16  
**Recomendación:** Jetson Orin Nano Developer Kit 8GB @ $499 USD ✅
