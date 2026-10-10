# ILI9488 SPI TFT Display Drivers

This directory contains multiple independent driver variants for ILI9488-based SPI displays. They are alternatives, not a single unified library. Inspect the source and select **one implementation** for your project; do not compile both variants together.

## Files and variants

- `ili9488.h` + `ili9488.c`: C-style driver with global functions such as `ILI9488_Init`, `ILI9488_SetWindow`, `ILI9488_WritePixel`, `LCD_Clear`, and bitmap drawing. It is wired to project-defined GPIO macros and the global `hspi2` handle in the current source.
- `ili9488_2.h` + `ili9488_2.c`: another C-style graphics driver with drawing primitives, text rendering, rotation, scrolling, and color conversion. The header currently includes STM32F4 HAL directly and expects project-specific `TFT_*` GPIO names and global SPI handles.
- The C++ `ILI9488` class is present in `ili9488.h` after the C declarations. It is a separate API variant and expects CMSIS-RTOS functions. Review this section carefully before enabling it.

## Hardware and project setup

1. Confirm the display controller is ILI9488 and identify the panel resolution, SPI mode/clock limits, and interface wiring.
2. Wire SCK, MOSI, CS, D/C (RS), RESET, and backlight as required by the selected implementation. MISO is only needed if the driver reads from the panel.
3. Configure SPI and GPIO outputs in STM32CubeMX; map the exact pin names used in the selected source.
4. Add the required HAL/CMSIS-RTOS headers and source files to the project.
5. Start with a conservative SPI clock and validate reset, initialization, orientation, and a solid-color fill before drawing images.
6. Confirm pixel byte order, RGB/BGR order, and the panel's 16-bit input versus 18-bit RGB transfer expectations.

## API overview

The first C driver exposes initialization, display reset/clear, address-window setup, pixel writing and bitmap transfer. The second driver exposes initialization, address windows, pixel/line/rectangle drawing, text rendering, rotation, inversion, scrolling, and RGB565 conversion. Exact signatures are defined in the respective headers.

## Important source-level caveats

- Both variants use overlapping names and the header name `ili9488.h`; including or linking them together can cause conflicting declarations or duplicate symbols.
- The source files expect different project-specific pin macros and SPI handle names. Adapt these mappings deliberately; do not assume the files compile unchanged.
- One variant uses DMA and another has blocking SPI paths. DMA transfer completion, buffer lifetime, and chip-select timing must be handled according to the selected implementation.
- Review buffer sizes and image dimensions before use. Full-screen RGB data can consume substantial RAM; the code is not a universal memory-safe framebuffer.
- The current source includes legacy/inconsistent code paths. Compile and validate the chosen variant in the exact target project before use.

No repository-wide build or physical display test is claimed. Use the panel datasheet and STM32 reference manual for final configuration.