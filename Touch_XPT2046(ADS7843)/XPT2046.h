#pragma once

#include "main.h"
#include <stdint.h>

class XPT2046 {
 private:
 //------------------------------------------------------------------------------------------------
    // ثابت‌ها در بخش خصوصی
    static constexpr uint8_t TP_PRES_DOWN = 0x80;
    static constexpr uint8_t TP_CATH_PRES = 0x40;
    static constexpr uint8_t CMD_RDX = 0xD0;
    static constexpr uint8_t CMD_RDY = 0x90;
    static constexpr uint32_t MSEC_THRESHOLD = 50;
    static constexpr int32_t Z_THRESHOLD = 200;
    static const int16_t XPT_XMIN = 300;
    static const int16_t XPT_YMIN = 300;
    static const int16_t XPT_XMAX = 3900;
    static const int16_t XPT_YMAX = 3700;
    static const int16_t XPT_WIDTH = XPT_XMAX - XPT_XMIN;
    static const int16_t XPT_HEIGHT = XPT_YMAX - XPT_YMIN;
    
    // متغیرهای GUI width و height
    const uint16_t GUI_WIDTH;
    const uint16_t GUI_HEIGHT;
 //------------------------------------------------------------------------------------------------   
    // تنظیمات داخلی
    SPI_HandleTypeDef* spiHandle;
    GPIO_TypeDef* csPort;
    uint16_t csPin;
    bool initialized;
    uint32_t lastUpdateTime = 0;
    int16_t intx, inty;
//------------------------------------------------------------------------------------------------
    void setCS() {
        HAL_GPIO_WritePin(csPort, csPin, GPIO_PIN_SET);
    }
//------------------------------------------------------------------------------------------------
    void resetCS() {
        HAL_GPIO_WritePin(csPort, csPin, GPIO_PIN_RESET);
    }
//------------------------------------------------------------------------------------------------
    void writeByte(uint8_t num) {
        spiHandle->Instance->CR2 &= ~(SPI_DATASIZE_16BIT);
        __HAL_SPI_ENABLE(spiHandle);
        if (HAL_SPI_Transmit(spiHandle, &num, 1, 1000) != HAL_OK) {
            Error_Handler();
        }
        __HAL_SPI_DISABLE(spiHandle);
    }
//------------------------------------------------------------------------------------------------
    uint16_t readAD(uint8_t cmd) {
        uint8_t num[2];
        uint16_t ret;

        resetCS();
        writeByte(cmd);
        
        spiHandle->Instance->CR2 |= SPI_DATASIZE_16BIT;
        __HAL_SPI_ENABLE(spiHandle);
        if (HAL_SPI_Receive(spiHandle, num, 1, 1000) != HAL_OK) {
            Error_Handler();
        }
        __HAL_SPI_DISABLE(spiHandle);
        setCS();

        ret = (num[0] << 8) | num[1];
        ret >>= 3;
        ret &= (1 << 12) - 1;

        return ret;
    }
//------------------------------------------------------------------------------------------------
    int16_t bestTwoAvg(int16_t x, int16_t y, int16_t z) const {
        int16_t minVal = (x < y) ? ((x < z) ? x : z) : ((y < z) ? y : z);
        int16_t maxVal = (x > y) ? ((x > z) ? x : z) : ((y > z) ? y : z);
        return (x + y + z - minVal - maxVal) / 2;
    }
//------------------------------------------------------------------------------------------------
    bool readZ() {
        int16_t z1 = readAD(0xb1);
        int16_t z2 = readAD(0xc1);
        int32_t z = z1 + 4095 - z2;
        return z >= Z_THRESHOLD;
    }
//------------------------------------------------------------------------------------------------
    void readCoordinates() {
        int16_t data[3];
        data[0] = readAD(0x91);
        data[1] = readAD(0xd1);
        data[2] = readAD(0x91);
        
        intx = bestTwoAvg(data[0], data[1], data[2]);
        inty = bestTwoAvg(data[1], data[2], data[0]);
    }
//------------------------------------------------------------------------------------------------
    bool inRange(uint16_t value, int16_t min, int16_t max) const {
        return (value >= min && value <= max);
    }
//------------------------------------------------------------------------------------------------
    // تابع clamp در بخش خصوصی
    static uint16_t clamp(int32_t value, int32_t min, int32_t max) {
        return static_cast<uint16_t>((value < min) ? min : (value > max ? max : value));
    }
//------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------
public:
    // سازنده کلاس با ورودی‌های SPI و پین‌ها و سایز GUI
    XPT2046(SPI_HandleTypeDef* spi, GPIO_TypeDef* csPort, uint16_t csPin, uint16_t guiWidth, uint16_t guiHeight)
        : spiHandle(spi)
        , csPort(csPort)
        , csPin(csPin)
        , initialized(true)
        , intx(0)
        , inty(0)
        , GUI_WIDTH(guiWidth)
        , GUI_HEIGHT(guiHeight) {}
//------------------------------------------------------------------------------------------------
    bool update(uint16_t *x, uint16_t *y) {
        if (!initialized) return false;

        if (HAL_GetTick() - lastUpdateTime < MSEC_THRESHOLD) return false;
        lastUpdateTime = HAL_GetTick();

        if (!readZ()) return false;

        readCoordinates();

        *x = intx;
        *y = inty;

        return inRange(*x, XPT_XMIN, XPT_XMAX) && inRange(*y, XPT_YMIN, XPT_YMAX);
    }
//------------------------------------------------------------------------------------------------
    void getCoordinates(uint16_t *x, uint16_t *y) const {
        *x = static_cast<uint16_t>(intx);
        *y = static_cast<uint16_t>(inty);
    }

    void enableTouch(bool enable) {
        initialized = enable;
    }
//------------------------------------------------------------------------------------------------
    void ConvXPTtoILI(uint16_t *x, uint16_t *y) {
        int32_t tx = ((*x - XPT_XMIN) * GUI_WIDTH) / XPT_WIDTH;
        int32_t ty = ((*y - XPT_YMIN) * GUI_HEIGHT) / XPT_HEIGHT;

        *x = clamp(tx, 0, GUI_WIDTH - 1);
        *y = clamp(ty, 0, GUI_HEIGHT - 1);
    }
//------------------------------------------------------------------------------------------------

};
