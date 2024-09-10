#include "gpio.h"
#include "structure.h"

#include <assert.h>
#include <stddef.h>

uint8_t GPIO_Read(const GPIO* gpio)
{
	assert(gpio != NULL);

	return gpio->Pin;
}

uint8_t GPIO_ReadPin(const GPIO* gpio, const uint8_t pinNum)
{
	assert(gpio != NULL);

	return (gpio->Pin >> pinNum) & 0x01;
}

void GPIO_Write(GPIO* gpio, const uint8_t value)
{
	assert(gpio != NULL);

	gpio->Port = value;
}

void GPIO_WritePin(GPIO* gpio, const uint8_t pinNum, const uint8_t value)
{
	assert(gpio != NULL);

	if (value)
		gpio->Port |= 1 << pinNum;
	else
		gpio->Port &= ~(1 << pinNum);
}

void GPIO_Toggle(GPIO* gpio, const uint8_t mask)
{
	assert(gpio != NULL);

	gpio->Port ^= mask;
}

void GPIO_TogglePin(GPIO* gpio, const uint8_t pinNum)
{
	assert(gpio != NULL);

	gpio->Port ^= (1 << pinNum);
}
