# کلاس IC_IO_Write_74Hc595

کلاس `IC_IO_Write_74Hc595` برای ارتباط با شیفت رجیستر 74HC595 با استفاده از پین‌های GPIO در میکروکنترلرهای STM32 طراحی شده است. این کلاس متدهایی را برای ارسال داده به شیفت رجیستر، مدیریت پین‌های فردی و کنترل چندین آی‌سی ارائه می‌دهد.

## ویژگی‌ها

- **انتقال داده**: ارسال داده به یک یا چند 74HC595 IC.
- **مدیریت پین**: کنترل وضعیت پین‌های فردی و تغییر وضعیت آن‌ها.
- **پیکربندی انعطاف‌پذیر**: مقداردهی اولیه با تعداد مختلفی از شیفت رجیسترها و پیکربندی‌های پین.

## نیازمندی‌ها

- کتابخانه STM32 HAL برای کنترل GPIO.
- پشتیبانی از FreeRTOS (اختیاری).

## سازنده کلاس

```cpp
IC_IO_Write_74Hc595(GPIO_TypeDef* DS_Port,
                    uint16_t DS_Pin,
                    GPIO_TypeDef* SH_Port,
                    uint16_t SH_Pin,
                    GPIO_TypeDef* ST_Port,
                    uint16_t ST_Pin,
                    GPIO_TypeDef* MR_Port,
                    uint16_t MR_Pin,
                    uint8_t NumberOfIC=1);

```
* DS_Port: پورت GPIO برای Data Serial (DS).
* DS_Pin: شماره پین GPIO برای Data Serial (DS).
* SH_Port: پورت GPIO برای Shift (SH).
* SH_Pin: شماره پین GPIO برای Shift (SH).
* ST_Port: پورت GPIO برای Storage (ST).
* ST_Pin: شماره پین GPIO برای Storage (ST).
* MR_Port: پورت GPIO برای Master Reset (MR).
* MR_Pin: شماره پین GPIO برای Master Reset (MR).
* NumberOfIC: تعداد 74HC595 IC‌هایی که باید کنترل شوند (پیش‌فرض 1).

## متدهای عمومی
```cpp
void write(const uint8_t* Data)
```

یک آرایه از داده‌ها را به 74HC595 IC‌ها ارسال می‌کند. داده‌ها به صورت بیت به بیت انتقال می‌یابند.
* Data: اشاره‌گر به یک آرایه از بایت‌ها که باید به IC‌ها ارسال شود. 

## 
```cpp
void statePin(volatile uint8_t* Data, uint8_t indexIC, uint8_t pinNumber, GPIO_PinState state)
```

وضعیت یک پین خاص را برای یک آی‌سی مشخص تنظیم می‌کند.

* Data: اشاره‌گر به آرایه داده‌ها که وضعیت پین‌ها را نشان می‌دهد. 
* indexIC: ایندکس آی‌سی در آرایه داده. 
* pinNumber: شماره پینی که باید وضعیت آن تنظیم شود (0-7). 
* state: وضعیت مورد نظر (GPIO_PIN_SET یا GPIO_PIN_RESET). 

## 
```cpp
void togglePin(volatile uint8_t* Data, uint8_t indexIC, uint8_t pinNumber)
```

وضعیت یک پین خاص را برای یک آی‌سی مشخص تغییر می‌دهد.

* Data: اشاره‌گر به آرایه داده‌ها که وضعیت پین‌ها را نشان می‌دهد. 
* indexIC: ایندکس آی‌سی در آرایه داده. 
* pinNumber: شماره پینی که باید وضعیت آن تغییر کند (0-7). 

## متدهای داخلی
```cpp
void delayms(uint32_t delay)
```

برای یک تعداد مشخصی از میلی‌ثانیه‌ها تأخیر می‌کند. اگر FreeRTOS در دسترس باشد، از آن استفاده می‌کند؛ در غیر این صورت، از HAL_Delay استفاده می‌کند.

## مثال استفاده

```cpp
// ایجاد یک نمونه از کلاس
IC_IO_Write_74Hc595 shiftRegister(GPIOA, GPIO_PIN_0, GPIOA, GPIO_PIN_1, GPIOA, GPIO_PIN_2, GPIOA, GPIO_PIN_3, 2);

// داده برای ارسال
uint8_t dataToSend[2] = {0b10101010, 0b11001100};

// نوشتن داده به شیفت رجیسترها
shiftRegister.write(dataToSend);

// تنظیم وضعیت یک پین خاص
shiftRegister.statePin(dataToSend, 0, 3, GPIO_PIN_SET);

// تغییر وضعیت یک پین
shiftRegister.togglePin(dataToSend, 1, 5);
```

## نکات
* قبل از استفاده از این کلاس، اطمینان حاصل کنید که تنظیمات GPIO مناسب پیکربندی شده‌اند. 
تابع HAL_GPIO_WritePin برای کنترل پین‌های GPIO استفاده می‌شود که نیاز به کتابخانه HAL STM32 دارد.

<mark> پین های میکروکنترلر باید Open-drain باشند.</mark>


![نمونه شماتیک](media/2024-11-02%20182732.png)