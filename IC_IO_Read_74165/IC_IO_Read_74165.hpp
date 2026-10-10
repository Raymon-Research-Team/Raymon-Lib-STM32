#pragma once
#include "main.h"
#include <new>

class IC_IO_Read_74165 {
private:
    GPIO_TypeDef* _LDPort;
    uint16_t _LDPin;
    GPIO_TypeDef* _INHPort;
    uint16_t _INHPin;
    GPIO_TypeDef* _CLKPort;
    uint16_t _CLKPin;
    GPIO_TypeDef** _inputPorts;
    uint16_t* _inputPins;
    uint8_t* io_data_read;
    uint8_t _numberOfIC;

public:
    IC_IO_Read_74165(GPIO_TypeDef* IC_LD_Port, uint16_t IC_LD_Pin,
        GPIO_TypeDef* IC_INH_Port, uint16_t IC_INH_Pin,
        GPIO_TypeDef* IC_CLK_Port, uint16_t IC_CLK_Pin,
        GPIO_TypeDef* IC_inputPorts[], uint16_t IC_inputPins[],
        uint8_t numberOfIC)
        : _LDPort(IC_LD_Port), _LDPin(IC_LD_Pin),
          _INHPort(IC_INH_Port), _INHPin(IC_INH_Pin),
          _CLKPort(IC_CLK_Port), _CLKPin(IC_CLK_Pin),
          _inputPorts(nullptr), _inputPins(nullptr), io_data_read(nullptr),
          _numberOfIC(numberOfIC) {
        if (_numberOfIC == 0U || IC_inputPorts == nullptr || IC_inputPins == nullptr) {
            _numberOfIC = 0U;
            return;
        }

        _inputPorts = new (std::nothrow) GPIO_TypeDef*[_numberOfIC];
        _inputPins = new (std::nothrow) uint16_t[_numberOfIC];
        io_data_read = new (std::nothrow) uint8_t[_numberOfIC];
        if (_inputPorts == nullptr || _inputPins == nullptr || io_data_read == nullptr) {
            delete[] _inputPorts;
            delete[] _inputPins;
            delete[] io_data_read;
            _inputPorts = nullptr;
            _inputPins = nullptr;
            io_data_read = nullptr;
            _numberOfIC = 0U;
            return;
        }

        for (uint16_t i = 0U; i < _numberOfIC; ++i) {
            _inputPorts[i] = IC_inputPorts[i];
            _inputPins[i] = IC_inputPins[i];
            io_data_read[i] = 0U;
        }
        HAL_GPIO_WritePin(_INHPort, _INHPin, GPIO_PIN_SET);
    }

    ~IC_IO_Read_74165() {
        delete[] _inputPorts;
        delete[] _inputPins;
        delete[] io_data_read;
    }

    IC_IO_Read_74165(const IC_IO_Read_74165&) = delete;
    IC_IO_Read_74165& operator=(const IC_IO_Read_74165&) = delete;
    IC_IO_Read_74165(IC_IO_Read_74165&&) = delete;
    IC_IO_Read_74165& operator=(IC_IO_Read_74165&&) = delete;

    // Returns nullptr if construction failed; otherwise returns internal data
    // for _numberOfIC chained registers. The pointer remains owned by this object.
    uint8_t* read() {
        if (_numberOfIC == 0U || _inputPorts == nullptr ||
            _inputPins == nullptr || io_data_read == nullptr) return nullptr;

        HAL_GPIO_WritePin(_LDPort, _LDPin, GPIO_PIN_RESET);
        delay_ms(1U);
        HAL_GPIO_WritePin(_LDPort, _LDPin, GPIO_PIN_SET);
        delay_ms(1U);
        HAL_GPIO_WritePin(_INHPort, _INHPin, GPIO_PIN_RESET);
        delay_ms(1U);

        for (uint8_t bit = 0U; bit < 8U; ++bit) {
            read_pin(bit);
            HAL_GPIO_WritePin(_CLKPort, _CLKPin, GPIO_PIN_SET);
            delay_ms(1U);
            HAL_GPIO_WritePin(_CLKPort, _CLKPin, GPIO_PIN_RESET);
            delay_ms(1U);
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

    void read_pin(uint8_t bit) {
        for (uint16_t indexIC = 0U; indexIC < _numberOfIC; ++indexIC) {
            if (HAL_GPIO_ReadPin(_inputPorts[indexIC], _inputPins[indexIC]) == GPIO_PIN_SET) {
                io_data_read[indexIC] |= static_cast<uint8_t>(1U << bit);
            } else {
                io_data_read[indexIC] &= static_cast<uint8_t>(~(1U << bit));
            }
        }
    }
};