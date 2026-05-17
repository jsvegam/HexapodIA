#include "Kinematics.h"

Kinematics::Kinematics() {
    _coxaLength = COXA_LENGTH;
    _femurLength = FEMUR_LENGTH;
    _tibiaLength = TIBIA_LENGTH;
}

float Kinematics::constrainAngle(float angle, float minAngle, float maxAngle) {
    return constrain(angle, minAngle, maxAngle);
}

float Kinematics::lawOfCosines(float a, float b, float c) {
    // Returns angle in radians opposite to side c
    float cosValue = (a*a + b*b - c*c) / (2.0 * a * b);
    cosValue = constrain(cosValue, -1.0, 1.0);
    return acos(cosValue);
}

void Kinematics::getWorkspaceBounds(float &minReach, float &maxReach) {
    // Minimum reach: coxa + difference of other segments
    minReach = _coxaLength + abs(_femurLength - _tibiaLength);

    // Maximum reach: sum of all segments
    maxReach = _coxaLength + _femurLength + _tibiaLength;
}

bool Kinematics::isReachable(const Vector3 &footPos) {
    float minReach, maxReach;
    getWorkspaceBounds(minReach, maxReach);

    float distance = footPos.magnitude();

    // Check if within reachable sphere
    if (distance < minReach || distance > maxReach) {
        return false;
    }

    // Check height constraints
    if (footPos.z < -(maxReach) || footPos.z > maxReach) {
        return false;
    }

    return true;
}

JointAngles Kinematics::inverseKinematics(const Vector3 &footPos) {
    JointAngles result;
    result.valid = false;

    // Check if target is reachable
    if (!isReachable(footPos)) {
        Serial.println("Target unreachable");
        return result;
    }

    // Step 1: Calculate Coxa angle (rotation around Z axis)
    float coxaAngleRad = atan2(footPos.y, footPos.x);
    result.coxa = radToDeg(coxaAngleRad);

    // Constrain coxa angle
    result.coxa = constrainAngle(result.coxa, COXA_MIN_ANGLE, COXA_MAX_ANGLE);

    // Step 2: Project to 2D plane for femur-tibia calculation
    float horizontalDistance = sqrt(footPos.x * footPos.x + footPos.y * footPos.y);

    // Distance from coxa joint to foot in 2D
    float distFromCoxa = horizontalDistance - _coxaLength;
    float verticalDistance = footPos.z;

    // Distance from femur joint to foot
    float targetDistance = sqrt(distFromCoxa * distFromCoxa + verticalDistance * verticalDistance);

    // Check if femur-tibia system can reach
    if (targetDistance > _femurLength + _tibiaLength) {
        Serial.println("Femur-tibia unreachable");
        return result;
    }

    // Step 3: Calculate femur and tibia angles using law of cosines

    // Angle at foot (between tibia and target line)
    float angleAtFoot = lawOfCosines(_femurLength, targetDistance, _tibiaLength);

    // Angle at femur joint (between femur and target line)
    float angleAtFemur = lawOfCosines(_femurLength, _tibiaLength, targetDistance);

    // Angle of target line from horizontal
    float targetAngleRad = atan2(verticalDistance, distFromCoxa);

    // Final angles
    result.femur = radToDeg(targetAngleRad + angleAtFemur);
    result.tibia = 180.0 - radToDeg(angleAtFoot);

    // Constrain angles to servo limits
    result.femur = constrainAngle(result.femur, FEMUR_MIN_ANGLE, FEMUR_MAX_ANGLE);
    result.tibia = constrainAngle(result.tibia, TIBIA_MIN_ANGLE, TIBIA_MAX_ANGLE);

    result.valid = true;
    return result;
}

Vector3 Kinematics::forwardKinematics(const JointAngles &angles) {
    if (!angles.valid) {
        return Vector3(0, 0, 0);
    }

    // Convert to radians
    float coxaRad = degToRad(angles.coxa);
    float femurRad = degToRad(angles.femur);
    float tibiaRad = degToRad(angles.tibia);

    // Step 1: Position after coxa rotation (in XY plane)
    float coxaEndX = _coxaLength * cos(coxaRad);
    float coxaEndY = _coxaLength * sin(coxaRad);

    // Step 2: Position after femur (pitch in vertical plane)
    // Femur rotates in the plane defined by coxa direction
    float femurProjection = _femurLength * cos(femurRad);  // horizontal component
    float femurVertical = _femurLength * sin(femurRad);    // vertical component

    float femurEndX = coxaEndX + femurProjection * cos(coxaRad);
    float femurEndY = coxaEndY + femurProjection * sin(coxaRad);
    float femurEndZ = femurVertical;

    // Step 3: Position after tibia
    // CRITICAL FIX: tibia angle is relative to femur
    // If tibia = 180°, leg is straight (colinear with femur)
    // If tibia = 90°, makes right angle with femur
    float tibiaRelativeAngle = tibiaRad - degToRad(180.0);  // 180° = straight leg
    float tibiaAbsoluteAngle = femurRad + tibiaRelativeAngle;

    float tibiaProjection = _tibiaLength * cos(tibiaAbsoluteAngle);
    float tibiaVertical = _tibiaLength * sin(tibiaAbsoluteAngle);

    float footX = femurEndX + tibiaProjection * cos(coxaRad);
    float footY = femurEndY + tibiaProjection * sin(coxaRad);
    float footZ = femurEndZ + tibiaVertical;

    return Vector3(footX, footY, footZ);
}
