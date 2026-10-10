#pragma once

#include <cstdint>
#include <new>

class QBuffer {
public:
    explicit QBuffer(uint32_t capacity)
        : bufferSize(capacity),
          buffer(capacity == 0 ? nullptr : new (std::nothrow) uint8_t[capacity]),
          readIndex(0),
          writeIndex(0),
          length(0) {}

    ~QBuffer() {
        delete[] buffer;
    }

    // This class owns its allocation; copying would cause double-free.
    QBuffer(const QBuffer&) = delete;
    QBuffer& operator=(const QBuffer&) = delete;
    QBuffer(QBuffer&&) = delete;
    QBuffer& operator=(QBuffer&&) = delete;

    void Flush() {
        readIndex = 0;
        writeIndex = 0;
        length = 0;
    }

    void Push(uint8_t data) {
        if (buffer == nullptr || bufferSize == 0) {
            return;
        }

        buffer[writeIndex] = data;
        writeIndex = (writeIndex + 1U) % bufferSize;
        if (length < bufferSize) {
            ++length;
        }
    }

    int Pop() {
        if (length == 0 || buffer == nullptr) {
            return -1;
        }

        const uint8_t data = buffer[readIndex];
        readIndex = (readIndex + 1U) % bufferSize;
        --length;
        return static_cast<int>(data);
    }

    uint32_t GetLength() const {
        return length;
    }

    int PeekAt(uint32_t index) const {
        if (index >= length || buffer == nullptr) {
            return -1;
        }

        const uint32_t position = (readIndex + index) % bufferSize;
        return static_cast<int>(buffer[position]);
    }

    void Remove(uint32_t num) {
        if (num >= length) {
            Flush();
            return;
        }

        if (bufferSize != 0) {
            readIndex = (readIndex + num) % bufferSize;
        }
        length -= num;
    }

private:
    const uint32_t bufferSize;
    uint8_t* buffer;
    uint32_t readIndex;
    uint32_t writeIndex;
    uint32_t length;
};
