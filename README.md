<div align="center">

# Raymon-Lib-STM32

**A collection of C/C++ utilities and examples for STM32 firmware**

[![Platform](https://img.shields.io/badge/Platform-STM32%20%2F%20ARM%20Cortex--M-orange.svg?style=for-the-badge)]()
[![Language](https://img.shields.io/badge/Language-C%2B%2B-blue.svg?style=for-the-badge)]()
[![License: BUSL-1.1](https://img.shields.io/badge/License-BUSL--1.1-blue.svg?style=for-the-badge)](LICENSE)

[فارسی](docs/README_FA.md) | [English](README.md)

</div>

---

## Overview

Raymon-Lib-STM32 is a collection of embedded C/C++ utilities, peripheral examples, and experiments for STM32 microcontrollers. The repository is organized by feature rather than as a single installable package. Each component may have its own hardware, STM32 HAL, compiler, or library requirements.

## Component Catalogue

| Component | Purpose | Requirements / notes |
|---|---|---|
| [QBuffer](QBuffer/QBuffer.hpp) | Fixed-capacity byte buffer | Header-only; review ownership/copy semantics |
| [QPacket](QBuffer/QPacket.hpp) | Packet framing with markers, length, counter and CRC | Uses [QCRC](QBuffer/QCRC.hpp); inspect QCommand too |
| [FlashEEPROM](FlashEEPROM/README.md) | Store and restore a structure in internal flash | STM32 HAL and project main.h; target flash layout must be checked |
| [PrintfUart](PrintfUart/README.md) | Formatted UART input/output | STM32 HAL and project main.h |
| [Servo Motor SG90](ServoMotor%20SG90/Readme.md) | PWM-based servo example | Existing guide uses STM32F1/TIM2 as its example |
| [FFT for STM32H743](FFT(STM32H743VITx)/README.md) | FFT example using CMSIS-DSP | Compatible CMSIS-DSP and target configuration |
| [Bootloader jump](BOOTLOADER/jump_to_bootloader.c) | MCU-specific bootloader jump | Verify exact MCU and memory map |
| [Lock program in code](Lock_program_in_code/README.md) | Read-protection / firmware-locking notes | Potentially destructive; review before use |
| [Bluetooth example](Bluetooth(doBT_01)/doBT_01.hpp) | Module-specific Bluetooth code | Hardware and protocol dependent |

This catalogue is based on files currently present in the repository. It does not claim that every component has been compiled or hardware-validated.

## Getting Started

1. Clone the repository.
2. Open the folder for the component you need and read its local README.
3. Start from an STM32CubeIDE / STM32CubeMX project configured for the exact MCU, clock tree, pins and peripherals.
4. Add only the required files and configure include paths, STM32 HAL, and external dependencies.
5. Build for the target and test on hardware with a safe recovery procedure.

```bash
git clone https://github.com/Raymon-Research-Team/Raymon-Lib-STM32.git
```

There is no verified repository-wide build system or single installation command documented here. Components must be integrated individually.

## Safety and Portability

- **Internal flash:** FlashEEPROM erases a calculated flash region before writing. Verify the reference manual, erase granularity, banks, reserved pages, and linker map before use.
- **Bootloader / read protection:** Incorrect addresses or protection settings can make firmware inaccessible or require a full-chip erase.
- **PWM / servo:** Timer period, prescaler, channel, and GPIO settings depend on the MCU and clock tree.
- **FFT:** CMSIS-DSP version, compiler settings, memory placement, and FPU configuration affect integration.
- **No blanket compatibility guarantee:** Source presence does not establish support for every STM32 family or board.

## Documentation

- [Persian README](docs/README_FA.md)
- [Getting Started and API Notes](docs/GETTING_STARTED.md)
- [License and third-party dependency notes](docs/LICENSING.md)

## Contributing

Include the MCU part number, board, toolchain version, HAL/CMSIS versions, build output, and reproducible steps in bug reports. Do not describe a component as hardware-validated unless it was tested on the specified target.

## License

The repository's top-level LICENSE declares **Business Source License 1.1 (BUSL-1.1)**. BUSL-1.1 is not an Open Source license. Review the full terms before use in production or a commercial product. External dependencies remain subject to their own licenses; see [Licensing Notes](docs/LICENSING.md).

Maintained by **Raymon Research Team**.