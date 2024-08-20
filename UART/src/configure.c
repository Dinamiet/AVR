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

	*uart->PWR &= ~(1 << PRUSART0); /// Power on HW
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

void UART_SetBaud(UART* uart, uint32_t baud)
{
	// Best BAUD setting
	uint16_t UBRR_slow = (F_CPU / (baud * 16)) - 1;
	uint16_t UBRR_fast = (F_CPU / (baud * 8)) - 1;

	uint32_t baud_slow = F_CPU / (16 * (UBRR_slow + 1));
	uint32_t baud_fast = F_CPU / (8 * (UBRR_fast + 1));

	// Fast or slow lowest error rate
	uint8_t useFast = abs(baud - baud_fast) < abs(baud - baud_slow);

	uart->Registers->BaudRate = useFast ? UBRR_fast : UBRR_slow;
	uart->Registers->StatusA  = useFast << U2X0; // Enable/Disable fast mode
}

void UART_SetFormat(UART* uart, size_t dataBits, UARTParity parity, size_t stopBits)
{
	(void)dataBits;
	(void)stopBits;
	(void)parity;

	uart->Registers->StatusC = 0x06; /** TODO: Check datasheet for actual calculations */
}
