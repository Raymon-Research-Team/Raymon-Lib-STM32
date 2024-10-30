#pragma once
#include <cstdint>
#include <cstring> 

/*
========================================================
Paket Format:
========================================================
Start Packet(0xAA)
Packet Counter ,
Packet len_MSB ,
Packet len_LSB ,
crc1 ,
data[] ,
crc2 ,
End packet(0xBB)
========================================================
*/

class QPacket {

private:
    constexpr static uint8_t Start_Packet = 0xAA;
    constexpr static uint8_t END_Packet = 0xBB;
    inline static uint8_t Packet_Counter = 1; // مقداردهی اولیه Counter
    QPacket();
    
public:
    // تابع ساخت بسته
    static uint8_t Create(const uint8_t* inputData, uint16_t inputSize, uint8_t* outputData, uint16_t& outputSize) {
        // بررسی سایز ورودی
        if (inputSize < 1 || inputSize > 65000 || outputData == nullptr) {
            return 0;
        }

        outputSize = inputSize + 7;
        uint8_t len_MSB =  (uint8_t)((inputSize + 3) >> 8) & 0xFF;
        uint8_t len_LSB =  (uint8_t)((inputSize + 3) & 0xFF);
        
        
        uint8_t crc1 = Start_Packet ^ QPacket::Packet_Counter ^ len_MSB ^ len_LSB;

        // شروع بسته
        outputData[0] = Start_Packet;
        outputData[1] = QPacket::Packet_Counter;
        
        // length of packet
        outputData[2] = len_MSB;
        outputData[3] = len_LSB;
        
        outputData[4] = crc1;
        
        uint8_t crc2 =  crc1;

        // کپی داده‌ها و محاسبه CRC
        for (int i = 0; i < inputSize; i++) {
            outputData[i + 5] = inputData[i];
            crc2 ^= inputData[i];
        }

        // اضافه کردن CRC و پایان بسته
        crc2 ^= END_Packet;
        outputData[inputSize + 5] = crc2;
        outputData[inputSize + 6] = END_Packet;
        
        QPacket::Packet_Counter += 1;
        if (QPacket::Packet_Counter == 0) QPacket::Packet_Counter = 1;
        return QPacket::Packet_Counter;
    }
};
