#pragma once
#include <cstdint>
#include <iostream>

class QBuffer {
public:
    explicit QBuffer(uint32_t bufferSize)
        : bufferSize(bufferSize), buffer(new uint8_t[bufferSize]), readIndex(0), writeIndex(0), length(0) {}

    ~QBuffer() {
        delete[] buffer;
    }
    //-------------------------------------------------------------------------------------------
    void Flush() {
        readIndex = 0;
        writeIndex = 0;
        length = 0;
    }
    //-------------------------------------------------------------------------------------------
    void Push(uint8_t data) {
        buffer[writeIndex] = data;
        writeIndex = (writeIndex + 1) % bufferSize;
        if (length < bufferSize) {
            length++;
        }
    }
    //-------------------------------------------------------------------------------------------
    int Pop() {
        if (length == 0) return -1;
        uint8_t data = buffer[readIndex];
        readIndex = (readIndex + 1) % bufferSize;
        length--;
        return data;
    }
    //-------------------------------------------------------------------------------------------
    int GetLength() const {
        return length;
    }
    //-------------------------------------------------------------------------------------------
    int PeekAt(uint32_t index) const {
        if (index >= length) return -1;
        uint32_t position = (readIndex + index) % bufferSize;
        return buffer[position];
    }
    //-------------------------------------------------------------------------------------------
    void Remove(uint32_t num) {
        if (num >= length) {
            Flush();
        } else {
            readIndex = (readIndex + num) % bufferSize;
            length -= num;
        }
    }
    //-------------------------------------------------------------------------------------------
private:
    const uint32_t bufferSize;
    uint8_t* buffer;
    uint32_t readIndex;
    uint32_t writeIndex;
    uint32_t length;
};
