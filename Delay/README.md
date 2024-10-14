# کتابخانه Delay برای STM32

توضیحات به زبان فارسی

کتابخانه Delay برای ایجاد تأخیرهای دقیق بر اساس میکروثانیه و میلی‌ثانیه در پروژه‌های STM32 طراحی شده است. این کتابخانه از تایمر DWT (Data Watchpoint and Trace) که بخشی از واحد Cortex-M در میکروکنترلرهای STM32 است، استفاده می‌کند. تایمر DWT یک شمارنده بسیار دقیق است که می‌تواند زمان دقیق اجرای دستورات را با استفاده از کلاک اصلی پردازنده اندازه‌گیری کند.

## استفاده

برای استفاده از این کتابخانه، ابتدا فایل Delay.h را در پروژه خود اضافه کنید. سپس می‌توانید از تابع‌های زیر برای ایجاد تأخیر استفاده کنید:

## تابع delay_us(uint32_t us):

این تابع برای ایجاد تأخیر بر اساس میکروثانیه استفاده می‌شود. ورودی آن تعداد میکروثانیه‌های موردنظر برای تأخیر است.

مثال:

```
Delay::delay_us(1000); // ایجاد تأخیر به مدت 1000 میکروثانیه (1 میلی‌ثانیه)
```

## تابع delay_ms(uint32_t ms):

این تابع برای ایجاد تأخیر بر اساس میلی‌ثانیه استفاده می‌شود. ورودی آن تعداد میلی‌ثانیه‌های موردنظر برای تأخیر است. این تابع با استفاده از تابع delay_us برای ایجاد تأخیر 1000 میکروثانیه برای هر میلی‌ثانیه کار می‌کند.

مثال:

```
Delay::delay_ms(100); // ایجاد تأخیر به مدت 100 میلی‌ثانیه
```

## دقت تأخیر

کتابخانه Delay از تایمر DWT استفاده می‌کند که دقت بالایی در اندازه‌گیری زمان دارد. به طور کلی، دقت تأخیر بستگی به فرکانس کلاک پردازنده دارد. با توجه به این که این تایمر به صورت سخت‌افزاری است، تأخیرهای ایجاد شده با استفاده از این کتابخانه می‌توانند دقیق و قابل اعتماد باشند.

***

# Library Delay for STM32

## Description in English

The Delay library is designed to create precise delays based on microseconds and milliseconds in STM32 projects. This library uses the DWT (Data Watchpoint and Trace) timer, which is part of the Cortex-M core in STM32 microcontrollers. The DWT timer is a very accurate counter that can measure the exact execution time of instructions using the main processor clock.

## Usage

To use this library, first include the Delay.h file in your project. You can then use the following functions to create delays:

## Function delay_us(uint32_t us):

This function is used to create a delay based on microseconds. Its input is the number of microseconds for the desired delay.

## Example:

```
Delay::delay_us(1000); // Create a delay of 1000 microseconds (1 millisecond)
```

## Function delay_ms(uint32_t ms):

This function is used to create a delay based on milliseconds. Its input is the number of milliseconds for the desired delay. This function uses the delay_us function to create a delay of 1000 microseconds for each millisecond.

## Example:

```
Delay::delay_ms(100); // Create a delay of 100 milliseconds
```

## Delay Accuracy

The Delay library uses the DWT timer, which has a high accuracy in measuring time. Generally, the accuracy of the delay depends on the processor clock frequency. Since this timer is hardware-based, delays created using this library can be precise and reliable.
