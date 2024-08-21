#include "structure.h"
#include "uart.h"

#include <avr/io.h>
#include <stdlib.h>

void UART_Init(UART* uart)
{
	uint8_t registerSetup = 1 << RXEN0; // Receiver
	registerSetup |= 1 << TXEN0;        // Transmitter
	registerSetup |= 1 << RXC0;         // RX interrupt

	uart->Registers->StatusB = registerSetup;

	*uart->PWR &= ~(1 << PRUSART0); /// Power on HW
}

void UART_Deinit(UART* uart)
{
	uint8_t registerSetup = 1 << RXEN0; // Receiver
	registerSetup |= 1 << TXEN0;        // Transmitter
	registerSetup |= 1 << RXC0;         // RX Interrupt
	registerSetup |= 1 << UDRE0;        // TX Interrupt
	uart->Registers->StatusB &= ~registerSetup;

	*uart->PWR |= (1 << PRUSART0); // Power off HW
}

void UART_AssignRxBuffer(UART* uart, uint8_t* buff, const size_t size) { FifoBuffer_Init(&uart->RxBuffer, buff, sizeof(*buff), size); }

void UART_AssignTxBuffer(UART* uart, uint8_t* buff, const size_t size) { FifoBuffer_Init(&uart->TxBuffer, buff, sizeof(*buff), size); }

void UART_SetBaud(const UART* uart, const uint32_t baud)
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

void UART_SetFormat(const UART* uart, const size_t dataBits, const UARTParity parity, const size_t stopBits)
{
	(void)dataBits; /** TODO: Check data bits within range */
	(void)stopBits; /** TODO: Check stop bits within range */
	(void)parity;   /** TODO: Check parity */

	uint8_t registerVal = 0;
	registerVal |= (dataBits - 5) << UCSZ00;
	registerVal |= (stopBits - 1) << USBS0;
	registerVal |= parity << UPM00;

	uart->Registers->StatusC = registerVal;
}
