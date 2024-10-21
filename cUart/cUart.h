#pragma once
#include "main.h"
#include <iostream>

// پیاده‌سازی کلاس سفارشی برای هدایت خروجی (cout) به USART
class UartOutput : public std::streambuf {
private:
    UART_HandleTypeDef* huart; // اشاره‌گر به huart برای انتقال داده
public:
    // سازنده برای دریافت huart
    UartOutput(UART_HandleTypeDef* huart)
        : huart(huart) {}

protected:
    virtual int overflow(int c) override {
        if (c == '\n') {
            HAL_UART_Transmit(huart, (uint8_t*)"\r", 1, HAL_MAX_DELAY); // ارسال '\r' قبل از '\n'
        }
        HAL_UART_Transmit(huart, (uint8_t*)&c, 1, HAL_MAX_DELAY); // ارسال کاراکتر به پورت سریال
        return c;
    }
};

// پیاده‌سازی کلاس سفارشی برای دریافت ورودی (cin) از USART
class UartInput : public std::streambuf {
private:
    UART_HandleTypeDef* huart; // اشاره‌گر به huart برای دریافت داده
public:
    // سازنده برای دریافت huart
    UartInput(UART_HandleTypeDef* huart)
        : huart(huart) {}

protected:
    virtual int underflow() override {
        uint8_t ch;
        HAL_UART_Receive(huart, &ch, 1, HAL_MAX_DELAY); // دریافت یک کاراکتر از پورت سریال
        if (ch == '\r') {
            ch = '\n'; // تبدیل '\r' به '\n'
        }
        setg((char*)&ch, (char*)&ch, (char*)&ch + 1); // قرار دادن کاراکتر در بافر ورودی
        return ch;
    }
};

// کلاس UartIO که همه چیز را در خود نگه می‌دارد
class cUart {
public:
    UartOutput uart_output; // شیء برای خروجی (cout)
    UartInput uart_input; // شیء برای ورودی (cin)
    std::ostream cout; // ostream برای ارسال داده به سریال
    std::istream cin; // istream برای دریافت داده از سریال

    // سازنده‌ای که همه اشیاء را مقداردهی می‌کند و huart را می‌گیرد
    cUart(UART_HandleTypeDef* huart)
        : uart_output(huart)
        , uart_input(huart)
        , cout(&uart_output)
        , cin(&uart_input) {}
};

