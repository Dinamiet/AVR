#include "structure.h"
#include "uart.h"

#include <avr/io.h>

static UART instances[] = {
		{.Registers = (UARTMemory*)&UCSR0A, .PWR = &PRR},
};

UART* UART_GetInstance(UARTInstance instance) { return &instances[instance]; }
