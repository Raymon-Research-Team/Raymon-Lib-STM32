#ifndef SERVO_H
#define SERVO_H

#include "stm32f1xx_hal.h"  // یا هر چی متناسب با MCU شماست

class Servo {
private:
    TIM_HandleTypeDef* timer;
    uint32_t channel;
    uint16_t minPulse;  // microseconds (e.g. 1000)
    uint16_t maxPulse;  // microseconds (e.g. 2000)

public:
    Servo(TIM_HandleTypeDef* htim, uint32_t ch,
          uint16_t min_us = 1000, uint16_t max_us = 2000)
        : timer(htim), channel(ch), minPulse(min_us), maxPulse(max_us) {}

    void begin() {
        HAL_TIM_PWM_Start(timer, channel);
    }

    void setAngle(uint8_t angle) {
        if (angle > 180) angle = 180;
        uint16_t pulse = minPulse + (angle * (maxPulse - minPulse)) / 180;
        __HAL_TIM_SET_COMPARE(timer, channel, pulse);
    }
};

#endif
