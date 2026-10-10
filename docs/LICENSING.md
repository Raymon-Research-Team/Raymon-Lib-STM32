# Licensing and Third-Party Dependencies

[English](LICENSING.md) | [فارسی](LICENSING_FA.md)

## Repository license

The top-level LICENSE declares Business Source License 1.1 (BUSL-1.1):

- Licensor: Raymon Research Team
- Licensed Work: Raymon-Lib-STM32
- Additional Use Grant: None
- Change Date: 2030-10-10
- Change License: GNU GPL version 2.0 or later

BUSL-1.1 is not an Open Source license. With no Additional Use Grant, the stated grant allows copying, modification, redistribution, and non-production use, subject to the license terms; it does not grant production use before the change takes effect. A commercial license or another permission from the licensor may be needed for production use before then.

**Important timing detail:** for each version, the change to the Change License takes effect on the earlier of the stated Change Date or the fourth anniversary of that version's first publicly available distribution under BUSL-1.1. Therefore, 2030-10-10 is not necessarily the transition date for every version. Read the complete top-level LICENSE before use.

## Dependency boundaries

The repository contains code that integrates with or documents external components, including STM32 HAL / STM32Cube firmware packages, ARM CMSIS-DSP in the FFT example, and STM32CubeIDE / STM32CubeMX tooling in consuming projects.

A complete, verified Software Bill of Materials (SBOM) and per-file provenance record were not established during this pass. The list above identifies apparent integration dependencies; it does not mean all dependency source code is vendored into this repository.

External libraries, generated files, and third-party snippets remain subject to their upstream license terms. The repository's BUSL file does not replace or relicense third-party code. Before redistribution or commercial deployment, identify exact versions, preserve required notices, and verify compatibility with the intended distribution model.

## Review findings and unresolved items

- A complete third-party license inventory has not been established.
- SPDX headers or per-component license declarations have not been verified for every source file.
- This document does not claim that the whole repository or every module has passed legal review.
- FlashEEPROM and bootloader/read-protection examples need technical review independent of license review.

## Recommended release checklist

- [ ] Identify origin and copyright holder for every imported or copied file.
- [ ] Record exact STM32 HAL, CMSIS-DSP, compiler, and other dependency versions.
- [ ] Preserve upstream copyright and license notices.
- [ ] Confirm that each file can be distributed under the repository's declared terms.
- [ ] Obtain legal review for commercial distribution if ownership or compatibility is uncertain.
