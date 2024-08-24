#ifndef _GPIO_H_
#define _GPIO_H_

#include <stdint.h>

typedef enum _GPIOMode_
{
	GPIO_MODE_INPUT,
	GPIO_MODE_OUTPUT,
	GPIO_MODE_INPUT_PULLUP,
} GPIOMode;

typedef enum _GPIOInstance_
{
	GPIO_B,
	GPIO_C,
	GPIO_D,
	GPIO_ISR0,
	GPIO_ISR1,
} GPIOInstance;

typedef enum _GPIOInterruptTrigger_
{
	GPIO_INTERRUPT_TRIGGER_LOW,
	GPIO_INTERRUPT_TRIGGER_CHANGE,
	GPIO_INTERRUPT_TRIGGER_FALLING,
	GPIO_INTERRUPT_TRIGGER_RISING,
} GPIOInterruptTrigger;

typedef void (*GPIO_InterruptFunction)();

typedef struct _GPIO_ GPIO;

GPIO* GPIO_GetInstance(GPIOInstance instance);

void GPIO_Init(GPIO* gpio);
void GPIO_Deinit(GPIO* gpio);

void GPIO_SetMode(GPIO* gpio, uint8_t mask, GPIOMode mode);
void GPIO_SetModePin(GPIO* gpio, uint8_t pinNum, GPIOMode mode);

uint8_t GPIO_Read(GPIO* gpio);
uint8_t GPIO_ReadPin(GPIO* gpio, uint8_t pinNum);
void    GPIO_Write(GPIO* gpio, uint8_t value);
void    GPIO_WritePin(GPIO* gpio, uint8_t pinNum, uint8_t value);
void    GPIO_Toggle(GPIO* gpio, uint8_t mask);
void    GPIO_TogglePin(GPIO* gpio, uint8_t pinNum);

void GPIO_EnableIRQ(GPIOInstance instance, GPIOInterruptTrigger trigger, GPIO_InterruptFunction isr);
void GPIO_DisableIRQ(GPIOInstance instance);

#endif
