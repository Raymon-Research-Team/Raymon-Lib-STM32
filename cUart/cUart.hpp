#pragma once
#include "main.h"
#include <iostream>
#include <streambuf>
#include <cstdint>

// Blocking UART stream adapters. Do not use from an ISR.
class UartOutput : public std::streambuf {
public:
    explicit UartOutput(UART_HandleTypeDef* uart) : huart(uart) {}

protected:
    int_type overflow(int_type c) override {
        if (traits_type::eq_int_type(c, traits_type::eof())) {
            return traits_type::not_eof(c);
        }
        if (huart == nullptr) return traits_type::eof();
        uint8_t byte = static_cast<uint8_t>(traits_type::to_char_type(c));
        if (HAL_UART_Transmit(huart, &byte, 1U, HAL_MAX_DELAY) != HAL_OK) {
            return traits_type::eof();
        }
        return traits_type::not_eof(c);
    }

    std::streamsize xsputn(const char* data, std::streamsize count) override {
        if (huart == nullptr || data == nullptr || count <= 0) return 0;
        std::streamsize sent = 0;
        while (sent < count) {
            const std::streamsize chunk = (count - sent > 0xFFFF)
                ? 0xFFFF : count - sent;
            if (HAL_UART_Transmit(huart,
                    reinterpret_cast<uint8_t*>(const_cast<char*>(data + sent)),
                    static_cast<uint16_t>(chunk), HAL_MAX_DELAY) != HAL_OK) break;
            sent += chunk;
        }
        return sent;
    }

private:
    UART_HandleTypeDef* huart;
};

class UartInput : public std::streambuf {
public:
    explicit UartInput(UART_HandleTypeDef* uart) : huart(uart), currentByte(0U) {
        setg(&currentByte, &currentByte, &currentByte);
    }

protected:
    int_type underflow() override {
        if (gptr() < egptr()) return traits_type::to_int_type(*gptr());
        if (huart == nullptr ||
            HAL_UART_Receive(huart, &currentByte, 1U, HAL_MAX_DELAY) != HAL_OK) {
            return traits_type::eof();
        }
        setg(&currentByte, &currentByte, &currentByte + 1);
        return traits_type::to_int_type(currentByte);
    }

private:
    UART_HandleTypeDef* huart;
    char currentByte; // persistent storage: remains valid after underflow returns
};

class cUart {
public:
    UartOutput uart_output;
    UartInput uart_input;
    std::ostream cout;
    std::istream cin;

    explicit cUart(UART_HandleTypeDef* huart)
        : uart_output(huart), uart_input(huart),
          cout(&uart_output), cin(&uart_input) {}
};