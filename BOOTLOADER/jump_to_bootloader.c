#define BOOTLOADER_ADDRESS  0x1FFFF000
void jump_to_bootloader(void)
{
    // غیر فعال کردن SysTick
    SysTick->CTRL = 0;

    // غیر فعال کردن همه وقفه‌ها
    for (int i = 0; i < 8; i++) {
        NVIC->ICER[i] = 0xFFFFFFFF;
        NVIC->ICPR[i] = 0xFFFFFFFF;
    }

    // جدا کردن تمامی کلاک‌های جانبی
    RCC->APB1ENR = 0;
    RCC->APB2ENR = 0;
    RCC->AHBENR  = 0;

    // تنظیم Main Stack Pointer به مقدار داخل bootloader
    __set_MSP(*((volatile uint32_t*) BOOTLOADER_ADDRESS));

    // فراخوانی تابع بوتلودر
    void(*bootloader)(void);
    bootloader = (void(*)(void)) *((volatile uint32_t*)(BOOTLOADER_ADDRESS + 4));
    bootloader(); // پرش به بوتلودر
}