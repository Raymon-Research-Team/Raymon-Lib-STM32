#pragma once
#include "main.h"
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <limits>

/*
 * SAFETY: This backend intentionally supports STM32F1 HAL only.
 * The caller must reserve the complete region in the linker script.
 * Do not enable on F4/L4/G4/H7 without a family-specific backend.
 */
#if !defined(STM32F1)
#error "FlashEEPROM supports STM32F1 only; implement and validate a family-specific backend for this MCU."
#endif

class FlashEEPROM {
public:
    FlashEEPROM(void* structPtr, size_t size, uint32_t reservedFlashStartAddress)
        : userStruct(structPtr), structSize(size), flashStartAddress(reservedFlashStartAddress) {}

    bool TryWriteStruct() {
        if (!IsConfigurationValid()) return false;
        const uint32_t totalPages = static_cast<uint32_t>((structSize + PAGE_SIZE - 1U) / PAGE_SIZE);
        const uint32_t roundedSize = totalPages * PAGE_SIZE;
        const uint32_t flashEnd = GetFlashEndExclusive();
        if (flashStartAddress > flashEnd || roundedSize > flashEnd - flashStartAddress) return false;

        if (HAL_FLASH_Unlock() != HAL_OK) return false;
        bool success = false;
        FLASH_EraseInitTypeDef erase{};
        uint32_t pageError = 0xFFFFFFFFU;
        erase.TypeErase = FLASH_TYPEERASE_PAGES;
        erase.PageAddress = flashStartAddress;
        erase.NbPages = totalPages;

        if (HAL_FLASHEx_Erase(&erase, &pageError) == HAL_OK) {
            success = true;
            const auto* source = static_cast<const uint8_t*>(userStruct);
            for (size_t offset = 0; offset < structSize; offset += sizeof(uint32_t)) {
                uint32_t word = 0xFFFFFFFFU;
                const size_t remaining = structSize - offset;
                const size_t count = remaining < sizeof(word) ? remaining : sizeof(word);
                std::memcpy(&word, source + offset, count);
                if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD,
                        flashStartAddress + static_cast<uint32_t>(offset), word) != HAL_OK) {
                    success = false;
                    break;
                }
            }
        }
        if (HAL_FLASH_Lock() != HAL_OK) success = false;
        return success;
    }

    // Legacy wrapper; use TryWriteStruct() to inspect failures.
    void WriteStruct() { (void)TryWriteStruct(); }

    bool TryReadStruct() {
        if (!IsConfigurationValid()) return false;
        const uint32_t flashEnd = GetFlashEndExclusive();
        if (flashStartAddress > flashEnd || structSize > flashEnd - flashStartAddress) return false;
        std::memcpy(userStruct, reinterpret_cast<const void*>(flashStartAddress), structSize);
        return true;
    }

    // Legacy wrapper; use TryReadStruct() to inspect failures.
    void ReadStruct() { (void)TryReadStruct(); }

private:
    void* userStruct;
    size_t structSize;
    uint32_t flashStartAddress;

    static uint32_t GetFlashEndExclusive() {
        const uint32_t flashSizeKiB = static_cast<uint32_t>(*(__IO uint16_t*)FLASHSIZE_BASE);
        return FLASH_BASE + flashSizeKiB * 1024U;
    }

    bool IsConfigurationValid() const {
        if (userStruct == nullptr || structSize == 0U) return false;
        if ((flashStartAddress % PAGE_SIZE) != 0U ||
            (flashStartAddress % sizeof(uint32_t)) != 0U) return false;
        if (structSize > (std::numeric_limits<uint32_t>::max)() - (PAGE_SIZE - 1U)) return false;
        return true;
    }
};
