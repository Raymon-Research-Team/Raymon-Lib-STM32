# PrintfUart — STM32 HAL UART formatted I/O

`PrintfUart.hpp` contains the implementation. `PrintfUart.h` is a compatibility wrapper that includes the canonical header so both include paths do not define duplicate classes.

## Requirements

- STM32 HAL UART and the consuming project's `main.h`.
- An initialized `UART_HandleTypeDef* `.
- C++ standard-library support for `std::string` and type traits.

## Example

```cpp
#include "PrintfUart.h"
extern UART_HandleTypeDef huart2;

void uart_example() {
    PrintfUart uart(&huart2, true); // echo received characters
    uart.printf("Value: %d\\r\\n", 42);
    int value = 0;
    uart.scanf("%d", value); // blocks until CR/LF
}
```

## Current API and limitations

- Constructor: `PrintfUart(UART_HandleTypeDef* huart, bool isFeedBackInput = false)`.
- `printf(format, ...)` uses a fixed 256-byte buffer; overlong output is discarded. Use format specifiers that match the exact argument types.
- `scanf(format, ...)` blocks on UART input using `HAL_MAX_DELAY`. It uses simple whitespace splitting and `sscanf`; it is not a full scanf-compatible parser.
- Input accumulation uses dynamically growing `std::string` without a maximum line length. Untrusted or unbounded input can exhaust heap memory.
- HAL receive/transmit status is not consistently propagated to callers. Avoid use in ISRs and timing-critical tasks.
- The class uses the same UART for input and output; synchronize access if multiple tasks use it.
- With `USE_FREERTOS`, verify that the appropriate FreeRTOS headers and tick configuration are present.

This is a convenience utility, not a hardened command-line parser. It has not been compiled across all STM32 toolchains or hardware-tested in this repository.
