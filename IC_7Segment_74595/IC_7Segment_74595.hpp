#pragma once
#include "main.h"
#include <string>
#include <cctype>
#include <cstdio>

class IC_7Segment_74595 {
private:
    GPIO_TypeDef* _dsPort;
    uint16_t _dsPin;
    GPIO_TypeDef* _shPort;
    uint16_t _shPin;
    GPIO_TypeDef* _stPort;
    uint16_t _stPin;
    GPIO_TypeDef* _mrPort;
    uint16_t _mrPin;
    uint8_t _numberOf7segment;
    bool _Inverted_Output;

public:
    IC_7Segment_74595(GPIO_TypeDef* DS_Port, uint16_t DS_Pin,
        GPIO_TypeDef* SH_Port, uint16_t SH_Pin,
        GPIO_TypeDef* ST_Port, uint16_t ST_Pin,
        GPIO_TypeDef* MR_Port, uint16_t MR_Pin,
        uint8_t NumberOf7segment = 1, bool Inverted_Output = false)
        : _dsPort(DS_Port), _dsPin(DS_Pin),
          _shPort(SH_Port), _shPin(SH_Pin),
          _stPort(ST_Port), _stPin(ST_Pin),
          _mrPort(MR_Port), _mrPin(MR_Pin),
          _numberOf7segment(NumberOf7segment > 8U ? 8U : NumberOf7segment),
          _Inverted_Output(Inverted_Output) {
        HAL_GPIO_WritePin(_mrPort, _mrPin, GPIO_PIN_SET);
    }

    void DisplayDecimal(double number, int precision = 3) {
        char result[32] = {};
        if (precision < 0) precision = 0;
        if (precision > 6) precision = 6;
        DoubleToString(number, result, sizeof(result), precision);
        DisplayString(result);
    }

    void DisplayNumber(int32_t number) { DisplayString(std::to_string(number)); }

    void DisplayString(const std::string& str) {
        if (_numberOf7segment == 0U) return;
        int len = static_cast<int>(str.length());
        if (len > _numberOf7segment) len = _numberOf7segment;
        uint8_t data_SegmentPattern[8] = {};
        for (int i = 0; i < len; ++i) {
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

    uint8_t segmentPatterns[25] = {
        0b00111111, 0b00000110, 0b01011011, 0b01001111, 0b01100110,
        0b01101101, 0b01111101, 0b00000111, 0b01111111, 0b01101111,
        0b01000000, 0b01110111, 0b01111100, 0b00111001, 0b01011110,
        0b01111001, 0b01110001, 0b10000000, 0b01110110, 0b00111000,
        0b01011100, 0b01110011, 0b00111110, 0b00001000, 0b00000000
    };

    void SendToIC(uint8_t data_SegmentPattern[]) {
        HAL_GPIO_WritePin(_mrPort, _mrPin, GPIO_PIN_RESET);
        delay_ms(1);
        HAL_GPIO_WritePin(_mrPort, _mrPin, GPIO_PIN_SET);
        for (uint8_t j = 0; j < _numberOf7segment; ++j) {
            for (int i = 7; i >= 0; --i) {
                GPIO_PinState pin = ((data_SegmentPattern[j] >> i) & 1U)
                    ? GPIO_PIN_SET : GPIO_PIN_RESET;
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

    uint8_t GetSegmentPattern(char character_in) {
        const char character = static_cast<char>(toupper(static_cast<unsigned char>(character_in)));
        uint8_t segment = segmentPatterns[24];
        if (character >= '0' && character <= '9') segment = segmentPatterns[character - '0'];
        else if (character >= 'A' && character <= 'F') segment = segmentPatterns[character - 'A' + 11];
        else {
            switch (character) {
                case '-': segment = segmentPatterns[10]; break;
                case '.': segment = segmentPatterns[17]; break;
                case 'H': segment = segmentPatterns[18]; break;
                case 'L': segment = segmentPatterns[19]; break;
                case 'O': segment = segmentPatterns[20]; break;
                case 'P': segment = segmentPatterns[21]; break;
                case 'U': segment = segmentPatterns[22]; break;
                case '_': segment = segmentPatterns[23]; break;
                case ' ': segment = segmentPatterns[24]; break;
                default: break;
            }
        }
        return _Inverted_Output ? static_cast<uint8_t>(~segment) : segment;
    }

    static void DoubleToString(double number, char* buffer, size_t bufferSize, int precision) {
        if (buffer == nullptr || bufferSize == 0U) return;
        const int result = snprintf(buffer, bufferSize, "%.*f", precision, number);
        if (result < 0) buffer[0] = '\0';
    }
};