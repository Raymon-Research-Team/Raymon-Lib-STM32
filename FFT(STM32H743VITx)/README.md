# 📊 محاسبه FFT در STM32H743VIT6 با استفاده از CMSIS-DSP

این پروژه نحوه‌ی محاسبه تبدیل فوریه سریع (FFT) را در میکروکنترلر STM32H743VIT6 با استفاده از کتابخانه CMSIS-DSP نشان می‌دهد.

---

## 🧠 FFT چیست؟

FFT (Fast Fourier Transform) الگوریتمی سریع برای محاسبه تبدیل فوریه گسسته (DFT) است. این تبدیل، یک سیگنال در حوزه زمان را به حوزه فرکانس تبدیل می‌کند.

مثال‌ها:
- موج سینوسی در حوزه زمان تبدیل به یک پیک در فرکانس می‌شود.
- FFT برای آنالیز سیگنال، فیلتر کردن، تشخیص الگو و بسیاری موارد دیگر کاربرد دارد.

---

## ⚙️ مشخصات میکروکنترلر

- **مدل:** STM32H743VIT6  
- **هسته:** ARM Cortex-M7 (حداکثر ۴۸۰ مگاهرتز)  
- **واحد ممیز شناور:** دارد (تک و دو دقتی)

---

## 🧰 کتابخانه‌های مورد استفاده

- **CMSIS-DSP**: کتابخانه DSP بهینه‌سازی‌شده توسط ARM  
- **CMSIS-Core**: هدرهای پایه هسته  
- **HAL/LL Drivers**: برای ADC، DMA و سایر سخت‌افزارها (در صورت نیاز)

---

## 📈 مراحل اجرای پروژه

1. آماده‌سازی داده‌ها (تولیدی یا خوانده‌شده از ADC)
2. اجرای FFT با استفاده از CMSIS-DSP
3. محاسبه دامنه‌ی فرکانس‌ها برای نمایش یا تحلیل

---

## 🧪 کد نمونه

```cpp
#include "arm_math.h"
#include "math.h"

// طول FFT (باید توان 2 باشه)
#define FFT_SIZE 1024

// با CMSIS-DSP
float32_t input[FFT_SIZE];       // سیگنال ورودی
float32_t output[FFT_SIZE];      // خروجی FFT (شامل real و imag به صورت بینابینی)
float32_t magnitude[FFT_SIZE/2]; // دامنه فرکانس‌ها

// ساختار CMSIS
arm_rfft_fast_instance_f32 fft_instance;

void run_fft()
{
    // 1. مقداردهی اولیه FFT
    arm_rfft_fast_init_f32(&fft_instance, FFT_SIZE);

    // 2. پر کردن input[] با داده‌ها (مثلاً موج سینوسی نمونه‌سازی شده)
    for (int i = 0; i < FFT_SIZE; i++) {
        input[i] = sinf(2 * M_PI * 1000 * i / 48000.0); // فرض 1kHz سیگنال، Sampling = 48kHz
    }

    // 3. انجام FFT
    arm_rfft_fast_f32(&fft_instance, input, output, 0); // 0 = forward FFT

    // 4. محاسبه دامنه (magnitude) فرکانس‌ها
    for (int i = 0; i < FFT_SIZE/2; i++) {
        float real = output[2*i];
        float imag = output[2*i+1];
        magnitude[i] = sqrtf(real*real + imag*imag);
    }

    // حالا magnitude[] قدرت فرکانس‌ها رو داره
    // می‌تونی بزرگ‌ترین مقدار رو پیدا کنی تا فرکانس غالب رو بفهمی
}
```

## تحلیل فرکانس
اگه بخوای بدونی فرکانس غالب چیه:
```cpp
uint32_t max_index;
float max_value;

arm_max_f32(magnitude, FFT_SIZE/2, &max_value, &max_index);

// فرکانس غالب:
float frequency = ((float)max_index * 48000.0f) / FFT_SIZE;
```

