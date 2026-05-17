#include "STS3215.h"

STS3215::STS3215(HardwareSerial &serial, int directionPin) {
    _serial = &serial;
    _directionPin = directionPin;
}

bool STS3215::begin(long baudRate) {
    _serial->begin(baudRate, SERIAL_8N1);

    if (_directionPin >= 0) {
        pinMode(_directionPin, OUTPUT);
        setDirection(false); // RX mode
    }

    delay(100);
    return true;
}

void STS3215::setDirection(bool tx) {
    if (_directionPin >= 0) {
        digitalWrite(_directionPin, tx ? HIGH : LOW);
        delayMicroseconds(50);
    }
}

uint8_t STS3215::calculateChecksum(uint8_t *data, uint8_t length) {
    uint8_t checksum = 0;
    for (uint8_t i = 2; i < length - 1; i++) {
        checksum += data[i];
    }
    return ~checksum;
}

bool STS3215::writePacket(uint8_t id, uint8_t instruction, uint8_t *params, uint8_t paramLen) {
    uint8_t length = paramLen + 2;
    uint8_t packet[64];

    packet[0] = 0xFF;
    packet[1] = 0xFF;
    packet[2] = id;
    packet[3] = length;
    packet[4] = instruction;

    for (uint8_t i = 0; i < paramLen; i++) {
        packet[5 + i] = params[i];
    }

    packet[5 + paramLen] = calculateChecksum(packet, 6 + paramLen);

    setDirection(true);
    _serial->write(packet, 6 + paramLen);
    _serial->flush();
    setDirection(false);

    return true;
}

int STS3215::readPacket(uint8_t *buffer, uint8_t bufferSize, uint32_t timeout) {
    uint32_t startTime = millis();
    uint8_t index = 0;

    while (millis() - startTime < timeout) {
        if (_serial->available()) {
            buffer[index++] = _serial->read();

            if (index >= 4) {
                uint8_t length = buffer[3];
                if (index >= length + 4) {
                    return index;
                }
            }

            if (index >= bufferSize) {
                return -1;
            }
        }
    }

    return index > 0 ? index : -1;
}

bool STS3215::writeByte(uint8_t id, uint8_t address, uint8_t value) {
    uint8_t params[2] = {address, value};
    return writePacket(id, INST_WRITE, params, 2);
}

bool STS3215::writeWord(uint8_t id, uint8_t address, uint16_t value) {
    uint8_t params[3] = {address, (uint8_t)(value & 0xFF), (uint8_t)(value >> 8)};
    return writePacket(id, INST_WRITE, params, 3);
}

int STS3215::readByte(uint8_t id, uint8_t address) {
    uint8_t params[2] = {address, 1};
    if (!writePacket(id, INST_READ, params, 2)) {
        return -1;
    }

    uint8_t response[16];
    int len = readPacket(response, sizeof(response));

    if (len >= 6 && response[2] == id) {
        return response[5];
    }

    return -1;
}

int STS3215::readWord(uint8_t id, uint8_t address) {
    uint8_t params[2] = {address, 2};
    if (!writePacket(id, INST_READ, params, 2)) {
        return -1;
    }

    uint8_t response[16];
    int len = readPacket(response, sizeof(response));

    if (len >= 7 && response[2] == id) {
        return response[5] | (response[6] << 8);
    }

    return -1;
}

bool STS3215::ping(uint8_t id) {
    writePacket(id, INST_PING, nullptr, 0);
    uint8_t response[16];
    int len = readPacket(response, sizeof(response));
    return (len >= 6 && response[2] == id);
}

bool STS3215::setPosition(uint8_t id, uint16_t position, uint16_t time, uint16_t speed) {
    uint8_t params[7];
    params[0] = STS_GOAL_POSITION_L;
    params[1] = position & 0xFF;
    params[2] = (position >> 8) & 0xFF;
    params[3] = time & 0xFF;
    params[4] = (time >> 8) & 0xFF;
    params[5] = speed & 0xFF;
    params[6] = (speed >> 8) & 0xFF;

    return writePacket(id, INST_WRITE, params, 7);
}

bool STS3215::setTorqueEnable(uint8_t id, bool enable) {
    return writeByte(id, STS_TORQUE_ENABLE, enable ? 1 : 0);
}

bool STS3215::setLED(uint8_t id, bool on) {
    return writeByte(id, STS_LED, on ? 1 : 0);
}

int16_t STS3215::readPosition(uint8_t id) {
    return readWord(id, STS_PRESENT_POSITION_L);
}

int16_t STS3215::readSpeed(uint8_t id) {
    return readWord(id, STS_PRESENT_SPEED_L);
}

int16_t STS3215::readLoad(uint8_t id) {
    return readWord(id, STS_PRESENT_LOAD_L);
}

uint8_t STS3215::readVoltage(uint8_t id) {
    int result = readByte(id, STS_PRESENT_VOLTAGE);
    return result >= 0 ? result : 0;
}

uint8_t STS3215::readTemperature(uint8_t id) {
    int result = readByte(id, STS_PRESENT_TEMPERATURE);
    return result >= 0 ? result : 0;
}

bool STS3215::isMoving(uint8_t id) {
    int result = readByte(id, STS_MOVING);
    return result > 0;
}

bool STS3215::syncWritePosition(uint8_t *ids, uint16_t *positions, uint8_t count, uint16_t time) {
    uint8_t params[256];
    params[0] = STS_GOAL_POSITION_L;
    params[1] = 4; // Data length per servo (pos_L, pos_H, time_L, time_H)

    for (uint8_t i = 0; i < count; i++) {
        params[2 + i * 5] = ids[i];
        params[3 + i * 5] = positions[i] & 0xFF;
        params[4 + i * 5] = (positions[i] >> 8) & 0xFF;
        params[5 + i * 5] = time & 0xFF;
        params[6 + i * 5] = (time >> 8) & 0xFF;
    }

    return writePacket(STS_BROADCAST_ID, INST_SYNC_WRITE, params, 2 + count * 5);
}

uint16_t STS3215::angleToPulse(float angle, float minAngle, float maxAngle) {
    float normalized = (angle - minAngle) / (maxAngle - minAngle);
    normalized = constrain(normalized, 0.0, 1.0);
    return (uint16_t)(normalized * 4095.0);
}

float STS3215::pulseToAngle(uint16_t pulse, float minAngle, float maxAngle) {
    float normalized = (float)pulse / 4095.0;
    return minAngle + normalized * (maxAngle - minAngle);
}

bool STS3215::setID(uint8_t oldID, uint8_t newID) {
    return writeByte(oldID, STS_ID, newID);
}

bool STS3215::setBaudRate(uint8_t id, uint32_t baudRate) {
    uint8_t baudValue = 0;

    switch(baudRate) {
        case 1000000: baudValue = 0; break;
        case 500000:  baudValue = 1; break;
        case 250000:  baudValue = 2; break;
        case 128000:  baudValue = 3; break;
        case 115200:  baudValue = 4; break;
        case 76800:   baudValue = 5; break;
        case 57600:   baudValue = 6; break;
        case 38400:   baudValue = 7; break;
        default: return false;
    }

    return writeByte(id, STS_BAUD_RATE, baudValue);
}

bool STS3215::setAngleLimits(uint8_t id, uint16_t minAngle, uint16_t maxAngle) {
    bool success = writeWord(id, STS_MIN_ANGLE_LIMIT_L, minAngle);
    success &= writeWord(id, STS_MAX_ANGLE_LIMIT_L, maxAngle);
    return success;
}

bool STS3215::setMaxTorque(uint8_t id, uint16_t torque) {
    return writeWord(id, STS_MAX_TORQUE_L, torque);
}
