#include "structure.h"
#include "uart.h"

#include <avr/io.h>

static UART uart0 = {.Registers = (UARTMemory*)&UCSR0A, .PWR = &PRR};

UART* UART_GetInstance(UARTInstance instance)
{
	switch (instance)
	{
		case UART0:
			return &uart0;

		default:
			return NULL;
	}
}
