#ifndef _GPIO_H_
#define _GPIO_H_

typedef enum _GPIOMode_
{
	GPIO_MODE_INPUT,
	GPIO_MODE_OUTPUT,
	GPIO_MODE_PULLUP,
} GPIOMode;

typedef enum _GPIOInstance_
{
	GPIO0,
	GPIO1,
	GPIO2,
} GPIOInstance;

typedef struct _GPIO_ GPIO;

//
GPIO* GPIO_GetInstance(GPIOInstance instance);

// Init
void GPIO_Init(GPIO* gpio);
void GPIO_Deinit(GPIO* gpio);

// Setup
void GPIO_SetMode(GPIO* gpio); // Input, output, I2C, UART, output enable/high impedence, Pullup, pulldown, None
void GPIO_Mode(GPIO* gpio);    // Get GPIO mode

// Read/Write
void GPIO_Read(); // Single and whole bank functions?
void GPIO_Write();
void GPIO_Toggle();

// IRQ
void GPIO_SetIRQ(); // Set IRQ, irq mode, irq enabled
void GPIO_IRQ();    // Get IRQ status

#endif
