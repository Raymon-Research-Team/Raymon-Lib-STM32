#pragma once
#include "main.h"
#include <string>
#include <cctype>  // برای استفاده از تابع toupper

class IC_7Segment_74Hc595 {
private:
    GPIO_TypeDef* _dsPort; // پورت مربوط به DS
    uint16_t _dsPin; // پین مربوط به DS
    GPIO_TypeDef* _shPort; // پورت مربوط به SH
    uint16_t _shPin; // پین مربوط به SH
    GPIO_TypeDef* _stPort; // پورت مربوط به ST
    uint16_t _stPin; // پین مربوط به ST
    GPIO_TypeDef* _mrPort; // پورت مربوط به MR
    uint16_t _mrPin; // پین مربوط به MR
    
    uint8_t _numberOf7segment; // تعداد سون سگمنت
    bool _Inverted_Output;
    
    

public:
    // سازنده برای مقدار دهی اولیه پورت‌ها و پین‌ها
    IC_7Segment_74Hc595(GPIO_TypeDef* DS_Port,
        uint16_t DS_Pin,
        GPIO_TypeDef* SH_Port,
        uint16_t SH_Pin,
        GPIO_TypeDef* ST_Port,
        uint16_t ST_Pin,
        GPIO_TypeDef* MR_Port,
        uint16_t MR_Pin,
        uint8_t NumberOf7segment=1,  // Number of seven-segment displays
        bool   Inverted_Output=false)   // Inverted output pin for CC or CA 7-segment
        : _dsPort(DS_Port)
        , _dsPin(DS_Pin)
        , _shPort(SH_Port)
        , _shPin(SH_Pin)
        , _stPort(ST_Port)
        , _stPin(ST_Pin)
        , _mrPort(MR_Port)
        , _mrPin(MR_Pin) 
        , _numberOf7segment(NumberOf7segment)   
        , _Inverted_Output(Inverted_Output)       
    {
        HAL_GPIO_WritePin(_mrPort, _mrPin, GPIO_PIN_SET); // فعال کردن
    }

    // تابع نمایش عدد اعشاری
    void DisplayDecimal(double number, int precision = 3) {
        char result[255]; // فضای بیشتر برای اطمینان
        DoubleToString(number, result, precision);
        DisplayString(result);

    }

    // تابع نمایش عدد صحیح
    void DisplayNumber(int32_t number) {
        DisplayString(std::to_string(number));
    }

    // تابع نمایش رشته
    void DisplayString(const std::string& str) {
        int len = str.length();
        if (len > _numberOf7segment) {
            len = _numberOf7segment;
        }
        uint8_t data_SegmentPattern[8] = { 0 };
        for (int i = 0; i < len; i++) {
            data_SegmentPattern[_numberOf7segment - len + i] = GetSegmentPattern(str[i]);
        }
        SendToIC(data_SegmentPattern);
    }

private:
    void delay_ms(uint32_t delay) {
#ifdef USE_FREERTOS
        vTaskDelay(pdMS_TO_TICKS(delay));
#else
        HAL_Delay(delay);
#endif
    }

    // الگوهای سگمنت برای اعداد و حروف
    uint8_t segmentPatterns[25] = {
        0b00111111,        // 0
        0b00000110,        // 1
        0b01011011,        // 2
        0b01001111,        // 3
        0b01100110,        // 4
        0b01101101,        // 5
        0b01111101,        // 6
        0b00000111,        // 7
        0b01111111,        // 8
        0b01101111,        // 9
        0b01000000,        // -
        0b01110111,        // A
        0b01111100,        // B
        0b00111001,        // C
        0b01011110,        // D
        0b01111001,        // E
        0b01110001,        // F
        0b10000000,        // .
        0b01110110,        // H
        0b00111000,        // L
        0b01011100,        // O
        0b01110011,        // P
        0b00111110,        // U
        0b00001000,        // _
        0b00000000         // 
    };

    // تابع ارسال داده به 74HC595
    void SendToIC(uint8_t data_SegmentPattern[]) {
        HAL_GPIO_WritePin(_mrPort, _mrPin, GPIO_PIN_RESET);
        delay_ms(1);
        HAL_GPIO_WritePin(_mrPort, _mrPin, GPIO_PIN_SET);

        for (int j = 0; j < _numberOf7segment; j++) {
            for (int i = 7; i >= 0; i--) {
                GPIO_PinState pin = ((data_SegmentPattern[j] >> i) & 0x1) ? GPIO_PIN_SET : GPIO_PIN_RESET;
                HAL_GPIO_WritePin(_dsPort, _dsPin, pin);

                delay_ms(1);
                HAL_GPIO_WritePin(_shPort, _shPin, GPIO_PIN_SET);
                delay_ms(1);
                HAL_GPIO_WritePin(_shPort, _shPin, GPIO_PIN_RESET);
            }
        }

        HAL_GPIO_WritePin(_stPort, _stPin, GPIO_PIN_SET);
        delay_ms(1);
        HAL_GPIO_WritePin(_stPort, _stPin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(_mrPort, _mrPin, GPIO_PIN_RESET);
    }

    // تابع دریافت الگوی سگمنت
    uint8_t GetSegmentPattern(char character_in) {
        char character = toupper(character_in);
        uint8_t segment = 0;
        if (character >= '0' && character <= '9') {
            segment = segmentPatterns[character - '0'];
        }
        else if (character >= 'A' && character <= 'F') {
            segment = segmentPatterns[character - 'A' + 11];
        }
        switch (character)
        {
        case '-':
            segment = segmentPatterns[10];   
            break;
        case '.':
            segment = segmentPatterns[17];  
            break;
        case 'H':
            segment = segmentPatterns[18];  
            break;  
        case 'L':
            segment = segmentPatterns[19];  
            break; 
        case 'O':
            segment = segmentPatterns[20];  
            break; 
        case 'P':
            segment = segmentPatterns[21];  
            break; 
        case 'U':
            segment = segmentPatterns[22];  
            break; 
        case '_':
            segment = segmentPatterns[23];  
            break; 
        case ' ':
            segment = segmentPatterns[24];  
            break; 
        default:
            break;
        }
        
        return (_Inverted_Output) ? ~segment : segment;
    }

    // تابع تبدیل عدد اعشاری به رشته
    void DoubleToString(double number, char* buffer, int precision) {
        char format[10];
        snprintf(format, sizeof(format), "%%.%df", precision); // قالب سازی
        snprintf(buffer, 20, format, number); // تبدیل عدد به متن
    }
};
