#pragma once
#include "main.h"

class RotaryEncoder {
private:
    GPIO_TypeDef* portA;
    GPIO_TypeDef* portB;
    uint16_t pinA;
    uint16_t pinB;
    uint8_t reversePhase;  // مرحله مربوط به چرخش در جهت منفی
    uint8_t forwardPhase;  // مرحله مربوط به چرخش در جهت مثبت
    int volume;            // حجم فعلی
    bool powerStatus;      // وضعیت پاور

public:
    RotaryEncoder(GPIO_TypeDef* aPort, uint16_t aPin, GPIO_TypeDef* bPort, uint16_t bPin, bool initialPowerStatus = true)
        : portA(aPort), pinA(aPin), portB(bPort), pinB(bPin), reversePhase(0), forwardPhase(0), volume(0), powerStatus(initialPowerStatus) {}

    void setPowerStatus(bool status) {
        powerStatus = status;
    }

    int getVolume() const {
        return volume;
    }
    
    void setVolume(int value) {
        volume = value;
    }

    void handleInterrupt(uint16_t gpioPin) {
        if (powerStatus == false) return;

        // بررسی جهت چرخش مثبت
        if (gpioPin == pinA && HAL_GPIO_ReadPin(portA, pinA) == 0 && HAL_GPIO_ReadPin(portB, pinB) == 0 && forwardPhase == 0) {
            forwardPhase = 1;
            reversePhase = 0;
        } else if (gpioPin == pinB && HAL_GPIO_ReadPin(portB, pinB) == 1 && HAL_GPIO_ReadPin(portA, pinA) == 0 && forwardPhase == 1) {
            forwardPhase = 2;
            reversePhase = 0;
        } else if (gpioPin == pinA && HAL_GPIO_ReadPin(portA, pinA) == 1 && HAL_GPIO_ReadPin(portB, pinB) == 1 && forwardPhase == 2) {
            forwardPhase = 3;
            reversePhase = 0;
        } else if (gpioPin == pinB && HAL_GPIO_ReadPin(portB, pinB) == 0 && HAL_GPIO_ReadPin(portA, pinA) == 1 && forwardPhase == 3) {
            forwardPhase = 0;
            reversePhase = 0;
            volume++;
        }

        // بررسی جهت چرخش منفی
        if (gpioPin == pinA && HAL_GPIO_ReadPin(portA, pinA) == 0 && HAL_GPIO_ReadPin(portB, pinB) == 1 && reversePhase == 0) {
            reversePhase = 1;
            forwardPhase = 0;
        } else if (gpioPin == pinB && HAL_GPIO_ReadPin(portB, pinB) == 0 && HAL_GPIO_ReadPin(portA, pinA) == 0 && reversePhase == 1) {
            reversePhase = 2;
            forwardPhase = 0;
        } else if (gpioPin == pinA && HAL_GPIO_ReadPin(portA, pinA) == 1 && HAL_GPIO_ReadPin(portB, pinB) == 0 && reversePhase == 2) {
            reversePhase = 3;
            forwardPhase = 0;
        } else if (gpioPin == pinB && HAL_GPIO_ReadPin(portB, pinB) == 1 && HAL_GPIO_ReadPin(portA, pinA) == 1 && reversePhase == 3) {
            reversePhase = 0;
            forwardPhase = 0;
            volume--;
        }
    }
};

