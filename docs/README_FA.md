<div align="center">

# Raymon-Lib-STM32

**مجموعه‌ای از ابزارها و مثال‌های C/C++ برای توسعهٔ firmware روی STM32**

[![Platform](https://img.shields.io/badge/Platform-STM32%20%2F%20ARM%20Cortex--M-orange.svg?style=for-the-badge)]()
[![Language](https://img.shields.io/badge/Language-C%2B%2B-blue.svg?style=for-the-badge)]()
[![License: BUSL-1.1](https://img.shields.io/badge/License-BUSL--1.1-blue.svg?style=for-the-badge)](../LICENSE)

[فارسی](README_FA.md) | [English](../README.md)

</div>

---

## معرفی

Raymon-Lib-STM32 مجموعه‌ای از ابزارهای C/C++، مثال‌های وسایل جانبی و آزمایش‌های مرتبط با میکروکنترلرهای STM32 است. مخزن یک بستهٔ واحد قابل نصب نیست؛ هر بخش ممکن است به HAL، نسخهٔ کامپایلر، سخت‌افزار یا کتابخانهٔ جداگانه‌ای نیاز داشته باشد.

## فهرست اجزا

| بخش | کاربرد | وابستگی و نکته |
|---|---|---|
| [QBuffer](../QBuffer/QBuffer.hpp) | بافر بایتی با ظرفیت ثابت | فقط هدر؛ رفتار مالکیت حافظه را بررسی کنید |
| [QPacket](../QBuffer/QPacket.hpp) | قاب‌بندی بسته با نشانگر، طول، شمارنده و CRC | وابسته به [QCRC](../QBuffer/QCRC.hpp) |
| [FlashEEPROM](../FlashEEPROM/README.md) | ذخیره و بازیابی ساختار در فلش داخلی | STM32 HAL و main.h؛ بررسی آدرس فلش ضروری است |
| [PrintfUart](../PrintfUart/README.md) | ورودی/خروجی قالب‌بندی‌شده UART | STM32 HAL و main.h |
| [سروو SG90](../ServoMotor%20SG90/Readme.md) | کنترل سروو با PWM | مثال موجود از STM32F1/TIM2 استفاده می‌کند |
| [FFT برای STM32H743](../FFT%28STM32H743VITx%29/README.md) | مثال FFT با CMSIS-DSP | نیازمند پیکربندی سازگار CMSIS-DSP |
| [پرش به Bootloader](../BOOTLOADER/jump_to_bootloader.c) | مثال وابسته به MCU | نقشهٔ حافظه باید تأیید شود |
| [قفل برنامه](../Lock_program_in_code/README.md) | توضیحات محافظت firmware | پیش از اجرا اثرهای مخرب احتمالی را بررسی کنید |
| [نمونهٔ Bluetooth](../Bluetooth%28doBT_01%29/doBT_01.hpp) | کد وابسته به ماژول | وابسته به سخت‌افزار و پروتکل |

این فهرست بر اساس فایل‌های موجود تهیه شده است؛ وجود فایل به معنی کامپایل موفق یا آزمایش روی سخت‌افزار نیست.

## شروع کار

1. مخزن را دریافت کنید.
2. پوشهٔ قابلیت موردنیاز را باز کرده و README همان بخش را بخوانید.
3. پروژه‌ای در STM32CubeIDE/STM32CubeMX بسازید که با MCU، کلاک، پایه‌ها و وسایل جانبی هدف مطابقت داشته باشد.
4. فقط فایل‌های لازم را اضافه و مسیرهای include، STM32 HAL و وابستگی‌های خارجی را تنظیم کنید.
5. پروژه را بسازید و با روش بازیابی مطمئن روی سخت‌افزار آزمایش کنید.

```bash
git clone https://github.com/Raymon-Research-Team/Raymon-Lib-STM32.git
```

برای کل مخزن سیستم build واحد یا دستور نصب عمومی تأییدشده‌ای وجود ندارد؛ هر جزء باید جداگانه ادغام شود.

## هشدارهای فنی

- **فلش داخلی:** FlashEEPROM پیش از نوشتن، ناحیه‌ای از فلش را پاک می‌کند. نقشهٔ حافظه، اندازهٔ صفحه، بانک‌ها و محدوده‌های رزروشده را از مستندات MCU و linker map بررسی کنید.
- **Bootloader و read protection:** آدرس یا تنظیم نادرست ممکن است دسترسی به firmware را از بین ببرد یا پاک‌کردن کامل تراشه را لازم کند.
- **سروو و PWM:** پریود و prescaler تایمر به کلاک و MCU وابسته‌اند.
- **FFT:** نسخهٔ CMSIS-DSP و تنظیمات کامپایلر/FPU مهم‌اند.
- وجود کد به‌تنهایی تضمین‌کنندهٔ پشتیبانی از همهٔ خانواده‌های STM32 نیست.

## مستندات بیشتر

- [راهنمای شروع و فهرست API](GETTING_STARTED.md)
- [مجوز و وابستگی‌های شخص ثالث](LICENSING.md)
- [README انگلیسی](../README.md)

## مجوز

فایل LICENSE ریشه، **Business Source License 1.1 (BUSL-1.1)** را اعلام می‌کند. این مجوز متن‌باز محسوب نمی‌شود. پیش از استفادهٔ تولیدی یا تجاری، شرایط کامل مجوز و [یادداشت‌های وابستگی](LICENSING.md) را بررسی کنید.

نگهداری: **Raymon Research Team**