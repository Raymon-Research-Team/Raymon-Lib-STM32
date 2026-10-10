# Delay — DWT-based busy-wait delays

`Delay.hpp` implements microsecond/millisecond busy-wait delays using the Cortex-M DWT cycle counter and `HAL_RCC_GetHCLKFreq()`.

## Requirements

- A Cortex-M target whose core implements `DWT->CYCCNT`.
- CMSIS device headers and initialized HAL clock configuration.
- A running HCLK frequency.

## Example

```cpp
#include "Delay.hpp"
Delay::delay_us(10);
Delay::delay_ms(2);
```

## Limitations

- This is a CPU busy-wait; it does not yield to an RTOS and must not be used for long waits in a real-time task.
- Some Cortex-M variants or security/debug configurations may not provide a usable cycle counter. Verify support for the exact MCU.
- Interrupts can lengthen the observed delay.
- If HCLK is below 1 MHz, the current implementation returns without waiting rather than entering a zero-cycle infinite loop.
- No timing-accuracy guarantee or hardware test is claimed for all clock configurations.
