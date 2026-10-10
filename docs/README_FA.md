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

<div dir="rtl" >
Raymon-Lib-STM32  مجموعه‌ای از ابزارهای C/C++، مثال‌های وسایل جانبی و آزمایش‌های مرتبط با میکروکنترلرهای STM32 است. مخزن یک بستهٔ واحد قابل نصب نیست؛ هر بخش ممکن است به HAL، نسخهٔ کامپایلر، سخت‌افزار یا کتابخانهٔ جداگانه‌ای نیاز داشته باشد.

## فهرست اجزا

| بخش | کاربرد | وابستگی و نکته |
|---|---|---|
| [QBuffer](../QBuffer/README.md) | بافر بایتی، قاب‌بندی بسته، CRC و پردازش فرمان | C++؛ محدودیت ظرفیت و API را بررسی کنید |
| [QPacket](../QBuffer/QPacket.hpp) | قاب‌بندی بسته با نشانگر، طول، شمارنده و CRC | وابسته به [QCRC](../QBuffer/QCRC.hpp) |
| [FlashEEPROM](../FlashEEPROM/README.md) | ذخیره و بازیابی ساختار در فلش داخلی | STM32 HAL و main.h؛ بررسی آدرس فلش ضروری است |
| [PrintfUart](../PrintfUart/README.md) | ورودی/خروجی قالب‌بندی‌شده UART | STM32 HAL و main.h |
| [سروو SG90](../ServoMotor%20SG90/Readme.md) | کنترل سروو با PWM | مثال موجود از STM32F1/TIM2 استفاده می‌کند |
| [FFT برای STM32H743](../FFT%28STM32H743VITx%29/README.md) | مثال FFT با CMSIS-DSP | نیازمند پیکربندی سازگار CMSIS-DSP |
| [پرش به Bootloader](../BOOTLOADER/README.md) | مثال وابسته به MCU | نقشهٔ حافظه باید تأیید شود؛ بالقوه خطرناک |
| [قفل برنامه](../Lock_program_in_code/README.md) | توضیحات محافظت firmware | پیش از اجرا اثرهای مخرب احتمالی را بررسی کنید |
| [نمونهٔ Bluetooth](../Bluetooth%28doBT_01%29/README.md) | کد وابسته به ماژول | وابسته به سخت‌افزار و پروتکل |
| [Bluetooth PW05](../Bluetooth%28PW05%29/README.md) | کنترل ماژول از طریق UART | وابسته به firmware، AT command و سیم‌بندی |
| [نمایشگر ILI9488](../LCD_ILI9488/README.md) | درایور SPI و توابع رسم TFT | چند پیاده‌سازی ناسازگار؛ فقط یکی را انتخاب کنید |
| [cUart](../cUart/README.md) | اتصال streamهای C++ به UART | ورودی/خروجی blocking است و تایم‌اوت ندارد |
| [ابزار تغییر پسوند C/C++](../000_New_File_C%2B%2B/README.md) | تغییر نام main.c و main.cpp | فقط تغییر نام؛ کد را تبدیل نمی‌کند |

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

---

## 💖 حمایت از Raymon-Lib-STM32

اگر این ابزارها و مثال‌های STM32 برای شما مفید بوده‌اند، کمک مالی داوطلبانه به نگهداری، آزمایش سخت‌افزاری و بهبود مستندات کمک می‌کند. حمایت مالی، مجوز مخزن را تغییر نمی‌دهد و حق استفادهٔ اضافه‌ای ایجاد نمی‌کند.
</div>

<div dir="rtl" align="center">

<a href="https://donatr.ee/raymon-research-team/" target="_blank" rel="noopener noreferrer"><img src="https://raw.githubusercontent.com/Raymon-Research-Team/ZenOS-RTOS/main/docs/guide-html/donate-qr.png" alt="کد QR صفحهٔ حمایت مالی Raymon Research Team" width="190" /></a>

**اسکن کد QR یا ورود به صفحهٔ حمایت مالی:**  
[donatr.ee/raymon-research-team](https://donatr.ee/raymon-research-team/)

</div>

### 💸 حمایت با رمزارز

> پیش از ارسال، شبکه را دقیق بررسی کنید. BTC فقط روی شبکهٔ Bitcoin و آدرس ETH/USDT زیر فقط روی شبکهٔ Ethereum (ERC-20) است. انتخاب شبکهٔ اشتباه ممکن است باعث از دست رفتن دائمی دارایی شود.

<div align="center">
  
<table>
<tr>
<td align="center" width="50%">

### 🟠 بیت‌کوین (BTC)

[`bc1qd39vgmnweuzh5hp2cqm4cnh782xga6wph3v650`](bitcoin:bc1qd39vgmnweuzh5hp2cqm4cnh782xga6wph3v650)

</td>
<td align="center" width="50%">

### 🔵 اتریوم (ETH) / تتر (USDT – ERC-20)

[`0x1C21c39324F65a38Fb8de9ccB92aB01FdeD1534C`](ethereum:0x1C21c39324F65a38Fb8de9ccB92aB01FdeD1534C)

</td>
</tr>
</table>

</div>

<div dir="rtl" align="center" >
  
### 🌟 راه‌های دیگر حمایت

<table width="100%">
<tr width="100%">
<td align="center" width="25%">⭐<br><b>ستاره بدهید</b><br>به مخزن</td>
<td align="center" width="25%">🐛<br><b>گزارش کنید</b><br>ایرادها را</td>
<td align="center" width="25%">🧪<br><b>آزمایش کنید</b><br>اجزا را</td>
<td align="center" width="25%">📝<br><b>بهبود دهید</b><br>مستندات را</td>
</tr>
</table>

</div>

برای اطلاع‌رسانی دربارهٔ کمک مالی می‌توانید شناسهٔ تراکنش را به [rahman.h22@gmail.com](mailto:rahman.h22@gmail.com) ایمیل کنید. **هرگز کلید خصوصی یا عبارت بازیابی کیف پول را ارسال نکنید.**
