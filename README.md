# AVR Hardware libraries

## ATMega328P
Hardware support for ATMega328P microcontroller

## UART
Non-blocking UART communication

9-bit mode can be enabled with CMake option `UART_ENABLE_9BIT`. This will add a UART definition to enable 9bit UART mode.
In this mode, each read and write will consume 2 bytes (`uint16_t`), irrespective of the databit setting.

## GPIO
GPIO driver with support for the external interrupt interface
