# Licensing and Third-Party Dependencies

[English](LICENSING.md) | [فارسی](LICENSING_FA.md)

## Repository license

The repository is licensed under the MIT License. See the top-level [LICENSE](../LICENSE) file for the complete terms. The MIT License permits use, copying, modification, distribution, sublicensing, and sale, provided the copyright and permission notice are included in copies or substantial portions of the Software. It is provided without warranty.

## Third-party dependencies

The repository integrates with or documents external components, including STM32 HAL / STM32Cube firmware packages, ARM CMSIS-DSP in the FFT example, and STM32CubeIDE / STM32CubeMX tooling in consuming projects.

A complete, verified Software Bill of Materials (SBOM) and per-file provenance record have not been established. These are apparent integration dependencies; this does not mean all their source code is vendored into this repository.

External libraries, generated files, and third-party snippets remain subject to their own license terms. The repository's MIT License does not relicense third-party code or override upstream conditions. Before redistribution or commercial deployment, identify exact versions, preserve required notices, and verify compatibility with the intended distribution model.

## Review findings and unresolved items

- A complete third-party license inventory has not been established.
- SPDX headers or per-component license declarations have not been verified for every source file.
- This document does not claim that the whole repository or every module has passed legal review.
- FlashEEPROM and bootloader/read-protection examples need technical review independent of license review.

## Release checklist

- [ ] Identify origin and copyright holder for every imported or copied file.
- [ ] Record exact STM32 HAL, CMSIS-DSP, compiler, and other dependency versions.
- [ ] Preserve upstream copyright and license notices.
- [ ] Confirm that third-party files can be distributed under their applicable terms.
- [ ] Obtain legal review if ownership or license compatibility is uncertain.

## Commercial use

The MIT License permits commercial use of the repository's original code, subject to its terms. No separate commercial license from the repository maintainer is required for code actually covered by this MIT License. Third-party dependencies may have separate conditions and must be reviewed independently.

For licensing inquiries, contact: rahman.h22@gmail.com.
