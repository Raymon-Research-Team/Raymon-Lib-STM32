#pragma once
#include "main.h"
//===============================================================================
#define FLASH_BASE_ADDR 0x0801FC00 // آدرس شروع اولین صفحه فلش (باید بر اساس MCU تغییر کند)
#define FLASH_END_ADDR 0x0807F800  // آدرس آخرین صفحه فلش (باید بر اساس MCU تغییر کند)
//===============================================================================
class FlashEEPROM
{
  public:
	// نوشتن یک بایت در فلش
	static bool writeByte(uint32_t address, uint8_t value);

	// خواندن یک بایت از فلش
	static uint8_t readByte(uint32_t address);

	// نوشتن یک عدد ۴ بایتی (uint32_t)
	static bool writeUInt32(uint32_t address, uint32_t value);

	// خواندن یک عدد ۴ بایتی (uint32_t)
	static uint32_t readUInt32(uint32_t address);

	// نوشتن رشته (متن)
	static bool writeString(uint32_t address, const char *str);

	// خواندن رشته (متن)
	static bool readString(uint32_t address, char *buffer, uint16_t bufferSize);
	//===============================================================================
  private:
	// آزاد کردن دسترسی به فلش برای نوشتن
	static void unlockFlash();

	// قفل کردن دسترسی به فلش بعد از نوشتن
	static void lockFlash();

	// پاک کردن صفحه فلش
	static void erasePage(uint32_t pageAddr);

	// پیدا کردن اولین فضای خالی در فلش
	static uint32_t findNextFreeAddress();

	// نوشتن داده‌ها در فلش
	static bool writeData(uint32_t address, uint8_t *data, uint16_t size);
};
//===============================================================================
// باز کردن دسترسی به فلش برای نوشتن
void FlashEEPROM::unlockFlash()
{
	HAL_FLASH_Unlock();
}
//===============================================================================
// قفل کردن دسترسی به فلش بعد از نوشتن
void FlashEEPROM::lockFlash()
{
	HAL_FLASH_Lock();
}
//===============================================================================
// پاک کردن یک صفحه فلش
void FlashEEPROM::erasePage(uint32_t pageAddr)
{
	FLASH_EraseInitTypeDef eraseInit;
	uint32_t pageError;

	eraseInit.TypeErase = FLASH_TYPEERASE_PAGES;
	eraseInit.PageAddress = pageAddr;
	eraseInit.NbPages = 1;

	unlockFlash();
	HAL_FLASHEx_Erase(&eraseInit, &pageError);
	lockFlash();
}
//===============================================================================
// پیدا کردن اولین فضای خالی در فلش
uint32_t FlashEEPROM::findNextFreeAddress()
{
	uint32_t *ptr = (uint32_t *)FLASH_BASE_ADDR;
	uint32_t address = FLASH_BASE_ADDR;

	// چک کردن هر صفحه به ترتیب برای پیدا کردن اولین فضای خالی
	while (address < FLASH_END_ADDR)
	{
		for (uint32_t i = 0; i < FLASH_PAGE_SIZE / 4; i++)
		{
			if (ptr[i] == 0xFFFFFFFF)
			{ // پیدا کردن اولین فضای خالی
				return address + (i * 4);
			}
		}
		address += FLASH_PAGE_SIZE; // برو به صفحه بعدی
		ptr = (uint32_t *)address;	// حرکت به صفحه بعدی
	}
	return 0; // فلش پر شده است
}
//===============================================================================
// خواندن یک بایت از فلش
uint8_t FlashEEPROM::readByte(uint32_t address)
{
	// تبدیل آدرس نسبی به آدرس واقعی در فلش
	return *(uint8_t *)(FLASH_BASE_ADDR + address);
}
//===============================================================================
// نوشتن یک بایت در فلش
bool FlashEEPROM::writeByte(uint32_t address, uint8_t value)
{
	// تبدیل آدرس نسبی به آدرس واقعی در فلش
	return writeData(FLASH_BASE_ADDR + address, &value, 1);
}
//===============================================================================
// نوشتن یک عدد ۴ بایتی (uint32_t)
bool FlashEEPROM::writeUInt32(uint32_t address, uint32_t value)
{
	// تبدیل آدرس نسبی به آدرس واقعی در فلش
	return writeData(FLASH_BASE_ADDR + address, (uint8_t *)&value, sizeof(uint32_t));
}
//===============================================================================
// خواندن یک عدد ۴ بایتی (uint32_t)
uint32_t FlashEEPROM::readUInt32(uint32_t address)
{
	// تبدیل آدرس نسبی به آدرس واقعی در فلش
	return *(uint32_t *)(FLASH_BASE_ADDR + address);
}
//===============================================================================
// نوشتن رشته (متن)
bool FlashEEPROM::writeString(uint32_t address, const char *str)
{
	uint16_t len = 0;
	while (str[len] != '\0' && len < FLASH_PAGE_SIZE)
	{
		len++;
	}
	// تبدیل آدرس نسبی به آدرس واقعی در فلش
	return writeData(FLASH_BASE_ADDR + address, (uint8_t *)str, len);
}
//===============================================================================
// خواندن رشته (متن)
bool FlashEEPROM::readString(uint32_t address, char *buffer, uint16_t bufferSize)
{
	uint16_t i = 0;
	// تبدیل آدرس نسبی به آدرس واقعی در فلش
	while (i < bufferSize)
	{
		char c = readByte(address + i);
		if (c == '\0')
			break;
		buffer[i++] = c;
	}
	buffer[i] = '\0'; // پایان دادن به رشته
	return true;
}
//===============================================================================
// نوشتن داده‌ها در فلش
bool FlashEEPROM::writeData(uint32_t address, uint8_t *data, uint16_t size)
{
	unlockFlash();
	bool success = true;

	for (uint16_t i = 0; i < size; i++)
	{
		if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD, address + i, data[i]) != HAL_OK)
		{
			success = false;
			break;
		}
	}

	lockFlash();
	return success;
}
//===============================================================================
