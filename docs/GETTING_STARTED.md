# Getting Started and API Notes

[English](GETTING_STARTED.md) | [فارسی](GETTING_STARTED_FA.md)

## Repository model

This repository is a collection of feature folders, not a single library target. Integrate only the component you need. No repository-wide build/test configuration was verified during this documentation pass.

## Recommended integration workflow

1. Identify the exact MCU part number and board.
2. Create a clean STM32CubeIDE/STM32CubeMX project and configure clocks, GPIO, and peripherals.
3. Read the component's local README and inspect its source/header files.
4. Add required files and configure include paths.
5. Confirm HAL/CMSIS versions and the C++ language level.
6. Build with warnings enabled.
7. Test on hardware with a debugger and a recovery procedure. Do not test flash erase or read-protection changes on the only copy of important firmware.

## Verified API inventory

### QBuffer — QBuffer/QBuffer.hpp

- QBuffer(uint32_t bufferSize): allocates a buffer of the requested capacity.
- Flush(): clears logical contents.
- Push(uint8_t data): appends a byte; when full, ring-buffer behavior can overwrite unread data.
- Pop(): returns the next byte, or -1 when empty.
- GetLength() const: returns current length.
- PeekAt(uint32_t index) const: returns a byte or -1 for an invalid index.
- Remove(uint32_t num): discards bytes from the front.

The class owns a raw pointer and does not define copy/move operations. Review object-copy behavior to avoid double-free or memory corruption.

### QCRC — QBuffer/QCRC.hpp

QCRC::CRC_8_CCITT(data, length) uses initial value 0xFF and polynomial 0x07. Do not assume it matches every protocol called CRC-8/CCITT; verify the complete CRC parameters against the peer protocol.

### QPacket / QCommand — QBuffer/

QPacket.hpp creates frames using start marker 0xAA, end marker 0xBB, a counter, length field, and CRC bytes. QCommand.hpp validates/parses frames from QBuffer. Test malformed, truncated, maximum-size, and back-to-back packets before deployment.

### FlashEEPROM — FlashEEPROM/FlashEEPROM.hpp

The class accepts a pointer and size, calculates a flash address from compile-time family macros, and exposes WriteStruct() and ReadStruct(). WriteStruct() erases the calculated region before writing.

**Critical warning:** do not use until the address calculation, HAL erase fields, flash programming granularity, reserved pages/banks, and linker layout have been verified for the exact MCU. A wrong range can erase firmware or application data. The current public methods do not report HAL operation errors to the caller.

### PrintfUart — PrintfUart/PrintfUart.h

The constructor accepts a UART handle and an echo option. printf(...) sends formatted output; scanf(...) reads until CR/LF and parses the text. The implementation is header-based and depends on the consuming project's main.h.

Input reception is blocking and uses HAL_MAX_DELAY. Review timeout/error handling before production use.

### Servo — ServoMotor SG90/Servo.h

The header exposes a Servo constructor, begin(), and setAngle(uint8_t). The included HAL header is STM32F1-specific. Timer period/prescaler must be calibrated to the actual timer clock and target MCU.

### FFT — FFT(STM32H743VITx)/README.md

The example documents CMSIS-DSP real FFT on STM32H743. Confirm CMSIS-DSP version, FFT size support, linker inclusion, floating-point settings, and sample rate before relying on results.

### Bootloader and read-protection examples

BOOTLOADER/ and Lock_program_in_code/ contain MCU-specific examples and instructions. Treat them as experimental until the memory map, option bytes, and recovery process are verified against the exact device reference manual.

## Build and test status

No universal build command is documented or verified here. No compile, static analysis, or hardware test was performed during this documentation change. Record MCU, compiler version, HAL/CMSIS versions, build output, and hardware test results for each validated integration.

## Before production

- Check buffer bounds and malformed input behavior.
- Check HAL return values and timeout paths.
- Validate flash boundaries and erase/program granularity.
- Review concurrency, interrupt, and task usage.
- Confirm the license of each component and every external dependency.