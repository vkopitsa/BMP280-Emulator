#ifndef BMP280Emulator_h
#define BMP280Emulator_h

#include <Arduino.h>
#include <Wire.h>

// Address and register definitions
#define ADDRESS 0x76

#define BME280_REG_CHIPID 0xD0 // Chip ID register
#define BME280_REG_RESET 0xE0  // Reset register

#define BMX280_REG_DIG_T1 0x88 // Temperature coefficient register
#define BMX280_REG_DIG_T2 0x8A // Temperature coefficient register
#define BMX280_REG_DIG_T3 0x8C // Temperature coefficient register

#define BMX280_REG_DIG_P1 0x8E // Pressure coefficient register
#define BMX280_REG_DIG_P2 0x90 // Pressure coefficient register
#define BMX280_REG_DIG_P3 0x92 // Pressure coefficient register
#define BMX280_REG_DIG_P4 0x94 // Pressure coefficient register
#define BMX280_REG_DIG_P5 0x96 // Pressure coefficient register
#define BMX280_REG_DIG_P6 0x98 // Pressure coefficient register
#define BMX280_REG_DIG_P7 0x9A // Pressure coefficient register
#define BMX280_REG_DIG_P8 0x9C // Pressure coefficient register
#define BMX280_REG_DIG_P9 0x9E // Pressure coefficient register

#define BME280_REG_DIG_H1 0xA1 // Humidity coefficient register
#define BME280_REG_DIG_H2 0xE1 // Humidity coefficient register
#define BME280_REG_DIG_H3 0xE3 // Humidity coefficient register
#define BME280_REG_DIG_H4 0xE4 // Humidity coefficient register
#define BME280_REG_DIG_H5 0xE5 // Humidity coefficient register
#define BME280_REG_DIG_H6 0xE7 // Humidity coefficient register

// Bit defines
#define CHIP_ID_BMP280 0x58 // BMP280 chip ID
#define RESET_KEY 0xB6      // Reset value for reset register
#define STATUS_IM_UPDATE 0  // im_update bit in status register

#define BME280_REG_CTRL_HUM 0xF2  // BME280: Control humidity register
#define BMX280_REG_STATUS 0XF3    // Status register
#define BMX280_REG_CTRL_MEAS 0xF4 // Control measure register
#define BMX280_REG_CONFIG 0xF5    // Config register
#define BMX280_REG_PRESS 0xF7     // Pressure data register
#define BMX280_REG_TEMP 0xFA      // Temperature data register
#define BME280_REG_HUM 0xFD       // Humidity data register

class BMP280Emulator {
public:
    BMP280Emulator(uint8_t address = ADDRESS);
    void begin();
    void update();
    float getMappedTemperature();
    float getMappedPressure();

private:
    uint8_t _address;
    static const int NUM_REGISTERS = 255;
    int _registers[NUM_REGISTERS];
    int _requestedRegister;

    long Temperature;
    long Pressure;

    void initializeRegisters();
    void receiveEvent(int bytes);
    void requestEvent();
    void write8(uint8_t value);
    void write16_LE(uint16_t value);
    void write24(uint32_t value);
};

#endif
