/*
 * ili9341.c
 *
 *  Created on: 2019/12/26
 *      Author: Kotetsu Yamamoto
 *      Copyright [Kotetsu Yamamoto]

I refer https://github.com/dtnghia2206/TFTLCD/blob/master/TFTLCD/ILI9341/ILI9341_Driver.c

from Original source:

MIT License

Copyright (c) 2019 NghiaDT

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

 */


#include <ili9488.h>
#include "main.h" // Hardware setting
#include "cmsis_os.h"

//static uint8_t __attribute__((section(".RAM_D3"))) LCD_SPI_buffer[28800];
static uint8_t LCD_SPI_buffer[28803];

// This function is for compatible HiLetgo ILI9341

typedef enum {
	ROTATE_0,
	ROTATE_90,
	ROTATE_180,
	ROTATE_270
} LCD_Horizontal_t;

extern void Error_Handler(void);
extern SPI_HandleTypeDef hspi2;

void ILI9488_Reset(void);
void writecommand(uint8_t c);
void writedata(uint8_t c);
static void LCD_direction(LCD_Horizontal_t direction);

void LCD_WR_REG(uint8_t data);
static void LCD_WR_DATA(uint8_t data);

#define LCD_RST_CLR 		HAL_GPIO_WritePin(LCD_RESET_GPIO_Port, LCD_RESET_Pin, GPIO_PIN_RESET);
#define LCD_RST_SET			HAL_GPIO_WritePin(LCD_RESET_GPIO_Port, LCD_RESET_Pin, GPIO_PIN_SET);
#define LCD_CS_CLR			HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_RESET);
#define LCD_CS_SET			HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_SET);
#define LCD_RS_CLR			HAL_GPIO_WritePin(LCD_CD_RS_GPIO_Port, LCD_CD_RS_Pin, GPIO_PIN_RESET);
#define LCD_RS_SET			HAL_GPIO_WritePin(LCD_CD_RS_GPIO_Port, LCD_CD_RS_Pin, GPIO_PIN_SET);
#define LED_H				HAL_GPIO_WritePin(LCD_LED_GPIO_Port, LCD_LED_Pin, GPIO_PIN_SET);
#define LED_L				HAL_GPIO_WritePin(LCD_LED_GPIO_Port, LCD_LED_Pin, GPIO_PIN_RESET);

static void SPIv_WriteData(uint8_t data);

// Initialization
void ILI9488_Init(void)
{
	LCD_Init();
}



void ILI9488_SetWindow(uint16_t start_x, uint16_t start_y, uint16_t end_x, uint16_t end_y)
{

	// Set Window
	LCD_WR_REG(0x2a);
	LCD_WR_DATA(start_x >> 8);
	LCD_WR_DATA(0x00FF & start_x);
	LCD_WR_DATA(end_x >> 8);
	LCD_WR_DATA(0x00FF & end_x);

	LCD_WR_REG(0x2b);
	LCD_WR_DATA(start_y >> 8);
	LCD_WR_DATA(0x00FF & start_y);
	LCD_WR_DATA(end_y >> 8);
	LCD_WR_DATA(0x00FF & end_y);

	LCD_WR_REG(0x2c);

}

void ILI9488_WritePixel(uint16_t x, uint16_t y, uint16_t color)
{
	LCD_SetCursor(x, y); //设置光标位置
	Lcd_WriteData_16Bit(color);
}



// Call this function after ILI9341_SetWindow
// This function is non blocked
// The variable for Callback is open. User should set by himself

void ILI9488_DrawBitmap(uint16_t w, uint16_t h, uint8_t *buff)
{
	//ILI9488_SetWindow(0, 0, 479, 319);
	
	LCD_CS_CLR;
	LCD_RS_SET;
	uint32_t LCD_SPIcount=0;
	uint32_t LCD_SPIv_count=0;
	uint16_t LCD_SPIcolor;
	for (int i = 0; i < w*h; i++)
	{
		LCD_SPIcolor = buff[LCD_SPIcount + 1] << 8 | buff[LCD_SPIcount];
		LCD_SPIcount += 2;
		LCD_SPI_buffer[LCD_SPIv_count] = ((LCD_SPIcolor >> 8) & 0xF8); //RED
		LCD_SPIv_count++;
		LCD_SPI_buffer[LCD_SPIv_count] = ((LCD_SPIcolor >> 3) & 0xFC); //GREEN
		LCD_SPIv_count++;
		LCD_SPI_buffer[LCD_SPIv_count] = (LCD_SPIcolor << 3); //BLUE
		LCD_SPIv_count++;
	}

	//HAL_SPI_Transmit_IT(&hspi1, v, w*h*3);
	HAL_SPI_Transmit_DMA(&hspi2, (uint8_t*)LCD_SPI_buffer, w*h * 3);
	

	/*	for (j = 0; j < 8; j++)
		{
			for (i = 0; i < 19200; i++)
			{
				color = buff[count + 1] << 8 | buff[count];
				count += 2;
				LCD_SPI_buffer[v_count] = ((color >> 8) & 0xF8); //RED
				v_count++;
				LCD_SPI_buffer[v_count] = ((color >> 3) & 0xFC); //GREEN
				v_count++;
				LCD_SPI_buffer[v_count] = (color << 3); //BLUE
				v_count++;
		}
		HAL_SPI_Transmit(&hspi2, (uint8_t*)LCD_SPI_buffer, 57600, 1000);
		v_count = 0;
	}
	*/
}



// User should call it at callback
void ILI9488_EndOfDrawBitmap(void)
{
	LCD_CS_SET;
}

void ILI9488_Reset(void)
{
	LCD_RESET();
}

//===============================================================
void LCD_SetCursor(uint16_t Xpos, uint16_t Ypos)
{
	ILI9488_SetWindow(Xpos, Ypos, Xpos, Ypos);
}

void writecommand(uint8_t c)
{
	LCD_RS_CLR;
	LCD_CS_CLR;
	SPIv_WriteData(c);
	LCD_CS_SET;
}

void writedata(uint8_t c)
{
	LCD_RS_SET;
	LCD_CS_CLR;

	SPIv_WriteData(c);

	LCD_CS_SET;
}



void LCD_Init(void)
{
	//LCD_GPIOInit();//LCD GPIO初始化
	LCD_RESET(); //LCD 复位
	//************* ILI9488初始化**********//
	LCD_WR_REG(0XF7);
	LCD_WR_DATA(0xA9);
	LCD_WR_DATA(0x51);
	LCD_WR_DATA(0x2C);
	LCD_WR_DATA(0x82);
	LCD_WR_REG(0xC0);
	LCD_WR_DATA(0x11);
	LCD_WR_DATA(0x09);
	LCD_WR_REG(0xC1);
	LCD_WR_DATA(0x41);
	LCD_WR_REG(0XC5);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x0A);
	LCD_WR_DATA(0x80);
	LCD_WR_REG(0xB1);
	LCD_WR_DATA(0xB0);
	LCD_WR_DATA(0x11);
	LCD_WR_REG(0xB4);
	LCD_WR_DATA(0x02);
	LCD_WR_REG(0xB6);
	LCD_WR_DATA(0x02);
	LCD_WR_DATA(0x42);
	LCD_WR_REG(0xB7);
	LCD_WR_DATA(0xc6);
	LCD_WR_REG(0xBE);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x04);
	LCD_WR_REG(0xE9);
	LCD_WR_DATA(0x00);
	LCD_WR_REG(0x3A);
	LCD_WR_DATA(0x66);
	LCD_WR_REG(0xE0);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x07);
	LCD_WR_DATA(0x10);
	LCD_WR_DATA(0x09);
	LCD_WR_DATA(0x17);
	LCD_WR_DATA(0x0B);
	LCD_WR_DATA(0x41);
	LCD_WR_DATA(0x89);
	LCD_WR_DATA(0x4B);
	LCD_WR_DATA(0x0A);
	LCD_WR_DATA(0x0C);
	LCD_WR_DATA(0x0E);
	LCD_WR_DATA(0x18);
	LCD_WR_DATA(0x1B);
	LCD_WR_DATA(0x0F);
	LCD_WR_REG(0XE1);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x17);
	LCD_WR_DATA(0x1A);
	LCD_WR_DATA(0x04);
	LCD_WR_DATA(0x0E);
	LCD_WR_DATA(0x06);
	LCD_WR_DATA(0x2F);
	LCD_WR_DATA(0x45);
	LCD_WR_DATA(0x43);
	LCD_WR_DATA(0x02);
	LCD_WR_DATA(0x0A);
	LCD_WR_DATA(0x09);
	LCD_WR_DATA(0x32);
	LCD_WR_DATA(0x36);
	LCD_WR_DATA(0x0F);
	LCD_WR_REG(0x11);
	osDelay(120);
	//HAL_Delay(120);
	LCD_WR_REG(0x29);

	LCD_direction(ROTATE_90);
	//LCD_WR_REG(0x36);    // rotate LCD
	//LCD_WR_DATA(0xe8);
	LED_H;
	//LCD_Clear(BLUE);//清全屏白色
}

void LCD_RESET(void)
{
	LCD_RST_CLR;
	osDelay(100);
	LCD_RST_SET;
	osDelay(50);
}

void LCD_Clear(uint16_t Color)
{
	unsigned int i, m;
	ILI9488_SetWindow(0, 0, GUI_WIDTH - 1, GUI_HEIGHT - 1);
	LCD_CS_CLR;
	LCD_RS_SET;
	for (i = 0; i < GUI_HEIGHT; i++)
	{
		for (m = 0; m < GUI_WIDTH; m++)
		{
			Lcd_WriteData_16Bit(Color);
		}
	}
	LCD_CS_SET;
}


void LCD_WR_REG(uint8_t data)
{
	LCD_CS_CLR;
	LCD_RS_CLR;
	SPIv_WriteData(data);
	LCD_CS_SET;
}

static void SPIv_WriteData(uint8_t data)
{

	if (HAL_SPI_Transmit(&hspi2, &data, 1, 1000) != HAL_OK) {
		Error_Handler();
	}
}

static void LCD_WR_DATA(uint8_t data)
{
	LCD_CS_CLR;
	LCD_RS_SET;
	SPIv_WriteData(data);
	LCD_CS_SET;
}

void LCD_WriteReg(uint8_t LCD_Reg, uint16_t LCD_RegValue)
{
	LCD_WR_REG(LCD_Reg);
	LCD_WR_DATA(LCD_RegValue);
}

void Lcd_WriteData_16Bit(uint16_t Data)
{
	//18Bit
	LCD_WR_DATA((Data >> 8) & 0xF8); //RED
	LCD_WR_DATA((Data >> 3) & 0xFC); //GREEN
	LCD_WR_DATA(Data << 3); //BLUE
}
//===============================================================





static void LCD_direction(LCD_Horizontal_t direction)
{
	switch (direction) {
	case ROTATE_0:
		LCD_WR_REG(0x36);
		LCD_WR_DATA((1 << 3) | (0 << 6) | (0 << 7)); //BGR==1,MY==0,MX==0,MV==0
		break;
	case ROTATE_90:
		LCD_WR_REG(0x36);
		LCD_WR_DATA((1 << 3) | (0 << 7) | (1 << 6) | (1 << 5)); //BGR==1,MY==1,MX==0,MV==1
		break;
	case ROTATE_180:
		LCD_WR_REG(0x36);
		LCD_WR_DATA((1 << 3) | (1 << 6) | (1 << 7)); //BGR==1,MY==0,MX==0,MV==0
		break;
	case ROTATE_270:
		LCD_WR_REG(0x36);
		LCD_WR_DATA((1 << 3) | (1 << 7) | (1 << 5)); //BGR==1,MY==1,MX==0,MV==1
		break;
	}
}




