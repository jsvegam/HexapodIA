# FASE 1 - Pierna Única: Objetivos y Validación

## Estado
🔄 **EN PROGRESO**

## Objetivos de la Fase

### 1. Validación Mecánica
- [ ] Imprimir piezas base en PETG (coxa, femur, tibia)
- [ ] Ensamblar pierna única con servos STS3215
- [ ] Validar rigidez estructural (sin flexión excesiva)
- [ ] Verificar backlash mínimo en articulaciones
- [ ] Confirmar rango de movimiento completo

### 2. Validación Eléctrica
- [ ] Conexión serial bus STS3215 @ 1Mbps
- [ ] Comunicación estable con 3 servos
- [ ] Medición de consumo (idle, movimiento, peak)
- [ ] Validación térmica (temperatura servos < 60°C)
- [ ] Verificar voltaje estable durante operación

### 3. Validación Software
- [x] Librería STS3215 funcional
- [x] Librería Kinematics (IK/FK)
- [x] Control individual de articulaciones
- [x] Control coordinado vía IK
- [ ] Test de precisión posicionamiento (±2mm)
- [ ] Test de repetibilidad

### 4. Validación Cinemática
- [ ] IK: posición objetivo → ángulos articulares
- [ ] FK: ángulos articulares → posición real
- [ ] Verificar workspace reachable
- [ ] Validar movimientos suaves (interpolación)
- [ ] Test de trayectorias complejas (círculos, líneas)

### 5. Validación de Torque
- [ ] Carga estática: pierna sosteniendo 1/6 peso robot
- [ ] Carga dinámica: movimiento bajo carga
- [ ] Margen de seguridad > 30%
- [ ] Sin stalls o sobrecalentamiento

## Criterios de Éxito

### Mecánico ✅
- Pierna soporta peso sin flexión > 2mm
- Backlash < 1°
- Rango completo: Coxa ±90°, Femur 0-180°, Tibia 0-180°

### Eléctrico ✅
- Comunicación serial estable (< 1% errores)
- Consumo peak < 2A por servo
- Temperatura < 60°C en operación continua

### Software ✅
- IK converge en < 5ms
- Precisión posicionamiento ±5mm
- Control loop estable @ 50Hz

### Funcional ✅
- Movimiento fluido entre posiciones
- Trayectorias precisas
- Sin vibraciones o jitter
- Respuesta inmediata a comandos

## Tests Implementados

### Test 1: Individual Joint Movement
**Objetivo:** Validar cada articulación independientemente

**Procedimiento:**
1. Mover coxa -45° a +45° (2s)
2. Mover femur 30° a 90° (2s)
3. Mover tibia 45° a 135° (2s)
4. Repetir ciclo

**Validación:**
- [ ] Movimiento suave sin saltos
- [ ] Sin stalls o bloqueos
- [ ] Rango completo alcanzado

### Test 2: IK Position Test
**Objetivo:** Validar cinemática inversa con posiciones estáticas

**Procedimiento:**
1. Forward: (200, 0, -60)
2. Up: (150, 0, -40)
3. Side: (150, 50, -60)
4. Home: (150, 0, -80)

**Validación:**
- [ ] IK solución válida para todas
- [ ] Posición real vs objetivo < 5mm
- [ ] Transiciones suaves

### Test 3: Circular Motion
**Objetivo:** Validar trayectorias complejas

**Procedimiento:**
1. Círculo radio 30mm en plano XZ
2. Centro: (150, 0, -60)
3. Período: 5s

**Validación:**
- [ ] Trayectoria circular precisa
- [ ] Sin vibraciones
- [ ] Velocidad constante

## Métricas a Recolectar

### Durante Tests
- Posición actual vs objetivo (error RMS)
- Temperatura servos (max, avg)
- Voltaje batería (min, avg)
- Corriente consumida (max, avg)
- Tiempo cálculo IK (max, avg)
- Errores comunicación serial

### Post-Test
- Backlash medido
- Precisión absoluta
- Repetibilidad (desviación estándar)
- Rango efectivo vs teórico
- Workspace alcanzable real

## Problemas Conocidos a Validar

### Potenciales Issues
1. **Backlash:** Posible juego en transmisión servo-estructura
2. **Flexión:** Piezas PETG pueden flexar bajo carga
3. **Comunicación:** Interferencias en bus serial
4. **Térmico:** Sobrecalentamiento en movimiento continuo
5. **Precisión IK:** Errores acumulativos en cadena cinemática

### Mitigaciones Planificadas
1. Diseño con tolerancias ajustadas + insertos térmicos
2. Refuerzos en femur/tibia, considerar relleno > 50%
3. Cable corto, shielded, termination apropiada
4. Thermal throttling en firmware, ventilación pasiva
5. Validación FK después de movimiento, corrección iterativa

## Siguiente Fase

### Condiciones para Avanzar a FASE 2
- [ ] Todos los tests pasados exitosamente
- [ ] Métricas dentro de rangos aceptables
- [ ] Issues críticos resueltos
- [ ] Documentación completa de resultados
- [ ] Aprobación para replicar diseño a 6 piernas

### FASE 2 Preview
- Diseñar estructura central
- Integrar 2-3 piernas
- Sincronización multi-pierna
- Gait básico (2 piernas alternadas)

## Notas
- Priorizar validación de locomoción sobre estética
- Documentar TODO: problemas, soluciones, mejoras
- Fotos/videos de cada test
- Baseline para comparación futura
