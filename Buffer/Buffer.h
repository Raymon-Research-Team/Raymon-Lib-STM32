#pragma once
#include <cstdint>
#include <iostream>

class Buffer {
public:
    explicit Buffer(uint16_t bufferSize)
        : bufferSize(bufferSize), buffer(new uint8_t[bufferSize]), readIndex(0), writeIndex(0), length(0) {}

    ~Buffer() {
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
    int PeekAt(uint16_t index) const {
        if (index >= length) return -1;
        uint16_t position = (readIndex + index) % bufferSize;
        return buffer[position];
    }
    //-------------------------------------------------------------------------------------------
    void Remove(uint16_t num) {
        if (num >= length) {
            Flush();
        } else {
            readIndex = (readIndex + num) % bufferSize;
            length -= num;
        }
    }
    //-------------------------------------------------------------------------------------------
private:
    const uint16_t bufferSize;
    uint8_t* buffer;
    uint16_t readIndex;
    uint16_t writeIndex;
    uint16_t length;
};
