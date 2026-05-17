# HexapodIA 🕷️🤖

**Robot hexápodo 18DOF con arquitectura modular, control tiempo real ESP32 y preparado para IA autónoma con Jetson Orin Nano.**

[![Phase](https://img.shields.io/badge/Phase-FASE%201-blue)]()
[![Status](https://img.shields.io/badge/Status-Development-yellow)]()
[![License](https://img.shields.io/badge/License-MIT-green)]()

---

## 🎯 Filosofía del Proyecto

> **"Locomoción primero, IA después"**

Construir un sistema de locomoción robusto y validado ANTES de añadir IA. Arquitectura modular que escala de una pierna a hexápodo autónomo completo sin rediseñar.

---

## 📊 Especificaciones Principales

| Componente | Spec | FASE |
|------------|------|------|
| **Servos** | 18× Feetech STS3215 (20kg·cm) | 1-3 |
| **MCU** | ESP32-WROOM-32 (dual-core 240MHz) | 1+ |
| **IA** | Jetson Orin Nano 8GB | 5 |
| **IMU** | BNO085 9-axis | 3+ |
| **RC** | ELRS EP1 (4-16ms latency) | 4+ |
| **Batería** | LiPo 3S 10000mAh | 5 |
| **Peso Total** | 3288g con Insta360 | 5 |
| **Autonomía** | 90min grabando travesías | 5 |
| **Torque Margin** | 6.2× static, 3.7× walking | ✅ |

### Dimensiones (RookiDroid-inspired)
```
Coxa:    55mm
Femur:   80mm  
Tibia:   130mm
Body:    180×160mm (stack central)
Altura:  ~150mm (standing)
Reach:   ~265mm (extended)
```

---

## 🚀 Roadmap por Fases

| Fase | Objetivo | Hardware | Status |
|------|----------|----------|--------|
| **FASE 1** | Pierna única validada | 3× servos + ESP32 | 🟡 En desarrollo |
| **FASE 2** | 2-3 piernas coordinadas | 6-9× servos + body base | ⚪ Pendiente |
| **FASE 3** | Hexápodo walking completo | 18× servos + IMU + RC | ⚪ Pendiente |
| **FASE 4** | Control remoto FPV | + ELRS + FPV cam + VTX | ⚪ Pendiente |
| **FASE 5** | IA autónoma + Insta360 | + Jetson + cámaras + audio | ⚪ Pendiente |

---

## 💰 Presupuesto

```
FASE 1 (pierna única):          €151
FASE 3 (hexápodo completo):     €719 (acumulado)
FASE 5 (IA + Insta360):        €1543 (total proyecto)
```

**Desglose FASE 5:**
- Servos 18×: €324
- Jetson Orin Nano: €499
- Electrónica (ESP32, IMU, RC, cámaras, audio): €157
- Power (batería 10000mAh, bucks): €227
- Mecánica (filamento PETG/TPU): €45
- Tornillería y hardware: €41
- Consumibles: €27
- Insta360 mounting: €10

Ver BOM completo: **[docs/BOM_COMPLETE.md](docs/BOM_COMPLETE.md)**

---

## 🏗️ Arquitectura

### Hardware

**Body Design:** Stack central modular sobre base RookiDroid 180×160mm
```
Nivel 4: Insta360 mount (quick release 1/4"-20)
Nivel 3: Jetson Orin Nano + heatsink + fan
Nivel 2: ESP32 + bucks + IMU + RC
Nivel 1: Body original (batería 10000mAh, CG bajo)
Nivel 0: Bottom plate (6 piernas montadas)
```

**Ventajas stack central:**
- ✅ No modifica STLs RookiDroid originales
- ✅ Modular (añadir stacks por fase)
- ✅ Más liviano (278g estructura vs 560g ampliado)
- ✅ Mejor torque margin (6.2× vs 5.6×)
- ✅ Acceso fácil componentes (removible)

Ver diseño: **[hardware/mechanical/BODY_STACKED_DESIGN.md](hardware/mechanical/BODY_STACKED_DESIGN.md)**

### Software

**ESP32 (Real-Time Locomotion):**
- Servo control STS3215 (1Mbps serial)
- Inverse/Forward Kinematics (analytical)
- Cross-validation IK↔FK (<5mm error)
- Gait generator (Tripod, Ripple)
- Safety monitoring (temp, voltage, emergency)
- Control loop: 50Hz

**Jetson Orin Nano (High-Level AI):**
- ROS2 Humble
- YOLOv8 object detection
- RTAB-Map SLAM (RGB-D)
- Nav2 autonomous navigation
- Voice commands processing
- Behavior Trees

Ver arquitectura: **[docs/technical/HYBRID_CONTROL_ARCHITECTURE.md](docs/technical/HYBRID_CONTROL_ARCHITECTURE.md)**

---

## 📂 Estructura del Proyecto

```
HexapodIA/
├── firmware/
│   └── esp32_locomotion/          # PlatformIO ESP32
│       ├── src/main.cpp
│       ├── lib/STS3215/           # Servo driver
│       └── lib/Kinematics/        # IK/FK engine
├── hardware/
│   ├── mechanical/                # STLs, CAD
│   │   ├── BODY_STACKED_DESIGN.md
│   │   └── INSTA360_TOP_MOUNT.md
│   ├── electronics/               # Diagramas, pinouts
│   └── servos_config/
├── docs/
│   ├── technical/                 # Specs técnicas
│   ├── BOM_COMPLETE.md            # Lista completa componentes
│   └── QUICK_START.md
├── simulation/                    # Python IK testing
├── ros2_ws/                       # ROS2 workspace (FASE 5)
├── CLAUDE.md                      # Project context
└── README.md                      # Este archivo
```

---

## 🎮 Características Principales

### Locomoción Validada
- ✅ Sistema IK/FK con cross-validation automática
- ✅ 16 tests automáticos en boot
- ✅ Sin necesidad de conocimientos matemáticos para depurar
- ✅ Torque margin 6.2× (muy seguro)
- ✅ Safety features múltiples

### Control Dual-Mode
- **Manual:** RadioMaster Boxer + ELRS (4-16ms latency)
- **Autónomo:** Jetson con SLAM + Nav2 + voice commands
- **Override:** RC siempre tiene control (safety)

### Interacción IA (FASE 5)
- 🎤 Comandos por voz (USB mic + speaker)
- 📹 Video streaming FPV (5.8GHz, 5-10ms latency)
- 🖥️ Display OLED (telemetría en tiempo real)
- 🤖 Object detection + obstacle avoidance
- 📷 Insta360 Ace Pro 2 payload (grabar travesías)

### Autonomía Objetivo
- **1 hora+** con IA activa y grabando (10000mAh)
- **Recording Mode:** 90min walking slow con Insta360
- **Consumo:** 76-102W según modo operación

---

## 🚀 Quick Start

### Requisitos

**Hardware mínimo (FASE 1):**
- ESP32-WROOM-32 DevKit
- 3× Feetech STS3215 servos
- LiPo 3S 2200mAh
- Buck converters (12V + 5V)
- Impresora 3D (PETG capability)

**Software:**
- PlatformIO (firmware ESP32)
- Python 3.8+ (simulation)
- FreeCAD/Fusion 360 (CAD, opcional)

### Instalación

```bash
# Clonar repo
git clone https://github.com/jsvegam/HexapodIA.git
cd HexapodIA

# Compilar firmware ESP32
cd firmware/esp32_locomotion
pio run -t upload
pio device monitor -b 115200

# Ejecutar tests automáticos
# En serial monitor, presionar 'v'
```

### Validación Kinematics

```bash
cd simulation
python kinematics_validation.py
# Debe mostrar: "✅ ALL TESTS PASSED"
```

Ver guía completa: **[docs/QUICK_START.md](docs/QUICK_START.md)**

---

## 📋 Documentación

| Documento | Descripción |
|-----------|-------------|
| **[BOM_COMPLETE.md](docs/BOM_COMPLETE.md)** | Lista exhaustiva componentes (cables, tornillos, TODO) |
| **[BODY_STACKED_DESIGN.md](hardware/mechanical/BODY_STACKED_DESIGN.md)** | Diseño stack central modular |
| **[INSTA360_TOP_MOUNT.md](hardware/mechanical/INSTA360_TOP_MOUNT.md)** | Soporte cámara 360° con quick release |
| **[WEIGHT_AND_POWER_BUDGET.md](docs/technical/WEIGHT_AND_POWER_BUDGET.md)** | Análisis peso/energía/autonomía |
| **[KINEMATICS_ROBUSTNESS.md](docs/technical/KINEMATICS_ROBUSTNESS.md)** | Sistema validación IK auto-diagnóstico |
| **[HYBRID_CONTROL_ARCHITECTURE.md](docs/technical/HYBRID_CONTROL_ARCHITECTURE.md)** | Control manual + autónomo |
| **[AUTONOMOUS_AI_CAPABILITIES.md](docs/technical/AUTONOMOUS_AI_CAPABILITIES.md)** | Pipeline IA completo Jetson |

---

## 🎯 Features Únicos

### 1. Sistema Kinematics Robusto
**Problema:** Usuario no tiene conocimientos profundos de física/matemáticas.  
**Solución:** Sistema auto-validante con 4 capas:
- Input validation (rangos, workspace)
- Cross-validation IK↔FK (<5mm error)
- 16 tests automáticos en boot
- Hardware feedback (servos reportan posición real)

### 2. Body Stack Central
**Problema:** Modificar body completo requiere rediseñar todo.  
**Solución:** Stack modular sobre base original:
- No modifica STLs RookiDroid
- Añadir stacks por fase
- 50% más liviano que body ampliado
- Mejor torque margin

### 3. Payload Insta360
**Problema:** Grabar travesías sin afectar autonomía.  
**Solución:** 
- Top-mount 1/4"-20 con quick release
- Walking slow mode (4.9× torque margin)
- 90min autonomía grabando
- Insta360 batería independiente

---

## ⚡ Performance

### Torque Analysis

| Config | Peso | Carga/Pierna | Torque Tibia | Margin |
|--------|------|--------------|--------------|--------|
| FASE 3 (6 piernas) | 2365g | 394g | 2.56 kg·cm | 7.8× ✅ |
| FASE 5 (IA) | 2775g | 463g | 3.01 kg·cm | 6.6× ✅ |
| + Insta360 | 3288g | 548g | 3.56 kg·cm | 5.6× ✅ |
| Walking normal | - | - | ×1.5 dynamic | 3.7× ✅ |
| **Walking slow** | - | - | ×1.2 dynamic | **4.9×** ✅⭐ |

### Autonomía (LiPo 10000mAh)

| Modo | Consumo | Duración | Uso |
|------|---------|----------|-----|
| Idle monitoring | 26W | 4.3h | Standby |
| Patrol light | 68W | 1.6h | Exploración lenta |
| Interactive (voice+IA) | 86W | 1.3h | Interacción usuario |
| Full Interactive | 102W | 1.1h | Todo activo |
| **Recording Mode** | **76W** | **1.5h** | Grabar travesías ⭐ |

---

## 🛡️ Safety Features

### Hardware
- ✅ Emergency stop button (RC CH8)
- ✅ Low-voltage cutoff (9.5V critical)
- ✅ Voltage alarm buzzer
- ✅ Over-current protection (bucks)
- ✅ ELRS failsafe (RC timeout)

### Software
- ✅ Thermal monitoring servos (>70°C shutdown)
- ✅ IMU fall detection (>45° stop)
- ✅ RC watchdog (100ms timeout)
- ✅ Cross-validation IK/FK (reject errors)
- ✅ Jetson timeout → manual mode

---

## 📷 Payload: Insta360 Ace Pro 2

**Características:**
- Quick release 1/4"-20 (Manfrotto compatible)
- Top-mount centrado (balance CG)
- Peso sistema: 242g (182g cámara + 60g mount)
- Safety factor estructural: 26× (muy seguro)

**Recording Mode optimizado:**
- Walking slow (50% velocidad)
- 90min autonomía
- Insta360 FlowState stabilization
- Footage suave y estable

Ver diseño: **[hardware/mechanical/INSTA360_TOP_MOUNT.md](hardware/mechanical/INSTA360_TOP_MOUNT.md)**

---

## 🤝 Referencias

**Inspirado en:**
- [Sir-Kuhnhero/Hexapod](https://github.com/Sir-Kuhnhero/Hexapod) - Arquitectura software
- [RookiDroid v2](https://rookidroid.com/product/hexapod-v2-stl/) - Dimensiones validadas

**Diferencias clave:**
- Smart servos (STS3215) desde inicio
- Dual MCU (ESP32 + Jetson)
- Sistema validación IK robusto
- Stack modular (no ampliar body)
- Diseñado para payload (Insta360)

---

## 🛠️ Tools Necesarias

**Software:**
- PlatformIO (ESP32)
- Python 3.8+
- FreeCAD/Fusion 360 (CAD)
- Cura/PrusaSlicer (3D print)
- ROS2 Humble (FASE 5)

**Hardware:**
- Impresora 3D (PETG)
- Soldador (insertos térmicos)
- Multímetro (ESENCIAL)
- Herramientas básicas (Allen, destornilladores)

Ver lista completa: **[docs/BOM_COMPLETE.md](docs/BOM_COMPLETE.md)** sección Herramientas

---

## 📝 TODO

### FASE 1 (Actual)
- [ ] Imprimir pierna en PETG
- [ ] Ensamblar con servos
- [ ] Validar firmware hardware real
- [ ] Medir torque/temperatura real

### FASE 3
- [ ] Diseñar/imprimir stacks centrales
- [ ] Integrar 18 servos
- [ ] Implementar gait tripod/ripple
- [ ] RC control ELRS

### FASE 5
- [ ] Jetson + ROS2 setup
- [ ] YOLOv8 + RTAB-Map
- [ ] Voice commands
- [ ] Insta360 top-mount

---

## 📄 Licencia

MIT License - Ver [LICENSE](LICENSE)

---

## 👤 Autor

**José Vega** - [GitHub](https://github.com/jsvegam)

---

## 🌟 Estado del Proyecto

🟡 **FASE 1 en desarrollo activo**

**Última actualización:** 2026-05-16  
**Versión:** 0.1.0-alpha  
**Repo:** https://github.com/jsvegam/HexapodIA

---

**¿Dudas o contribuciones?** Abre un [Issue](https://github.com/jsvegam/HexapodIA/issues) 🚀
