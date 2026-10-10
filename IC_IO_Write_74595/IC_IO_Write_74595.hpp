#pragma once
#include "main.h"
#include <string>

class IC_IO_Write_74595 {
private:
    GPIO_TypeDef* _dsPort; // پورت مربوط به DS
    uint16_t _dsPin; // پین مربوط به DS
    GPIO_TypeDef* _shPort; // پورت مربوط به SH
    uint16_t _shPin; // پین مربوط به SH
    GPIO_TypeDef* _stPort; // پورت مربوط به ST
    uint16_t _stPin; // پین مربوط به ST
    GPIO_TypeDef* _mrPort; // پورت مربوط به MR
    uint16_t _mrPin; // پین مربوط به MR
    
    uint8_t _numberOfIC; // تعداد آی سی استفاده شده

public:
    // سازنده برای مقدار دهی اولیه پورت‌ها و پین‌ها
    IC_IO_Write_74595(GPIO_TypeDef* DS_Port,
        uint16_t DS_Pin,
        GPIO_TypeDef* SH_Port,
        uint16_t SH_Pin,
        GPIO_TypeDef* ST_Port,
        uint16_t ST_Pin,
        GPIO_TypeDef* MR_Port,
        uint16_t MR_Pin,
        uint8_t NumberOfIC=1)  // Number of PIN
        : _dsPort(DS_Port)
        , _dsPin(DS_Pin)
        , _shPort(SH_Port)
        , _shPin(SH_Pin)
        , _stPort(ST_Port)
        , _stPin(ST_Pin)
        , _mrPort(MR_Port)
        , _mrPin(MR_Pin) 
        , _numberOfIC(NumberOfIC)   
    {
        HAL_GPIO_WritePin(_mrPort, _mrPin, GPIO_PIN_SET); // فعال کردن
    }

    // تابع ارسال داده به 74HC595
    void write(const uint8_t* Data) {
        if (Data == nullptr || _numberOfIC == 0U) return;
        HAL_GPIO_WritePin(_mrPort, _mrPin, GPIO_PIN_RESET);
        delayms(1);
        HAL_GPIO_WritePin(_mrPort, _mrPin, GPIO_PIN_SET);
         

        for (int j = 0; j < _numberOfIC; j++) {
            for (int i = 8; i > 0; i--) {
                GPIO_PinState pin = ((Data[j] >> (i - 1)) & 0x1) ? GPIO_PIN_SET : GPIO_PIN_RESET;
                HAL_GPIO_WritePin(_dsPort, _dsPin, pin);

                delayms(1);
                HAL_GPIO_WritePin(_shPort, _shPin, GPIO_PIN_SET);
                delayms(1);
                HAL_GPIO_WritePin(_shPort, _shPin, GPIO_PIN_RESET);
            }
        }

        HAL_GPIO_WritePin(_stPort, _stPin, GPIO_PIN_SET);
        delayms(1);
        HAL_GPIO_WritePin(_stPort, _stPin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(_mrPort, _mrPin, GPIO_PIN_RESET);
    }
//---------------------------------------------------------------------------------------
    // تنظیم وضعیت یک پین خاص
    void statePin(volatile uint8_t* Data, uint8_t indexIC, uint8_t pinNumber, GPIO_PinState state)
    {
        state == GPIO_PIN_SET ? Data[indexIC] |= (1 << pinNumber) : Data[indexIC] &= ~(1 << pinNumber);
    }
//---------------------------------------------------------------------------------------
    // تغییر وضعیت یک پین خاص
    void togglePin(volatile uint8_t* Data, uint8_t indexIC, uint8_t pinNumber)
    {
        Data[indexIC] ^= (1 << pinNumber);
    }
    
    


    private :
        void delayms(uint32_t delay) {
#ifdef USE_FREERTOS
        vTaskDelay(pdMS_TO_TICKS(delay)) ;
#else
        HAL_Delay(delay) ;
#endif
    }
    

};
