#include "structure.h"
#include "uart.h"

#include <avr/interrupt.h>

// TX Ready interrupt handler
ISR(USART_UDRE_vect)
{
	UART*    uart = UART_GetInstance(UART0);
	uint8_t* data = FifoBuffer_Remove(&uart->TxBuffer);
	if (data)
		uart->Registers->Data = *data;
	else
		uart->Registers->StatusB &= ~(1 << UDRE0); // Disable TX interrupt
}

// RX Ready interrupt handler
ISR(USART_RX_vect)
{
	UART*    uart = UART_GetInstance(UART0);
	uint8_t* data = FifoBuffer_Add(&uart->RxBuffer);
	uint8_t  tmp  = uart->Registers->Data; // Force read
	if (data)
		*data = tmp;
}
