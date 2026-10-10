# STM32F1 Read-Out Protection (RDP) Example

The file lock_program_in_code.c demonstrates configuring STM32F1 Flash read-out protection (RDP) to Level 1 through STM32 HAL option-byte APIs.

## ⚠️ Destructive security behavior

**Do not run this code unless you deliberately intend to enable read-out protection and understand the recovery consequences.** On STM32F1, lowering RDP from Level 1 back to Level 0 typically triggers a mass erase of user flash. Firmware and stored data may be irrecoverably lost. Confirm the behavior for the exact part in its reference manual.

## Scope and requirements

- The source includes stm32f1xx_hal.h and is specific to STM32F1 HAL.
- It expects SystemClock_Config() to be supplied by the application.
- The example defines main(); do not add it unchanged to a project that already has its own entry point.
- Option-byte programming and launch behavior depend on MCU and HAL version.

## Review before integration

1. Confirm the exact MCU and RDP semantics in its reference manual and datasheet.
2. Back up source code and important data; ensure a known recovery/programming path is available.
3. Check all HAL return values and handle failures explicitly before adopting this code.
4. Keep option-byte changes out of normal application startup unless this is an intentional, reviewed security policy.
5. Verify debug/programming access on a sacrificial device first.

## Limitations

This is a minimal example, not a complete production security mechanism. It does not check all HAL return values, authenticate firmware, encrypt stored secrets, or provide a rollback path. RDP is not a substitute for secure boot, signed updates, key management, or physical threat analysis.

No compile or hardware validation is claimed. Never assume this setting is reversible without data loss.