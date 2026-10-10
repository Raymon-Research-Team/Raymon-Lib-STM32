# PrintfUart

The PrintfUart class provides formatted output and line-oriented input over an STM32 HAL UART handle.

## Dependencies

- PrintfUart.h
- The consuming project's main.h, exposing the STM32 HAL definitions
- A UART handle initialized by the consuming STM32CubeMX/STM32CubeIDE project

## Integration

Add PrintfUart.h to the project and configure the include path. The repository currently has no PrintfUart.cpp; the implementation is in the header.

Example, assuming huart2 is initialized by the project:

```cpp
#include "PrintfUart.h"
extern UART_HandleTypeDef huart2;

void uart_example() {
    PrintfUart uart(&huart2, true);
    uart.printf("Value: %d\r\n", 42);
    int value = 0;
    uart.scanf("%d", value); // waits for a line of input
}
```

## API notes

- Constructor: PrintfUart(UART_HandleTypeDef* huart, bool isFeedBackInput). The boolean controls echo of received characters.
- printf(format, ...): sends formatted output; the current implementation uses a fixed 256-byte local buffer.
- scanf(format, ...): receives characters until CR/LF, then parses the accumulated input.
- getchar(): check the declaration and implementation in the header before relying on this method.

## Limitations

- Input reception is blocking and uses HAL_MAX_DELAY; the caller can wait indefinitely for input.
- HAL return values, receive errors, empty input, input length, and format validation need review before production use.
- Output strings that exceed the internal buffer capacity are not sent by the current implementation.
- If USE_FREERTOS is enabled, verify the FreeRTOS delay integration and required headers.
- This example has not been compiled or tested on hardware as part of this documentation change.