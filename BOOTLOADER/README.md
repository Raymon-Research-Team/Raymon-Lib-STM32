# STM32 Bootloader Jump Example

`jump_to_bootloader.c` contains a direct branch to a bootloader entry point. It is an MCU-specific example, not a portable STM32 bootloader API.

## Before use

**Do not run this code until the bootloader address is confirmed for the exact MCU and boot configuration.** The current source hard-codes `BOOTLOADER_ADDRESS = 0x1FFFF000`. That address is not valid for every STM32 family or part number.

The implementation assumes that the first two 32-bit words at the address contain:

1. The initial Main Stack Pointer (MSP).
2. The reset-handler address.

Both values must be valid for the selected device. The code also disables SysTick and clears a fixed number of NVIC interrupt banks, then resets selected RCC enable registers. Those assumptions must be reviewed against the MCU reference manual and the boot ROM requirements.

## Integration checklist

1. Identify the exact STM32 part number and consult its reference manual and system-memory boot documentation.
2. Confirm the bootloader base address, vector table format, and valid SRAM range.
3. Ensure the boot mode/pins and option bytes permit entry into the intended bootloader.
4. Stop peripherals and DMA, disable interrupts, and restore clock/peripheral state as required by that MCU.
5. Validate the stack pointer and reset-handler address before branching.
6. Test recovery and programming on a non-critical board before deploying.

## API and dependencies

The function is declared as `void jump_to_bootloader(void)`. The source expects CMSIS device definitions such as `SysTick`, `NVIC`, `RCC`, and `__set_MSP`; include the correct device header and integrate it into an STM32Cube project.

## Limitations

- No runtime validation of the address, stack pointer, or reset vector is implemented.
- The number of NVIC banks and RCC register layout are device-specific.
- The routine does not select a boot mode; it only attempts to branch to the configured address.
- It has not been verified across STM32 families or hardware in this repository.

Treat this example as a starting point, not production-ready code.