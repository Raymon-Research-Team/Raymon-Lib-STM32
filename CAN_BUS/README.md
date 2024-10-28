# CAN_BUS_PCA82C251T Library

This library provides a convenient interface to work with CAN bus communication using the PCA82C251 transceiver in STM32 microcontrollers. It allows sending and receiving messages through configurable CAN filters.

---

## Table of Contents
- [Features](#features)
- [Requirements](#requirements)
- [Getting Started](#getting-started)
- [CubeMX Configuration](#cubemx-configuration)
- [Usage](#usage)
- [License](#license)

---

## Features
- **Send CAN Messages**: Supports message transmission with configurable data lengths.
- **Receive CAN Messages**: Handles incoming messages with filter-based ID matching.
- **Timeout Handling**: Configurable timeout for message sending to avoid lock-ups.
- **CAN Filter Configuration**: Set CAN filters with custom IDs.

---

## Requirements
- **STM32 HAL library**
- **PCA82C251 CAN transceiver**
- **CubeMX**: For generating CAN configuration and setting up interrupts.

---

## Getting Started
1. Clone or download the repository and add the `CAN_BUS_PCA82C251T` class to your project.
2. Ensure that `CAN` peripheral and the appropriate interrupts are enabled in CubeMX.

---

## CubeMX Configuration

To set up CAN for this class, follow these steps:

1. **Enable CAN Peripheral**:
   - Open CubeMX.
   - Enable the CAN peripheral (e.g., CAN1) and set it to operate in Normal Mode.

2. **Configure CAN Filters**:
   - Open CAN Configuration.
   - Set up a filter (e.g., Filter Bank 0) with appropriate IDs or masks.
   - Configure the filter scale to 32-bit and mode to Mask Mode, as this class handles 32-bit filter IDs.

3. **Enable CAN Interrupts**:
   - Enable `CAN_RX0` (FIFO 0) and `CAN_RX1` (FIFO 1) interrupts under the NVIC Configuration tab.
   - Adjust the interrupt priorities if necessary.

4. **Generate Code**:
   - Click on Generate Code to export the CubeMX configuration.

---

## Usage

### Class Initialization

Initialize the class with the required CAN handle and FIFO selection (0 or 1). You can also set a default filter ID.

```cpp
CAN_HandleTypeDef hcan1;
CAN_BUS_PCA82C251T canBus(&hcan1, 0, 0x321);
```

## Sending Messages
To send a message, use the SendMessage method with the ID, length, and a pointer to the data buffer.


```cpp
uint8_t data[8] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08};
uint8_t status = canBus.SendMessage(0x123, 8, data);
if (status != 0) {
    // Handle send error
}
```


## Receiving Messages
To handle incoming messages, call the ReceiveMessageCallback method in the appropriate CAN receive interrupt.


```cpp
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan) {
    canBus.ReceiveMessageCallback(CAN_IT_RX_FIFO0_MSG_PENDING);
}
```

Note: Ensure that the interrupt handler in stm32xx_it.c calls the HAL_CAN_RxFifo0MsgPendingCallback function.

&#8209;


---

# کتابخانه CAN_BUS_PCA82C251T

این کتابخانه یک رابط مناسب برای کار با ارتباطات CAN باس با استفاده از ترنسیور PCA82C251 و میکروکنترلرهای STM32 فراهم می‌کند. این کتابخانه امکان ارسال و دریافت پیام‌ها با فیلترهای قابل تنظیم CAN را فراهم می‌کند.

---

## فهرست مطالب
- [ویژگی‌ها](#ویژگی‌ها)
- [نیازمندی‌ها](#نیازمندی‌ها)
- [شروع به کار](#شروع-به-کار)
- [پیکربندی CubeMX](#پیکربندی-cubemx)
- [نحوه استفاده](#نحوه-استفاده)
- [لایسنس](#لایسنس)

---

## ویژگی‌ها
- **ارسال پیام‌های CAN**: پشتیبانی از ارسال پیام با طول داده‌های قابل تنظیم.
- **دریافت پیام‌های CAN**: مدیریت پیام‌های دریافتی با تطبیق شناسه فیلتر.
- **مدیریت تایم‌اوت**: تایم‌اوت قابل تنظیم برای جلوگیری از قفل شدن در ارسال پیام.
- **پیکربندی فیلترهای CAN**: تنظیم فیلترهای CAN با شناسه‌های سفارشی.

---

## نیازمندی‌ها
- **کتابخانه HAL برای STM32**
- **ترنسیور CAN از نوع PCA82C251**
- **CubeMX**: برای تولید تنظیمات CAN و فعال‌سازی وقفه‌ها.

---

## شروع به کار
1. ریپازیتوری را کلون کرده یا دانلود کنید و کلاس `CAN_BUS_PCA82C251T` را به پروژه خود اضافه کنید.
2. اطمینان حاصل کنید که `CAN` و وقفه‌های مربوطه در CubeMX فعال هستند.

---

## پیکربندی CubeMX

برای راه‌اندازی CAN برای این کلاس، مراحل زیر را دنبال کنید:

1. **فعال‌سازی محیط CAN**:
   - برنامه CubeMX را باز کنید.
   - محیط CAN (مثلاً CAN1) را فعال کنید و آن را در حالت Normal قرار دهید.

2. **تنظیم فیلترهای CAN**:
   - تنظیمات CAN را باز کنید.
   - یک فیلتر (مثلاً فیلتر بانک 0) با شناسه‌ها یا ماسک‌های مناسب تنظیم کنید.
   - مقیاس فیلتر را به ۳۲ بیتی و حالت را به Mask Mode تنظیم کنید.

3. **فعال‌سازی وقفه‌های CAN**:
   - وقفه‌های `CAN_RX0` (FIFO 0) و `CAN_RX1` (FIFO 1) را تحت تنظیمات NVIC فعال کنید.
   - در صورت نیاز اولویت‌های وقفه را تنظیم کنید.

4. **تولید کد**:
   - روی Generate Code کلیک کنید تا پیکربندی CubeMX صادر شود.

---

## نحوه استفاده

### مقداردهی اولیه کلاس

کلاس را با استفاده از هندل CAN و انتخاب FIFO (0 یا 1) مقداردهی کنید. می‌توانید یک شناسه فیلتر پیش‌فرض نیز تنظیم کنید.

```cpp
CAN_HandleTypeDef hcan1;
CAN_BUS_PCA82C251T canBus(&hcan1, 0, 0x321);
```

## ارسال پیام‌ها
برای ارسال پیام، از تابع SendMessage استفاده کنید و شناسه، طول و یک اشاره‌گر به بافر داده را ارسال کنید.


```cpp
uint8_t data[8] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08};
uint8_t status = canBus.SendMessage(0x123, 8, data);
if (status != 0) {
    // خطای ارسال را مدیریت کنید
}
```


## دریافت پیام‌ها
برای مدیریت پیام‌های ورودی، تابع ReceiveMessageCallback را در وقفه مناسب CAN دریافت فراخوانی کنید.


```cpp
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan) {
    canBus.ReceiveMessageCallback(CAN_IT_RX_FIFO0_MSG_PENDING);
}
```


نکته: اطمینان حاصل کنید که در وقفه stm32xx_it.c تابع HAL_CAN_RxFifo0MsgPendingCallback فراخوانی می‌شود.