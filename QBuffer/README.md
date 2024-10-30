# QPacket System Documentation
This repository includes three main classes designed for buffer and packet processing within embedded systems or low-level applications. These classes are as follows:

### QBuffer: A ring buffer for efficient data storage.
### QPacket: A packet creation utility for encoding and sending data.
### QCommand: A command parser that reads and validates packets from the buffer.
The code and examples are provided in both English and Persian.

## Classes Overview
# QBuffer
The QBuffer class provides a simple circular buffer implementation for storing and managing data. It’s designed to handle data in a FIFO (First In, First Out) manner.

## Example Usage:

```cpp
#include "QBuffer.h"

int main() {
    QBuffer buffer(10);  // Create a buffer of size 10

    buffer.Push(0x12);    // Push data into the buffer
    buffer.Push(0x34);

    int data = buffer.Pop();   // Read data from the buffer
    std::cout << "Popped Data: " << data << std::endl;

    buffer.Flush();            // Clear the buffer
}
```

## Methods
* `Push(uint8_t data)`: Add data to the buffer. 
* `Pop()`: Remove and return the oldest data from the buffer.
* `GetLength()`: Return the current number of items in the buffer.
* `PeekAt(uint32_t index)`: Peek at data at a given index without removal.
* `Remove(uint32_t num)`: Remove a specified number of items from the buffer.
* `Flush()`: Clear all buffer contents.


# QPacket
The QPacket class creates a data packet format. Packets contain a start byte, packet counter, length, CRC check, data, and an end byte.

## Example Usage:

```cpp
#include "QPacket.h"

int main() {
    uint8_t inputData[] = {0x45, 0x78, 0x01};
    uint8_t outputData[20];
    uint16_t outputSize;

    uint8_t packetCounter = QPacket::Create(inputData, 3, outputData, outputSize);

    std::cout << "Packet Counter: " << (int)packetCounter << "\n";
    std::cout << "Packet Size: " << outputSize << std::endl;
}
```

## Methods
* `Create(const uint8_t* inputData, uint16_t inputSize, uint8_t* outputData, uint16_t& outputSize)`: Constructs a packet with provided data and CRC, returning a counter for the packet.

# QCommand
The QCommand class checks and decodes packets received in the buffer, ensuring valid packet structure and CRC.

## Example Usage:

```cpp
#include "QCommand.h"

int main() {
    QBuffer buffer(20);
    uint8_t inputData[] = {0xAA, 0x01, 0x00, 0x03, 0xAB, 0x45, 0x78, 0x01, 0xBB};
    for (uint8_t byte : inputData) {
        buffer.Push(byte);
    }

    uint8_t outputData[10];
    uint16_t outputSize;
    uint8_t packetCounter = QCommand::Check(&buffer, outputData, outputSize);

    std::cout << "Packet Counter: " << (int)packetCounter << "\n";
    std::cout << "Data Length: " << outputSize << std::endl;
}
```

## Methods
* `Check(QBuffer* _buffer, uint8_t* outputData, uint16_t& outputSize)`: Verifies the structure and CRC of packets, extracting data if valid.



>
# کلاس‌ها
# QBuffer
کلاس QBuffer یک بافر دایره‌ای است که داده‌ها را به صورت FIFO (اول وارد، اول خارج) ذخیره می‌کند و مدیریت می‌کند.

## مثال:

```cpp
#include "QBuffer.h"

int main() {
    QBuffer buffer(10);  // ایجاد یک بافر با اندازه 10

    buffer.Push(0x12);    // افزودن داده به بافر
    buffer.Push(0x34);

    int data = buffer.Pop();   // دریافت داده از بافر
    std::cout << "داده حذف شده: " << data << std::endl;

    buffer.Flush();            // پاکسازی بافر
}
```

## توابع

* `Push(uint8_t data)`:
افزودن داده به بافر.
* `Pop()`:
حذف و بازگردانی قدیمی‌ترین داده از بافر.
* `GetLength()`:
بازگرداندن تعداد آیتم‌های موجود در بافر.
* `PeekAt(uint32_t index)`: 
مشاهده داده در یک اندیس خاص بدون حذف.
* `Remove(uint32_t num)`:
حذف تعداد مشخصی از آیتم‌ها از بافر.
* `Flush()`:
پاکسازی تمامی محتوای بافر.


# QPacket

کلاس QPacket برای ایجاد یک بسته داده طراحی شده است. بسته‌ها شامل یک بایت شروع، شمارنده بسته، طول، CRC، داده‌ها و یک بایت پایان هستند.

## مثال:
```cpp
 #include "QPacket.h"
 
 int main() {
     uint8_t inputData[] = {0x45, 0x78, 0x01};
     uint8_t outputData[20];
     uint16_t outputSize;
     
     uint8_t packetCounter = QPacket::Create(inputData, 3, outputData, outputSize);
     
     std::cout << "شمارنده بسته: " << (int)packetCounter << "\n";
     std::cout << "اندازه بسته: " << outputSize << std::endl;
 }
```


## توابع
* `Create(const uint8_t* inputData, uint16_t inputSize, uint8_t* outputData, uint16_t& outputSize)`:

یک بسته با داده‌ها و CRC فراهم شد
ه ایجاد می‌کند و شمارنده بسته را برمی‌گرداند.


# QCommand

کلاس QCommand بسته‌های دریافتی در بافر را بررسی و رمزگشایی می‌کند و از ساختار صحیح و CRC معتبر اطمینان می‌دهد.

## مثال:
```cpp
#include "QCommand.h"

int main() {
    QBuffer buffer(20);
    uint8_t inputData[] = {0xAA, 0x01, 0x00, 0x03, 0xAB, 0x45, 0x78, 0x01, 0xBB};
    for (uint8_t byte : inputData) {
        buffer.Push(byte);
    }

    uint8_t outputData[10];
    uint16_t outputSize;
    uint8_t packetCounter = QCommand::Check(&buffer, outputData, outputSize);

    std::cout << "شمارنده بسته: " << (int)packetCounter << "\n";
    std::cout << "طول داده: " << outputSize << std::endl;
}
```

## توابع
`Check(QBuffer* _buffer, uint8_t* outputData, uint16_t& outputSize)`: 

ساختار و CRC بسته‌ها را بررسی کرده و در صورت معتبر بودن، داده‌ها را استخراج می‌کند.