#pragma once
#include "QBuffer.hpp"
#include "QCRC.hpp"

/*
========================================================
Packet Format:
========================================================
Start Packet (0xAA)
Packet Counter
Packet len_MSB
Packet len_LSB
crc1
data[]
crc2
End Packet (0xBB)
========================================================
*/

class QCommand {
private:
    
    constexpr static uint8_t Start_Packet = 0xAA;
    constexpr static uint8_t END_Packet = 0xBB;
    constexpr static uint16_t Max_inputSize = 512;
    QCommand();
    //------------------------------------------------------------------------------

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
        // دریافت طول بسته و بررسی محدودیت آن
        uint16_t len = ((uint16_t)_buffer->PeekAt(2) << 8) | _buffer->PeekAt(3);
        if (len - 3 > Max_inputSize)
        {
            _buffer->Remove(1);
            return 0; 
        }
        //------------------------------------------------------------------------------
        // بررسی CRC1
        uint8_t tempCRC1[] = { Start_Packet, Packet_Counter, (uint8_t)(len >> 8), (uint8_t)len, END_Packet };
        uint8_t crc1 = QCRC::CRC_8_CCITT(tempCRC1, sizeof(tempCRC1));
        if (crc1 != _buffer->PeekAt(4)) {
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
        for (int i = 0; i < (len - 3); i++) {
            outputData[i] = _buffer->PeekAt(i + 5);
        }
        
        uint8_t crc2 =  QCRC::CRC_8_CCITT(outputData, len - 3);
        uint8_t tempCRC2[] = { crc1, crc2 };
        crc2 =  QCRC::CRC_8_CCITT(tempCRC2, sizeof(tempCRC2));

        //------------------------------------------------------------------------------
        // بررسی CRC
        if (crc2 != _buffer->PeekAt(len + 2)) {
            _buffer->Remove(1);
            return 0;
        }
        //------------------------------------------------------------------------------
        // حذف بسته پردازش شده
        _buffer->Remove(len + 5);
        //------------------------------------------------------------------------------
        // تعیین اندازه خروجی
        outputSize = len - 3;
        //------------------------------------------------------------------------------
        // خروج دادن عدد کانتر به نشانه صحت عملیات
        return Packet_Counter;
    }
};


        
