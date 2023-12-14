#include "BMP280Emulator.h"

BMP280Emulator::BMP280Emulator(uint8_t address) : _address(address) {
}

void BMP280Emulator::begin() {
    Wire.begin(_address);
    Wire.onReceive([this](int bytes) { this->receiveEvent(bytes); });
    Wire.onRequest([this]() { this->requestEvent(); });
    initializeRegisters();
}

void BMP280Emulator::initializeRegisters() {
    // Initialize registers with default values
    _registers[BME280_REG_CHIPID] = CHIP_ID_BMP280;
    _registers[BME280_REG_RESET] = RESET_KEY;

    // DIG T1..3
    _registers[BMX280_REG_DIG_T1] = 27504;
    _registers[BMX280_REG_DIG_T2] = 26435;
    _registers[BMX280_REG_DIG_T3] = -1000;

    // DIG P1..9
    _registers[BMX280_REG_DIG_P1] = 36477;
    _registers[BMX280_REG_DIG_P2] = -10685;
    _registers[BMX280_REG_DIG_P3] = 3024;
    _registers[BMX280_REG_DIG_P4] = 2855;
    _registers[BMX280_REG_DIG_P5] = 140;
    _registers[BMX280_REG_DIG_P6] = -7;
    _registers[BMX280_REG_DIG_P7] = 15500;
    _registers[BMX280_REG_DIG_P8] = -14600;
    _registers[BMX280_REG_DIG_P9] = 6000;

    // DIG H1..6
    _registers[BME280_REG_DIG_H1] = 0x4B;
    _registers[BME280_REG_DIG_H2] = 0x0171;
    _registers[BME280_REG_DIG_H3] = 0x00;
    _registers[BME280_REG_DIG_H4] = 0x012F;
    _registers[BME280_REG_DIG_H5] = 0x0000;
    _registers[BME280_REG_DIG_H6] = 0x1E;

    // Other registers
    _registers[BME280_REG_CTRL_HUM] = 0x00;
    _registers[BMX280_REG_STATUS] = 0x00;
    _registers[BMX280_REG_CTRL_MEAS] = 0x00;
    _registers[BMX280_REG_CONFIG] = 0x00;
    _registers[BMX280_REG_PRESS] = 0x00;
    _registers[BMX280_REG_TEMP] = 8059200;
    _registers[BME280_REG_HUM] = 0x00;
}

void BMP280Emulator::update() {
    // Assuming Temperature and Pressure are class members
    // Update temperature and pressure readings
    Temperature = getMappedTemperature();
    Pressure = getMappedPressure();

    // Update corresponding registers with new values
    _registers[BMX280_REG_TEMP] = Temperature << 4; // Temperature register
    _registers[BMX280_REG_PRESS] = Pressure << 4;   // Pressure register
}


float BMP280Emulator::getMappedTemperature() {
    // Read a value from an analog pin (like A1 in the original code)
    // This assumes you have a temperature sensor connected to that pin
    int sensorValue = analogRead(A1);

    // Map the sensor value to a desired range
    // The original code maps from the range 0-1023 to a specific temperature range
    // Adjust the range according to your sensor's specifications or desired output
    return map(sensorValue, 0, 1023, 5019200 >> 4, 11296000 >> 4);
}

float BMP280Emulator::getMappedPressure() {
    // Read a value from an analog pin (like A0 in the original code)
    // This assumes you have a pressure sensor connected to that pin
    int sensorValue = analogRead(A0);

    // Map the sensor value to a desired range
    // The original code maps from the range 0-1023 to a specific pressure range
    // Adjust the range according to your sensor's specifications or desired output
    return map(sensorValue, 0, 1023, 14806688 >> 4, 39806688 >> 4);
}

void BMP280Emulator::receiveEvent(int bytes) {
    if (Wire.available() != 0) {
        for (int i = 0; i < bytes; i++) {
            _requestedRegister = Wire.read();
        }
    }
}

void BMP280Emulator::requestEvent() {
    if (_requestedRegister >= 0) {
        switch (_requestedRegister) {
            case BMX280_REG_DIG_T1:
                write16_LE(_registers[BMX280_REG_DIG_T1]);
                break;
            case BMX280_REG_DIG_T2:
                write16_LE(_registers[BMX280_REG_DIG_T2]);
                break;
            case BMX280_REG_DIG_T3:
                write16_LE(_registers[BMX280_REG_DIG_T3]);
                break;
            case BMX280_REG_DIG_P1:
                write16_LE(_registers[BMX280_REG_DIG_P1]);
                break;
            case BMX280_REG_DIG_P2:
                write16_LE(_registers[BMX280_REG_DIG_P2]);
                break;
            case BMX280_REG_DIG_P3:
                write16_LE(_registers[BMX280_REG_DIG_P3]);
                break;
            case BMX280_REG_DIG_P4:
                write16_LE(_registers[BMX280_REG_DIG_P4]);
                break;
            case BMX280_REG_DIG_P5:
                write16_LE(_registers[BMX280_REG_DIG_P5]);
                break;
            case BMX280_REG_DIG_P6:
                write16_LE(_registers[BMX280_REG_DIG_P6]);
                break;
            case BMX280_REG_DIG_P7:
                write16_LE(_registers[BMX280_REG_DIG_P7]);
                break;
            case BMX280_REG_DIG_P8:
                write16_LE(_registers[BMX280_REG_DIG_P8]);
                break;
            case BMX280_REG_DIG_P9:
                write16_LE(_registers[BMX280_REG_DIG_P8]);
                break;
            case BMX280_REG_TEMP:
                write24(_registers[BMX280_REG_TEMP]);
                break;
            case BMX280_REG_PRESS:
                write24(_registers[BMX280_REG_PRESS]);
                break;
            default:
                if (_requestedRegister < NUM_REGISTERS) {
                    write8(_registers[_requestedRegister]); // Send data from the requested register
                } else {
                    // Handle invalid register request, for example, send a default value
                    write8(0);
                }
                break;
        }
    } else {
        // Handle invalid register request, for example, send a default value
        write8(0);
    }
}

void BMP280Emulator::write8(uint8_t value) {
    Wire.write(value & 0xFF);
}

void BMP280Emulator::write16_LE(uint16_t value) {
    Wire.write(value & 0xFF);
    Wire.write(value >> 8 & 0xFF);
}

void BMP280Emulator::write24(uint32_t value) {
    Wire.write((value >> 16) & 0xFF);
    Wire.write((value >> 8) & 0xFF);
    Wire.write(value & 0xFF);
}
