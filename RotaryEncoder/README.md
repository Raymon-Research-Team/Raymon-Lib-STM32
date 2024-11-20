
# RotaryEncoder Library | کتابخانه RotaryEncoder

This library provides an easy-to-use interface for handling rotary encoders with STM32 microcontrollers using HAL.

این کتابخانه یک رابط ساده برای مدیریت روتاری انکودرها با میکروکنترلرهای STM32 با استفاده از HAL فراهم می‌کند.

## Features | ویژگی‌ها

- Detects and tracks rotary encoder turns (both positive and negative directions).
- شناسایی و پیگیری چرخش‌های انکودر (در جهت مثبت و منفی).
- Provides methods to get and set volume.
- ارائه متدهایی برای دریافت و تنظیم حجم.
- Supports enabling and disabling the encoder based on power status.
- پشتیبانی از فعال/غیرفعال کردن انکودر بر اساس وضعیت پاور.

## Class Definition | تعریف کلاس

```cpp
class RotaryEncoder {
private:
    GPIO_TypeDef* portA;
    GPIO_TypeDef* portB;
    uint16_t pinA;
    uint16_t pinB;
    uint8_t reversePhase;  // Current step in the reverse (negative) direction
    uint8_t forwardPhase;  // Current step in the forward (positive) direction
    int volume;            // Current volume level
    bool powerStatus;      // Power status (enabled/disabled)

public:
    RotaryEncoder(GPIO_TypeDef* aPort, uint16_t aPin, GPIO_TypeDef* bPort, uint16_t bPin, bool initialPowerStatus = true);

    void setPowerStatus(bool status);
    int getVolume() const;
    void handleInterrupt(uint16_t gpioPin);
};
```

## Example Usage | مثال استفاده

### Initialization | مقداردهی اولیه

To use the `RotaryEncoder` class, initialize it with the GPIO ports and pins connected to the rotary encoder:

برای استفاده از کلاس `RotaryEncoder`، آن را با پورت‌ها و پایه‌های GPIO متصل به انکودر مقداردهی کنید:

```cpp
#include "RotaryEncoder.h"

RotaryEncoder rotaryEncoder(GPIOA, GPIO_PIN_0, GPIOA, GPIO_PIN_1, true);
```

### Handling Interrupts | مدیریت وقفه‌ها

In your external interrupt callback, call the `handleInterrupt` method:

در تابع کال‌بک وقفه خارجی خود، متد `handleInterrupt` را فراخوانی کنید:

```cpp
extern "C" void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
    rotaryEncoder.handleInterrupt(GPIO_Pin);
    int currentVolume = rotaryEncoder.getVolume();
    // Use the currentVolume as needed
}
```

### Enabling/Disabling the Encoder | فعال/غیرفعال کردن انکودر

You can enable or disable the rotary encoder functionality based on the power status:

شما می‌توانید عملکرد انکودر را بر اساس وضعیت پاور فعال یا غیرفعال کنید:

```cpp
rotaryEncoder.setPowerStatus(true); // Enable | فعال کردن
rotaryEncoder.setPowerStatus(false); // Disable | غیرفعال کردن
```

### Getting the Current Volume | دریافت حجم فعلی

Get the current volume level at any point:

در هر زمانی می‌توانید سطح فعلی حجم را دریافت کنید:

```cpp
int volume = rotaryEncoder.getVolume();
```

## Example Project | پروژه نمونه

Below is a simple example demonstrating how to use the `RotaryEncoder` class in a project:

در زیر یک مثال ساده از نحوه استفاده از کلاس `RotaryEncoder` در یک پروژه آورده شده است:

```cpp
#include "main.h"
#include "RotaryEncoder.h"

RotaryEncoder rotaryEncoder(GPIOA, GPIO_PIN_0, GPIOA, GPIO_PIN_1, true);

extern "C" void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
    rotaryEncoder.handleInterrupt(GPIO_Pin);
    int currentVolume = rotaryEncoder.getVolume();
    printf("Current Volume: %d\n", currentVolume); // Assuming printf is configured
}

int main() {
    // Initialization code here

    while (1) {
        // Main loop code
    }
}
```

## Notes | نکات

- Ensure that the external interrupt lines are properly configured in STM32CubeMX for the pins connected to the rotary encoder.
- اطمینان حاصل کنید که خطوط وقفه خارجی برای پایه‌های متصل به انکودر به درستی در STM32CubeMX تنظیم شده‌اند.
- Make sure to debounce the input signals if necessary.
- در صورت نیاز، از دیبانس برای سیگنال‌های ورودی استفاده کنید.
- This library assumes the use of STM32 HAL library.
- این کتابخانه استفاده از STM32 HAL را فرض می‌کند.
