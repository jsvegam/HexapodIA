#ifndef STS3215_H
#define STS3215_H

#include <Arduino.h>

// STS3215 Control Table
#define STS_MODEL_L             0x00
#define STS_MODEL_H             0x01
#define STS_FIRMWARE            0x02
#define STS_ID                  0x05
#define STS_BAUD_RATE           0x06
#define STS_RETURN_DELAY        0x07
#define STS_RETURN_LEVEL        0x08
#define STS_MIN_ANGLE_LIMIT_L   0x09
#define STS_MIN_ANGLE_LIMIT_H   0x0A
#define STS_MAX_ANGLE_LIMIT_L   0x0B
#define STS_MAX_ANGLE_LIMIT_H   0x0C
#define STS_MAX_TEMPERATURE     0x0D
#define STS_MAX_VOLTAGE         0x0E
#define STS_MIN_VOLTAGE         0x0F
#define STS_MAX_TORQUE_L        0x10
#define STS_MAX_TORQUE_H        0x11
#define STS_ALARM_LED           0x12
#define STS_ALARM_SHUTDOWN      0x13
#define STS_TORQUE_ENABLE       0x28
#define STS_LED                 0x29
#define STS_GOAL_POSITION_L     0x2A
#define STS_GOAL_POSITION_H     0x2B
#define STS_GOAL_TIME_L         0x2C
#define STS_GOAL_TIME_H         0x2D
#define STS_GOAL_SPEED_L        0x2E
#define STS_GOAL_SPEED_H        0x2F
#define STS_PRESENT_POSITION_L  0x38
#define STS_PRESENT_POSITION_H  0x39
#define STS_PRESENT_SPEED_L     0x3A
#define STS_PRESENT_SPEED_H     0x3B
#define STS_PRESENT_LOAD_L      0x3C
#define STS_PRESENT_LOAD_H      0x3D
#define STS_PRESENT_VOLTAGE     0x3E
#define STS_PRESENT_TEMPERATURE 0x3F
#define STS_PRESENT_STATUS      0x41
#define STS_MOVING              0x42
#define STS_PRESENT_CURRENT_L   0x45
#define STS_PRESENT_CURRENT_H   0x46

// Broadcast ID
#define STS_BROADCAST_ID        0xFE

class STS3215 {
public:
    STS3215(HardwareSerial &serial, int directionPin = -1);

    bool begin(long baudRate = 1000000);

    // Basic Control
    bool setPosition(uint8_t id, uint16_t position, uint16_t time = 0, uint16_t speed = 0);
    bool setTorqueEnable(uint8_t id, bool enable);
    bool setLED(uint8_t id, bool on);

    // Read Functions
    int16_t readPosition(uint8_t id);
    int16_t readSpeed(uint8_t id);
    int16_t readLoad(uint8_t id);
    uint8_t readVoltage(uint8_t id);
    uint8_t readTemperature(uint8_t id);
    bool isMoving(uint8_t id);

    // Configuration
    bool setID(uint8_t oldID, uint8_t newID);
    bool setBaudRate(uint8_t id, uint32_t baudRate);
    bool setAngleLimits(uint8_t id, uint16_t minAngle, uint16_t maxAngle);
    bool setMaxTorque(uint8_t id, uint16_t torque);

    // Sync Write (multiple servos)
    bool syncWritePosition(uint8_t *ids, uint16_t *positions, uint8_t count, uint16_t time = 0);

    // Utility
    bool ping(uint8_t id);
    uint16_t angleToPulse(float angle, float minAngle = 0.0, float maxAngle = 180.0);
    float pulseToAngle(uint16_t pulse, float minAngle = 0.0, float maxAngle = 180.0);

private:
    HardwareSerial *_serial;
    int _directionPin;

    void setDirection(bool tx);
    bool writePacket(uint8_t id, uint8_t instruction, uint8_t *params, uint8_t paramLen);
    int readPacket(uint8_t *buffer, uint8_t bufferSize, uint32_t timeout = 50);
    uint8_t calculateChecksum(uint8_t *data, uint8_t length);

    bool writeByte(uint8_t id, uint8_t address, uint8_t value);
    bool writeWord(uint8_t id, uint8_t address, uint16_t value);
    int readByte(uint8_t id, uint8_t address);
    int readWord(uint8_t id, uint8_t address);

    // Instructions
    static const uint8_t INST_PING = 0x01;
    static const uint8_t INST_READ = 0x02;
    static const uint8_t INST_WRITE = 0x03;
    static const uint8_t INST_REG_WRITE = 0x04;
    static const uint8_t INST_ACTION = 0x05;
    static const uint8_t INST_SYNC_WRITE = 0x83;
};

#endif
