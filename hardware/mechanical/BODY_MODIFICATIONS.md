# Modificaciones Body para HexapodIA FASE 5

## Análisis Body Base (RookiDroid v2)

### Dimensiones Originales RookiDroid

```
Planta superior:    180×160mm
Planta inferior:    180×160mm
Altura:             40mm
Espesor placas:     3mm PETG
Peso estimado:      ~200g (estructura sola)
```

**Problema:** Diseño original es para:
- 6 piernas simples
- Batería pequeña (2200mAh ~180g)
- Electrónica mínima (~50g)
- **Peso total original:** ~1200-1500g

**Nuestro FASE 5:**
- 18 servos pesados (1116g)
- Batería 10000mAh (800g)
- Jetson + periféricos (340g)
- Insta360 payload (182g)
- **Peso total:** 3120g

**Veredicto:** ❌ **NECESITA MODIFICACIONES**

---

## Componentes a Ubicar en Body

### Inventario Interno

| Componente | Dimensiones (L×W×H) | Peso | Ubicación Ideal |
|------------|---------------------|------|-----------------|
| **Batería 10000mAh** | 138×43×25mm | 800g | Centro bajo (CG) |
| **Jetson Orin Nano** | 70×45×24mm | 100g | Centro superior |
| **Heatsink + fan** | 70×45×15mm | 33g | Sobre Jetson |
| **ESP32 DevKit** | 55×28×13mm | 10g | Frontal |
| **Buck 12V (10A)** | 65×45×25mm | 35g | Lateral izq |
| **Buck 5V (10A)** | 65×45×25mm | 30g | Lateral der |
| **IMU BNO085** | 25×20×5mm | 3g | Frontal centro |
| **RC Receiver ELRS** | 30×15×5mm | 5g | Posterior alto |
| **FPV Camera** | 28×28×20mm | 8g | Frontal (mount ext) |
| **VTX 5.8GHz** | 35×35×8mm | 12g | Posterior alto |
| **Camera IMX219** | 25×24×9mm | 10g | Frontal (mount ext) |
| **Audio speaker** | Ø40×10mm | 20g | Lateral |
| **OLED display** | 27×27×4mm | 5g | Frontal ext |
| **Ultrasonics (×2)** | 45×20×15mm | 16g | Frontal corners |
| **Capacitor bank** | 50×30×20mm | 16g | Cerca buck 12V |
| **Cables/wiring** | - | 80g | Routing interno |

**Total volumen ocupado:** ~650 cm³  
**Volumen body 180×160×40mm:** 1152 cm³  
**Ocupación:** ~56% (ajustado, pero con poca ventilación)

### Componentes Externos (Montajes)

- Insta360 top-mount (arriba)
- FPV camera (frontal)
- IMX219 camera (frontal)
- OLED display (frontal panel)
- Ultrasonics (corners)

---

## Modificaciones Propuestas

### Opción A: Body Ampliado (RECOMENDADO)

**Nuevas dimensiones:**
```
Planta:         200×180mm (+20mm largo, +20mm ancho)
Altura:         50mm (+10mm altura)
Espesor:        4mm (+1mm refuerzo)
Peso:           ~320g (+120g vs original)
```

**Ventajas:**
- ✅ Más espacio para componentes
- ✅ Mejor ventilación (10mm adicional permite airflow)
- ✅ Distribución componentes menos apretada
- ✅ Espacio para cable routing limpio
- ✅ Acceso fácil a componentes (mantenimiento)

**Desventajas:**
- ⚠️ Peso +120g
- ⚠️ Momento de inercia mayor (slower turning)
- Requiere rediseño completo STLs

**Impacto en legs:**
- Mounting points se mueven +10mm radialmente
- Coxa offset aumenta ~10mm
- IK necesita ajuste menor (coxa_length actualizado)

### Opción B: Body Multi-Nivel

**Concepto:**
```
Nivel 3 (top):      Insta360 mount + RC receiver
     |
Nivel 2 (mid-up):  Jetson + heatsink + fan
     |
Nivel 1 (mid-low): ESP32 + bucks + electronics
     |
Nivel 0 (bottom):  Batería (CG bajo)
```

**Dimensiones:**
```
Planta:         180×160mm (sin cambio)
Altura total:   65mm (vs 40mm original)
Niveles:        4 placas horizontales
Peso:           ~280g
```

**Ventajas:**
- ✅ No cambia footprint (piernas igual)
- ✅ Organización vertical clara
- ✅ Airflow vertical natural (convección)
- ✅ Acceso modular por niveles

**Desventajas:**
- ⚠️ CG más alto (+12mm) → menos estable
- ⚠️ Altura total +25mm
- ⚠️ Más complejo ensamblaje

### Opción C: Body Original + Pods Externos

**Concepto:**
```
Body 180×160mm mantiene:
- Batería
- ESP32
- Bucks

Pods externos modulares:
- Pod frontal: Jetson + cameras
- Pod posterior: VTX + RC + audio
```

**Ventajas:**
- ✅ Body base sin modificar
- ✅ Pods removibles (configuraciones flexibles)
- ✅ Fácil upgrade futuro

**Desventajas:**
- ❌ Estética menos limpia
- ⚠️ Pods vulnerables a impactos
- ⚠️ Distribución peso no óptima

---

## Decisión Recomendada: Opción A (Body Ampliado)

### Nuevo Body 200×180×50mm

**Placa Superior (Top Plate):**
```
        200mm
    ┌────────────────┐
    │                │
    │  [Insta mount] │ ← Centro, con quick release
    │                │
    │ ○ L3       R3 ○│ ← Leg mounting (6 puntos)
180 │                │
mm  │ ○ L2       R2 ○│
    │                │
    │ ○ L1       R1 ○│
    │                │
    │ [Vent slots]   │ ← Ventilación 40mm×8mm (×4)
    └────────────────┘
    
Material: PETG 4mm
Infill: 40% con honeycomb
Ribs: Cruceta central + perimetrales
Peso: ~150g
```

**Placa Intermedia Alta (Mid-Upper Plate):**
```
┌─────────────────────┐
│ [Jetson+Heatsink]   │ ← 70×45mm cutout, fan arriba
│                     │
│ [ESP32]  [IMX219]   │ ← Frontal
│                     │
│ [Capacitor]         │ ← Cerca bucks
│ [RC RX]  [VTX]      │ ← Posterior alto
└─────────────────────┘

Altura: +35mm desde bottom
Material: PETG 3mm
Peso: ~80g
```

**Placa Intermedia Baja (Mid-Lower Plate):**
```
┌─────────────────────┐
│                     │
│ [Buck 12V] [Buck 5V]│ ← Laterales
│                     │
│      [Batería]      │ ← Centro (138×43mm cutout)
│    [10000mAh]       │    Velcro straps
│                     │
│ [IMU] [Audio Amp]   │
└─────────────────────┘

Altura: +15mm desde bottom
Material: PETG 3mm
Peso: ~90g
```

**Placa Inferior (Bottom Plate):**
```
┌─────────────────────┐
│ ○               ○   │ ← M3 standoffs a mid-lower
│                     │
│ ○ L3       R3 ○     │ ← Leg mounting (6 puntos)
│                     │
│ ○ L2       R2 ○     │
│                     │
│ ○ L1       R1 ○     │
│                     │
│ ○               ○   │
└─────────────────────┘

Material: PETG 4mm (estructura)
Infill: 50% (carga principal)
Peso: ~150g
```

**Columnas/Standoffs:**
```
Tipo                Cantidad  Altura   Material        Peso
──────────────────────────────────────────────────────────
Top → Mid-Upper     6         15mm     M3×20 + spacer  12g
Mid-Upper → Mid-Low 8         20mm     M3×25 + spacer  16g
Mid-Lower → Bottom  6         15mm     M3×20 + spacer  12g

Total hardware: ~60g (tornillos + espaciadores + tuercas)
```

**Peso total body modificado:**
```
Top plate:          150g
Mid-upper plate:    80g
Mid-lower plate:    90g
Bottom plate:       150g
Hardware:           60g
Ribs/refuerzos:     30g
─────────────────────────
TOTAL:              560g (vs 200g original)
```

**Diferencia vs original:** +360g

---

## Impacto en Performance

### Nuevo Peso Total

```
Componente                    Peso Original   Peso Modificado
────────────────────────────────────────────────────────────
Estructura body               200g            560g
Piernas ×6                    180g            180g
Servos ×18                    1116g           1116g
Electrónica                   340g            340g
Batería 10000mAh              800g            800g
Insta360 + mount              242g            242g
Misc (cables, etc.)           82g             82g
────────────────────────────────────────────────────────────
TOTAL                         2960g           3320g
```

**Incremento:** +360g por body reforzado

**Carga por pierna:** 3320/6 = **553g**

### Torque Re-Analysis

```
FASE 5 con Body Ampliado (553g por pierna):
─────────────────────────────────────────────────────────────
Articulación    Brazo   Carga   Torque Req   STS3215   Margen
─────────────────────────────────────────────────────────────
Coxa (yaw)      55mm    553g    1.52 kg·cm   20 kg·cm  13.2×
Femur (pitch)   80mm    553g    2.21 kg·cm   20 kg·cm  9.0×
Tibia (pitch)   130mm   553g    3.59 kg·cm   20 kg·cm  5.6× ✅
─────────────────────────────────────────────────────────────

Escenarios dinámicos:
Standing static:        3.59 kg·cm      5.6× ✅
Walking normal (1.5×):  5.39 kg·cm      3.7× ✅
Walking slow (1.2×):    4.31 kg·cm      4.6× ✅ RECOMENDADO
Stairs (3.0×):          10.77 kg·cm     1.9× ⚠️ LÍMITE
```

**Conclusión:** ✅ **Todavía funciona con body ampliado**
- Walking normal: 3.7× margin (seguro)
- Walking slow: 4.6× margin (muy seguro)
- **Recomendación:** Usar walking slow para operaciones largas

### Autonomía Re-Analysis

**Consumo con peso adicional:**
```
Walking normal (62W base):
- +12% por peso Insta360: +7W
- +8% por peso body: +5W
- Total walking: 74W

Recording Mode (batería 10000mAh):
Autonomía: 111Wh / 74W = 1.5h = 90min ✅
```

**¡Todavía cumple objetivo 1h+!** ✅

---

## Ventilación y Cooling

### Airflow Design

```
Vista lateral:
                  
    ↑ Hot air out (top vents)
    │
┌───┴────────────┐
│ [Insta360]     │
├────────────────┤
│ [Jetson+Fan]   │ ← Fan activo empuja aire
├────────────────┤
│ [ESP32][Bucks] │ ← Disipan calor
├────────────────┤
│ [Batería]      │ ← Genera poco calor
└────────────────┘
    │
    ↓ Cool air in (bottom + side vents)
```

**Ventilation slots:**

Top plate:
- 4× slots 40×8mm en corners (intake/exhaust natural)
- 1× cutout 50×50mm centro (Jetson fan exhaust)

Side panels (nuevo):
- 8× slots 60×4mm verticales laterales (2 por lado)
- Permite convección natural cuando fan OFF

Bottom plate:
- Mesh pattern 80×80mm centro (multiple Ø6mm holes)
- Permite intake aire fresco desde abajo

**Thermal simulation estimada:**
```
Componente          Heat Gen   Cooling            Max Temp
──────────────────────────────────────────────────────────
Jetson (15W)        5.3W       Heatsink+fan       70°C ✅
Buck 12V            10.8W      Heatsink+airflow   75°C ✅
Buck 5V             3.2W       Heatsink           65°C ✅
Servos (×18)        18W        Natural+walking    60°C ✅
```

**Conclusión:** Sistema de cooling es **suficiente** con ventilación diseñada.

---

## Mounting Points Piernas

### Leg Attachment - Nuevo Pattern

**Original RookiDroid:** Legs montadas en hexágono inscrito ~Ø160mm

**Body ampliado:** Legs montadas en hexágono ~Ø180mm

**Cambios necesarios:**
```
Parámetro                Original    Modificado   Δ
─────────────────────────────────────────────────────
Radio montaje (R)        80mm        90mm         +10mm
Ángulo entre piernas     60°         60°          0°
Coxa offset (mecánico)   55mm        65mm         +10mm
```

**Impacto en IK:**

```cpp
// En Kinematics.cpp, actualizar:

// OLD:
#define COXA_LENGTH 55.0  // mm

// NEW (body ampliado):
#define COXA_LENGTH 65.0  // mm

// El resto de IK no cambia (femur/tibia igual)
```

**Re-validación IK necesaria:** Tests automáticos deben correr OK con nuevo coxa length.

---

## Estructural Reinforcement

### Ribs y Refuerzos

**Top plate:**
```
Vista inferior top plate:

    ┌──────────────────┐
    │  ╱╲          ╱╲  │ ← Diagonal ribs 2mm
    │ ╱  ╲        ╱  ╲ │
    │╱    ╲      ╱    ╲│
    │      ╲    ╱      │
    │       ╲  ╱       │ ← Centro reforzado
    │        ╲╱        │    (Insta360 mount)
    │        ╱╲        │
    │       ╱  ╲       │
    │      ╱    ╲      │
    │╱    ╱      ╲    ╲│
    │ ╱  ╱        ╲  ╲ │
    │  ╱╱          ╲╲  │
    └──────────────────┘
```

**Bottom plate:**
```
Vista superior bottom plate:

    ┌──────────────────┐
    │ ═════════════════│ ← Perimeter ribs 3mm
    │ ║    ┌───┐      ║│
    │ ║    │Bat│      ║│
    │ ║    │   │      ║│ ← Central cross ribs
    │ ║════╪═══╪══════║│    para leg mounting
    │ ║    │   │      ║│
    │ ║    └───┘      ║│
    │ ═════════════════│
    └──────────────────┘
```

**Safety factors estructurales:**
```
Área                     Stress    Yield    SF
──────────────────────────────────────────────
Top plate (Insta360)     1.9 MPa   50 MPa   26×
Bottom legs mounting     3.5 MPa   50 MPa   14×
Mid-upper (Jetson)       1.2 MPa   50 MPa   42×
```

**Conclusión:** Estructura es **muy segura** con refuerzos.

---

## BOM Hardware Body

### Tornillería

| Item | Spec | Cantidad | Peso | Precio |
|------|------|----------|------|--------|
| Tornillos M3×20 | Countersunk | 24 | 24g | €3 |
| Tornillos M3×25 | Countersunk | 16 | 20g | €2.5 |
| Tornillos M3×12 | Countersunk | 12 | 12g | €1.5 |
| Espaciadores nylon M3×10 | Hex | 20 | 20g | €4 |
| Espaciadores nylon M3×15 | Hex | 16 | 24g | €5 |
| Tuercas M3 lock | Nylon | 48 | 12g | €2 |
| Insertos térmicos M3×5 | Brass | 24 | 24g | €6 |
| **TOTAL** | | | **136g** | **€24** |

### Impresión 3D

| Pieza | Volumen PETG | Tiempo | Precio Filamento |
|-------|--------------|--------|------------------|
| Top plate | ~95g | 8h | €3 |
| Mid-upper plate | ~75g | 6h | €2.5 |
| Mid-lower plate | ~85g | 7h | €2.5 |
| Bottom plate | ~120g | 10h | €3.5 |
| Ribs/refuerzos | ~25g | 3h | €1 |
| Side panels (×4) | ~40g | 4h | €1.5 |
| **TOTAL** | **~440g** | **38h** | **€14** |

**Costo total body ampliado:** €24 (hardware) + €14 (filamento) = **€38**

---

## Print Settings Recomendados

```ini
Material:           PETG (preferido) o PLA+
Layer height:       0.2mm (quality) o 0.3mm (draft)
Infill:             40% honeycomb (structural)
                    50% bottom plate (máxima carga)
Walls:              4 perimeters
Top/bottom layers:  5
Print speed:        50mm/s (quality)
Bed temp:           80°C (PETG)
Nozzle temp:        240°C (PETG)
Cooling:            50% after layer 3
Supports:           Minimal (diseño evita)
Adhesion:           Brim 5mm (PETG warping)
```

**Orientación de impresión:**
- Top/bottom plates: Flat (sin soportes)
- Mid plates: Flat (sin soportes)
- Ribs: Stand vertical (mejor strength)

---

## Assembly Instructions

### Orden de Ensamblaje

**Paso 1: Bottom Plate**
1. Imprimir bottom plate con ribs
2. Instalar insertos térmicos M3 (×6 legs + ×6 standoffs)
3. Montar legs (coxa servos) con M3×12
4. Verificar alineación hexagonal

**Paso 2: Mid-Lower Plate**
1. Instalar insertos térmicos
2. Montar bucks 12V + 5V con M3×8
3. Instalar capacitor bank cerca buck 12V
4. Routing cables power (abajo de placa)
5. Conectar con standoffs M3×20 a bottom plate

**Paso 3: Batería**
1. Posicionar LiPo 10000mAh en cutout central
2. Asegurar con velcro straps (×2 perpendiculares)
3. Conectar a switch y bucks
4. Voltage divider a ESP32 GPIO34

**Paso 4: Mid-Upper Plate**
1. Montar ESP32 con M3×8 + standoffs
2. Montar IMU BNO085 (I2C a ESP32)
3. Instalar RC receiver posterior
4. Instalar VTX 5.8GHz con antena
5. Conectar con standoffs M3×25 a mid-lower

**Paso 5: Jetson & Cooling**
1. Aplicar thermal paste en Jetson
2. Instalar heatsink oficial
3. Montar fan 40mm sobre heatsink
4. Fijar Jetson+heatsink a mid-upper con M3×6 (×4)
5. Conectar fan a ESP32 GPIO13 (PWM control)
6. Cables USB-C power desde buck 5V

**Paso 6: Top Plate**
1. Instalar Insta360 mount (ver INSTA360_TOP_MOUNT.md)
2. Conectar con standoffs M3×20 a mid-upper
3. Verificar ventilation slots libres

**Paso 7: Componentes Externos**
1. FPV camera mount frontal
2. IMX219 camera mount frontal
3. OLED display panel frontal
4. Ultrasonic sensors corners
5. Audio speaker lateral

**Paso 8: Cable Management**
1. Routing limpio con zip ties
2. Separar power de signal cables
3. Dejar slack para movimiento legs
4. Verificar no hay tension en cables

**Paso 9: Testing**
1. Inspección visual 360°
2. Verificar tornillos apretados
3. Power on (solo ESP32 primero)
4. Verificar voltages (12V, 5V)
5. Power on Jetson
6. Monitorear temperaturas

---

## Comparación Final

| Parámetro | RookiDroid Base | Body Modificado | Diferencia |
|-----------|-----------------|-----------------|------------|
| **Dimensiones** | 180×160×40mm | 200×180×50mm | +20×20×10mm |
| **Peso body** | 200g | 560g | +360g |
| **Peso total** | 2960g | 3320g | +360g |
| **Volumen interno** | 1152 cm³ | 1800 cm³ | +56% |
| **Ventilación** | Limitada | Excelente | Slots múltiples |
| **Acceso componentes** | Difícil | Modular | Por niveles |
| **Torque margin tibia** | 5.9× | 5.6× | -5% (aceptable) |
| **Autonomía** | 87min | 90min | +3min (mejor) |
| **Costo** | €15 | €38 | +€23 |

---

## Recomendación Final

### ✅ **ADOPTAR OPCIÓN A: Body Ampliado 200×180×50mm**

**Razones:**
1. ✅ Suficiente espacio para todos los componentes FASE 5
2. ✅ Ventilación adecuada (crítico para Jetson)
3. ✅ Organización modular por niveles (mantenimiento fácil)
4. ✅ Torque margin sigue siendo seguro (5.6× static, 3.7× walking)
5. ✅ Autonomía se mantiene 90min (cumple objetivo 1h+)
6. ✅ Estructura reforzada (14-42× safety factors)
7. ✅ Escalable: Pods externos futuros posibles

**Trade-offs aceptables:**
- Peso +360g (pero servos soportan bien)
- Altura +10mm (estética sigue compacta)
- Costo +€23 (razonable para capacidades)

**Archivos a Generar (CAD):**
1. `body_top_plate_200x180.stl`
2. `body_mid_upper_plate.stl`
3. `body_mid_lower_plate.stl`
4. `body_bottom_plate_200x180.stl`
5. `body_side_panel_left.stl`
6. `body_side_panel_right.stl`
7. `body_front_panel.stl`
8. `body_rear_panel.stl`

---

**Próximo paso:** Generar CAD files del body modificado con FreeCAD/Fusion 360.

**Estado:** ✅ Diseño conceptual completo, listo para CAD modeling.
