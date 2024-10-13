#pragma once
#include "main.h"
#include <iostream>
#include <cstring>
#include <cstdio>  
#include <string>  

//===============================================================================
class PrintfUart {
    
private:
    UART_HandleTypeDef* huart;
    UART_HandleTypeDef UART;
    bool isFeedBackInput;

    void DelayTransmit(int len) {
        #ifdef USE_FREERTOS            
                vTaskDelay((10 * 1000 * len / UART.Init.BaudRate)); // تاخیر بر اساس میلی‌ثانیه
        #else
                HAL_Delay((10 * 1000 * len / UART.Init.BaudRate));
        #endif
    }

public:
    PrintfUart(UART_HandleTypeDef* huart, bool isFeedBackInput)
        : huart(huart), UART(*huart), isFeedBackInput(isFeedBackInput) {}

    
    
//=============================================================================================    
public:    
    template<typename... Args>
    void printf(const std::string& format, Args... args) {
        char buffer[256]; 
        int len = snprintf(buffer, sizeof(buffer), format.c_str(), ConvertToCStr(args)...); // فرمت‌دهی رشته

        // بررسی اینکه آیا فرمت‌دهی موفقیت‌آمیز بوده است
        if (len < 0 || len >= sizeof(buffer)) {
            // در صورت خطا، می‌توانید پیام مناسبی را ارسال کنید یا رفتار مناسبی را اجرا کنید
            return;
        }

        HAL_UART_Transmit(huart, reinterpret_cast<uint8_t*>(buffer), len, HAL_MAX_DELAY); // ارسال داده‌ها به UART
        DelayTransmit(len);
    }

private:
    // تبدیل رشته به const char* و سایر انواع به خودشان
    const char* ConvertToCStr(const std::string& value) {
        return value.c_str();  // برای std::string، c_str برگردانده می‌شود
    }

    template<typename T>
    T ConvertToCStr(T value) {
        return value;  // برای انواع دیگر، خود مقدار برگردانده می‌شود
    }
//=============================================================================================
public:    
    template<typename... Args>
    void scanf(const std::string& format, Args&... args) {
        std::string buffer;
        char ch;

        // دریافت داده‌ها از UART
        while (true) {
            HAL_UART_Receive(huart, reinterpret_cast<uint8_t*>(&ch), 1, HAL_MAX_DELAY);
            if (isFeedBackInput) {
                HAL_UART_Transmit(huart, reinterpret_cast<uint8_t*>(&ch), 1, HAL_MAX_DELAY);
                DelayTransmit(1);
            }
            if (ch == '\n' || ch == '\r') {
                break;
            }
            buffer += ch;
        }

         // اگر تنها یک آرگمان وجود داشته باشد، همه ورودی را به آن اختصاص می‌دهیم
        if constexpr (sizeof...(args) == 1) {
            assignSingleArg(buffer, format, args...);
        } else {
            // برای چندین آرگمان از sscanf استفاده می‌کنیم
            // پردازش آرگومان‌ها
            parseArgs(buffer, format, args...);
        }
        
        
    }

private:
    // تابع برای پردازش هر آرگومان
    template<typename T>
    void parseSingleArg(std::string& buffer, std::string& format, T& arg) {
        size_t formatPos = format.find(' ');
        size_t bufferPos = buffer.find(' ');

        std::string bufferChunk = buffer.substr(0, bufferPos);  // استخراج بخش buffer
        std::string formatChunk = format.substr(0, formatPos);
        
        if constexpr (std::is_same_v<std::decay_t<T>, std::string>) {
            arg = bufferChunk.c_str(); 
        } else {
            
            sscanf(bufferChunk.c_str(), formatChunk.c_str(), &arg); // برای انواع دیگر
        }                                  

        // جلو بردن buffer و format
        buffer = (bufferPos == std::string::npos) ? "" : buffer.substr(bufferPos + 1);
        format = (formatPos == std::string::npos) ? "" : format.substr(formatPos + 1);
    }

    // تابع برای پیمایش و پردازش همه آرگومان‌ها
    template<typename... Args>
    void parseArgs(std::string buffer, std::string format, Args&... args) {
        // اجرای parseSingleArg برای هر آرگومان
        ((parseSingleArg(buffer, format, args)), ...);
    }
    
    template<typename T>
    void assignSingleArg(std::string buffer , std::string format, T& arg) {
        if constexpr (std::is_same<T, std::string>::value) {
            arg = buffer; // اگر آرگمان از نوع std::string باشد
        } 
        else if constexpr (std::is_array<T>::value && std::is_same<std::remove_extent_t<T>, char>::value) {
            // اگر آرگمان یک آرایه کاراکتری باشد
            strncpy(arg, buffer.c_str(), sizeof(arg) - 1);
            arg[sizeof(arg) - 1] = '\0'; // اطمینان از null-terminate بودن آرایه
        }
        else if constexpr (std::is_array<T>::value && std::is_same<std::remove_extent_t<T>,unsigned char>::value) {
            // اگر آرگمان یک آرایه کاراکتری باشد
            strncpy(reinterpret_cast<char*>(arg), buffer.c_str(), sizeof(arg) - 1);
            arg[sizeof(arg) - 1] = '\0'; // اطمینان از null-terminate بودن آرایه
        }
        else 
        {
            parseSingleArg(buffer, format, arg);
        }
        

    }
    

//=============================================================================================    
public:
    char getchar() {
        char ch;
        HAL_UART_Receive(huart, reinterpret_cast<uint8_t*>(&ch), 1, HAL_MAX_DELAY);
        if (isFeedBackInput) {
            HAL_UART_Transmit(huart, reinterpret_cast<uint8_t*>(&ch), 1, HAL_MAX_DELAY);
            DelayTransmit(1);
        }
        return ch;
    }
};
