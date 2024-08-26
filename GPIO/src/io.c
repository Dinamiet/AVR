#include "gpio.h"
#include "structure.h"

#include <assert.h>
#include <stddef.h>

uint8_t GPIO_Read(GPIO* gpio)
{
	assert(gpio != NULL);

	return gpio->Pin;
}

uint8_t GPIO_ReadPin(GPIO* gpio, uint8_t pinNum)
{
	assert(gpio != NULL);

	return (gpio->Pin >> pinNum) & 0x01;
}

void GPIO_Write(GPIO* gpio, uint8_t value)
{
	assert(gpio != NULL);

	gpio->Pin = value;
}

void GPIO_WritePin(GPIO* gpio, uint8_t pinNum, uint8_t value)
{
	assert(gpio != NULL);

	if (value)
		gpio->Pin |= 1 << pinNum;
	else
		gpio->Pin &= ~(1 << pinNum);
}

void GPIO_Toggle(GPIO* gpio, uint8_t mask)
{
	assert(gpio != NULL);

	gpio->Pin ^= mask;
}

void GPIO_TogglePin(GPIO* gpio, uint8_t pinNum)
{
	assert(gpio != NULL);

	gpio->Pin ^= (1 << pinNum);
}
