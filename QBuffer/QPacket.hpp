#pragma once
#include <cstdint>
#include <cstring>
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

class QPacket {
private:
    constexpr static uint8_t Start_Packet = 0xAA;
    constexpr static uint8_t End_Packet = 0xBB;
    inline static uint8_t Packet_Counter = 0; // مقداردهی اولیه Counter
    constexpr static uint16_t Max_inputSize = 512;
    QPacket();

public:
    // تابع ساخت بسته
    static uint8_t Create(const uint8_t* inputData, uint16_t inputSize, uint8_t* outputData, uint16_t outputSize) {
        // بررسی ورودی‌ها و تعیین طول نهایی بسته
        if (inputSize < 1 || inputSize > Max_inputSize || outputData == nullptr) {
            return 0;
        }

        outputSize = inputSize + 7;
        uint8_t len_MSB = static_cast<uint8_t>((inputSize + 3) >> 8);
        uint8_t len_LSB = static_cast<uint8_t>(inputSize + 3);

        // به‌روزرسانی و مقداردهی Packet Counter
        Packet_Counter = (Packet_Counter == 0xFF) ? 1 : Packet_Counter + 1;

        uint8_t headerCRCInput[] = { Start_Packet, Packet_Counter, len_MSB, len_LSB, End_Packet };
        uint8_t crc1 = QCRC::CRC_8_CCITT(headerCRCInput, sizeof(headerCRCInput));

        // شروع بسته
        outputData[0] = Start_Packet;
        outputData[1] = Packet_Counter;
        outputData[2] = len_MSB;
        outputData[3] = len_LSB;
        outputData[4] = crc1;

        // کپی داده‌ها
        std::memcpy(&outputData[5], inputData, inputSize);

        // محاسبه و اضافه کردن crc2 و پایان بسته
        uint8_t crc2 = QCRC::CRC_8_CCITT(inputData, inputSize);
        uint8_t tempCRC2[] = { crc1, crc2 };
        crc2 =  QCRC::CRC_8_CCITT(tempCRC2, sizeof(tempCRC2));

        outputData[inputSize + 5] = crc2;
        outputData[inputSize + 6] = End_Packet;

        return Packet_Counter;
    }

    // تابع ریست کردن Packet Counter
    static void Reset() {
        Packet_Counter = 0;
    }
};
