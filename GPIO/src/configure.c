#include "gpio.h"
#include "structure.h"

void GPIO_Init(GPIO* gpio)
{
	(void)gpio;
	// Power on hardware?
}
void GPIO_Deinit(GPIO* gpio)
{
	(void)gpio;
	// Power off hardware?
}

void GPIO_SetMode(GPIO* gpio, uint8_t mask, GPIOMode mode)
{
	switch (mode)
	{
		case GPIO_MODE_INPUT_PULLUP:
			gpio->Port |= mask;
			gpio->DDR &= ~mask;
			break;

		case GPIO_MODE_INPUT:
			gpio->Port &= ~mask;
			gpio->DDR &= ~mask;
			break;

		case GPIO_MODE_OUTPUT:
			gpio->DDR |= mask;
			break;

			// No default to ensure all cases are captured
	}
}

void GPIO_SetModePin(GPIO* gpio, uint8_t pinNum, GPIOMode mode) { GPIO_SetMode(gpio, 1 << pinNum, mode); }
