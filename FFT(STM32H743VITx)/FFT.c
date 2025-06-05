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