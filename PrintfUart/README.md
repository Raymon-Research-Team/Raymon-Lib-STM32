# PrintfUart Library

## Overview

The \`PrintfUart\` class provides an easy interface for formatted input and output over UART in STM32 applications. It allows you to send formatted strings and read input with specified formats, making serial communication more convenient.

## Features

\- Formatted output using printf-style formatting.

\- Input reading with support for multiple data types.

\- Optional feedback for input received over UART.

\- Automatic delay adjustment based on baud rate.

## \#\# Installation

To use the \`PrintfUart\` library, include the header file in your project and link it with your STM32 HAL library.


```cpp
#include "PrintfUart.h"
```



## Usage

## Initialization

Create an instance of PrintfUart by passing the UART handle and feedback option.


```cpp
UART_HandleTypeDef huart; // Your UART handle
PrintfUart uart(&huart, true); // Set feedback input
```



## 

## Sending Data

You can send formatted data using the printf method:

```cpp
uart.printf("Hello, %s! Number: %d\n", "World", 42);
```

## 

## Receiving Data

To receive formatted input, use the scanf method:

```cpp
int number;
std::string text;
uart.scanf("%s %d", text, number);
```

## 

## Reading a Character

Use the getchar method to read a single character from UART:

```cpp
char receivedChar = uart.getchar();
```

## 

## Example

```cpp
extern "C" void main() {
    UART_HandleTypeDef huart; // Initialize your UART here
    PrintfUart uart(&huart, true);
    
    uart.printf("UART Initialized.\n");

    std::string name;
    int value;
    uart.scanf("%s %d", name, value);
    uart.printf("Received: %s %d\n", name.c_str(), value);
}
```

# Persian README content

# PrintfUart

## مرور کلی

کلاس PrintfUart یک رابط ساده برای ورودی و خروجی فرمت‌دار از طریق UART در برنامه‌های STM32 فراهم می‌کند. این کلاس به شما امکان می‌دهد رشته‌های فرمت‌دار را ارسال کرده و ورودی را با فرمت‌های مشخص شده بخوانید و ارتباط سری را راحت‌تر می‌سازد.

## ویژگی‌ها

-   خروجی فرمت‌دار با استفاده از فرمت‌دهی به سبک printf.
-   خواندن ورودی با پشتیبانی از انواع داده‌های مختلف.
-   بازخورد اختیاری برای ورودی دریافتی از طریق UART.
-   تنظیم خودکار تأخیر بر اساس نرخ Baud.
-   

## نصب

برای استفاده از کتابخانه PrintfUart، فایل هدر را در پروژه خود گنجانده و آن را با کتابخانه HAL STM32 خود لینک کنید.

```cpp
#include "PrintfUart.h"
```

## استفاده

## راه‌اندازی

یک نمونه از PrintfUart با گذراندن دسته UART و گزینه بازخورد ایجاد کنید.

```cpp
UART_HandleTypeDef huart; // دسته UART شما
PrintfUart uart(&huart, true); // تنظیم بازخورد ورودی 
```

## ارسال داده‌ها

شما می‌توانید با استفاده از متد printf داده‌های فرمت‌دار ارسال کنید:

```cpp
uart.printf("سلام، %s! شماره: %d\n", "دنیا", 42);
```

## دریافت داده‌ها

برای دریافت ورودی فرمت‌دار، از متد scanf استفاده کنید:

```cpp
int number;
std::string text;
uart.scanf("%s %d", text, number);
```

## خواندن یک کاراکتر

از متد getchar برای خواندن یک کاراکتر از UART استفاده کنید:

```cpp
char receivedChar = uart.getchar();
```

## مثال

```cpp
bool feedback = true;                    // feedback Of insert Character of uart to uart
PrintfUart io(&huart1, feedback);        // initial uart for printf or scanf
-----------------------------------------------------
char c = io.getchar(); 
-----------------------------------------------------
int i_num = 10;
io.printf("int number is %d\r\n", i_num);
-----------------------------------------------------
float f_num  = -100.54;
io.printf("Float number is %f\r\n", f_num);
-----------------------------------------------------
double d_num = 1623.92377;
io.printf("Double number is %lf\r\n", d_num);
-----------------------------------------------------
char chr1 =  'C';
io.printf("char is %c\r\n", chr1);
-----------------------------------------------------      
char chr2[]  = "Hello World!";
io.printf("Array char is: %s\r\n", chr2);
-----------------------------------------------------
std::string name = "Rahman";
io.printf("My Name is %s\r\n", name);
-----------------------------------------------------
        
io.scanf("%s %d", name, i_num);
io.printf("My Name is %s and my Age is %d\r\n", name , i_num);
```
