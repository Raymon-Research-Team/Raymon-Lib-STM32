#pragma once
#include "QBuffer.h"

class QCommand {
private:
    constexpr static uint8_t Start_Packet = 0xAA;
    constexpr static uint8_t END_Packet = 0xBB;
    QCommand();

public:
    static uint8_t Check(QBuffer* _buffer, uint8_t* outputData, uint16_t& outputSize) {
        outputSize = 0;
        //------------------------------------------------------------------------------
        // بررسی طول اولیه
        if (_buffer->GetLength() < 8)   return 0;

        //------------------------------------------------------------------------------
        // بررسی شروع بسته
        if (_buffer->PeekAt(0) != Start_Packet) {
            _buffer->Remove(1);
            return 0;
        }

        //------------------------------------------------------------------------------
        //بررسی کانتر . همیشه بیشتر از 0 هست
        uint8_t Packet_Counter =  _buffer->PeekAt(1);
        if (Packet_Counter == 0) {
            _buffer->Remove(1);
            return 0;
        }
        //------------------------------------------------------------------------------
        //بررسی ماکزیمم مقدار مجاز طول بسته
        uint8_t len_MSB = _buffer->PeekAt(2);
        uint8_t len_LSB = _buffer->PeekAt(3);
        
        uint16_t len =  ((uint16_t)len_MSB << 8) + (uint16_t)len_LSB;
        if (len + 3 > 65007)            
        {
            _buffer->Remove(1);
            return 0;  
        }
        
        //------------------------------------------------------------------------------
        //بررسی صخت crc1
        uint8_t crc1 = Start_Packet ^ Packet_Counter ^ len_MSB ^ len_LSB;
        if (crc1 != _buffer->PeekAt(4))
        {
            _buffer->Remove(1);
            return 0;   
        }

        //------------------------------------------------------------------------------
        // بررسی مجدد طول بسته بر مبنای عدد len
        if (_buffer->GetLength() < (len + 4)) return false;

        //------------------------------------------------------------------------------
        // بررسی پایان بسته
        if (_buffer->PeekAt(len + 3) != END_Packet) {
            _buffer->Remove(1);
            return false;
        }
        
        //------------------------------------------------------------------------------
        // محاسبه CRC2 و کپی داده‌ها
        uint8_t crc2 = crc1;
        for (int i = 0; i < (len - 3); i++) {
            outputData[i] = _buffer->PeekAt(i + 5);
            crc2 ^= outputData[i];
        }

        crc2 ^= END_Packet;

        // بررسی CRC
        if (crc2 != _buffer->PeekAt(len + 2)) {
            _buffer->Remove(1);
            return 0;
        }

        // حذف بسته پردازش شده
        _buffer->Remove(len + 5);

        // تعیین اندازه خروجی
        outputSize = len - 3;
        return Packet_Counter;
    }
};
