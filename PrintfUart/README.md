printf and scanf and getchar() for STM32-Uart.

Sample:
-----------------------------------------------------

#include "PrintfUart.h"

extern UART_HandleTypeDef huart1;

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
