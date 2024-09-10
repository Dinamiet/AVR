#include "gpio.h"
#include "structure.h"

#include <avr/io.h>
#include <stddef.h>

static GPIO* GPIOB = (GPIO*)&PINB;
static GPIO* GPIOC = (GPIO*)&PINC;
static GPIO* GPIOD = (GPIO*)&PIND;

GPIO* GPIO_GetInstance(const GPIOInstance instance)
{
	switch (instance)
	{
		case GPIO_B:
			return GPIOB;

		case GPIO_C:
			return GPIOC;

		case GPIO_D:
			return GPIOD;

		default:
			return NULL;
	}
}
