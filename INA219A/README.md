# INA219 — STM32 I2C driver

This component is a minimal register-level driver for the Texas Instruments INA219 current/power monitor.

## Integration

- Configure and initialize an I2C peripheral in STM32CubeMX.
- Pass the 7-bit I2C address (default `0x40`); the driver shifts it for the STM32 HAL API.
- Confirm the module's shunt resistance and expected current range before selecting the calibration register.
- Check `status()` after a measurement to detect the last I2C transaction failure.

## Current API

- `getVoltage()`: bus voltage in volts, using the INA219 bus-voltage register's 4 mV LSB.
- `getCurrentRaw()`: signed raw current-register value, not calibrated amperes or milliamperes.
- `getPowerRaw()`: raw power-register value, not a physical unit conversion.
- `status()`: last HAL I2C status.

The calibration value `4096` is preserved as an example and is **not universally correct**. Physical current/power units require a calibration value derived from the shunt resistance and chosen current LSB. Do not treat the raw values as engineering units.

The implementation uses bounded 100 ms I2C timeouts and returns zero on a read error; therefore a legitimate zero measurement must be distinguished from failure using `status()`. No hardware validation is claimed.
