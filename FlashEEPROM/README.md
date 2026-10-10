# FlashEEPROM — STM32F1 only

This component stores and restores a structure in internal flash using the STM32F1 HAL.

## Important safety constraints

- This implementation intentionally fails compilation for non-STM32F1 targets. F4, L4, G4, and H7 have different erase/program APIs and must use separate validated backends.
- The caller must reserve the entire flash region in the linker script. The library cannot detect whether the application image, bootloader, calibration values, or other data already occupies that region.
- The constructor requires an explicit reserved flash start address. Do not guess the address from a nominal flash capacity.
- The start address must be aligned to the exact page size and 32-bit programming alignment.
- Define `PAGE_SIZE` to the actual erase-page size for the selected part if the HAL does not define `FLASH_PAGE_SIZE`.
- Every write erases all pages occupied by the structure before programming. It is not wear-levelled and does not provide power-fail atomicity; power loss during write can destroy the stored record.
- Validate the MCU reference manual, linker map, option bytes, and bootloader layout before enabling writes on real hardware.

## API

```cpp
MySettings settings{};
FlashEEPROM storage(&settings, sizeof(settings), RESERVED_FLASH_START);

// Prefer checked APIs so failures are not silently ignored.
if (!storage.TryWriteStruct()) {
    // Handle flash erase/program failure.
}

if (!storage.TryReadStruct()) {
    // Handle invalid configuration or address.
}
```

The legacy `WriteStruct()` and `ReadStruct()` wrappers remain available but discard the status result. Prefer `TryWriteStruct()` and `TryReadStruct()`.

## Limitations

- Structure layout and compiler/firmware version changes are not automatically migrated.
- No CRC, version field, wear levelling, dual-slot recovery, or atomic commit is implemented.
- Do not use for frequent writes or safety-critical settings without adding a versioned record format and power-failure recovery.
