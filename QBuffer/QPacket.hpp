#pragma once

#include <cstdint>
#include <cstring>
#include "QCRC.hpp"

/*
Packet format:
  Start (0xAA), counter, payload length MSB, payload length LSB,
  header CRC, payload bytes, payload CRC, End (0xBB).

The encoded length field is payload length + 3, preserving the existing
wire format. Total output size is payload length + 7 bytes.
*/

class QPacket {
private:
    static constexpr uint8_t Start_Packet = 0xAA;
    static constexpr uint8_t End_Packet = 0xBB;
    inline static uint8_t Packet_Counter = 0;
    static constexpr uint16_t Max_inputSize = 512;

    QPacket() = delete;

public:
    // Returns the packet counter on success; returns 0 on invalid input/capacity.
    static uint8_t Create(const uint8_t* inputData,
                          uint16_t inputSize,
                          uint8_t* outputData,
                          uint16_t outputSize) {
        if (inputData == nullptr || outputData == nullptr ||
            inputSize == 0 || inputSize > Max_inputSize) {
            return 0;
        }

        const uint16_t requiredSize = static_cast<uint16_t>(inputSize + 7U);
        if (outputSize < requiredSize) {
            return 0;
        }

        const uint16_t encodedLength = static_cast<uint16_t>(inputSize + 3U);
        const uint8_t len_MSB = static_cast<uint8_t>((encodedLength >> 8U) & 0xFFU);
        const uint8_t len_LSB = static_cast<uint8_t>(encodedLength & 0xFFU);

        Packet_Counter = (Packet_Counter == 0xFFU)
            ? 1U
            : static_cast<uint8_t>(Packet_Counter + 1U);

        const uint8_t headerCRCInput[] = {
            Start_Packet, Packet_Counter, len_MSB, len_LSB, End_Packet
        };
        const uint8_t crc1 = QCRC::CRC_8_CCITT(headerCRCInput, sizeof(headerCRCInput));

        outputData[0] = Start_Packet;
        outputData[1] = Packet_Counter;
        outputData[2] = len_MSB;
        outputData[3] = len_LSB;
        outputData[4] = crc1;

        // memmove also supports callers whose input overlaps the output buffer.
        std::memmove(&outputData[5], inputData, inputSize);

        const uint8_t dataCRC = QCRC::CRC_8_CCITT(inputData, inputSize);
        const uint8_t combinedCRCInput[] = { crc1, dataCRC };
        outputData[inputSize + 5U] =
            QCRC::CRC_8_CCITT(combinedCRCInput, sizeof(combinedCRCInput));
        outputData[inputSize + 6U] = End_Packet;

        return Packet_Counter;
    }

    static void Reset() {
        Packet_Counter = 0;
    }
};
