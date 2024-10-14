#pragma once
#include "main.h"

#ifdef USE_FREERTOS
#include "FreeRTOS.h"
#include "task.h"
#endif

class Delay
{
public:
    // تابع تاخیر بر اساس میکروثانیه
    static void delay_us(uint32_t us)
    {
        if (isFreeRTOSRunning())
        {
            // اگر FreeRTOS فعال باشد و تاخیر بیشتر از 1000 میکروثانیه باشد، از vTaskDelay استفاده کن
            if (us >= 1000)
            {
#ifdef USE_FREERTOS
                vTaskDelay(us / 1000); // تبدیل میکروثانیه به میلی‌ثانیه و استفاده از vTaskDelay
#endif
            }
            else
            {
                // اگر تاخیر کمتر از 1000 میکروثانیه است، از busy-wait استفاده کن
                busy_wait_us(us);
            }
        }
        else
        {
            // اگر FreeRTOS فعال نباشد، از روش busy-wait استفاده کن
            busy_wait_us(us);
        }
    }

    // تابع تاخیر بر اساس میلی‌ثانیه
    static void delay_ms(uint32_t ms)
    {
        if (isFreeRTOSRunning())
        {
            // اگر FreeRTOS فعال است، از vTaskDelay برای تاخیر استفاده کن
#ifdef USE_FREERTOS            
            vTaskDelay(ms); // تاخیر بر اساس میلی‌ثانیه
#endif
        }
        else
        {
            // اگر FreeRTOS فعال نیست، از busy-wait برای میلی‌ثانیه استفاده کن
            busy_wait_ms(ms);
        }
    }

private:
    // تابع تشخیص اجرای FreeRTOS
    static bool isFreeRTOSRunning()
    {
#ifdef USE_FREERTOS
        return (xTaskGetSchedulerState() != taskSCHEDULER_NOT_STARTED);
#else
        return false;
#endif
    }

    // تابع برای اجرای busy-wait برای تاخیر میکروثانیه
    static void busy_wait_us(uint32_t us)
    {
        uint32_t clockFreq = HAL_RCC_GetHCLKFreq(); 
        uint32_t cycles_per_us = clockFreq / 1000000L;
        uint32_t total_cycles = us * cycles_per_us;

        for (uint32_t i = 0; i < total_cycles; i++)
        {
            __NOP(); // اجرای یک دستورالعمل بی‌عمل (No Operation)
        }
    }

    // تابع برای اجرای busy-wait برای تاخیر میلی‌ثانیه
    static void busy_wait_ms(uint32_t ms)
    {
        // چون هر میلی‌ثانیه برابر با 1000 میکروثانیه است، از busy_wait_us استفاده می‌کنیم
        for (uint32_t i = 0; i < ms; i++)
        {
            busy_wait_us(1000); // تاخیر 1000 میکروثانیه برای هر میلی‌ثانیه
        }
    }
};
