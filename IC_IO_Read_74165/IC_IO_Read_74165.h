#pragma once
#include "main.h"

class IC_IO_Read_74165 {
private:
    GPIO_TypeDef* _LDPort;
    uint16_t _LDPin;
    GPIO_TypeDef* _INHPort;
    uint16_t _INHPin;
    GPIO_TypeDef* _CLKPort;
    uint16_t _CLKPin;

    GPIO_TypeDef** _inputPorts;  // اشاره‌گر پویا به آرایه‌ای از پورت‌های ورودی
    uint16_t* _inputPins;        // اشاره‌گر پویا به آرایه‌ای از پین‌های ورودی
    uint8_t* io_data_read;       // اشاره‌گر پویا به آرایه داده‌های خوانده شده

    uint8_t _numberOfIC;         // تعداد آی‌سی‌ها

public:
    // سازنده برای مقدار دهی اولیه پورت‌ها و پین‌ها
    IC_IO_Read_74165(GPIO_TypeDef* IC_LD_Port,
        uint16_t IC_LD_Pin,
        GPIO_TypeDef* IC_INH_Port,
        uint16_t IC_INH_Pin,
        GPIO_TypeDef* IC_CLK_Port,
        uint16_t IC_CLK_Pin,
        GPIO_TypeDef* IC_inputPorts[],
        uint16_t IC_inputPins[],
        uint8_t numberOfIC)
        : _LDPort(IC_LD_Port)
        , _LDPin(IC_LD_Pin)
        , _INHPort(IC_INH_Port)
        , _INHPin(IC_INH_Pin)
        , _CLKPort(IC_CLK_Port)
        , _CLKPin(IC_CLK_Pin)
        , _numberOfIC(numberOfIC)
    {
        // تخصیص حافظه پویا برای آرایه‌های پورت‌ها و پین‌های ورودی و داده‌های خوانده شده
        _inputPorts = new GPIO_TypeDef*[_numberOfIC];
        _inputPins = new uint16_t[_numberOfIC];
        io_data_read = new uint8_t[_numberOfIC];

        // مقدار دهی پورت‌ها و پین‌های ورودی هر آی‌سی
        for (int i = 0; i < _numberOfIC; i++) {
            _inputPorts[i] = IC_inputPorts[i];
            _inputPins[i] = IC_inputPins[i];
            io_data_read[i] = 0; // مقداردهی اولیه داده‌های هر IC
        }

        HAL_GPIO_WritePin(_INHPort, _INHPin, GPIO_PIN_SET);
    }

    // مخرب برای آزادسازی حافظه
    ~IC_IO_Read_74165() {
        delete[] _inputPorts;
        delete[] _inputPins;
        delete[] io_data_read;
    }

    // تابع خواندن داده‌ها از ICها
    uint8_t* read() {
        HAL_GPIO_WritePin(_LDPort, _LDPin, GPIO_PIN_RESET);
        delay_ms(1);
        HAL_GPIO_WritePin(_LDPort, _LDPin, GPIO_PIN_SET);
        delay_ms(1);
        HAL_GPIO_WritePin(_INHPort, _INHPin, GPIO_PIN_RESET);
        delay_ms(1);

        for (int i = 0; i < 8; i++) {
            read_pin(i);
            HAL_GPIO_WritePin(_CLKPort, _CLKPin, GPIO_PIN_SET);
            delay_ms(1);
            HAL_GPIO_WritePin(_CLKPort, _CLKPin, GPIO_PIN_RESET);
            delay_ms(1);
        }

        HAL_GPIO_WritePin(_INHPort, _INHPin, GPIO_PIN_SET);
        return io_data_read;
    }

private:
    void delay_ms(uint32_t delay) {
#ifdef USE_FREERTOS
        vTaskDelay(pdMS_TO_TICKS(delay));
#else
        HAL_Delay(delay);
#endif
    }

    // تابع خواندن هر بیت از هر IC
    void read_pin(uint8_t n) {
        for (int indexIC = 0; indexIC < _numberOfIC; indexIC++) {
            if (HAL_GPIO_ReadPin(_inputPorts[indexIC], _inputPins[indexIC]) == GPIO_PIN_SET) {
                io_data_read[indexIC] |= (1 << n);  // تنظیم بیت مورد نظر
            } else {
                io_data_read[indexIC] &= ~(1 << n); // پاک کردن بیت مورد نظر
            }
        }
    }
};
