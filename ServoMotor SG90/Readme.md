# کنترل یک سروو موتور استاندارد (مثل SG90) با سیگنال PWM از طریق تایمرهای STM32. 

## 🧠 اطلاعات مهم در مورد سروو:
* پریود سیگنال PWM: ~20ms (یعنی 50Hz)
* پهنای پالس برای زاویه:
* 0° → 1ms پالس (duty ~5%)
* 90° → 1.5ms پالس (duty ~7.5%)
* 180° → 2ms پالس (duty ~10%)

## 🛠️ مرحله‌به‌مرحله در STM32CubeMX (ورژن 6.14.1)
1. انتخاب میکروکنترلر
مثل همیشه: STM32F103C8Tx

## 2. تنظیم تایمر (مثلاً TIM2)
در Pinout روی PA0 کلیک کن → انتخاب TIM2_CH1

در بخش Timers روی TIM2 کلیک کن و تنظیمات رو انجام بده:
* تنظیم	مقدار
* Prescaler	= 71
* Counter Period =	19999
* Clock Division =	No Division
* Mode =	PWM Generation CH1
* Output Compare	= Enable 
* GPIO Mode	 = AF Push Pull 

### 🔍 چرا؟

کلاک تایمر: 72MHz → Prescaler = 71 → شمارش با 1MHz (1µs)

Period = 19999 → تایمر هر 20ms overflow می‌کنه → فرکانس PWM = 50Hz

## 3. فعال‌سازی GPIO برای خروجی PWM
پین PA0 باید به عنوان AF_PP (Alternate Function Push-Pull) تنظیم بشه.

## 4. Code
✍️ نوشتن کد در C++ برای کنترل زاویه سروو

```cpp
extern TIM_HandleTypeDef htim2;

void set_servo_angle(uint8_t angle) {
    // زاویه بین 0 تا 180
    if(angle > 180) angle = 180;
    
    // تبدیل زاویه به طول پالس بین 1000 تا 2000 میکروثانیه
    uint16_t pulse_length = 1000 + (angle * 1000) / 180;

    // مقدار CCR برای تایمر (1 واحد = 1µs)
    __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, pulse_length);
}
```

## 5. Sample
```cpp
HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);  // شروع PWM

while (1) {
    for (uint8_t angle = 0; angle <= 180; angle += 10) {
        set_servo_angle(angle);
        HAL_Delay(500);
    }
    for (uint8_t angle = 180; angle > 0; angle -= 10) {
        set_servo_angle(angle);
        HAL_Delay(500);
    }
}
```

## 6.نکته
* سیم زرد (سیگنال) سروو → PA0
* سیم قرمز → 5V
* سیم قهوه‌ای یا مشکی → GND


## 7. استفاده از کلاس servo.h
```cpp
#include "Servo.h"

extern TIM_HandleTypeDef htim2;

Servo myServo(&htim2, TIM_CHANNEL_1);

int main(void) {
    HAL_Init();
    SystemClock_Config();
    MX_GPIO_Init();
    MX_TIM2_Init();

    myServo.begin();

    while (1) {
        for (uint8_t angle = 0; angle <= 180; angle += 10) {
            myServo.setAngle(angle);
            HAL_Delay(300);
        }
        for (uint8_t angle = 180; angle >= 10; angle -= 10) {
            myServo.setAngle(angle);
            HAL_Delay(300);
        }
    }
}
```

