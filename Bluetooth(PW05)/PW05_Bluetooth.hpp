#pragma once

#include "main.h"
#include <string.h>
#include <stdio.h>
#include <ctype.h>

class PW05_Bluetooth {
private:
    UART_HandleTypeDef *huart; // اشاره‌گر به پورت UART
    GPIO_TypeDef *ATMode_Port;
    uint16_t ATMode_Pin;
    GPIO_TypeDef *Sleep_Port;
    uint16_t Sleep_Pin;
    GPIO_TypeDef *Status_Port;
    uint16_t Status_Pin;

    //===============================================================================
    // ********** تابع خالی کردن بافر UART **********
    void Usart_Flush() {
        uint8_t temp;
        while (__HAL_UART_GET_FLAG(huart, UART_FLAG_RXNE)) {
            HAL_UART_Receive(huart, &temp, 1, 10);
        }
    }

    //===============================================================================
    // ********** ارسال دستورات AT و دریافت پاسخ **********
    void sendATCommand(const char *cmd, char *response, uint16_t responseSize, uint32_t timeout) {
        setATMode(true);
        char command[50];
        snprintf(command, sizeof(command), "%s\r\n", cmd);

        Usart_Flush();

        HAL_UART_Transmit(huart, (uint8_t *)command, strlen(command), HAL_MAX_DELAY);
       
        HAL_UART_Receive(huart, (uint8_t *)response, responseSize - 1, timeout);
        response[responseSize - 1] = '\0'; // اضافه کردن کاراکتر پایان رشته
        
        setATMode(false);
    }

    //===============================================================================
    // ********** تبدیل Baud Rate به کد مربوطه **********
    uint8_t baudRateToCode(int baudRate) {
        switch (baudRate) {
        case 9600:
            return 0;
        case 19200:
            return 1;
        case 38400:
            return 2;
        case 57600:
            return 3;
        case 115200:
            return 4;
        default:
            return 255; // Invalid baud rate
        }
    }

    //===============================================================================
    // ********** معکوس کردن MAC آدرس **********
    void reverseMAC(const char *mac, char *reversedMAC) {
        snprintf(reversedMAC,
            13,
            "%c%c%c%c%c%c%c%c%c%c%c%c",
            mac[10],
            mac[11],
            mac[8],
            mac[9],
            mac[6],
            mac[7],
            mac[4],
            mac[5],
            mac[2],
            mac[3],
            mac[0],
            mac[1]);
    }

public:
    struct ADVTime {
        static constexpr const char* T100ms  = "A000";
        static constexpr const char* T300ms  = "E001";
        static constexpr const char* T600ms  = "C003";
        static constexpr const char* T800ms  = "0005";
        static constexpr const char* T1000ms = "4006";
        static constexpr const char* T2000ms = "800C";
    };
    
    struct TXPower {
        static constexpr const char* P_10dbm = "0A";
        static constexpr const char* P_5dbm  = "02";
        static constexpr const char* P_3dbm  = "01";
        static constexpr const char* P_0dbm  = "00";
        static constexpr const char* P_n3dbm = "03";
        static constexpr const char* P_n5dbm = "04";
        static constexpr const char* P_n10dbm = "EC";
    };
    
    //===============================================================================
    // ********** تابع سازنده کلاس **********
    PW05_Bluetooth(UART_HandleTypeDef *huart, 
        GPIO_TypeDef *ATMode_Port,
        uint16_t ATMode_Pin, 
        GPIO_TypeDef *Sleep_Port,
        uint16_t Sleep_Pin, 
        GPIO_TypeDef *Status_Port,
        uint16_t Status_Pin) {
        this->huart = huart;
        this->ATMode_Port = ATMode_Port;
        this->ATMode_Pin = ATMode_Pin;
        this->Sleep_Port = Sleep_Port;
        this->Sleep_Pin = Sleep_Pin;
        this->Status_Port = Status_Port;
        this->Status_Pin = Status_Pin;
    }

    //===============================================================================
    // ********** تغییر به حالت AT Command **********
    void setATMode(bool action) {
        HAL_GPIO_WritePin(ATMode_Port, ATMode_Pin, action ? GPIO_PIN_SET : GPIO_PIN_RESET);
        HAL_Delay(50);
    }

    //===============================================================================
    // ********** فعال‌سازی ماژول **********
    void enable() {
        HAL_GPIO_WritePin(Sleep_Port, Sleep_Pin, GPIO_PIN_RESET);
        HAL_Delay(50);
    }

    //===============================================================================
    // ********** غیرفعال‌سازی ماژول **********
    void disable() {
        HAL_GPIO_WritePin(Sleep_Port, Sleep_Pin, GPIO_PIN_SET);
        HAL_Delay(50);
    }

    //===============================================================================
    // ********** بررسی وضعیت اتصال **********
    bool isConnected() {
        return HAL_GPIO_ReadPin(Status_Port, Status_Pin) == GPIO_PIN_RESET;
    }

    //===============================================================================
    // ********** تنظیم Baud Rate **********
    bool setBaudRate(int baudRate) {
        char command[20];
        uint8_t code = baudRateToCode(baudRate);
        if (code > 4) return false; // بررسی معتبر بودن Baud Rate

        snprintf(command, sizeof(command), "AT+BAUD=%d", code);
        char response[50];
        sendATCommand(command, response, sizeof(response), 100);
        return strstr(response, "OK") != nullptr;
    }

    //===============================================================================
    // ********** ریست ماژول **********
    bool reset() {
        char response[50];
        sendATCommand("AT+RESET", response, sizeof(response), 100);
        return strstr(response, "OK") != nullptr;
    }

    //===============================================================================
    // ********** تنظیم نام بلوتوث **********
    bool setName(const char *name) {
        if (name == nullptr) return false;
        char command[40];
        const int commandLength = snprintf(command, sizeof(command), "AT+NAME=%s", name);
        if (commandLength < 0 || static_cast<size_t>(commandLength) >= sizeof(command)) return false;
        char response[50];
        sendATCommand(command, response, sizeof(response), 100);
        return strstr(response, "Set NAME") != nullptr;
    }

    //===============================================================================
    // ********** دریافت MAC Address **********
    void getMAC(char *macBuffer) {
        char reversedMAC[13];
        sendATCommand("AT+MAC?", reversedMAC, 13, 100);
        reverseMAC(reversedMAC, macBuffer);
    }

    //===============================================================================
    // ********** تنظیم MAC Address **********
    bool setMAC(const char *macAddress) {
        if (strlen(macAddress) != 12) {
            return false; // طول MAC آدرس نامعتبر
        }
        for (int i = 0; i < 12; i++) {
            if (!isxdigit(macAddress[i])) {
                return false; // کاراکتر نامعتبر در MAC آدرس
            }
        }
        char command[30];
        char reversedMAC[13];
        reverseMAC(macAddress, reversedMAC);
    
        snprintf(command, sizeof(command), "AT+MAC=%s", reversedMAC);
        char response[50];
        sendATCommand(command, response, sizeof(response), 100);
        return strstr(response, "Set MAC") != nullptr;
    }

    //===============================================================================
    // ********** ارسال داده در حالت Transparent **********
    bool sendData(const uint8_t *data, uint16_t len) {
        if (huart == nullptr || (data == nullptr && len != 0U)) return false;
        if (len == 0U) return true;
        return HAL_UART_Transmit(huart, data, len, HAL_MAX_DELAY) == HAL_OK;
    }

    //===============================================================================
    // ********** دریافت داده **********
    bool receiveData(uint8_t *buffer, uint16_t len, uint16_t *receivedLen, uint32_t timeout) {
        if (huart == nullptr || buffer == nullptr || receivedLen == nullptr || len == 0U) {
            return false;
        }
        *receivedLen = 0U;
        HAL_StatusTypeDef status = HAL_UART_Receive(huart, buffer, len, timeout);
        if (status == HAL_OK) {
            *receivedLen = len;
            return true;
        }
        return false;
    }

    //===============================================================================
    // ********** قطع اتصال **********
    bool disconnect() {
        char response[50];
        sendATCommand("AT+DISCON", response, sizeof(response), 100);
        return strstr(response, "OK") != nullptr;
    }

    //===============================================================================
    // ********** ذخیره تنظیمات **********
    bool saveSettings() {
        char response[50];
        sendATCommand("AT+STORE=0", response, sizeof(response), 100);
        HAL_Delay(50);
        sendATCommand("AT+STORE=1", response, sizeof(response), 100);
        return strstr(response, "Set STORE:1") != nullptr;
    }

    //===============================================================================
    // ********** تنظیم قدرت ارسال (TX Power) **********
    bool setTXPower(const char *powerIndex) {
        char command[20];
        snprintf(command, sizeof(command), "AT+TXPW=%s", powerIndex);
        char response[50];
        sendATCommand(command, response, sizeof(response), 100);
        return strstr(response, "Set TXPW") != nullptr;
    }

    //===============================================================================
    // ********** دریافت قدرت سیگنال (RSSI) **********
    void getRSSI(char *rssiBuffer, uint16_t rssiBufferlen) {
        sendATCommand("AT+RSSI?", rssiBuffer, rssiBufferlen, 500);
    }

    //===============================================================================
    // ********** تنظیم بازه‌ی تبلیغات (Advertising Interval) **********
    bool setAdvertisingInterval(const char *interval) {
        char command[20];
        snprintf(command, sizeof(command), "AT+ADVT=%s", interval);
        char response[50];
        sendATCommand(command, response, sizeof(response), 100);
        return strstr(response, "Set ADVT") != nullptr;
    }

    //===============================================================================
    // ********** فعال‌سازی یا غیرفعال‌سازی تبلیغات **********
    bool enableAdvertising(bool enable) {
        char command[20];
        snprintf(command, sizeof(command), "AT+ENADV=%d", enable ? 1 : 0);
        char response[50];
        sendATCommand(command, response, sizeof(response), 100);
        return strstr(response, enable ? "Set ENADV:1" : "Set ENADV:0") != nullptr;
    }

    //===============================================================================
    // ********** تنظیم حالت SPP **********
    bool setSPPMode(bool enable) {
        char command[20];
        snprintf(command, sizeof(command), "AT+BTDISCOV=%d", enable ? 1 : 0);
        char response[50];
        sendATCommand(command, response, sizeof(response), 100);
        return strstr(response, enable ? "Set BTDISCOV:1" : "Set BTDISCOV:0") != nullptr;
    }

    //===============================================================================
    // ********** دریافت ولتاژ باتری **********
    float getBatteryVoltage() {
        char voltage[20];
        sendATCommand("AT+POWER?", voltage, 20, 500);
        int value = 0;
        if (sscanf(voltage, "%d", &value) == 1) {
            return ((float)value / 100);
        }
        return 0.0f; // مقدار نامعتبر
    }

    //===============================================================================
    // ********** تنظیم حالت ماژول (Master/Slave) **********
    bool setRole(bool role) {
        char command[20];
        snprintf(command, sizeof(command), "AT+ROLE=%d", role ? 1 : 0);
        char response[50];
        sendATCommand(command, response, sizeof(response), 100);
        return strstr(response, role ? "Set ROLE:1" : "Set ROLE:0") != nullptr;
    }

    //===============================================================================
    // ********** اسکن دستگاه‌های بلوتوث (فقط در حالت Master) **********
    void startScan(char *Buffer, uint16_t BufferLen) {
        sendATCommand("AT+SCAN=1", Buffer, BufferLen, 20000);
    }

    bool stopScan() {
        char response[50];
        sendATCommand("AT+SCAN=0", response, sizeof(response), 100);
        return strstr(response, "Set SCAN:0") != nullptr;
    }

    //===============================================================================
    // ********** اتصال به دستگاه بلوتوث (فقط در حالت Master) **********
    bool connectToDevice(const char *macAddress) {
        if (!isMasterMode()) {
            return false; // ماژول در حالت Master نیست
        }
        char command[30];
        snprintf(command, sizeof(command), "AT+CRETCONN=%s", macAddress);
        char response[50];
        sendATCommand(command, response, sizeof(response), 20000);
        return strstr(response, "CONOK") != nullptr;
    }

    bool isMasterMode() {
        char response[50];
        sendATCommand("AT+ROLE?", response, sizeof(response), 100);
        return strstr(response, "Read ROLE:1") != nullptr; // اگر پاسخ شامل "Read ROLE:1" باشد، ماژول در حالت Master است
    }
    //===============================================================================
    // ********** دریافت نسخه‌ی نرم‌افزار ماژول **********
    void getSoftwareVersion(char *versionBuffer, uint16_t versionBufferlen) {
        sendATCommand("AT+VERS?", versionBuffer, versionBufferlen, 100);
    }

    //===============================================================================
    // ********** بازگردانی تنظیمات کارخانه **********
    void initialConfig(char* BTName , char* BTMAC) {
        // 2. بازگردانی Baud Rate به حالت پیش‌فرض
        enable();
        
        disconnect();
        
        setBaudRate(115200);   

        setRole(false);
        
        // 7. فعال‌سازی حالت SPP
        setSPPMode(true);
        
         // 1. بازگردانی نام ماژول به حالت پیش‌فرض
        setName(BTName);

        // 3. بازگردانی MAC Address به حالت پیش‌فرض
        setMAC(BTMAC);        

        // 4. بازگردانی قدرت ارسال به حالت پیش‌فرض
        setTXPower(TXPower::P_10dbm);    
        
        // 6. فعال‌سازی تبلیغات
        enableAdvertising(false);   

        // 5. بازگردانی بازه‌ی تبلیغات به حالت پیش‌فرض
       //setAdvertisingInterval(ADVTime::T800ms);

        saveSettings();
    }
    //===============================================================================
};

