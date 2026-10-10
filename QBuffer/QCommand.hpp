#pragma once
#include "QBuffer.hpp"
#include "QCRC.hpp"
#include <cstdint>

class QCommand {
private:
    static constexpr uint8_t Start_Packet = 0xAA;
    static constexpr uint8_t END_Packet = 0xBB;
    static constexpr uint16_t Max_inputSize = 512;
    QCommand() = delete;

public:
    // Returns packet counter on success, otherwise 0. outputCapacity is the
    // actual size of outputData in bytes and is required to prevent overflow.
    static uint8_t Check(QBuffer* buffer, uint8_t* outputData,
                        uint16_t outputCapacity, uint16_t& outputSize) {
        outputSize = 0U;
        if (buffer == nullptr) return 0U;
        if (buffer->GetLength() < 5U) return 0U;

        if (buffer->PeekAt(0U) != Start_Packet) {
            buffer->Remove(1U);
            return 0U;
        }

        const int counterValue = buffer->PeekAt(1U);
        const int lengthMsb = buffer->PeekAt(2U);
        const int lengthLsb = buffer->PeekAt(3U);
        if (counterValue <= 0 || lengthMsb < 0 || lengthLsb < 0) {
            buffer->Remove(1U);
            return 0U;
        }

        const uint8_t packetCounter = static_cast<uint8_t>(counterValue);
        const uint16_t encodedLength =
            static_cast<uint16_t>((static_cast<uint16_t>(lengthMsb) << 8U) |
                                  static_cast<uint16_t>(lengthLsb));

        // Wire format defines encodedLength = payload length + 3.
        if (encodedLength < 3U || encodedLength > Max_inputSize + 3U) {
            buffer->Remove(1U);
            return 0U;
        }
        const uint16_t payloadLength = static_cast<uint16_t>(encodedLength - 3U);
        if (payloadLength > outputCapacity || (payloadLength != 0U && outputData == nullptr)) {
            // Do not consume the packet; caller can retry with a larger output buffer.
            return 0U;
        }

        const uint16_t totalPacketSize = static_cast<uint16_t>(encodedLength + 5U);
        if (buffer->GetLength() < totalPacketSize) return 0U;

        const int receivedHeaderCrc = buffer->PeekAt(4U);
        const int endByte = buffer->PeekAt(static_cast<uint32_t>(encodedLength + 4U));
        if (receivedHeaderCrc < 0 || endByte != END_Packet) {
            buffer->Remove(1U);
            return 0U;
        }

        const uint8_t headerCrcInput[] = {
            Start_Packet, packetCounter,
            static_cast<uint8_t>(encodedLength >> 8U),
            static_cast<uint8_t>(encodedLength & 0xFFU), END_Packet
        };
        const uint8_t headerCrc = QCRC::CRC_8_CCITT(headerCrcInput, sizeof(headerCrcInput));
        if (headerCrc != static_cast<uint8_t>(receivedHeaderCrc)) {
            buffer->Remove(1U);
            return 0U;
        }

        for (uint16_t i = 0U; i < payloadLength; ++i) {
            const int value = buffer->PeekAt(static_cast<uint32_t>(i) + 5U);
            if (value < 0) {
                buffer->Remove(1U);
                return 0U;
            }
            outputData[i] = static_cast<uint8_t>(value);
        }

        const int receivedPayloadCrc = buffer->PeekAt(static_cast<uint32_t>(encodedLength) + 3U);
        if (receivedPayloadCrc < 0) {
            buffer->Remove(1U);
            return 0U;
        }
        const uint8_t dataCrc = QCRC::CRC_8_CCITT(outputData, payloadLength);
        const uint8_t combined[] = { headerCrc, dataCrc };
        const uint8_t expectedPayloadCrc = QCRC::CRC_8_CCITT(combined, sizeof(combined));
        if (expectedPayloadCrc != static_cast<uint8_t>(receivedPayloadCrc)) {
            buffer->Remove(1U);
            return 0U;
        }

        buffer->Remove(totalPacketSize);
        outputSize = payloadLength;
        return packetCounter;
    }
};