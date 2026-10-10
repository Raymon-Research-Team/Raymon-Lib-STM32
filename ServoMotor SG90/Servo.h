#ifndef SERVO_H
#define SERVO_H

#include "stm32f1xx_hal.h"

class Servo {
private:
    TIM_HandleTypeDef* timer;
    uint32_t channel;
    uint16_t minPulse;
    uint16_t maxPulse;

public:
    Servo(TIM_HandleTypeDef* htim, uint32_t ch,
          uint16_t min_us = 1000U, uint16_t max_us = 2000U)
        : timer(htim), channel(ch),
          minPulse(min_us <= max_us ? min_us : max_us),
          maxPulse(min_us <= max_us ? max_us : min_us) {}

    // Returns HAL status; timer PWM base period/prescaler must make compare units microseconds.
    HAL_StatusTypeDef begin() {
        if (timer == nullptr) return HAL_ERROR;
        return HAL_TIM_PWM_Start(timer, channel);
    }

    void setAngle(uint8_t angle) {
        if (timer == nullptr) return;
        if (angle > 180U) angle = 180U;
        const uint32_t span = static_cast<uint32_t>(maxPulse) - minPulse;
        const uint32_t pulse = static_cast<uint32_t>(minPulse) +
            (static_cast<uint32_t>(angle) * span) / 180U;
        __HAL_TIM_SET_COMPARE(timer, channel, pulse);
    }
};

#endif