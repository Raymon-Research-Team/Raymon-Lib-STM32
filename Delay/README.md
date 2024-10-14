# Delay Library for STM32

این کتابخانه تاخیر برای STM32 طراحی شده است و امکان استفاده از FreeRTOS را نیز پشتیبانی می‌کند. با استفاده از این کتابخانه می‌توانید تاخیرهای میکروثانیه‌ای و میلی‌ثانیه‌ای را در پروژه‌های خود اعمال کنید. این کتابخانه به صورت خودکار بررسی می‌کند که آیا FreeRTOS در حال اجراست یا خیر و بر اساس آن بهترین روش برای تاخیر را انتخاب می‌کند.

## ویژگی‌ها

-   **پشتیبانی از FreeRTOS**: اگر FreeRTOS فعال باشد، برای تاخیرهای بیشتر از 1000 میکروثانیه از vTaskDelay استفاده می‌شود.
-   **تاخیر دقیق در حالت بدون FreeRTOS**: در صورتی که FreeRTOS فعال نباشد یا تاخیر کمتر از 1000 میکروثانیه باشد، از روش busy-wait برای تاخیر استفاده می‌شود.
-   **تاخیر میکروثانیه و میلی‌ثانیه**: دو تابع delay_us و delay_ms برای تاخیرهای میکروثانیه‌ای و میلی‌ثانیه‌ای در دسترس هستند.

## نحوه استفاده

1.  کد کتابخانه را در پروژه STM32 خود وارد کنید.
2.  در صورتی که از FreeRTOS استفاده می‌کنید، مطمئن شوید که ماکروی USE_FREERTOS تعریف شده باشد.
3.  از توابع delay_us و delay_ms برای ایجاد تاخیر استفاده کنید.

## نکات مهم

-   تاخیرهای طولانی مدت در حالت busy-wait ممکن است منابع CPU را مصرف کنند. در صورتی که FreeRTOS غیرفعال است، بهتر است از تایمرهای سخت‌افزاری برای تاخیرهای طولانی استفاده شود.
-   تابع HAL_RCC_GetHCLKFreq() برای محاسبه فرکانس استفاده می‌شود. مطمئن شوید که فرکانس سیستم به درستی تنظیم شده باشد.
-   واحد زمانی در FreeRTOS بر اساس تیک‌های سیستم است؛ بنابراین تنظیم صحیح configTICK_RATE_HZ ضروری است.

## پشتیبانی

این کتابخانه بر روی اکثر سیستم‌های STM32 با یا بدون FreeRTOS به خوبی کار می‌کند. با این حال، در صورت بروز مشکل یا نیاز به سفارشی‌سازی بیشتر، لطفاً تنظیمات سیستم (مثل فرکانس و تیک‌ریت) را بررسی کنید.

**English**

# Delay Library for STM32

This delay library is designed for STM32 and supports FreeRTOS. Using this library, you can implement microsecond and millisecond delays in your projects. It automatically checks if FreeRTOS is running and selects the most appropriate method for the delay based on that.

## Features

-   **FreeRTOS Support**: If FreeRTOS is running, vTaskDelay is used for delays greater than 1000 microseconds.
-   **Precise Delays Without FreeRTOS**: If FreeRTOS is not running or the delay is less than 1000 microseconds, a busy-wait method is used for delays.
-   **Microsecond and Millisecond Delays**: The delay_us and delay_ms functions are available for microsecond and millisecond delays.

## How to Use

1.  Include the library code in your STM32 project.
2.  If you are using FreeRTOS, ensure that the USE_FREERTOS macro is defined.
3.  Use the delay_us and delay_ms functions to create delays.

## Important Notes

-   Long delays in busy-wait mode may consume CPU resources. If FreeRTOS is disabled, it's recommended to use hardware timers for long delays.
-   The function HAL_RCC_GetHCLKFreq() is used for clock frequency calculation. Ensure that your system clock is correctly configured.
-   The time unit in FreeRTOS is based on system ticks, so setting the correct configTICK_RATE_HZ is crucial.

## Support

This library works well on most STM32 systems with or without FreeRTOS. However, if you encounter any issues or need further customization, please check your system settings (such as clock frequency and tick rate).

```Code Language
Sample Code - LED On/Off with Microsecond and Millisecond Delay
#include "main.h"
#include "delay.h" // Delay library

int main(void)
{
    // Initialization
    HAL_Init();
    SystemClock_Config();

    // Configure LED pin (e.g., onboard LED)
    __HAL_RCC_GPIOA_CLK_ENABLE(); // Assume LED is connected to port A
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    GPIO_InitStruct.Pin = GPIO_PIN_5; // Assume LED is connected to pin 5
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    while (1)
    {
        // Turn LED on
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);

        // Delay for 500 milliseconds
        Delay::delay_ms(500);

        // Turn LED off
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);

        // Delay for 500 microseconds
        Delay::delay_us(500);
    }
}
```

### 

## توضیحات:

### در این کد LED به پین 5 پورت A متصل است و با استفاده از توابع HAL_GPIO_WritePin روشن و خاموش می‌شود.

### ابتدا LED به مدت 500 میلی‌ثانیه روشن می‌ماند و سپس به مدت 500 میکروثانیه خاموش می‌شود.

### 
