# cUart — C++ Streams over STM32 HAL UART

`cUart.hpp` exposes UART-backed `std::ostream` and `std::istream` objects. It requires C++ standard-library stream support and an initialized STM32 HAL UART handle.

## Example

```cpp
#include "cUart.hpp"
cUart console(&huart1);
console.cout << "Hello " << 42 << '\\n';
int value = 0;
console.cin >> value; // blocking until input arrives
```

The wrapper does not replace global `std::cout` or `std::cin`.

## Current behavior and limitations

- The receive adapter now stores its byte in persistent object storage; it does not point the stream buffer at a local stack variable.
- Single-character output explicitly converts the stream character to a byte. HAL failures are reported to the stream as EOF/failure.
- Input and output use `HAL_MAX_DELAY` and are blocking. Do not call from an ISR or a timing-critical task.
- No line discipline, CR/LF normalization, DMA, asynchronous buffering, or thread-safe arbitration is implemented.
- Ensure no other task or ISR uses the same UART concurrently without synchronization.
- This header has not been compiled against every STM32 HAL/toolchain combination or hardware-tested in this repository.
