
# doBT_01 Class

This class is designed to manage a Bluetooth module using AT commands. It communicates with the Bluetooth module via a USART interface and allows various settings such as baud rate configuration, Bluetooth name modification, enabling/disabling BLE and SPP, and setting the transmission power.

## Class Functions

### Constructor

```cpp
doBT_01(UART_HandleTypeDef *huart)
```

**Description:** Class constructor that receives a pointer to the `UART_HandleTypeDef` object. This object is used for communication with the Bluetooth module.

**Parameters:**

- `huart`: Pointer to the `UART_HandleTypeDef` object used for communication with the Bluetooth module.

### queryBaudRate

```cpp
bool queryBaudRate(char *response, uint16_t responseSize)
```

**Description:** Retrieves the current baud rate of the Bluetooth module.

**Parameters:**

- `response`: Buffer to store the response received from the module.
- `responseSize`: Size of the response buffer.

**Returns:** `true` if the command is executed successfully, otherwise `false`.

### setBaudRate

```cpp
bool setBaudRate(int baudRate)
```

**Description:** Sets the baud rate of the Bluetooth module.

**Parameters:**

- `baudRate`: Desired baud rate.

**Returns:** `true` if the command is executed successfully, otherwise `false`.

### resetModule

```cpp
bool resetModule()
```

**Description:** Resets the Bluetooth module.

**Returns:** `true` if the command is executed successfully, otherwise `false`.

### queryBLEName

```cpp
bool queryBLEName(char *response, uint16_t responseSize)
```

**Description:** Retrieves the BLE name of the Bluetooth module.

**Parameters:**

- `response`: Buffer to store the response received from the module.
- `responseSize`: Size of the response buffer.

**Returns:** `true` if the command is executed successfully, otherwise `false`.

### setBLEName

```cpp
bool setBLEName(const char *name)
```

**Description:** Sets the BLE name of the Bluetooth module.

**Parameters:**

- `name`: New BLE name.

**Returns:** `true` if the command is executed successfully, otherwise `false`.

### querySPPName

```cpp
bool querySPPName(char *response, uint16_t responseSize)
```

**Description:** Retrieves the SPP name of the Bluetooth module.

**Parameters:**

- `response`: Buffer to store the response received from the module.
- `responseSize`: Size of the response buffer.

**Returns:** `true` if the command is executed successfully, otherwise `false`.

### setSPPName

```cpp
bool setSPPName(const char *name)
```

**Description:** Sets the SPP name of the Bluetooth module.

**Parameters:**

- `name`: New SPP name.

**Returns:** `true` if the command is executed successfully, otherwise `false`.

### queryBLEMACAddress

```cpp
bool queryBLEMACAddress(char *response, uint16_t responseSize)
```

**Description:** Retrieves the BLE MAC address of the Bluetooth module.

**Parameters:**

- `response`: Buffer to store the response received from the module.
- `responseSize`: Size of the response buffer.

**Returns:** `true` if the command is executed successfully, otherwise `false`.

### setBLEMACAddress

```cpp
bool setBLEMACAddress(const char *macAddress)
```

**Description:** Sets the BLE MAC address of the Bluetooth module.

**Parameters:**

- `macAddress`: New MAC address.

**Returns:** `true` if the command is executed successfully, otherwise `false`.

### setBLEEnable

```cpp
bool setBLEEnable(bool enable)
```

**Description:** Enables or disables BLE on the Bluetooth module.

**Parameters:**

- `enable`: `true` to enable, `false` to disable.

**Returns:** `true` if the command is executed successfully, otherwise `false`.

### setSPPEnable

```cpp
bool setSPPEnable(bool enable)
```

**Description:** Enables or disables SPP on the Bluetooth module.

**Parameters:**

- `enable`: `true` to enable, `false` to disable.

**Returns:** `true` if the command is executed successfully, otherwise `false`.

### restoreFactorySettings

```cpp
bool restoreFactorySettings()
```

**Description:** Restores the Bluetooth module to its factory settings.

**Returns:** `true` if the command is executed successfully, otherwise `false`.

### setLEDConnectionStatus

```cpp
bool setLEDConnectionStatus(bool alternate)
```

**Description:** Sets the connection status LED of the Bluetooth module.

**Parameters:**

- `alternate`: `true` for alternate mode, `false` for normal mode.

**Returns:** `true` if the command is executed successfully, otherwise `false`.

### setPowerOnLOG

```cpp
bool setPowerOnLOG(bool enable)
```

**Description:** Enables or disables LOG information on power-up of the Bluetooth module.

**Parameters:**

- `enable`: `true` to enable, `false` to disable.

**Returns:** `true` if the command is executed successfully, otherwise `false`.

### setTransmissionPower

```cpp
bool setTransmissionPower(int powerLevel)
```

**Description:** Sets the transmission power of the Bluetooth module.

**Parameters:**

- `powerLevel`: Transmission power level (between 0 and 9).

**Returns:** `true` if the command is executed successfully, otherwise `false`.

### setServiceUUID

```cpp
bool setServiceUUID(const char *uuid)
```

**Description:** Sets the service UUID for BLE.

**Parameters:**

- `uuid`: New service UUID.

**Returns:** `true` if the command is executed successfully, otherwise `false`.

### setCharacteristicUUID

```cpp
bool setCharacteristicUUID(int characteristicNumber, const char *uuid)
```

**Description:** Sets the characteristic UUID for BLE.

**Parameters:**

- `characteristicNumber`: Characteristic number (between 1 and 3).
- `uuid`: New characteristic UUID.

**Returns:** `true` if the command is executed successfully, otherwise `false`.

## Example Usage

```cpp
UART_HandleTypeDef huart2; // Assume this is USART2 object

doBT_01 btModule(&huart2);

char response[100];
btModule.queryBaudRate(response, sizeof(response));
printf("Current Baud Rate: %s\n", response);

btModule.setBLEName("MyBLEDevice");
btModule.setSPPName("MySPPDevice");

btModule.setBLEEnable(true);
btModule.setSPPEnable(true);

btModule.setTransmissionPower(9);
```

---

# کلاس doBT_01

این کلاس برای مدیریت ماژول بلوتوث با استفاده از دستورات AT طراحی شده است. این کلاس از طریق رابط USART با ماژول بلوتوث ارتباط برقرار می‌کند و امکان تنظیمات مختلفی مانند تنظیم نرخ باد، تغییر نام بلوتوث، فعال‌سازی یا غیرفعال‌سازی BLE و SPP، و تنظیم قدرت انتقال را فراهم می‌کند.

## توابع کلاس

### سازنده (Constructor)

```cpp
doBT_01(UART_HandleTypeDef *huart)
```

**توضیحات:** سازنده کلاس که یک اشاره‌گر به شیء `UART_HandleTypeDef` دریافت می‌کند. این شیء برای ارتباط با ماژول بلوتوث استفاده می‌شود.

**پارامترها:**

- `huart`: اشاره‌گر به شیء `UART_HandleTypeDef` که برای ارتباط با ماژول بلوتوث استفاده می‌شود.

### queryBaudRate

```cpp
bool queryBaudRate(char *response, uint16_t responseSize)
```

**توضیحات:** نرخ باد فعلی ماژول بلوتوث را دریافت می‌کند.

**پارامترها:**

- `response`: بافر برای ذخیره پاسخ دریافت شده از ماژول.
- `responseSize`: اندازه بافر پاسخ.

**مقدار بازگشتی:** `true` اگر دستور با موفقیت اجرا شود، در غیر این صورت `false`.

### setBaudRate

```cpp
bool setBaudRate(int baudRate)
```

**توضیحات:** نرخ باد ماژول بلوتوث را تنظیم می‌کند.

**پارامترها:**

- `baudRate`: نرخ باد مورد نظر.

**مقدار بازگشتی:** `true` اگر دستور با موفقیت اجرا شود، در غیر این صورت `false`.

### resetModule

```cpp
bool resetModule()
```

**توضیحات:** ماژول بلوتوث را ریست می‌کند.

**مقدار بازگشتی:** `true` اگر دستور با موفقیت اجرا شود، در غیر این صورت `false`.

### queryBLEName

```cpp
bool queryBLEName(char *response, uint16_t responseSize)
```

**توضیحات:** نام BLE ماژول بلوتوث را دریافت می‌کند.

**پارامترها:**

- `response`: بافر برای ذخیره پاسخ دریافت شده از ماژول.
- `responseSize`: اندازه بافر پاسخ.

**مقدار بازگشتی:** `true` اگر دستور با موفقیت اجرا شود، در غیر این صورت `false`.

### setBLEName

```cpp
bool setBLEName(const char *name)
```

**توضیحات:** نام BLE ماژول بلوتوث را تنظیم می‌کند.

**پارامترها:**

- `name`: نام جدید BLE.

**مقدار بازگشتی:** `true` اگر دستور با موفقیت اجرا شود، در غیر این صورت `false`.

### querySPPName

```cpp
bool querySPPName(char *response, uint16_t responseSize)
```

**توضیحات:** نام SPP ماژول بلوتوث را دریافت می‌کند.

**پارامترها:**

- `response`: بافر برای ذخیره پاسخ دریافت شده از ماژول.
- `responseSize`: اندازه بافر پاسخ.

**مقدار بازگشتی:** `true` اگر دستور با موفقیت اجرا شود، در غیر این صورت `false`.

### setSPPName

```cpp
bool setSPPName(const char *name)
```

**توضیحات:** نام SPP ماژول بلوتوث را تنظیم می‌کند.

**پارامترها:**

- `name`: نام جدید SPP.

**مقدار بازگشتی:** `true` اگر دستور با موفقیت اجرا شود، در غیر این صورت `false`.

### queryBLEMACAddress

```cpp
bool queryBLEMACAddress(char *response, uint16_t responseSize)
```

**توضیحات:** آدرس MAC BLE ماژول بلوتوث را دریافت می‌کند.

**پارامترها:**

- `response`: بافر برای ذخیره پاسخ دریافت شده از ماژول.
- `responseSize`: اندازه بافر پاسخ.

**مقدار بازگشتی:** `true` اگر دستور با موفقیت اجرا شود، در غیر این صورت `false`.

### setBLEMACAddress

```cpp
bool setBLEMACAddress(const char *macAddress)
```

**توضیحات:** آدرس MAC BLE ماژول بلوتوث را تنظیم می‌کند.

**پارامترها:**

- `macAddress`: آدرس MAC جدید.

**مقدار بازگشتی:** `true` اگر دستور با موفقیت اجرا شود، در غیر این صورت `false`.

### setBLEEnable

```cpp
bool setBLEEnable(bool enable)
```

**توضیحات:** BLE ماژول بلوتوث را فعال یا غیرفعال می‌کند.

**پارامترها:**

- `enable`: `true` برای فعال‌سازی، `false` برای غیرفعال‌سازی.

**مقدار بازگشتی:** `true` اگر دستور با موفقیت اجرا شود، در غیر این صورت `false`.

### setSPPEnable

```cpp
bool setSPPEnable(bool enable)
```

**توضیحات:** SPP ماژول بلوتوث را فعال یا غیرفعال می‌کند.

**پارامترها:**

- `enable`: `true` برای فعال‌سازی، `false` برای غیرفعال‌سازی.

**مقدار بازگشتی:** `true` اگر دستور با موفقیت اجرا شود، در غیر این صورت `false`.

### restoreFactorySettings

```cpp
bool restoreFactorySettings()
```

**توضیحات:** تنظیمات کارخانه ماژول بلوتوث را بازگردانی می‌کند.

**مقدار بازگشتی:** `true` اگر دستور با موفقیت اجرا شود، در غیر این صورت `false`.

### setLEDConnectionStatus

```cpp
bool setLEDConnectionStatus(bool alternate)
```

**توضیحات:** وضعیت LED اتصال ماژول بلوتوث را تنظیم می‌کند.

**پارامترها:**

- `alternate`: `true` برای حالت متناوب، `false` برای حالت عادی.

**مقدار بازگشتی:** `true` اگر دستور با موفقیت اجرا شود، در غیر این صورت `false`.

### setPowerOnLOG

```cpp
bool setPowerOnLOG(bool enable)
```

**توضیحات:** اطلاعات LOG هنگام روشن شدن ماژول بلوتوث را فعال یا غیرفعال می‌کند.

**پارامترها:**

- `enable`: `true` برای فعال‌سازی، `false` برای غیرفعال‌سازی.

**مقدار بازگشتی:** `true` اگر دستور با موفقیت اجرا شود، در غیر این صورت `false`.

### setTransmissionPower

```cpp
bool setTransmissionPower(int powerLevel)
```

**توضیحات:** قدرت انتقال ماژول بلوتوث را تنظیم می‌کند.

**پارامترها:**

- `powerLevel`: سطح قدرت انتقال (بین 0 تا 9).

**مقدار بازگشتی:** `true` اگر دستور با موفقیت اجرا شود، در غیر این صورت `false`.

### setServiceUUID

```cpp
bool setServiceUUID(const char *uuid)
```

**توضیحات:** UUID سرویس BLE را تنظیم می‌کند.

**پارامترها:**

- `uuid`: UUID جدید سرویس.

**مقدار بازگشتی:** `true` اگر دستور با موفقیت اجرا شود، در غیر این صورت `false`.

### setCharacteristicUUID

```cpp
bool setCharacteristicUUID(int characteristicNumber, const char *uuid)
```

**توضیحات:** UUID مشخصه BLE را تنظیم می‌کند.

**پارامترها:**

- `characteristicNumber`: شماره مشخصه (بین 1 تا 3).
- `uuid`: UUID جدید مشخصه.

**مقدار بازگشتی:** `true` اگر دستور با موفقیت اجرا شود، در غیر این صورت `false`.

## مثال استفاده از کلاس

```cpp
UART_HandleTypeDef huart2; // فرض کنید این شیء USART2 است

doBT_01 btModule(&huart2);

char response[100];
btModule.queryBaudRate(response, sizeof(response));
printf("Current Baud Rate: %s\n", response);

btModule.setBLEName("MyBLEDevice");
btModule.setSPPName("MySPPDevice");

btModule.setBLEEnable(true);
btModule.setSPPEnable(true);

btModule.setTransmissionPower(9);
```
```

متن بالا، محتوای فایل `README.md` است.
```tool_code
# این کد برای ایجاد یک فایل README.md با محتوای مورد نظر و ارائه لینک دانلود آن است

import os
import base64

file_content = """# doBT_01 Class

This class is designed to manage a Bluetooth module using AT commands. It communicates with the Bluetooth module via a USART interface and allows various settings such as baud rate configuration, Bluetooth name modification, enabling/disabling BLE and SPP, and setting the transmission power.

## Class Functions

### Constructor

```cpp
doBT_01(UART_HandleTypeDef *huart)
```

**Description:** Class constructor that receives a pointer to the `UART_HandleTypeDef` object. This object is used for communication with the Bluetooth module.

**Parameters:**

- `huart`: Pointer to the `UART_HandleTypeDef` object used for communication with the Bluetooth module.

### queryBaudRate

```cpp
bool queryBaudRate(char *response, uint16_t responseSize)
```

**Description:** Retrieves the current baud rate of the Bluetooth module.

**Parameters:**

- `response`: Buffer to store the response received from the module.
- `responseSize`: Size of the response buffer.

**Returns:** `true` if the command is executed successfully, otherwise `false`.

### setBaudRate

```cpp
bool setBaudRate(int baudRate)
```

**Description:** Sets the baud rate of the Bluetooth module.

**Parameters:**

- `baudRate`: Desired baud rate.

**Returns:** `true` if the command is executed successfully, otherwise `false`.

### resetModule

```cpp
bool resetModule()
```

**Description:** Resets the Bluetooth module.

**Returns:** `true` if the command is executed successfully, otherwise `false`.

### queryBLEName

```cpp
bool queryBLEName(char *response, uint16_t responseSize)
```

**Description:** Retrieves the BLE name of the Bluetooth module.

**Parameters:**

- `response`: Buffer to store the response received from the module.
- `responseSize`: Size of the response buffer.

**Returns:** `true` if the command is executed successfully, otherwise `false`.

### setBLEName

```cpp
bool setBLEName(const char *name)
```

**Description:** Sets the BLE name of the Bluetooth module.

**Parameters:**

- `name`: New BLE name.

**Returns:** `true` if the command is executed successfully, otherwise `false`.

### querySPPName

```cpp
bool querySPPName(char *response, uint16_t responseSize)
```

**Description:** Retrieves the SPP name of the Bluetooth module.

**Parameters:**

- `response`: Buffer to store the response received from the module.
- `responseSize`: Size of the response buffer.

**Returns:** `true` if the command is executed successfully, otherwise `false`.

### setSPPName

```cpp
bool setSPPName(const char *name)
```

**Description:** Sets the SPP name of the Bluetooth module.

**Parameters:**

- `name`: New SPP name.

**Returns:** `true` if the command is executed successfully, otherwise `false`.

### queryBLEMACAddress

```cpp
bool queryBLEMACAddress(char *response, uint16_t responseSize)
```

**Description:** Retrieves the BLE MAC address of the Bluetooth module.

**Parameters:**

- `response`: Buffer to store the response received from the module.
- `responseSize`: Size of the response buffer.

**Returns:** `true` if the command is executed successfully, otherwise `false`.

### setBLEMACAddress

```cpp
bool setBLEMACAddress(const char *macAddress)
```

**Description:** Sets the BLE MAC address of the Bluetooth module.

**Parameters:**

- `macAddress`: New MAC address.

**Returns:** `true` if the command is executed successfully, otherwise `false`.

### setBLEEnable

```cpp
bool setBLEEnable(bool enable)
```

**Description:** Enables or disables BLE on the Bluetooth module.

**Parameters:**

- `enable`: `true` to enable, `false` to disable.

**Returns:** `true` if the command is executed successfully, otherwise `false`.

### setSPPEnable

```cpp
bool setSPPEnable(bool enable)
```

**Description:** Enables or disables SPP on the Bluetooth module.

**Parameters:**

- `enable`: `true` to enable, `false` to disable.

**Returns:** `true` if the command is executed successfully, otherwise `false`.

### restoreFactorySettings

```cpp
bool restoreFactorySettings()
```

**Description:** Restores the Bluetooth module to its factory settings.

**Returns:** `true` if the command is executed successfully, otherwise `false`.

### setLEDConnectionStatus

```cpp
bool setLEDConnectionStatus(bool alternate)
```

**Description:** Sets the connection status LED of the Bluetooth module.

**Parameters:**

- `alternate`: `true` for alternate mode, `false` for normal mode.

**Returns:** `true` if the command is executed successfully, otherwise `false`.

### setPowerOnLOG

```cpp
bool setPowerOnLOG(bool enable)
```

**Description:** Enables or disables LOG information on power-up of the Bluetooth module.

**Parameters:**

- `enable`: `true` to enable, `false` to disable.

**Returns:** `true` if the command is executed successfully, otherwise `false`.

### setTransmissionPower

```cpp
bool setTransmissionPower(int powerLevel)
```

**Description:** Sets the transmission power of the Bluetooth module.

**Parameters:**

- `powerLevel`: Transmission power level (between 0 and 9).

**Returns:** `true` if the command is executed successfully, otherwise `false`.

### setServiceUUID

```cpp
bool setServiceUUID(const char *uuid)
```

**Description:** Sets the service UUID for BLE.

**Parameters:**

- `uuid`: New service UUID.

**Returns:** `true` if the command is executed successfully, otherwise `false`.

### setCharacteristicUUID

```cpp
bool setCharacteristicUUID(int characteristicNumber, const char *uuid)
```

**Description:** Sets the characteristic UUID for BLE.

**Parameters:**

- `characteristicNumber`: Characteristic number (between 1 and 3).
- `uuid`: New characteristic UUID.

**Returns:** `true` if the command is executed successfully, otherwise `false`.

## Example Usage

```cpp
UART_HandleTypeDef huart2; // Assume this is USART2 object

doBT_01 btModule(&huart2);

char response[100];
btModule.queryBaudRate(response, sizeof(response));
printf("Current Baud Rate: %s\n", response);

btModule.setBLEName("MyBLEDevice");
btModule.setSPPName("MySPPDevice");

btModule.setBLEEnable(true);
btModule.setSPPEnable(true);

btModule.setTransmissionPower(9);
```

---

# کلاس doBT_01

این کلاس برای مدیریت ماژول بلوتوث با استفاده از دستورات AT طراحی شده است. این کلاس از طریق رابط USART با ماژول بلوتوث ارتباط برقرار می‌کند و امکان تنظیمات مختلفی مانند تنظیم نرخ باد، تغییر نام بلوتوث، فعال‌سازی یا غیرفعال‌سازی BLE و SPP، و تنظیم قدرت انتقال را فراهم می‌کند.

## توابع کلاس

### سازنده (Constructor)

```cpp
doBT_01(UART_HandleTypeDef *huart)
```

**توضیحات:** سازنده کلاس که یک اشاره‌گر به شیء `UART_HandleTypeDef` دریافت می‌کند. این شیء برای ارتباط با ماژول بلوتوث استفاده می‌شود.

**پارامترها:**

- `huart`: اشاره‌گر به شیء `UART_HandleTypeDef` که برای ارتباط با ماژول بلوتوث استفاده می‌شود.

### queryBaudRate

```cpp
bool queryBaudRate(char *response, uint16_t responseSize)
```

**توضیحات:** نرخ باد فعلی ماژول بلوتوث را دریافت می‌کند.

**پارامترها:**

- `response`: بافر برای ذخیره پاسخ دریافت شده از ماژول.
- `responseSize`: اندازه بافر پاسخ.

**مقدار بازگشتی:** `true` اگر دستور با موفقیت اجرا شود، در غیر این صورت `false`.

### setBaudRate

```cpp
bool setBaudRate(int baudRate)
```

**توضیحات:** نرخ باد ماژول بلوتوث را تنظیم می‌کند.

**پارامترها:**

- `baudRate`: نرخ باد مورد نظر.

**مقدار بازگشتی:** `true` اگر دستور با موفقیت اجرا شود، در غیر این صورت `false`.

### resetModule

```cpp
bool resetModule()
```

**توضیحات:** ماژول بلوتوث را ریست می‌کند.

**مقدار بازگشتی:** `true` اگر دستور با موفقیت اجرا شود، در غیر این صورت `false`.

### queryBLEName

```cpp
bool queryBLEName(char *response, uint16_t responseSize)
```

**توضیحات:** نام BLE ماژول بلوتوث را دریافت می‌کند.

**پارامترها:**

- `response`: بافر برای ذخیره پاسخ دریافت شده از ماژول.
- `responseSize`: اندازه بافر پاسخ.

**مقدار بازگشتی:** `true` اگر دستور با موفقیت اجرا شود، در غیر این صورت `false`.

### setBLEName

```cpp
bool setBLEName(const char *name)
```

**توضیحات:** نام BLE ماژول بلوتوث را تنظیم می‌کند.

**پارامترها:**

- `name`: نام جدید BLE.

**مقدار بازگشتی:** `true` اگر دستور با موفقیت اجرا شود، در غیر این صورت `false`.

### querySPPName

```cpp
bool querySPPName(char *response, uint16_t responseSize)
```

**توضیحات:** نام SPP ماژول بلوتوث را دریافت می‌کند.

**پارامترها:**

- `response`: بافر برای ذخیره پاسخ دریافت شده از ماژول.
- `responseSize`: اندازه بافر پاسخ.

**مقدار بازگشتی:** `true` اگر دستور با موفقیت اجرا شود، در غیر این صورت `false`.

### setSPPName

```cpp
bool setSPPName(const char *name)
```

**توضیحات:** نام SPP ماژول بلوتوث را تنظیم می‌کند.

**پارامترها:**

- `name`: نام جدید SPP.

**مقدار بازگشتی:** `true` اگر دستور با موفقیت اجرا شود، در غیر این صورت `false`.

### queryBLEMACAddress

```cpp
bool queryBLEMACAddress(char *response, uint16_t responseSize)
```

**توضیحات:** آدرس MAC BLE ماژول بلوتوث را دریافت می‌کند.

**پارامترها:**

- `response`: بافر برای ذخیره پاسخ دریافت شده از ماژول.
- `responseSize`: اندازه بافر پاسخ.

**مقدار بازگشتی:** `true` اگر دستور با موفقیت اجرا شود، در غیر این صورت `false`.

### setBLEMACAddress

```cpp
bool setBLEMACAddress(const char *macAddress)
```

**توضیحات:** آدرس MAC BLE ماژول بلوتوث را تنظیم می‌کند.

**پارامترها:**

- `macAddress`: آدرس MAC جدید.

**مقدار بازگشتی:** `true` اگر دستور با موفقیت اجرا شود، در غیر این صورت `false`.

### setBLEEnable

```cpp
bool setBLEEnable(bool enable)
```

**توضیحات:** BLE ماژول بلوتوث را فعال یا غیرفعال می‌کند.

**پارامترها:**

- `enable`: `true` برای فعال‌سازی، `false` برای غیرفعال‌سازی.

**مقدار بازگشتی:** `true` اگر دستور با موفقیت اجرا شود، در غیر این صورت `false`.

### setSPPEnable

```cpp
bool setSPPEnable(bool enable)
```

**توضیحات:** SPP ماژول بلوتوث را فعال یا غیرفعال می‌کند.

**پارامترها:**

- `enable`: `true` برای فعال‌سازی، `false` برای غیرفعال‌سازی.

**مقدار بازگشتی:** `true` اگر دستور با موفقیت اجرا شود، در غیر این صورت `false`.

### restoreFactorySettings

```cpp
bool restoreFactorySettings()
```

**توضیحات:** تنظیمات کارخانه ماژول بلوتوث را بازگردانی می‌کند.

**مقدار بازگشتی:** `true` اگر دستور با موفقیت اجرا شود، در غیر این صورت `false`.

### setLEDConnectionStatus

```cpp
bool setLEDConnectionStatus(bool alternate)
```

**توضیحات:** وضعیت LED اتصال ماژول بلوتوث را تنظیم می‌کند.

**پارامترها:**

- `alternate`: `true` برای حالت متناوب، `false` برای حالت عادی.

**مقدار بازگشتی:** `true` اگر دستور با موفقیت اجرا شود، در غیر این صورت `false`.

### setPowerOnLOG

```cpp
bool setPowerOnLOG(bool enable)
```

**توضیحات:** اطلاعات LOG هنگام روشن شدن ماژول بلوتوث را فعال یا غیرفعال می‌کند.

**پارامترها:**

- `enable`: `true` برای فعال‌سازی، `false` برای غیرفعال‌سازی.

**مقدار بازگشتی:** `true` اگر دستور با موفقیت اجرا شود، در غیر این صورت `false`.

### setTransmissionPower

```cpp
bool setTransmissionPower(int powerLevel)
```

**توضیحات:** قدرت انتقال ماژول بلوتوث را تنظیم می‌کند.

**پارامترها:**

- `powerLevel`: سطح قدرت انتقال (بین 0 تا 9).

**مقدار بازگشتی:** `true` اگر دستور با موفقیت اجرا شود، در غیر این صورت `false`.

### setServiceUUID

```cpp
bool setServiceUUID(const char *uuid)
```

**توضیحات:** UUID سرویس BLE را تنظیم می‌کند.

**پارامترها:**

- `uuid`: UUID جدید سرویس.

**مقدار بازگشتی:** `true` اگر دستور با موفقیت اجرا شود، در غیر این صورت `false`.

### setCharacteristicUUID

```cpp
bool setCharacteristicUUID(int characteristicNumber, const char *uuid)
```

**توضیحات:** UUID مشخصه BLE را تنظیم می‌کند.

**پارامترها:**

- `characteristicNumber`: شماره مشخصه (بین 1 تا 3).
- `uuid`: UUID جدید مشخصه.

**مقدار بازگشتی:** `true` اگر دستور با موفقیت اجرا شود، در غیر این صورت `false`.

## مثال استفاده از کلاس

```cpp
UART_HandleTypeDef huart2; // فرض کنید این شیء USART2 است

doBT_01 btModule(&huart2);

char response[100];
btModule.queryBaudRate(response, sizeof(response));
printf("Current Baud Rate: %s\n", response);

btModule.setBLEName("MyBLEDevice");
btModule.setSPPName("MySPPDevice");

btModule.setBLEEnable(true);
btModule.setSPPEnable(true);

btModule.setTransmissionPower(9);
```

