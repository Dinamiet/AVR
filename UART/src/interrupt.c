#include "structure.h"
#include "uart.h"

#include <avr/interrupt.h>

// TX Ready interrupt handler
ISR(USART_UDRE_vect)
{
	UART*    uart = UART_GetInstance(UART0);
	uint8_t* data = FifoBuffer_Remove(&uart->TxBuffer);
#ifdef UART_ENABLE_9BIT
	uint8_t* data9 = FifoBuffer_Remove(&uart->TxBuffer);
	if (data9)
		uart->Registers->StatusB |= (*data9 & 0x01) << TXB80;
#endif
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
#ifdef UART_ENABLE_9BIT
	uint8_t* data9 = FifoBuffer_Add(&uart->RxBuffer);
	if (data9)
		*data9 = (uart->Registers->StatusB >> RXB80) & 0x01;
#endif
	uint8_t  tmp  = uart->Registers->Data; // Force read
	if (data)
		*data = tmp;
}
