#ifndef _GPIO_H_
#define _GPIO_H_

/**
 * \file
 * GPIO Driver
 */

#include <stdint.h>

/**
 * GPIO mode options
 */
typedef enum _GPIOMode_
{
	GPIO_MODE_INPUT,        /** Input */
	GPIO_MODE_OUTPUT,       /** Output */
	GPIO_MODE_INPUT_PULLUP, /** Input with pullup enabled */
} GPIOMode;

/**
 * Available GPIO instances
 */
typedef enum _GPIOInstance_
{
	GPIO_B,    /** GPIO Bank B */
	GPIO_C,    /** GPIO Bank C */
	GPIO_D,    /** GPIO Band D */
	GPIO_ISR0, /** External Interrupt 0 */
	GPIO_ISR1, /** External Interrupt 1 */
} GPIOInstance;

/** External Interrupt trigger modes */
typedef enum _GPIOInterruptTrigger_
{
	GPIO_INTERRUPT_TRIGGER_LOW,     /** Trigger on Low level */
	GPIO_INTERRUPT_TRIGGER_CHANGE,  /** Trigger on Changing level */
	GPIO_INTERRUPT_TRIGGER_FALLING, /** Trigger on changing from High to Low */
	GPIO_INTERRUPT_TRIGGER_RISING,  /** Trigger on changing from Low to High */
} GPIOInterruptTrigger;

/** External Interrupt callback function */
typedef void (*GPIO_InterruptFunction)();

/**
 * GPIO
 */
typedef struct _GPIO_ GPIO;

/**
 * Retrieves the provided GPIO instance
 * \param instance The GPIO instance to retrieve
 * \return Reference to GPIO instance
 */
GPIO* GPIO_GetInstance(GPIOInstance instance);

/**
 * Initialize and power-on GPIO
 * \param gpio GPIO to initialize
 */
void GPIO_Init(GPIO* gpio);

/**
 * Deinitializes and power-off GPIO
 * \param gpio GPIO to deinitialize
 */
void GPIO_Deinit(GPIO* gpio);

/**
 * Sets the mode of the GPIO bank
 * \param gpio GPIO bank to change
 * \param mask Individual IO channels to change, 0 - Unaffected by mode, 1 - Affected by mode
 * \param mode The mode to which IO channels should be set
 */
void GPIO_SetMode(GPIO* gpio, uint8_t mask, GPIOMode mode);

/**
 * Sets the mode of an individual GPIO channel
 * \param gpio GPIO bank in which the channel resides
 * \param pinNum Channel (bit) within the GPIO bank to change
 * \param mode The mode to which the channel should be set
 */
void GPIO_SetModePin(GPIO* gpio, uint8_t pinNum, GPIOMode mode);

/**
 * Reads the current status of the entire GPIO bank
 * \param gpio GPIO bank to read
 * \return Value of the entire GPIO bank
 */
uint8_t GPIO_Read(GPIO* gpio);

/**
 * Read the current status of a single GPIO channel
 * \param gpio GPIO bank in which channel resides
 * \param pinNum Channel (bit) within the channel to read
 * \return Value of the selected channel
 */
uint8_t GPIO_ReadPin(GPIO* gpio, uint8_t pinNum);

/**
 * Write to a GPIO bank
 * \param gpio GPIO bank to write
 * \param value The value to write
 */
void GPIO_Write(GPIO* gpio, uint8_t value);

/**
 * Write a individual GPIO channel
 * \param gpio GPIO bank in which channel resides
 * \param pinNum Channel (bit) within bank to write
 * \param value The value to write to the channel
 */
void GPIO_WritePin(GPIO* gpio, uint8_t pinNum, uint8_t value);

/**
 * Changes the current state of the GPIO bank from a mask
 * \param gpio GPIO bank to toggle
 * \param mask Bit mask for all channels within the bank to change
 */
void GPIO_Toggle(GPIO* gpio, uint8_t mask);

/**
 * Changes the current state of a single GPIO channel
 * \param gpio GPIO bank in which channel resides
 * \param pinNUm Channel (bit) within bank to toggle
 */
void GPIO_TogglePin(GPIO* gpio, uint8_t pinNum);

/**
 * Enable external interrupt
 * \param instance External interrupt instance to enable
 * \param trigger Trigger mode to assign to the interrupt
 * \param isr Interrupt service routine callback function
 */
void GPIO_EnableIRQ(GPIOInstance instance, GPIOInterruptTrigger trigger, GPIO_InterruptFunction isr);

/**
 * Disable external interrupt
 * \param instance External interrupt instance to disable
 */
void GPIO_DisableIRQ(GPIOInstance instance);

#endif
