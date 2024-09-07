#include "structure.h"
#include "uart.h"

#include <avr/interrupt.h>

static UART uart0 = {.Registers = (UARTMemory*)&UCSR0A, .PWR = &PRR};

UART* UART_GetInstance(const UARTInstance instance)
{
	switch (instance)
	{
		case UART0:
			return &uart0;

		default:
			return NULL;
	}
}

// RX Ready interrupt handler
ISR(USART_RX_vect)
{
	UART* uart = UART_GetInstance(UART0);
#ifdef UART_ENABLE_9BIT
	uint16_t data;
	data = ((uart->Registers->StatusB >> RXB80) & 0x01) << 8;
	data |= uart->Registers->Data;
#else
	uint8_t data = uart->Registers->Data;
#endif
	FifoBuffer_Add(&uart->RxBuffer, &data, sizeof(data));
}

// TX Ready interrupt handler
ISR(USART_UDRE_vect)
{
	UART* uart = UART_GetInstance(UART0);
#ifdef UART_ENABLE_9BIT
	uint16_t data;
	if (FifoBuffer_Remove(&uart->TxBuffer, &data, sizeof(data)) == sizeof(data))
	{
		uart->Registers->StatusB |= ((data >> 8) & 0x01) << TXB80;
		uart->Registers->Data = data & 0xFF;
	}
#else
	uint8_t data;
	if (FifoBuffer_Remove(&uart->TxBuffer, &data, sizeof(data)) == sizeof(data))
		uart->Registers->Data = data & 0xFF;
#endif
	else
		uart->Registers->StatusB &= ~(1 << UDRIE0); // Disable TX interrupt
}
