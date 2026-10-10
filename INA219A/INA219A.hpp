#pragma once
#include "main.h"
#include <cstdint>

class INA219 {
public:
    explicit INA219(I2C_HandleTypeDef* hi2c, uint8_t address = 0x40U)
        : i2c_handle(hi2c), i2c_address(static_cast<uint16_t>(address) << 1U),
          lastStatus(HAL_ERROR) {
        if (i2c_handle != nullptr && address <= 0x7FU) calibrate();
        else lastStatus = HAL_ERROR;
    }

    // Bus voltage in volts. Returns 0.0f on I2C error; inspect status().
    float getVoltage() {
        uint16_t raw = 0U;
        if (!readRegister(0x02U, raw)) return 0.0f;
        return static_cast<float>(raw >> 3U) * 0.004f;
    }

    // Current register is signed two's-complement; unit depends on calibration/shunt.
    int16_t getCurrentRaw() {
        uint16_t raw = 0U;
        if (!readRegister(0x04U, raw)) return 0;
        return static_cast<int16_t>(raw);
    }

    // Power register unit is 20 * Current_LSB; default calibration must match hardware.
    uint16_t getPowerRaw() {
        uint16_t raw = 0U;
        if (!readRegister(0x03U, raw)) return 0U;
        return raw;
    }

    HAL_StatusTypeDef status() const { return lastStatus; }

private:
    I2C_HandleTypeDef* i2c_handle;
    uint16_t i2c_address;
    HAL_StatusTypeDef lastStatus;

    bool calibrate() {
        // 4096 is not universally correct: this preserves the original example's
        // calibration constant and must be recalculated for shunt/current range.
        return writeRegister(0x05U, 4096U);
    }

    bool readRegister(uint8_t reg, uint16_t& value) {
        if (i2c_handle == nullptr) {
            lastStatus = HAL_ERROR;
            return false;
        }
        uint8_t buffer[2] = {0U, 0U};
        lastStatus = HAL_I2C_Mem_Read(i2c_handle, i2c_address, reg,
                                     I2C_MEMADD_SIZE_8BIT, buffer, 2U, 100U);
        if (lastStatus != HAL_OK) return false;
        value = static_cast<uint16_t>((static_cast<uint16_t>(buffer[0]) << 8U) |
                                      buffer[1]);
        return true;
    }

    bool writeRegister(uint8_t reg, uint16_t value) {
        if (i2c_handle == nullptr) {
            lastStatus = HAL_ERROR;
            return false;
        }
        uint8_t buffer[2] = {
            static_cast<uint8_t>(value >> 8U),
            static_cast<uint8_t>(value & 0xFFU)
        };
        lastStatus = HAL_I2C_Mem_Write(i2c_handle, i2c_address, reg,
                                      I2C_MEMADD_SIZE_8BIT, buffer, 2U, 100U);
        return lastStatus == HAL_OK;
    }
};