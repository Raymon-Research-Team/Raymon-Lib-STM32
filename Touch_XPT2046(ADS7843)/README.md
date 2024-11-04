
# کلاس XPT2046

این کلاس برای کار با صفحه لمسی XPT2046 طراحی شده است. این کلاس به شما اجازه می‌دهد که مختصات لمسی را از صفحه لمسی خوانده و آن‌ها را به مختصات GUI تبدیل کنید.

## ویژگی‌ها

- خواندن مختصات لمسی
- تبدیل مختصات لمسی به مختصات GUI
- تنظیم اندازه GUI
- پشتیبانی از عملکرد لمس (Touch)

## متغیرها و ثابت‌ها

- `TP_PRES_DOWN`: وضعیت فشردن تاچ
- `CMD_RDX`: دستور خواندن مختصات X
- `CMD_RDY`: دستور خواندن مختصات Y
- `Z_THRESHOLD`: آستانه فشار
- `XPT_XMIN`, `XPT_XMAX`, `XPT_YMIN`, `XPT_YMAX`: محدوده مختصات صفحه لمسی

## سازنده
```cpp
XPT2046(SPI_HandleTypeDef* spi, GPIO_TypeDef* csPort, uint16_t csPin, uint16_t guiWidth, uint16_t guiHeight)
```

## توابع

### `bool update(uint16_t *x, uint16_t *y)`
این تابع مختصات لمسی را به‌روزرسانی می‌کند و در صورتی که تاچ فعال باشد، مختصات را به‌روزرسانی می‌کند.

### `void getCoordinates(uint16_t *x, uint16_t *y) const`
این تابع مختصات فعلی را برمی‌گرداند.

### `void ConvXPTtoILI(uint16_t *x, uint16_t *y)`
این تابع مختصات لمسی را به مختصات GUI تبدیل می‌کند.

### `void enableTouch(bool enable)`
این تابع قابلیت تاچ را فعال یا غیرفعال می‌کند.

## مثال استفاده

در این مثال، ما از کلاس `XPT2046` برای خواندن مختصات لمسی و چاپ آن‌ها استفاده می‌کنیم.

```cpp
#include "XPT2046.h"
#include <cstdio> // برای استفاده از printf

// فرض کنید SPI_HandleTypeDef و GPIO تنظیمات اولیه شده‌اند
SPI_HandleTypeDef hspi1;
GPIO_TypeDef* csPort = GPIOB;
uint16_t csPin = GPIO_PIN_12;

int main() {
    // اندازه GUI را تنظیم می‌کنیم
    uint16_t guiWidth = 240;
    uint16_t guiHeight = 320;

    // ایجاد یک شیء از کلاس XPT2046
    XPT2046 touchScreen(&hspi1, csPort, csPin, guiWidth, guiHeight);

    // متغیرها برای نگه‌داری مختصات
    uint16_t x = 0, y = 0;

    // فعال کردن قابلیت تاچ
    touchScreen.enableTouch(true);

    while (1) {
        // بررسی و به‌روزرسانی مختصات در صورتی که تاچ فعال باشد
        if (touchScreen.update(&x, &y)) {
            touchScreen.ConvXPTtoILI(&x, &y);
            // نمایش مختصات با printf
            printf("X: %d, Y: %d
", x, y);
        }

        // می‌توان برای جلوگیری از بار زیاد روی CPU یک تأخیر کوچک قرار داد
        HAL_Delay(100);
    }

    return 0;
}
```

## نتیجه‌گیری

این کلاس به شما امکان می‌دهد تا با صفحه لمسی XPT2046 به‌سادگی کار کنید و مختصات لمسی را بخوانید و به مختصات GUI تبدیل کنید. برای استفاده از آن کافیست این کلاس را در پروژه‌ی خود اضافه کرده و از توابع آن بهره‌برداری کنید.
