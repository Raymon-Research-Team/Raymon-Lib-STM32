#pragma once
#include "main.h"
#include <cstdint>
#include <limits>

class Delay {
public:
    // Busy-wait delay; requires a Cortex-M core with DWT CYCCNT support.
    // Not suitable for long delays or use from contexts that must yield.
    static void delay_us(uint32_t us) {
        if (us == 0U) return;

        CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
        DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;

        const uint32_t clockFreq = HAL_RCC_GetHCLKFreq();
        if (clockFreq < 1000000U) return; // avoid a zero cycles/us infinite wait
        const uint32_t cyclesPerUs = clockFreq / 1000000U;
        const uint32_t start = DWT->CYCCNT;

        // Chunk the requested interval to avoid multiplication overflow and
        // keep each unsigned elapsed-cycle comparison below 2^31.
        uint64_t remaining = static_cast<uint64_t>(us) * cyclesPerUs;
        while (remaining != 0U) {
            const uint32_t chunk = remaining > 0x7FFFFFFFULL
                ? 0x7FFFFFFFU : static_cast<uint32_t>(remaining);
            const uint32_t chunkStart = DWT->CYCCNT;
            while (static_cast<uint32_t>(DWT->CYCCNT - chunkStart) < chunk) {}
            remaining -= chunk;
        }
        (void)start;
    }

    static void delay_ms(uint32_t ms) {
        while (ms != 0U) {
            const uint32_t chunk = ms > 1000U ? 1000U : ms;
            delay_us(chunk * 1000U);
            ms -= chunk;
        }
    }
};