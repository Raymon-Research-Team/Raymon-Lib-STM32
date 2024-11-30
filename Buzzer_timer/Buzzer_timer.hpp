#pragma once
#include "main.h"

//-------------------------------------------------------------------------------------------
class Buzzer
{
public:
    Buzzer(GPIO_TypeDef* BuzzerPort,
           uint16_t BuzzerPin,
           uint32_t BlinkCount = 1,
           uint32_t Ton = 100,
           uint32_t Toff = 100,
           bool invertLogic = false) // اضافه شدن پارامتر برای معکوس کردن
        : _BuzzerPort(BuzzerPort),
          _BuzzerPin(BuzzerPin),
          _BlinkCount(BlinkCount),
          _Ton(Ton),
          _Toff(Toff),
          _invertLogic(invertLogic),
          _counterTon(0),
          _counterToff(0),
          _counterBlink(0),
          _BuzzerEN(false)
    {}

    void on()
    {
        _BuzzerEN = true;
    }

    void off()
    {
        _BuzzerEN = false;
        _counterTon = 0;
        _counterToff = 0;
        _counterBlink = 0;
    }

    void handle()
    {
        if (!_BuzzerEN || _BlinkCount==0)
        {
            // خاموش کردن پین بر اساس مقدار invertLogic
            HAL_GPIO_WritePin(_BuzzerPort, _BuzzerPin, _invertLogic ? GPIO_PIN_SET : GPIO_PIN_RESET);
            return;
        }


        if (_counterTon < _Ton)
        {
            _counterTon++;
            // روشن کردن پین بر اساس مقدار invertLogic
            HAL_GPIO_WritePin(_BuzzerPort, _BuzzerPin, _invertLogic ? GPIO_PIN_RESET : GPIO_PIN_SET);
        }
        else if (_counterToff < _Toff)
        {
            _counterToff++;
            // خاموش کردن پین بر اساس مقدار invertLogic
            HAL_GPIO_WritePin(_BuzzerPort, _BuzzerPin, _invertLogic ? GPIO_PIN_SET : GPIO_PIN_RESET);
        }
        else if (_counterBlink+1 < _BlinkCount)
        {
            _counterBlink++;
            _counterTon = 0;
            _counterToff = 0;
        }
        else
        {
            off(); // Reset buzzer state
        }
    }

private:
    uint32_t _Ton;
    uint32_t _Toff;
    uint32_t _BlinkCount;

    uint32_t _counterTon;
    uint32_t _counterToff;
    uint32_t _counterBlink;

    bool _BuzzerEN;
    bool _invertLogic; // متغیر جدید برای معکوس کردن

    GPIO_TypeDef* _BuzzerPort;
    uint16_t _BuzzerPin;
};
