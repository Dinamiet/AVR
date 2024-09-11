#include "gpio.h"
#include "structure.h"

#include <assert.h>
#include <avr/interrupt.h>
#include <stdbool.h>
#include <stddef.h>

static GPIO_InterruptFunction isr0 = NULL;
static GPIO_InterruptFunction isr1 = NULL;

void GPIO_EnableIRQ(const GPIOInstance instance, const GPIOInterruptTrigger trigger, const GPIO_InterruptFunction isr)
{
	assert(trigger <= 0x03);
	assert(isr != NULL);

	uint8_t value = EICRA;
	switch (instance)
	{
		case GPIO_ISR0:
			isr0 = isr;
			value &= ~((1 << ISC01) | (1 << ISC00));
			value |= trigger;
			EICRA = value;
			EIMSK |= 1 << INT0;
			break;

		case GPIO_ISR1:
			isr1 = isr;
			value &= ~((1 << ISC11) | (1 << ISC10));
			value |= trigger << 2;
			EICRA = value;
			EIMSK |= 1 << INT1;
			break;

		default: // Only ISR0 and ISR1 should be handled here
			assert(false);
			break;
	}
}
void GPIO_DisableIRQ(const GPIOInstance instance)
{
	switch (instance)
	{
		case GPIO_ISR0:
			EIMSK &= ~(1 << INT0);
			isr0 = NULL;
			break;

		case GPIO_ISR1:
			EIMSK &= ~(1 << INT1);
			isr1 = NULL;
			break;

		default: // Only ISR0 and ISR1 should handled here
			assert(false);
			break;
	}
}

ISR(INT0_vect)
{
	if (isr0)
		isr0();
}

ISR(INT1_vect)
{
	if (isr1)
		isr1();
}
