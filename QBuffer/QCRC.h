#pragma once
#include <cstdint>
#include <cstring> 

class QCRC {

private:
    QCRC();
    //------------------------------------------------------------------------------
    static uint8_t updateCRC_8_CCITT(uint8_t crc) {
        for (int i = 0; i < 8; ++i) {
            if (crc & 0x80)
                crc = (crc << 1) ^ 0x07; // استفاده از پلی‌نوم 0x07 برای CRC-8-CCITT
            else
                crc <<= 1;
        }
        return crc;
    }
    //------------------------------------------------------------------------------
public:
    
    static uint8_t CRC_8_CCITT(const uint8_t* data, size_t length) {
        uint8_t crc = 0xFF; // مقدار اولیه‌ی CRC
        for (size_t i = 0; i < length; ++i) {
            crc ^= data[i];
            crc = updateCRC_8_CCITT(crc);
        }
        return crc;
    }
    //------------------------------------------------------------------------------
};
