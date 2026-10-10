# QBuffer / QPacket / QCommand

این پوشه سه ابزار مستقل برای بافر حلقوی و قاب‌بندی بسته دارد. فایل‌های اصلی `QBuffer.hpp`، `QCRC.hpp`، `QPacket.hpp` و `QCommand.hpp` هستند.

## 1. QBuffer

`QBuffer` بافر FIFO حلقوی با ظرفیت ثابت است. اگر بافر پر شود، `Push()` قدیمی‌ترین بایت را حذف می‌کند و جدیدترین بایت را نگه می‌دارد. در صورت خالی بودن، `Pop()` مقدار `-1` می‌دهد. تخصیص حافظه ممکن است شکست بخورد؛ در این حالت `Push()` داده‌ای ذخیره نمی‌کند.

```cpp
#include "QBuffer.hpp"

QBuffer buffer(16);
buffer.Push(0x12);
buffer.Push(0x34);
int value = buffer.Pop(); // 0x12
```

این کلاس thread-safe نیست. اگر از ISR و task مشترکاً استفاده می‌شود، همگام‌سازی یا صف مخصوص ISR لازم است.

## 2. QPacket

قالب بستهٔ فعلی:

| Offset | Field |
|---:|---|
| 0 | Start byte `0xAA` |
| 1 | Packet counter (مقادیر 1 تا 255) |
| 2–3 | طول کدشده، big-endian؛ برابر طول payload + 3 |
| 4 | CRC هدر |
| 5… | Payload |
| payload length + 5 | CRC دادهٔ ترکیبی |
| payload length + 6 | End byte `0xBB` |

طول کل بسته برابر `payload length + 7` است. حداکثر payload برابر 512 بایت است و payload خالی در `QPacket::Create` پذیرفته نمی‌شود.

```cpp
#include "QPacket.hpp"

uint8_t payload[] = {0x45, 0x78, 0x01};
uint8_t packet[sizeof(payload) + 7];
const uint8_t counter = QPacket::Create(
    payload, sizeof(payload), packet, sizeof(packet));

if (counter == 0) {
    // ورودی یا ظرفیت نامعتبر است.
}
```

شمارندهٔ داخلی static است و برای استفادهٔ هم‌زمان از چند task نیاز به محافظت دارد.

## 3. QCommand

`QCommand::Check` بسته را از ابتدای `QBuffer` بررسی می‌کند. مقدار بازگشتی صفر یعنی بسته هنوز آماده نیست یا نامعتبر است؛ مقدار غیرصفر شمارندهٔ بستهٔ معتبر است. ظرفیت واقعی خروجی باید صریحاً پاس داده شود تا از نوشتن خارج از محدوده جلوگیری شود.

```cpp
#include "QBuffer.hpp"
#include "QCommand.hpp"

QBuffer rx(1024);
uint8_t payload[512];
uint16_t payloadSize = 0;
uint8_t counter = QCommand::Check(
    &rx, payload, sizeof(payload), payloadSize);

if (counter != 0) {
    // payload[0..payloadSize-1] یک بسته معتبر است.
}
```

اگر ظرفیت خروجی برای payload کافی نباشد، بسته از بافر حذف نمی‌شود و تابع صفر برمی‌گرداند. در صورت خطای CRC یا ساختار، یک بایت حذف می‌شود تا جست‌وجوی هم‌ترازی مجدد ادامه یابد.

## CRC

`QCRC::CRC_8_CCITT` در این پروژه از polynomial برابر `0x07` و مقدار اولیهٔ `0xFF` استفاده می‌کند. نام «CRC-8-CCITT» در منابع مختلف ممکن است پارامترهای متفاوتی داشته باشد؛ برای ارتباط با دستگاه دیگر باید polynomial، مقدار اولیه، reflection و XOR نهایی در هر دو سمت یکسان باشند.

## محدودیت‌ها

- این کد پروتکل رمزنگاری‌شده یا احراز هویت‌شده نیست؛ CRC فقط خطاهای تصادفی را تشخیص می‌دهد.
- شمارندهٔ بسته به‌تنهایی تضمین تحویل یا جلوگیری از replay نیست.
- کلاس‌ها thread-safe نیستند.
- پس از تغییر امضای `QCommand::Check`، فراخوانی‌ها باید آرگومان ظرفیت خروجی را نیز بدهند.
- تست خودکار و سازگاری بین‌نسخه‌ای این پروتکل در این README ادعا نمی‌شود.
