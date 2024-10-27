# کلاس Buffer

## معرفی

کلاس Buffer یک بافر دایره‌ای با اندازه ثابت است که برای ذخیره و مدیریت داده‌های بایتی طراحی شده و عملیات‌هایی مانند push و pop را به صورت بهینه ارائه می‌دهد.

## ویژگی‌ها

-   پیاده‌سازی بافر دایره‌ای
-   قابلیت تنظیم اندازه بافر
-   متدهای Push و Pop برای مدیریت داده‌ها
-   قابلیت مشاهده (Peek) و حذف داده‌ها
-   امکان پاکسازی بافر با متد Flush

## متدها

**void Push(uint8_t data)**

-   یک بایت (data) به بافر اضافه می‌کند و در صورت پر بودن بافر، قدیمی‌ترین داده‌ها را بازنویسی می‌کند.

**int Pop()**

-   قدیمی‌ترین بایت را از بافر حذف و بازمی‌گرداند. اگر بافر خالی باشد، -1 بازمی‌گرداند.

**int GetLength() const**

-   تعداد عناصر فعلی در بافر را بازمی‌گرداند.

**int PeekAt(uint16_t index) const**

-   بایت در index مشخص را بدون حذف بازمی‌گرداند. اگر اندیس خارج از محدوده باشد، -1 بازمی‌گرداند.

**void Remove(uint16_t num)**

-   تعداد بایت‌های مشخص شده را از بافر حذف می‌کند. اگر num از طول بافر بیشتر باشد، بافر پاکسازی می‌شود.

**void Flush()**

-   تمام داده‌ها را از بافر حذف کرده و طول را به 0 تنظیم می‌کند.

## مثال استفاده

```C++
#include <iostream>
#include "Buffer.h"

int main() {
    Buffer buffer(10); // ایجاد یک بافر با ظرفیت 10 بایت

    // افزودن داده به بافر
    buffer.Push(5);
    buffer.Push(10);
    
    // مشاهده داده‌ها
    int peekedValue = buffer.PeekAt(1);
    std::cout << "مقدار مشاهده شده: " << peekedValue << std::endl;

    // برداشت داده
    int poppedValue = buffer.Pop();
    std::cout << "مقدار برداشته شده: " << poppedValue << std::endl;

    // بررسی طول بافر
    std::cout << "طول بافر: " << buffer.GetLength() << std::endl;

    // پاکسازی بافر
    buffer.Flush();
    std::cout << "طول بافر پس از پاکسازی: " << buffer.GetLength() << std::endl;

    return 0;
}
```

# Methods

**void Push(uint8_t data)**

-   Adds a byte (data) to the buffer, overwriting oldest data if buffer is full.

**int Pop()**

-   Removes and returns the oldest byte in the buffer. Returns -1 if buffer is empty.

**int GetLength() const**

-   Returns the current number of elements in the buffer.

**int PeekAt(uint16_t index) const**

-   Returns the byte at the given index without removing it. Returns -1 if index is out of range.

**void Remove(uint16_t num)**

-   Removes the specified number of bytes from the buffer. Clears buffer if num exceeds buffer length.

**void Flush()**

-   Clears all data in the buffer, resetting length to 0.
