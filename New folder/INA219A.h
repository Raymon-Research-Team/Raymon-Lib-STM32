#ifndef INA219_H
#define INA219_H

#include "main.h"

class INA219 {
public:
    INA219(I2C_HandleTypeDef *hi2c, uint8_t address = 0x40)
        : i2c_handle(hi2c), i2c_address(address << 1) 
		{
			calibrate();
		}

    float getVoltage() {
        uint16_t raw = readRegister(0x02); // خواندن رجیستر ولتاژ باس
        return raw * 0.004; // تبدیل مقدار خام به ولتاژ (در ولت)
    }

    float getCurrent() {
        uint16_t raw = readRegister(0x04); // خواندن رجیستر جریان
        return raw * 0.001; // تبدیل مقدار خام به جریان (در آمپر)
    }

    float getPower() {
        uint16_t raw = readRegister(0x03); // خواندن رجیستر توان
        return raw * 0.02; // تبدیل مقدار خام به توان (در وات)
    }

private:
    I2C_HandleTypeDef *i2c_handle;
    uint8_t i2c_address;

    void calibrate() {
        writeRegister(0x05, 4096); // تنظیم رجیستر کالیبراسیون
    }

    uint16_t readRegister(uint8_t reg) {
        uint8_t buffer[2];
        HAL_I2C_Mem_Read(i2c_handle, i2c_address, reg, 1, buffer, 2, HAL_MAX_DELAY);
        return (buffer[0] << 8) | buffer[1];
    }

    void writeRegister(uint8_t reg, uint16_t value) {
        uint8_t buffer[2];
        buffer[0] = (value >> 8) & 0xFF;
        buffer[1] = value & 0xFF;
        HAL_I2C_Mem_Write(i2c_handle, i2c_address, reg, 1, buffer, 2, HAL_MAX_DELAY);
    }
};

#endif // INA219_H
