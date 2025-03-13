#pragma once
#include "main.h"
#include <string>
#include <vector>

// تعیین اندازه صفحه برای هر خانواده میکروکنترلر
#ifndef PAGE_SIZE
#if defined(STM32F1)
#define PAGE_SIZE 1024
#define FLASH_END_ADDRESS 0x0801FFFF
#elif defined(STM32F4)
#define PAGE_SIZE 16384
#define FLASH_END_ADDRESS 0x080FFFFF
#elif defined(STM32L4)
#define PAGE_SIZE 2048
#define FLASH_END_ADDRESS 0x080FFFFF
#elif defined(STM32H7)
#define PAGE_SIZE 131072 // 128KB per page
#define FLASH_END_ADDRESS 0x081FFFFF
#elif defined(STM32G4)
#define PAGE_SIZE 2048
#define FLASH_END_ADDRESS 0x080FFFFF
#else
#error "Unsupported STM32 series"
#endif
#endif


class FlashEEPROM
{
  public:
    void *userStruct;
    size_t structSize;
    uint32_t flashStartAddress;

    FlashEEPROM(void *structPtr, size_t size) : userStruct(structPtr), structSize(size)
    {
        uint32_t totalPages = (structSize / PAGE_SIZE) + ((structSize % PAGE_SIZE) ? 1 : 0);
        flashStartAddress = FLASH_END_ADDRESS - (totalPages * PAGE_SIZE) + 1;
    }

    void WriteStruct()
    {
        uint32_t totalPages = (structSize / PAGE_SIZE) + ((structSize % PAGE_SIZE) ? 1 : 0);
        uint8_t buffer[structSize];
        memcpy(buffer, userStruct, structSize);

        HAL_FLASH_Unlock(); // باز کردن قفل فلش

        // پاک کردن صفحات مورد نیاز
        FLASH_EraseInitTypeDef eraseInitStruct;
        uint32_t pageError;
        eraseInitStruct.TypeErase = FLASH_TYPEERASE_PAGES;
        eraseInitStruct.PageAddress = flashStartAddress;
        eraseInitStruct.NbPages = totalPages;
        HAL_FLASHEx_Erase(&eraseInitStruct, &pageError);

        // نوشتن داده‌ها به‌صورت 32 بیتی
        for (uint32_t i = 0; i < structSize; i += 4)
        {
            uint32_t word = 0xFFFFFFFF;
            memcpy(&word, &buffer[i], (structSize - i) >= 4 ? 4 : (structSize - i));
            HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, flashStartAddress + i, word);
        }

        HAL_FLASH_Lock(); // قفل کردن مجدد فلش
    }

    void ReadStruct()
    {
        uint8_t buffer[structSize];
        memcpy(buffer, (uint8_t *)flashStartAddress, structSize);
        memcpy(userStruct, buffer, structSize);
    }
};


