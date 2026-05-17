# Capacidades de IA Autónoma - HexapodIA con Jetson Orin Nano

## Visión del Modo Autónomo

> **"Caminar inteligente: detectar, evitar, explorar."**

En modo autónomo, el HexapodIA debe:
1. ✅ **Caminar** de forma fluida y adaptativa
2. ✅ **Identificar objetos** en su camino
3. ✅ **NO chocar** (obstacle avoidance)
4. ✅ **Explorar** espacios de forma autónoma
5. ✅ **Aprender** del entorno (mapping)

## Jetson Orin Nano - Capacidades IA

### Especificaciones Hardware

```
NVIDIA Jetson Orin Nano 8GB:
├── GPU: NVIDIA Ampere (1024 CUDA cores)
├── AI: 40 TOPS (Tensor Cores)
├── CPU: 6-core ARM Cortex-A78AE
├── RAM: 8GB LPDDR5
├── Storage: microSD (128GB recomendado)
├── Video: Dual CSI camera input
├── USB: 4× USB 3.0
├── Network: Gigabit Ethernet + WiFi module
├── Power: 5V-20V (modo 7W, 15W, o 25W)
└── Precio: ~€500
```

**Comparación poder computacional:**

| Dispositivo | TOPS | CUDA Cores | Precio |
|-------------|------|------------|--------|
| Jetson Orin Nano 8GB | 40 | 1024 | €500 |
| Jetson Xavier NX | 21 | 384 | €450 |
| Jetson Nano (old) | 0.5 | 128 | €100 ❌ |
| Raspberry Pi 4 | ~0.1 | 0 | €60 ❌ |

**Veredicto:** Orin Nano es **80× más potente** que Jetson Nano original ✅

## Pipeline IA Completo

### Arquitectura de Software

```
┌──────────────────────────────────────────────────────────┐
│              Jetson Orin Nano (ROS2 Humble)              │
│                                                          │
│  ┌────────────────────────────────────────────────────┐ │
│  │              Perception Layer                      │ │
│  │  ┌──────────┐  ┌──────────┐  ┌─────────────┐     │ │
│  │  │ Camera   │→ │  YOLO    │→ │ Detection   │     │ │
│  │  │ USB/CSI  │  │  v8/v10  │  │ Tracking    │     │ │
│  │  └──────────┘  └──────────┘  └─────────────┘     │ │
│  │                                                    │ │
│  │  ┌──────────┐  ┌──────────┐  ┌─────────────┐     │ │
│  │  │ Depth    │→ │ Point    │→ │ Obstacle    │     │ │
│  │  │ Camera   │  │ Cloud    │  │ Map         │     │ │
│  │  └──────────┘  └──────────┘  └─────────────┘     │ │
│  └────────────────────────────────────────────────────┘ │
│                                                          │
│  ┌────────────────────────────────────────────────────┐ │
│  │              Mapping & Localization                │ │
│  │  ┌──────────────┐  ┌─────────────────────┐        │ │
│  │  │  ORB-SLAM3   │  │    Occupancy Grid   │        │ │
│  │  │  or RTAB-Map │→ │    (2D/3D map)     │        │ │
│  │  └──────────────┘  └─────────────────────┘        │ │
│  │                                                    │ │
│  │  ┌──────────────┐  ┌─────────────────────┐        │ │
│  │  │  IMU Fusion  │→ │   Pose Estimation   │        │ │
│  │  │  (ESP32→ROS) │  │   (x, y, θ)         │        │ │
│  │  └──────────────┘  └─────────────────────┘        │ │
│  └────────────────────────────────────────────────────┘ │
│                                                          │
│  ┌────────────────────────────────────────────────────┐ │
│  │           Planning & Navigation Layer              │ │
│  │  ┌──────────────┐  ┌─────────────────────┐        │ │
│  │  │  Global      │→ │   A* / Dijkstra     │        │ │
│  │  │  Planner     │  │   (waypoints)       │        │ │
│  │  └──────────────┘  └─────────────────────┘        │ │
│  │                             ↓                      │ │
│  │  ┌──────────────┐  ┌─────────────────────┐        │ │
│  │  │  Local       │→ │   DWA / TEB         │        │ │
│  │  │  Planner     │  │   (dynamic avoid)   │        │ │
│  │  └──────────────┘  └─────────────────────┘        │ │
│  │                             ↓                      │ │
│  │  ┌──────────────┐  ┌─────────────────────┐        │ │
│  │  │  Behavior    │→ │   State Machine     │        │ │
│  │  │  Tree        │  │   (explore/follow)  │        │ │
│  │  └──────────────┘  └─────────────────────┘        │ │
│  └────────────────────────────────────────────────────┘ │
│                                                          │
│  ┌────────────────────────────────────────────────────┐ │
│  │              Control Layer                         │ │
│  │  ┌──────────────┐                                  │ │
│  │  │  /cmd_vel    │──→ (twist message)               │ │
│  │  │  Publisher   │    linear.x, angular.z           │ │
│  │  └──────────────┘                                  │ │
│  └────────────────────────────────────────────────────┘ │
└──────────────────┬───────────────────────────────────────┘
                   │ WiFi (ROS2 DDS)
┌──────────────────▼───────────────────────────────────────┐
│                  ESP32-WROOM-32                          │
│           (Gait Generation + Servo Control)              │
└──────────────────────────────────────────────────────────┘
```

## 1. Detección de Objetos (Object Detection)

### YOLOv8 / YOLOv10 en Jetson

**YOLO (You Only Look Once):** Estado del arte para detección en tiempo real.

```python
# ROS2 Node ejemplo
import rclpy
from rclpy.node import Node
from sensor_msgs.msg import Image
from vision_msgs.msg import Detection2DArray
from ultralytics import YOLO
import cv2

class ObjectDetectorNode(Node):
    def __init__(self):
        super().__init__('object_detector')
        
        # Load YOLOv8 model (optimized for Jetson)
        self.model = YOLO('yolov8n.engine')  # TensorRT optimized
        
        # Subscribers
        self.image_sub = self.create_subscription(
            Image, '/camera/image_raw', 
            self.image_callback, 10)
        
        # Publishers
        self.detection_pub = self.create_publisher(
            Detection2DArray, '/detections', 10)
        
        self.get_logger().info('Object Detector ready')
    
    def image_callback(self, msg):
        # Convert ROS image to OpenCV
        frame = self.ros_to_cv2(msg)
        
        # Run YOLO inference
        results = self.model(frame, conf=0.5)
        
        # Parse detections
        detections = Detection2DArray()
        for r in results:
            for box in r.boxes:
                cls = int(box.cls[0])
                conf = float(box.conf[0])
                x1, y1, x2, y2 = box.xyxy[0].tolist()
                
                # Create detection message
                det = Detection2D()
                det.bbox.center.x = (x1 + x2) / 2
                det.bbox.center.y = (y1 + y2) / 2
                det.bbox.size_x = x2 - x1
                det.bbox.size_y = y2 - y1
                det.results[0].id = cls
                det.results[0].score = conf
                
                detections.detections.append(det)
        
        self.detection_pub.publish(detections)
```

**Clases detectables (COCO dataset):**
- Personas
- Animales (perro, gato, etc.)
- Vehículos (carro, bici, etc.)
- Objetos comunes (silla, mesa, laptop, etc.)
- **Total: 80 clases**

**Performance en Jetson Orin Nano:**
- YOLOv8n (nano): ~45 FPS @ 640×480 ✅
- YOLOv8s (small): ~30 FPS @ 640×480
- YOLOv8m (medium): ~15 FPS @ 640×480

**Recomendación:** YOLOv8n (balance velocidad/precisión)

### Identificación Específica

```python
# Objeto de interés detectado
def handle_detection(self, detection):
    class_name = self.get_class_name(detection.id)
    confidence = detection.score
    
    if class_name == "person" and confidence > 0.7:
        self.get_logger().info(f'Person detected! Conf: {confidence:.2f}')
        self.follow_person(detection.bbox)
    
    elif class_name == "dog" and confidence > 0.6:
        self.get_logger().info('Dog detected! Moving away...')
        self.avoid_target(detection.bbox)
    
    elif class_name in ["chair", "table", "couch"]:
        # Static obstacle
        self.mark_obstacle(detection.bbox)
```

## 2. Evitar Obstáculos (Obstacle Avoidance)

### Método 1: Vision-Based (Cámara + YOLO)

```python
class ObstacleAvoidance(Node):
    def __init__(self):
        # ...
        self.danger_zone_width = 0.5  # meters
        self.danger_zone_depth = 1.0  # meters
        
    def check_collision_risk(self, detections, depth_map):
        for det in detections:
            # Get 3D position from depth
            x, y, z = self.bbox_to_3d(det.bbox, depth_map)
            
            # Check if in danger zone
            if abs(x) < self.danger_zone_width/2 and \
               0 < z < self.danger_zone_depth:
                
                self.get_logger().warn(f'Obstacle ahead at {z:.2f}m!')
                return True, (x, y, z)
        
        return False, None
    
    def generate_avoidance_velocity(self, obstacle_pos):
        """Generate velocity to avoid obstacle"""
        x_obs, _, z_obs = obstacle_pos
        
        # Simple reactive avoidance
        if z_obs < 0.5:  # Very close
            # Stop and turn away
            linear_vel = 0.0
            angular_vel = 0.5 if x_obs > 0 else -0.5
        else:
            # Slow down and steer away
            linear_vel = 0.1
            angular_vel = 0.3 * x_obs  # Proportional steering
        
        return linear_vel, angular_vel
```

### Método 2: Depth Camera (Realsense D435i)

```python
# Intel RealSense D435i (~€200)
# Proporciona RGB + Depth en tiempo real

import pyrealsense2 as rs

class DepthObstacleAvoidance(Node):
    def __init__(self):
        # Configure depth camera
        self.pipeline = rs.pipeline()
        config = rs.config()
        config.enable_stream(rs.stream.depth, 640, 480, rs.format.z16, 30)
        config.enable_stream(rs.stream.color, 640, 480, rs.format.bgr8, 30)
        self.pipeline.start(config)
        
    def get_obstacle_map(self):
        frames = self.pipeline.wait_for_frames()
        depth_frame = frames.get_depth_frame()
        
        # Create 2D occupancy grid from depth
        depth_image = np.asanyarray(depth_frame.get_data())
        
        # Define ROI (region of interest) ahead of robot
        roi = depth_image[240:480, 160:480]  # Bottom-center
        
        # Find obstacles (depth < threshold)
        obstacles = roi < 1000  # < 1m is obstacle
        
        return obstacles
```

### Método 3: Costmap (ROS2 Nav2)

```python
# Nav2 Costmap-based obstacle avoidance
# Combina sensores múltiples en costmap unified

from nav2_msgs.msg import Costmap

class CostmapAvoidance(Node):
    def __init__(self):
        # Subscribe to costmap
        self.costmap_sub = self.create_subscription(
            Costmap, '/local_costmap/costmap',
            self.costmap_callback, 10)
        
        # DWA (Dynamic Window Approach) local planner
        self.setup_dwa_planner()
    
    def costmap_callback(self, costmap):
        # Costmap values:
        # 0 = free space
        # 100-253 = obstacles (closer = higher)
        # 254 = inscribed inflated obstacle
        # 255 = lethal obstacle
        
        # Find safe path through costmap
        safe_velocity = self.dwa_planner.compute_velocity(costmap)
        self.cmd_vel_pub.publish(safe_velocity)
```

## 3. Mapping & Localization (SLAM)

### SLAM: Simultaneous Localization and Mapping

**Construir mapa del entorno Y saber dónde está el robot en ese mapa.**

### Opción A: ORB-SLAM3 (Visual SLAM)

```bash
# Installation
cd ~/ros2_ws/src
git clone https://github.com/UZ-SLAMLab/ORB_SLAM3.git
cd ORB_SLAM3
./build.sh

# ROS2 wrapper
git clone https://github.com/thien94/orb_slam3_ros2.git
colcon build --packages-select orb_slam3_ros2
```

**Características:**
- ✅ Visual-only (no depth needed)
- ✅ Monocular, stereo, or RGB-D
- ✅ Loop closure detection
- ✅ Map reuse (save/load)
- ⚠️ CPU intensive

**Performance Jetson Orin:** ~20 FPS ✅

### Opción B: RTAB-Map (RGB-D SLAM)

```bash
# Installation
sudo apt install ros-humble-rtabmap-ros

# Launch
ros2 launch rtabmap_ros rtabmap.launch.py \
  rgb_topic:=/camera/color/image_raw \
  depth_topic:=/camera/depth/image_raw \
  camera_info_topic:=/camera/color/camera_info
```

**Características:**
- ✅ RGB-D optimized (usa depth)
- ✅ 2D + 3D mapping
- ✅ Memory management (large environments)
- ✅ ROS2 native
- ✅ Visualization tools (RViz)

**Recomendado para HexapodIA** ✅

### Mapping Output

```python
# Occupancy Grid Map (2D)
# Usado para path planning

from nav_msgs.msg import OccupancyGrid

class MapHandler(Node):
    def __init__(self):
        self.map_sub = self.create_subscription(
            OccupancyGrid, '/map',
            self.map_callback, 10)
        
        self.map_data = None
    
    def map_callback(self, msg):
        # msg.data: array de valores 0-100
        # 0 = free space (blanco)
        # 100 = occupied (negro)
        # -1 = unknown (gris)
        
        self.map_data = np.array(msg.data).reshape(
            msg.info.height, msg.info.width)
        
        # Save map to file
        self.save_map('/home/hexapod/maps/current.pgm')
```

## 4. Path Planning & Navigation

### Global Planner (A* / Dijkstra)

```python
from nav2_msgs.action import NavigateToPose

class GlobalNavigator(Node):
    def __init__(self):
        self.nav_client = ActionClient(
            self, NavigateToPose, 'navigate_to_pose')
    
    def navigate_to_waypoint(self, x, y, theta):
        goal = NavigateToPose.Goal()
        goal.pose.header.frame_id = 'map'
        goal.pose.pose.position.x = x
        goal.pose.pose.position.y = y
        goal.pose.pose.orientation.z = sin(theta / 2)
        goal.pose.pose.orientation.w = cos(theta / 2)
        
        self.nav_client.send_goal_async(goal)
        self.get_logger().info(f'Navigating to ({x}, {y})')
```

### Local Planner (Dynamic Window Approach)

```python
# DWA: Genera velocidades considerando:
# - Obstáculos dinámicos
# - Límites cinemáticos del robot
# - Distancia al goal

class DWAPlanner:
    def __init__(self):
        self.max_linear_vel = 0.3   # m/s
        self.max_angular_vel = 1.0  # rad/s
        self.acc_linear = 0.2       # m/s²
        self.acc_angular = 0.5      # rad/s²
        
    def compute_velocity(self, costmap, current_vel, goal):
        # Generate velocity samples
        samples = self.generate_velocity_samples(current_vel)
        
        # Simulate trajectory for each sample
        best_vel = None
        best_score = -inf
        
        for vel in samples:
            trajectory = self.simulate_trajectory(vel)
            
            # Check collision
            if self.check_collision(trajectory, costmap):
                continue
            
            # Score trajectory
            score = self.evaluate_trajectory(
                trajectory, goal, costmap)
            
            if score > best_score:
                best_score = score
                best_vel = vel
        
        return best_vel
```

## 5. Behaviors Autónomos

### Behavior Tree (BT)

```python
import py_trees

class ExplorationBehavior(py_trees.behaviour.Behaviour):
    """Explore unknown areas"""
    def update(self):
        if self.has_unexplored_areas():
            frontier = self.get_nearest_frontier()
            self.navigate_to(frontier)
            return py_trees.common.Status.RUNNING
        else:
            return py_trees.common.Status.SUCCESS

class FollowPersonBehavior(py_trees.behaviour.Behaviour):
    """Follow detected person"""
    def update(self):
        person = self.get_detected_person()
        if person is not None:
            # Mantener distancia 1-2m
            distance = self.get_distance_to(person)
            if distance > 2.0:
                self.move_towards(person, speed=0.2)
            elif distance < 1.0:
                self.move_away(person, speed=0.1)
            else:
                self.stop()
            return py_trees.common.Status.RUNNING
        else:
            return py_trees.common.Status.FAILURE

# Behavior Tree Structure
root = py_trees.composites.Selector("Root")
root.add_children([
    EmergencyStopBehavior(),
    FollowPersonBehavior(),
    ObstacleAvoidanceBehavior(),
    ExplorationBehavior(),
    IdleBehavior()
])
```

### Modos de Operación Autónoma

```python
class AutonomousMode(Enum):
    EXPLORE = 1      # Explorar área, build map
    PATROL = 2       # Patrullar waypoints conocidos
    FOLLOW = 3       # Seguir persona/objeto
    RETURN_HOME = 4  # Volver a punto inicio
    GUARD = 5        # Quedarse quieto, vigilar

class AutonomousController(Node):
    def __init__(self):
        self.mode = AutonomousMode.EXPLORE
        
    def run(self):
        if self.mode == AutonomousMode.EXPLORE:
            self.explore_behavior()
        
        elif self.mode == AutonomousMode.PATROL:
            self.patrol_waypoints([
                (1.0, 0.0), (1.0, 1.0), 
                (0.0, 1.0), (0.0, 0.0)
            ])
        
        elif self.mode == AutonomousMode.FOLLOW:
            person = self.detect_person()
            if person:
                self.follow_target(person)
        
        elif self.mode == AutonomousMode.RETURN_HOME:
            self.navigate_to_pose(0, 0, 0)
```

## Performance Estimado

### Latencias por Módulo

```
Módulo                    Latencia    FPS
────────────────────────────────────────────
Camera capture            33ms        30
YOLO detection            22ms        45
Depth processing          33ms        30
SLAM update               50ms        20
Local planning            50ms        20
Command generation        10ms        100
ESP32 execution           20ms        50
────────────────────────────────────────────
Total perception→action   ~150ms      6-7 Hz
```

**Muy aceptable para navegación autónoma** ✅

### CPU/GPU Load Jetson Orin Nano

```
Módulo              CPU Load   GPU Load   TOPS
──────────────────────────────────────────────
Camera drivers      10%        0%         0
YOLO inference      5%         60%        15
SLAM (RTAB-Map)     40%        20%        5
Planning (Nav2)     20%        0%         0
ROS2 overhead       10%        0%         0
──────────────────────────────────────────────
Total               85%        80%        20/40

Margen:             15%        20%        ✅
```

## Casos de Uso Demostrados

### Demo 1: Indoor Navigation

```
Escenario: Casa/oficina
Objetivo: Ir de punto A a punto B evitando muebles

1. Robot inicia en punto A
2. SLAM construye mapa mientras explora
3. Usuario da waypoint B por app
4. Global planner calcula ruta A→B
5. Local planner navega evitando obstáculos
6. Si encuentra persona, espera que pase
7. Llega a punto B, confirma llegada
```

**Dificultad:** Media  
**Success rate esperado:** >90%

### Demo 2: Outdoor Exploration

```
Escenario: Jardín/parque
Objetivo: Explorar área y crear mapa

1. Robot inicia en home
2. Modo EXPLORE activado
3. Busca fronteras (unknown→known)
4. Navega a fronteras sistemáticamente
5. Evita obstáculos (plantas, piedras, desniveles)
6. IMU detecta terreno irregular, adapta velocidad
7. Después 10min, mapa completo
8. RETURN_HOME automático
```

**Dificultad:** Alta (terreno irregular)  
**Success rate esperado:** >70%

### Demo 3: Person Following

```
Escenario: Indoor/outdoor
Objetivo: Seguir persona manteniendo distancia

1. YOLO detecta persona (class "person")
2. Tracking mantiene ID consistente
3. Calcula distancia con depth camera
4. Si distancia > 2m → avanzar
5. Si distancia < 1m → retroceder
6. Si persona se detiene → robot se detiene
7. Si persona desaparece > 5s → modo EXPLORE
```

**Dificultad:** Media  
**Success rate esperado:** >85%

### Demo 4: Object Inspection

```
Escenario: Warehouse/almacén
Objetivo: Encontrar e inspeccionar objeto específico

1. Usuario dice "find laptop"
2. Robot navega explorando
3. YOLO busca class "laptop"
4. Cuando detectado → navega hacia él
5. Se posiciona frente al objeto
6. Captura foto high-res
7. Envía foto a usuario
8. Espera siguiente comando
```

**Dificultad:** Media-Alta  
**Success rate esperado:** >75%

## Hardware Final FASE 5

```
HexapodIA Autónomo:
├── ESP32-WROOM-32 (locomoción)
├── 18× STS3215 servos
├── BNO085 IMU
├── ELRS RX (manual override)
├── OLED display
├── Buzzer/Speaker
├── Jetson Orin Nano 8GB
├── USB Camera (Logitech C270 o similar)
├── Depth Camera (Realsense D435i - opcional)
├── WiFi module (communication)
├── Batería 8000mAh 3S
└── Todas las estructuras mecánicas
```

## Software Stack FASE 5

```
ROS2 Humble (Jetson):
├── Navigation2 (Nav2)
│   ├── AMCL localization
│   ├── Costmap 2D
│   ├── DWA local planner
│   └── Behavior Trees
├── SLAM
│   └── RTAB-Map (RGB-D SLAM)
├── Perception
│   ├── YOLOv8 (object detection)
│   ├── Depth processing
│   └── Tracking
├── micro-ROS (ESP32)
│   ├── /cmd_vel subscriber
│   ├── /odom publisher
│   └── /imu publisher
└── Custom nodes
    ├── Behavior controller
    ├── Mode switcher
    └── Telemetry
```

## Roadmap Implementación IA

### Milestone 1: Basic Perception
```
□ Setup Jetson Orin Nano
□ Install ROS2 Humble
□ Camera driver working
□ YOLO detection running
□ Object classification OK
```

### Milestone 2: Obstacle Avoidance
```
□ Depth camera (or depth estimation)
□ Costmap generation
□ Reactive avoidance working
□ Safe navigation <1m/s
```

### Milestone 3: SLAM & Mapping
```
□ RTAB-Map running
□ Map building functional
□ Localization accurate (<10cm)
□ Loop closure working
□ Map save/load
```

### Milestone 4: Autonomous Navigation
```
□ Nav2 stack configured
□ Global planning working
□ Local planning (DWA) tuned
□ Waypoint navigation OK
□ Return-to-home reliable
```

### Milestone 5: Advanced Behaviors
```
□ Person following
□ Object search
□ Exploration mode
□ Patrol mode
□ Voice commands (TTS)
```

---

**Resumen:** Con Jetson Orin Nano, el HexapodIA tendrá capacidades de:
- ✅ Detectar 80+ clases de objetos
- ✅ Evitar obstáculos en tiempo real
- ✅ Construir mapas del entorno (SLAM)
- ✅ Navegar autónomamente punto A→B
- ✅ Explorar espacios desconocidos
- ✅ Seguir personas/objetos
- ✅ Adaptar comportamiento al contexto

Todo con **override manual instantáneo** via ELRS por seguridad.
