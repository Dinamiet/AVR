#include "structure.h"
#include "uart.h"

void UART_Init(UART* uart, void* rxBuff, size_t rxSize, void* txBuff, size_t txSize) {}

void UART_Enable(UART* uart) {}

void UART_Disable(UART* uart) {}

void UART_SetBaud(UART* uart, int baud) {}

void UART_SetFormat(UART* uart, size_t dataBits, size_t stopBits, UARTParity parity) {}
