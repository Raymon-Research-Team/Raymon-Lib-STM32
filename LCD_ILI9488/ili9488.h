
#ifndef INC_ILI9488_H_
#define INC_ILI9488_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "main.h" 

#define GUI_WIDTH 			480
#define GUI_HEIGHT 			320

#define ILI9488_SLPIN   	0x10
#define ILI9488_SLPOUT  	0x11
#define ILI9488_PTLON   	0x12
#define ILI9488_NORON   	0x13
#define ILI9488_NOP     	0x00
#define ILI9488_SWRESET 	0x01
#define ILI9488_RDDID   	0x04
#define ILI9488_RDDST   	0x09

#define ILI9488_SLPIN   	0x10
#define ILI9488_SLPOUT  	0x11
#define ILI9488_PTLON   	0x12
#define ILI9488_NORON   	0x13

#define ILI9488_RDMODE  	0x0A
#define ILI9488_RDMADCTL  	0x0B
#define ILI9488_RDPIXFMT  	0x0C
#define ILI9488_RDIMGFMT  	0x0D
#define ILI9488_RDSELFDIAG  0x0F

#define ILI9488_INVOFF  	0x20
#define ILI9488_INVON   	0x21
#define ILI9488_GAMMASET 	0x26
#define ILI9488_DISPOFF 	0x28
#define ILI9488_DISPON  	0x29

#define ILI9488_CASET   	0x2A
#define ILI9488_PASET   	0x2B
#define ILI9488_RAMWR   	0x2C
#define ILI9488_RAMRD   	0x2E

#define ILI9488_PTLAR   	0x30
#define ILI9488_MADCTL  	0x36
#define ILI9488_PIXFMT  	0x3A

#define ILI9488_FRMCTR1 	0xB1
#define ILI9488_FRMCTR2 	0xB2
#define ILI9488_FRMCTR3 	0xB3
#define ILI9488_INVCTR  	0xB4
#define ILI9488_DFUNCTR 	0xB6

#define ILI9488_PWCTR1  	0xC0
#define ILI9488_PWCTR2  	0xC1
#define ILI9488_PWCTR3  	0xC2
#define ILI9488_PWCTR4  	0xC3
#define ILI9488_PWCTR5  	0xC4
#define ILI9488_VMCTR1  	0xC5
#define ILI9488_VMCTR2  	0xC7

#define ILI9488_RDID1   	0xDA
#define ILI9488_RDID2   	0xDB
#define ILI9488_RDID3   	0xDC
#define ILI9488_RDID4   	0xDD

#define ILI9488_GMCTRP1 	0xE0
#define ILI9488_GMCTRN1 	0xE1
	/*
	#define ILI9488_PWCTR6  0xFC
	*/

	// Color definitions
#define ILI9488_BLACK       0x0000      /*   0,   0,   0 */
#define ILI9488_NAVY        0x000F      /*   0,   0, 128 */
#define ILI9488_DARKGREEN   0x03E0      /*   0, 128,   0 */
#define ILI9488_DARKCYAN    0x03EF      /*   0, 128, 128 */
#define ILI9488_MAROON      0x7800      /* 128,   0,   0 */
#define ILI9488_PURPLE      0x780F      /* 128,   0, 128 */
#define ILI9488_OLIVE       0x7BE0      /* 128, 128,   0 */
#define ILI9488_LIGHTGREY   0xC618      /* 192, 192, 192 */
#define ILI9488_DARKGREY    0x7BEF      /* 128, 128, 128 */
#define ILI9488_BLUE        0x001F      /*   0,   0, 255 */
#define ILI9488_GREEN       0x07E0      /*   0, 255,   0 */
#define ILI9488_CYAN        0x07FF      /*   0, 255, 255 */
#define ILI9488_RED         0xF800      /* 255,   0,   0 */
#define ILI9488_MAGENTA     0xF81F      /* 255,   0, 255 */
#define ILI9488_YELLOW      0xFFE0      /* 255, 255,   0 */
#define ILI9488_WHITE       0xFFFF      /* 255, 255, 255 */
#define ILI9488_ORANGE      0xFD20      /* 255, 165,   0 */
#define ILI9488_GREENYELLOW 0xAFE5      /* 173, 255,  47 */
#define ILI9488_PINK        0xF81F

#define WHITE       		0xFFFF
#define BLACK      			0x0000
#define BLUE       			0x001F
#define BRED        		0XF81F
#define GRED 				0XFFE0
#define GBLUE				0X07FF
#define RED         		0xF800
#define MAGENTA     		0xF81F
#define GREEN       		0x07E0
#define CYAN        		0x7FFF
#define YELLOW      		0xFFE0
#define BROWN 				0XBC40 //棕色
#define BRRED 				0XFC07 //棕红色
#define GRAY  				0X8430 //灰色
	 //GUI颜色

#define DARKBLUE      	 	0X01CF	//深蓝色
#define LIGHTBLUE      	 	0X7D7C	//浅蓝色
#define GRAYBLUE       	 	0X5458 //灰蓝色
	 //以上三色为PANEL的颜色

#define LIGHTGREEN     		0X841F //浅绿色
#define LIGHTGRAY     		0XEF5B //浅灰色(PANNEL)
#define LGRAY 			 	0XC618 //浅灰色(PANNEL),窗体背景色

#define LGRAYBLUE      		0XA651 //浅灰蓝色(中间层颜色)
#define LBBLUE          	0X2B12 //浅棕蓝色(选择条目的反色)

	void LCD_RESET(void);
	void LCD_Clear(uint16_t Color);
	void LCD_Init(void);
	void Lcd_LCD_WR_DATA_16Bit(uint16_t Data);
	void LCD_SetCursor(uint16_t Xpos, uint16_t Ypos);


	void ILI9488_Init(void);
	void ILI9488_SetWindow(uint16_t start_x, uint16_t start_y, uint16_t end_x, uint16_t end_y);
	void ILI9488_DrawBitmap(uint16_t w, uint16_t h, uint8_t *buff);
	
	void Lcd_WriteData_16Bit(uint16_t Data);
	void ILI9488_WritePixel(uint16_t x, uint16_t y, uint16_t color);
	void ILI9488_EndOfDrawBitmap(void);

	void LCD_WR_REG(uint8_t data);
	void LCD_IO_WriteMultipleData(uint8_t *pData, uint32_t Size);

#ifdef __cplusplus
}
#endif

#endif /* INC_ILI9341_H_ */
