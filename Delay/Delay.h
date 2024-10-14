#pragma once
#include "main.h"
//-------------------------------------------------------------------------------------------
class Delay
{
public:
    // تابع تاخیر بر اساس میکروثانیه
    static void delay_us(uint32_t us)
    {
        if (!(CoreDebug->DEMCR & CoreDebug_DEMCR_TRCENA_Msk)) {
            CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk; // فعال‌سازی DWT
        }
        DWT->CYCCNT = 0; // صفر کردن شمارنده
        DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk; // فعال کردن شمارنده
        // صفر کردن شمارنده
        DWT->CYCCNT = 0;
        
        
        uint32_t start =  DWT->CYCCNT;
        // به‌دست آوردن فرکانس کلاک پردازنده
        uint32_t clockFreq = HAL_RCC_GetHCLKFreq();
        uint32_t cycles_per_us = clockFreq / 1000000L;

        // محاسبه تعداد سیکل‌های موردنیاز
        uint32_t delay_cycles = us * cycles_per_us;

        // انتظار تا زمانی که زمان سپری‌شده به مقدار موردنظر برسد
        while ((DWT->CYCCNT - start) < delay_cycles) {
            // حلقه بی‌عمل
        }
    }
    //-------------------------------------------------------------------------------------------
    // تابع تاخیر بر اساس میلی‌ثانیه
    static void delay_ms(uint32_t ms)
    {
        for (uint32_t i = 0; i < ms; i++)
        {
            delay_us(999); // تاخیر 1000 میکروثانیه برای هر میلی‌ثانیه
        }
    }
    //-------------------------------------------------------------------------------------------
};
