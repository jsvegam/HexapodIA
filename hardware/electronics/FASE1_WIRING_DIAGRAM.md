# Diagrama de Conexión FASE 1 - Pierna Única

## Lista de Componentes

```
✅ 1× ESP32-WROOM-32 DevKit v1
✅ 3× Feetech STS3215 servos
✅ 1× LiPo 3S 11.1V 2200mAh
✅ 1× Buck converter 12V 5A (servos)
✅ 1× Buck converter 5V 2A (ESP32)
✅ 1× Capacitor 1000µF 16V (servos)
✅ 1× Resistor 100kΩ (voltage divider)
✅ 1× Resistor 33kΩ (voltage divider)
✅ 1× Switch 10A (power on/off)
✅ 1× XT60 connector (batería)
✅ Cables Dupont/JST
```

## Diagrama Completo

```
                              ┌─────────────────┐
                              │  LiPo 3S 11.1V  │
                              │   2200mAh       │
                              └────────┬────────┘
                                       │
                                       │ XT60
                                       │
                              ┌────────┴────────┐
                              │  Switch 10A     │
                              │   (ON/OFF)      │
                              └────────┬────────┘
                                       │
                   ┌───────────────────┴───────────────────┐
                   │                                       │
          ┌────────▼─────────┐                   ┌────────▼─────────┐
          │  Buck 12V → 12V  │                   │  Buck 11.1V → 5V │
          │  (5A servos)     │                   │  (2A ESP32)      │
          └────────┬─────────┘                   └────────┬─────────┘
                   │                                       │
                   │ 12V                                   │ 5V
                   │                                       │
       ┌───────────┴───────────┐                          │
       │  Capacitor 1000µF 16V │                          │
       │  (anti-spike)         │                          │
       └───────────┬───────────┘                          │
                   │                                       │
         ┌─────────┴─────────┐                            │
         │                   │                            │
    ┌────▼────┐         ┌────▼────┐                      │
    │ Servo 0 │         │ Servo 1 │                      │
    │ (Coxa)  │────┬────│ (Femur) │                      │
    └─────────┘    │    └─────────┘                      │
                   │         │                            │
                   │    ┌────▼────┐                       │
                   │    │ Servo 2 │                       │
                   │    │ (Tibia) │                       │
                   │    └─────────┘                       │
                   │         │                            │
                   │         │ Data Line (Yellow)         │
                   │         │                            │
                   │    ┌────┴─────────────────────────┐  │
                   │    │                              │  │
                   │    │     ESP32-WROOM-32 DevKit    │  │
                   │    │                              │  │
                   │    │  GND  ───────────────────────┼──┴──┬──┐
                   └────┼─ GPIO16 (RX2)                │     │  │
                        │  GPIO17 (TX2) ───────────────┘     │  │
                        │                                     │  │
                        │  GPIO34 (ADC) ─┬──[100kΩ]──────────┘  │
                        │                └──[ 33kΩ]──────────GND │
                        │                                        │
                        │  5V  ──────────────────────────────────┘
                        │  GND ───────────────────────────────────┐
                        │                                         │
                        │  GPIO2 (LED) ──────────────[ LED ]      │
                        │  GPIO0 (BTN) ──────────────[BOOT]       │
                        └─────────────────────────────────────────┘
                                                                  │
                                                                 GND
```

## Conexiones Detalladas Paso a Paso

### 1. Alimentación Principal

```
LiPo 3S 11.1V
   │
   ├─[+]─→ XT60 Male ─→ Switch ─┬─→ Buck 12V IN(+)
   │                             └─→ Buck 5V IN(+)
   │
   └─[-]─→ XT60 Male ─→ GND ────┬─→ Buck 12V IN(-)
                                 └─→ Buck 5V IN(-)
```

### 2. Buck Converter Servos (12V)

```
Buck 12V:
   OUT(+) ─┬─→ Capacitor 1000µF (+)
           └─→ Servo 0 VCC (Red)
   
   OUT(-) ─┬─→ Capacitor 1000µF (-)
           ├─→ Servo 0 GND (Black)
           ├─→ ESP32 GND
           └─→ Common GND
```

### 3. Buck Converter ESP32 (5V)

```
Buck 5V:
   OUT(+) ─→ ESP32 pin "5V" o "VIN"
   
   OUT(-) ─→ ESP32 pin "GND"
```

### 4. Servos STS3215 (Daisy Chain)

```
Servo 0 (Coxa):
   - VCC (Red)   → Buck 12V OUT(+)
   - GND (Black) → Common GND
   - Data (Yellow) → Servo 1 Data

Servo 1 (Femur):
   - VCC (Red)   → Buck 12V OUT(+)
   - GND (Black) → Common GND
   - Data (Yellow) → Servo 2 Data

Servo 2 (Tibia):
   - VCC (Red)   → Buck 12V OUT(+)
   - GND (Black) → Common GND
   - Data (Yellow) → ESP32 GPIO16 + GPIO17
```

**Importante:** El data line es un **bus**, todos los servos comparten la misma línea.

### 5. Conexión Data Line Servos ↔ ESP32

```
Servo Data Bus:
   - Conectar cable Data de último servo a:
     - ESP32 GPIO17 (TX2)
     - ESP32 GPIO16 (RX2)
   
   - Usar cable corto (<30cm recomendado)
   - Opcional: twisted pair para reducir ruido
```

### 6. Voltage Divider (Battery Monitor)

```
LiPo (+) ───[100kΩ]───┬─── ESP32 GPIO34
                      │
                   [33kΩ]
                      │
Common GND ───────────┴─── ESP32 GND
```

**Cálculo:**
- 12.6V (full) → 3.12V en GPIO34 ✅
- 11.1V (nominal) → 2.75V en GPIO34
- 9.0V (empty) → 2.23V en GPIO34

## Código para Leer Batería

```cpp
// En main.cpp o archivo separado

#define BATTERY_PIN 34
#define VDIV_R1 100000.0  // 100kΩ
#define VDIV_R2 33000.0   // 33kΩ

float readBatteryVoltage() {
    int rawADC = analogRead(BATTERY_PIN);
    float vADC = (rawADC / 4095.0) * 3.3;  // ESP32 ADC is 12-bit, 3.3V ref
    float vBat = vADC * ((VDIV_R1 + VDIV_R2) / VDIV_R2);
    return vBat;
}

void checkBattery() {
    float voltage = readBatteryVoltage();
    
    Serial.printf("Battery: %.2fV ", voltage);
    
    if (voltage > 12.0) {
        Serial.println("(Full)");
    } else if (voltage > 11.1) {
        Serial.println("(Good)");
    } else if (voltage > 10.5) {
        Serial.println("(Medium)");
    } else if (voltage > 10.0) {
        Serial.println("(Low - Warning)");
    } else if (voltage > 9.5) {
        Serial.println("(Critical - Return Home)");
    } else {
        Serial.println("(EMPTY - STOP!)");
        emergencyStop();
    }
}
```

## Tabla de Conexiones Completa

### ESP32-WROOM-32

| Pin ESP32 | Conexión | Cable | Notas |
|-----------|----------|-------|-------|
| **5V** | Buck 5V OUT(+) | Rojo | Alimentación ESP32 |
| **GND** | Common GND | Negro | Múltiples GND juntos |
| **GPIO 17** | Servo Data Bus | Amarillo | TX2 → servos |
| **GPIO 16** | Servo Data Bus | Amarillo | RX2 ← servos |
| **GPIO 34** | Voltage divider | Cable | ADC battery monitor |
| **GPIO 2** | LED onboard | - | Ya conectado en placa |
| **GPIO 0** | BOOT button | - | Ya conectado en placa |

### Servos STS3215

| Servo | ID | VCC | GND | Data | Función |
|-------|----|----|-----|------|---------|
| 0 | 0 | Buck 12V+ | Common GND | Bus | Coxa (yaw) |
| 1 | 1 | Buck 12V+ | Common GND | Bus | Femur (pitch) |
| 2 | 2 | Buck 12V+ | Common GND | Bus | Tibia (pitch) |

## Checklist Pre-Encendida

### Hardware
- [ ] Batería cargada (12.6V)
- [ ] Switch en OFF
- [ ] Todos los GND conectados (común)
- [ ] Buck 12V ajustado a 12.0V (sin carga)
- [ ] Buck 5V ajustado a 5.0V (sin carga)
- [ ] Capacitor 1000µF soldado/conectado cerca servos
- [ ] Voltage divider ensamblado (R1=100K, R2=33K)
- [ ] Servo IDs configurados (0, 1, 2)
- [ ] Servos conectados en daisy chain
- [ ] Data line conectado a GPIO16+17
- [ ] Polaridad verificada (NO invertir +/-)

### Software
- [ ] Firmware compilado sin errores
- [ ] PlatformIO instalado
- [ ] ESP32 reconocido por USB
- [ ] Upload port detectado

### Safety
- [ ] Área despejada (servos pueden moverse)
- [ ] Pierna NO montada (primera prueba sin carga)
- [ ] Multímetro a mano
- [ ] Extintor cerca (broma, pero batería LiPo)

## Procedimiento Primera Encendida

### Paso 1: Verificación Sin Batería
```
1. Conectar ESP32 por USB solamente
2. Verificar LED power enciende
3. Abrir serial monitor (115200)
4. Verificar boot messages
```

### Paso 2: Verificación Buck Converters (Sin Servos)
```
1. Desconectar servos de buck 12V
2. Conectar batería
3. Switch ON
4. Medir voltaje buck 12V: debe ser 12.0V ±0.2V
5. Medir voltaje buck 5V: debe ser 5.0V ±0.1V
6. Switch OFF
7. Si voltajes OK → continuar
```

### Paso 3: Test Servos (Sin ESP32 comandos)
```
1. Conectar servos a buck 12V
2. NO conectar data line aún
3. Switch ON
4. Verificar servos NO se mueven (normal)
5. Tocar servos, deben estar fríos
6. Switch OFF
```

### Paso 4: Subir Firmware
```
1. Desconectar batería (switch OFF)
2. ESP32 conectado por USB
3. cd firmware/esp32_locomotion
4. pio run -t upload
5. Esperar "SUCCESS"
```

### Paso 5: Test Con Todo Conectado
```
1. Conectar data line servos a GPIO16+17
2. Conectar batería
3. Switch ON
4. Abrir serial monitor
5. Observar tests automáticos
6. Esperar "ALL TESTS PASSED"
```

### Paso 6: Primer Movimiento
```
1. Si tests PASS → continuar
2. Presionar 'h' en serial monitor
3. Observar pierna moverse a home
4. Verificar:
   - Movimiento suave
   - Sin ruidos
   - Sin humo/olor
   - Temperatura OK
```

## Troubleshooting Conexiones

### Problema: ESP32 no enciende
```
Verificar:
□ Buck 5V genera 5.0V
□ Cable 5V→ESP32 conectado
□ GND común
□ USB funciona? (sin batería)
```

### Problema: Servos no responden
```
Verificar:
□ IDs servos configurados (0, 1, 2)
□ Baudrate 1Mbps
□ Data line conectado GPIO16+17
□ VCC servos = 12V
□ GND común ESP32-Servos
```

### Problema: Voltaje batería lee 0V
```
Verificar:
□ Voltage divider ensamblado
□ R1 = 100kΩ (café-negro-amarillo)
□ R2 = 33kΩ (naranja-naranja-naranja)
□ Conectado a GPIO34 (no otro pin)
```

### Problema: Servo hace ruido/vibra
```
Verificar:
□ Capacitor 1000µF instalado
□ Buck 12V estable (medir con multímetro)
□ No hay cortocircuito
□ Servo no obstruido mecánicamente
```

## Medidas de Seguridad

### ⚠️ NUNCA
- Invertir polaridad batería
- Conectar 12V directo a ESP32
- Tocar servos en movimiento
- Dejar batería conectada desatendida
- Usar batería hinchada/dañada

### ✅ SIEMPRE
- Verificar voltajes con multímetro ANTES
- Switch OFF al hacer cambios
- GND común todas las partes
- Capacitor en línea servos
- Monitorear temperatura

## Fotos de Referencia (Placeholders)

```
[ Foto 1: Buck converters con etiquetas ]
[ Foto 2: Voltage divider ensamblado ]
[ Foto 3: Servos daisy chain ]
[ Foto 4: ESP32 con todos los cables ]
[ Foto 5: Setup completo FASE 1 ]
```

Cuando tengas hardware, puedes agregar fotos reales aquí.

## Próximo Paso

Después de validar FASE 1:
→ Ver [FASE_1_OBJECTIVES.md](../../docs/phase_reports/FASE_1_OBJECTIVES.md)
