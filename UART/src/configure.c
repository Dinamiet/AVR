#include "structure.h"
#include "uart.h"

#include <avr/io.h>
#include <stdlib.h>

void UART_Init(UART* uart, void* rxBuff, size_t rxSize, void* txBuff, size_t txSize)
{
	FifoBuffer_Init(&uart->RxBuffer, rxBuff, sizeof(uint8_t), rxSize);
	FifoBuffer_Init(&uart->TxBuffer, txBuff, sizeof(uint8_t), txSize);
}

void UART_Enable(UART* uart)
{
	uint8_t registerSetup = 1 << RXEN0; // Receiver
	registerSetup |= 1 << TXEN0;        // Transmitter
	registerSetup |= 1 << RXC0;         // RX interrupt

	uart->Registers->StatusB = registerSetup;

	*uart->PWR &= (uint8_t)(~(1 << PRUSART0)); /// Power on HW
}

void UART_Disable(UART* uart)
{
	uint8_t registerSetup = 1 << RXEN0; // Receiver
	registerSetup |= 1 << TXEN0;        // Transmitter
	registerSetup |= 1 << RXC0;         // RX Interrupt
	registerSetup |= 1 << UDRE0;        // TX Interrupt
	uart->Registers->StatusB &= ~registerSetup;

	*uart->PWR |= (1 << PRUSART0); // Power off HW
}

void UART_SetBaud(UART* uart, int baud)
{
	// Best BAUD setting
	int UBRR_slow = ((int)F_CPU / (baud * 16)) - 1;
	int UBRR_fast = ((int)F_CPU / (baud * 8)) - 1;

	int baud_slow = (int)F_CPU / (16 * (UBRR_slow + 1));
	int baud_fast = (int)F_CPU / (8 * (UBRR_fast + 1));

	// Fast or slow lowest error rate
	uint8_t useFast = abs(baud - baud_fast) < abs(baud - baud_slow);

	uart->Registers->BaudRate = useFast ? (uint16_t)UBRR_fast : (uint16_t)UBRR_slow;
	uart->Registers->StatusA  = useFast << U2X0; // Enable/Disable fast mode
}

void UART_SetFormat(UART* uart, size_t dataBits, size_t stopBits, UARTParity parity)
{
	(void)dataBits;
	(void)stopBits;
	(void)parity;

	uart->Registers->StatusC = 0x06; /** TODO: Check datasheet for actual calculations */
}
