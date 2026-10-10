<div align="center">

# Raymon-Lib-STM32

**A C++ library for STM32 microcontrollers**

[![Platform](https://img.shields.io/badge/Platform-STM32%20%2F%20ARM%20Cortex--M-orange.svg?style=for-the-badge)]()
[![Language](https://img.shields.io/badge/Language-C%2B%2B-blue.svg?style=for-the-badge)]()
[![License: BUSL-1.1](https://img.shields.io/badge/License-BUSL--1.1-blue.svg?style=for-the-badge)](LICENSE)

[فارسی](docs/README_FA.md) | [English](README.md)

</div>

---

## Overview

Raymon-Lib-STM32 is a C++ library repository for STM32 embedded development. It is intended to provide reusable code for STM32-based firmware projects.

This repository is under active development. Check the source tree and project history for the components currently available; do not assume a peripheral, MCU family, or feature is supported unless it is implemented in the code.

## Getting Started

1. Clone or download this repository.
2. Review the source files and any project-specific configuration before integrating the library.
3. Use the library with a compatible STM32 toolchain and configure the target MCU, startup code, linker script, and peripheral initialization for your project.
4. Build and test on the actual target hardware before using it in a production system.

No universal build command is documented here because the correct build procedure depends on the consuming STM32 project and its toolchain configuration.

## Compatibility

- **Target platform:** STM32 microcontrollers (exact supported devices depend on the source code and project configuration).
- **Language:** C++.
- **Toolchain:** Use a toolchain compatible with the target MCU and the code in this repository.

## Repository Status

The README describes the repository at a high level. Consult the source files for the authoritative implementation and verify the behavior on the intended board. Hardware-dependent behavior, timing, memory usage, and peripheral compatibility must be validated in the target environment.

## Contributing

Bug reports, corrections, and focused improvements are welcome through GitHub Issues and Pull Requests. Include the target MCU, toolchain/version, steps to reproduce, and relevant build output when reporting a problem. Keep changes focused and test them on hardware where practical.

## License

This project is licensed under the **Business Source License 1.1 (BUSL-1.1)**. It permits copying, modification, redistribution, and non-production use subject to the license terms. Production or commercial use outside the license grant requires a separate commercial license. See [LICENSE](LICENSE) for the complete terms.

The Change Date and Change License are specified in the license file. Review the license before using this code in a product or commercial deployment.

---

Maintained by **Raymon Research Team**.
