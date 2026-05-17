#ifndef KINEMATICS_H
#define KINEMATICS_H

#include <Arduino.h>
#include <math.h>

// Segment lengths (mm) - based on RookiDroid proportions
#define COXA_LENGTH  55.0
#define FEMUR_LENGTH 80.0
#define TIBIA_LENGTH 130.0

// Joint limits (degrees)
#define COXA_MIN_ANGLE  -90.0
#define COXA_MAX_ANGLE   90.0
#define FEMUR_MIN_ANGLE   0.0
#define FEMUR_MAX_ANGLE 180.0
#define TIBIA_MIN_ANGLE   0.0
#define TIBIA_MAX_ANGLE 180.0

struct Vector3 {
    float x, y, z;

    Vector3(float _x = 0, float _y = 0, float _z = 0) : x(_x), y(_y), z(_z) {}

    float magnitude() const {
        return sqrt(x*x + y*y + z*z);
    }

    Vector3 normalize() const {
        float mag = magnitude();
        if (mag > 0.0001) {
            return Vector3(x/mag, y/mag, z/mag);
        }
        return Vector3(0, 0, 0);
    }
};

struct JointAngles {
    float coxa;   // Yaw angle (degrees)
    float femur;  // Pitch angle (degrees)
    float tibia;  // Pitch angle (degrees)
    bool valid;   // IK solution valid

    JointAngles(float c = 0, float f = 0, float t = 0, bool v = true)
        : coxa(c), femur(f), tibia(t), valid(v) {}
};

class Kinematics {
public:
    Kinematics();

    // Inverse Kinematics: foot position (x,y,z) -> joint angles
    JointAngles inverseKinematics(const Vector3 &footPos);

    // Forward Kinematics: joint angles -> foot position
    Vector3 forwardKinematics(const JointAngles &angles);

    // Validate if target position is reachable
    bool isReachable(const Vector3 &footPos);

    // Get workspace bounds
    void getWorkspaceBounds(float &minReach, float &maxReach);

    // Utility functions
    static float degToRad(float deg) { return deg * DEG_TO_RAD; }
    static float radToDeg(float rad) { return rad * RAD_TO_DEG; }
    static float constrainAngle(float angle, float minAngle, float maxAngle);

private:
    float _coxaLength;
    float _femurLength;
    float _tibiaLength;

    // Law of cosines helper
    float lawOfCosines(float a, float b, float c);
};

#endif
