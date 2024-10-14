# Calendar Class for RTC Date Conversion

This project includes the Calendar class, which provides functions for converting dates between the Gregorian calendar and the Persian calendar. This class is designed specifically for use with RTC (Real-Time Clock) and can convert dates stored in RTC to both formats.

## Contents

• **Class Calendar**: Contains functions for date conversion.

• **Struct Date**: Used to hold date information (year, month, day, hour, minute, second).

## Key Functions

### 1. RTCToPersian

cpp

static Date RTCToPersian(RTC_HandleTypeDef \*hrtc);

Converts the date from RTC to Persian date.

### 2. RTCToGregorian

cpp

static Date RTCToGregorian(RTC_HandleTypeDef \*hrtc);

Converts the date from RTC to Gregorian date.

### 3. GregorianToPersian

cpp

static Date GregorianToPersian(Date GregorianDate);

Converts Gregorian date to Persian date.

### 4. PersianToGregorian

cpp

static Date PersianToGregorian(Date PersianDate);

Converts Persian date to Gregorian date.

## Usage

To use the Calendar class, you must first define an instance of RTC_HandleTypeDef, and then you can use the available functions to convert dates. For example:

cpp

RTC_HandleTypeDef hrtc;

Calendar::Date persianDate = Calendar::RTCToPersian(&hrtc);

## Notes

• The input RTC date should be in BCD format.

• This class is designed to work with standard STM32 RTCs.

## License

This project is licensed under the MIT License. Please refer to the LICENSE file for more details.

**Sample:**

Code Language

\#include "Calendar.h" extern RTC_HandleTypeDef hrtc;

//get Date and Time of RTC:

```
Calendar::Date date= Calendar::RTCToPersian(\&hrtc);
```

//Convert Gregorian Date to Persian Date:

```
Calendar::Date gdate \= { 2024, 10, 1 };                  	   	// gdate.Year \= 2024; gdate.Month \= 10; gdate.Day \= 1;

Calendar::Date pdate \= Calendar::GregorianToPersian(gdate);     	// pdate.Year \= 1403; pdate.Month \= 7; pdate.Day \= 10;
```

//Convert Persian Date to Gregorian Date:

```
Calendar::Date Pdate \= { 1403, 7, 10 };                   	  	// Pdate.Year \= 1403; Pdate.Month \= 7; Pdate.Day \= 10;

Calendar::Date Gdate \= Calendar::PersianToGregorian(Pdate); 	// Gdate.Year \= 2024; Gdate.Month \= 10; Gdate.Day \= 1;
```

# کلاس تقویم برای تبدیل تاریخ RTC

این پروژه شامل کلاس Calendar است که توابعی برای تبدیل تاریخ بین تقویم میلادی و تقویم شمسی ارائه می‌دهد. این کلاس به ویژه برای استفاده با RTC (Real-Time Clock) طراحی شده است و می‌تواند تاریخ‌های موجود در RTC را به فرمت شمسی و میلادی تبدیل کند.

## محتویات

• **کلاس Calendar**: شامل توابعی برای تبدیل تاریخ.

• **ساختار Date**: برای نگهداری اطلاعات تاریخ (سال، ماه، روز، ساعت، دقیقه، ثانیه).

## توابع کلیدی

### 1. RTCToPersian

cpp

static Date RTCToPersian(RTC_HandleTypeDef \*hrtc);

تاریخ را از RTC به تاریخ شمسی تبدیل می‌کند.

### 2. RTCToGregorian

cpp

static Date RTCToGregorian(RTC_HandleTypeDef \*hrtc);

تاریخ را از RTC به تاریخ میلادی تبدیل می‌کند.

### 3. GregorianToPersian

cpp

static Date GregorianToPersian(Date GregorianDate);

تاریخ میلادی را به تاریخ شمسی تبدیل می‌کند.

### 4. PersianToGregorian

cpp

static Date PersianToGregorian(Date PersianDate);

تاریخ شمسی را به تاریخ میلادی تبدیل می‌کند.

## استفاده

برای استفاده از کلاس Calendar، ابتدا باید شیئی از نوع RTC_HandleTypeDef را تعریف کنید و سپس می‌توانید از توابع موجود برای تبدیل تاریخ استفاده کنید. به عنوان مثال:

cpp

RTC_HandleTypeDef hrtc;

Calendar::Date persianDate = Calendar::RTCToPersian(&hrtc);

## نکات

• تاریخ ورودی RTC باید به فرمت BCD باشد.

• این کلاس برای کار با RTC های استاندارد STM32 طراحی شده است.

## Sample:

\#include "Calendar.h" extern RTC_HandleTypeDef hrtc;

```
  //get Date and Time of RTC:
    Calendar::Date date= Calendar::RTCToPersian(&hrtc);
  
  //Convert Gregorian Date to Persian Date:
    Calendar::Date gdate = { 2024, 10, 1 };                      		// gdate.Year = 2024; gdate.Month = 10; gdate.Day = 1;
    Calendar::Date pdate = Calendar::GregorianToPersian(gdate);   	// pdate.Year = 1403; pdate.Month = 7; pdate.Day = 10;
  
  //Convert Persian Date to Gregorian Date:
    Calendar::Date Pdate = { 1403, 7, 10 };                       	// Pdate.Year = 1403; Pdate.Month = 7; Pdate.Day = 10;
```

Calendar::Date Gdate = Calendar::PersianToGregorian(Pdate); // Gdate.Year = 2024; Gdate.
