#include "structure.h"
#include "uart.h"

#include <assert.h>
#include <avr/interrupt.h>
#include <avr/io.h>

size_t UART_Write(UART* uart, const void* _data, const size_t size)
{
	assert(uart != NULL);
	assert(_data != NULL);

	const uint8_t* data    = _data;
	size_t         written = 0;

	uart->Registers->StatusB &= ~(1 << UDRIE0); // Disable TX interrupt

	uint8_t* bufferElement = FifoBuffer_Add(&uart->TxBuffer);
	while (bufferElement)
	{
		*bufferElement = data[written++];
		if (written >= size)
			break;
		bufferElement = FifoBuffer_Add(&uart->TxBuffer);
	}

	uart->Registers->StatusB |= 1 << UDRIE0; // Enable TX interrupt

	return written;
}

bool UART_IsBusy(const UART* uart)
{
	assert(uart != NULL);

	return (uart->Registers->StatusB & (1 << UDRIE0)); // Active if TX interrupt is enabled
}

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
		uart->Registers->StatusB &= ~(1 << UDRIE0); // Disable TX interrupt
}
