#pragma once

#include <cstdint>
#include <new>

class QBuffer {
public:
    explicit QBuffer(uint32_t capacity)
        : bufferSize(capacity),
          buffer(capacity == 0 ? nullptr : new (std::nothrow) uint8_t[capacity]),
          readIndex(0), writeIndex(0), length(0) {}

    ~QBuffer() { delete[] buffer; }

    QBuffer(const QBuffer&) = delete;
    QBuffer& operator=(const QBuffer&) = delete;
    QBuffer(QBuffer&&) = delete;
    QBuffer& operator=(QBuffer&&) = delete;

    void Flush() {
        readIndex = writeIndex = length = 0;
    }

    // Full-buffer policy: discard the oldest byte and keep the newest capacity bytes.
    void Push(uint8_t data) {
        if (buffer == nullptr || bufferSize == 0U) return;

        buffer[writeIndex] = data;
        writeIndex = (writeIndex + 1U) % bufferSize;

        if (length < bufferSize) {
            ++length;
        } else {
            // The write overwrote the oldest byte; advance the read cursor too.
            readIndex = (readIndex + 1U) % bufferSize;
        }
    }

    // Returns -1 when empty, otherwise a byte value in [0, 255].
    int Pop() {
        if (length == 0U || buffer == nullptr) return -1;
        const uint8_t data = buffer[readIndex];
        readIndex = (readIndex + 1U) % bufferSize;
        --length;
        return static_cast<int>(data);
    }

    uint32_t GetLength() const { return length; }

    int PeekAt(uint32_t index) const {
        if (index >= length || buffer == nullptr) return -1;
        return static_cast<int>(buffer[(readIndex + index) % bufferSize]);
    }

    void Remove(uint32_t num) {
        if (num >= length) {
            Flush();
            return;
        }
        readIndex = (readIndex + num) % bufferSize;
        length -= num;
    }

private:
    const uint32_t bufferSize;
    uint8_t* buffer;
    uint32_t readIndex;
    uint32_t writeIndex;
    uint32_t length;
};