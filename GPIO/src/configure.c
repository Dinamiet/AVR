#include "gpio.h"
#include "structure.h"

#include <assert.h>
#include <stddef.h>

void GPIO_Init(GPIO* gpio)
{
	assert(gpio != NULL);
	(void)gpio;
	// Power on hardware?
}
void GPIO_Deinit(GPIO* gpio)
{
	assert(gpio != NULL);
	(void)gpio;
	// Power off hardware?
}

void GPIO_SetMode(GPIO* gpio, const uint8_t mask, const GPIOMode mode)
{
	assert(gpio != NULL);

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

void GPIO_SetModePin(GPIO* gpio, const uint8_t pinNum, const GPIOMode mode)
{
	assert(gpio != NULL);

	GPIO_SetMode(gpio, 1 << pinNum, mode);
}
