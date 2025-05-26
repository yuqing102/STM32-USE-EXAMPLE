/**
 ******************************************************************************
 * @file    LP5024.h
 * @version 2.0
 * @author  Till Heuer
 * @brief   Headerfile for driver for NXP PCAL6524 IO expander IC.
 * @date 	Nov 22, 2023
 * @verbatim
 * Datasheet: https://www.nxp.com/docs/en/data-sheet/PCAL6524.pdf
 * @endverbatim
 ******************************************************************************
 */

#ifdef __cplusplus
extern "C"
{
#endif

    /** @addtogroup IC_Drivers
     * @{
     */

    /** @addtogroup IO_Expander
     * @{
     */

#ifndef CUSTOM_DRIVERS_INC_PCAL6524_H_
#define CUSTOM_DRIVERS_INC_PCAL6524_H_




#define STM32F1 //Select MCU

// Includes
/* For I2C and GPIO funcionality. */
#if defined(STM32WB)
#include "stm32wbxx_hal.h"
#elif defined(STM32F0)
#include "stm32f0xx_hal.h"
#elif defined(STM32F1)
#include "stm32f1xx_hal.h"
#elif defined(STM32F4)
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_gpio.h"
#elif defined(STM32L0)
#include "stm32l0xx_hal.h"
#elif defined(STM32L1)
#include "stm32l1xx_hal.h"
#elif defined(STM32L4)
#include "stm32l4xx_hal.h"
#elif defined(STM32L5)
#include "stm32l5xx_hal.h"
#elif defined(STM32F3)
#include "stm32f3xx_hal.h"
#elif defined(STM32H7)
#include "stm32h7xx_hal.h"
#elif defined(STM32F7)
#include "stm32f7xx_hal.h"
#elif defined(STM32G0)
#include "stm32g0xx_hal.h"
#elif defined(STM32G4)
#include "stm32g4xx_hal.h"
#else
#error "Enter MCU"
#endif

#include <stdint.h>        // For fixed width types.
#include <stdlib.h>        // For descriptive return values.

#define PCAL6524_I2C_TIMEOUT (100)      ///< Time before I2C timeout [ms].
#define PCAL6524_I2C_MAX_ATTEMPTS (3)   ///< Number of attempts, before error.
#define PCAL6524_I2C_ATTEMPT_DELAY (10) ///< Time between attempts [ms].

// Error codes
#define PCAL6524_SUCCESS (0)         ///< Error code for success.
#define PCAL6524_INPUTOUTOFRANGE (3) ///< Error code for wrong input.

/**
 * @brief Device address of PCAL6524 (7Bit Form).
 * Note that last two bits are configurable.
 * If A0 is connected to one of the following signals, the last two bits are:
 * SCL -> 0b00;
 * SDA -> 0b01;
 * GND -> 0b10;
 * Vdd -> 0b11
 */
#define PCAL6524_ADDRESS (0x20)

// Register addresses
/**
 * @brief Register to read input pins and clearing the interrupt.
 */
#define PCAL6524_REG_IN_PORT_0 (0x00)
#define PCAL6524_REG_IN_PORT_1 (0x01)
#define PCAL6524_REG_IN_PORT_2 (0x02)

/**
 * @brief Register to output data.
 */
#define PCAL6524_REG_OUT_PORT_0 (0x04)
#define PCAL6524_REG_OUT_PORT_1 (0x05)
#define PCAL6524_REG_OUT_PORT_2 (0x06)

/**
 * @brief Register to flip polarity on specific pins.
 */
#define PCAL6524_REG_POL_PORT_0 (0x08)
#define PCAL6524_REG_POL_PORT_1 (0x09)
#define PCAL6524_REG_POL_PORT_2 (0x0A)

/**
 * @brief Register to configure which pins are in- or outputs.
 */
#define PCAL6524_REG_CONF_PORT_0 (0x0C)
#define PCAL6524_REG_CONF_PORT_1 (0x0D)
#define PCAL6524_REG_CONF_PORT_2 (0x0E)

/**
 * @brief Register to enable or disable pull-up/pull-down resistors.
 */
#define PCAL6524_REG_PULL_EN_PORT_0 (0x4C)
#define PCAL6524_REG_PULL_EN_PORT_1 (0x4D)
#define PCAL6524_REG_PULL_EN_PORT_2 (0x4E)

/**
 * @brief Register to switch between pull-up and pull-down resistors.
 */
#define PCAL6524_REG_PULL_SEL_PORT_0 (0x50)
#define PCAL6524_REG_PULL_SEL_PORT_1 (0x51)
#define PCAL6524_REG_PULL_SEL_PORT_2 (0x52)

/**
 * @brief Register to enable or disable interrupts for pins.
 */
#define PCAL6524_REG_INT_MASK_PORT_0 (0x54)
#define PCAL6524_REG_INT_MASK_PORT_1 (0x55)
#define PCAL6524_REG_INT_MASK_PORT_2 (0x56)

/**
 * @brief Register to read out which inputs triggered the interrupt.
 */
#define PCAL6524_REG_INT_STAT_PORT_0 (0x58)
#define PCAL6524_REG_INT_STAT_PORT_1 (0x59)
#define PCAL6524_REG_INT_STAT_PORT_2 (0x5A)

/**
 * @brief Register to configure the trigger of the interrupt.
 */
#define PCAL6524_REG_INT_EGDE_PORT_0A (0x60)
#define PCAL6524_REG_INT_EGDE_PORT_0B (0x61)
#define PCAL6524_REG_INT_EGDE_PORT_1A (0x62)
#define PCAL6524_REG_INT_EGDE_PORT_1B (0x63)
#define PCAL6524_REG_INT_EGDE_PORT_2A (0x64)
#define PCAL6524_REG_INT_EGDE_PORT_2B (0x65)

/**
 * @brief Register to clear the interrupt selectively.
 */
#define PCAL6524_REG_INT_CLEAR_PORT_0 (0x68)
#define PCAL6524_REG_INT_CLEAR_PORT_1 (0x69)
#define PCAL6524_REG_INT_CLEAR_PORT_2 (0x6A)

/**
 * @brief Register to read input pins, without clearing the interrupt.
 */
#define PCAL6524_REG_IN_STATUS_PORT_0 (0x6C)
#define PCAL6524_REG_IN_STATUS_PORT_1 (0x6D)
#define PCAL6524_REG_IN_STATUS_PORT_2 (0x6E)

    /**
     * @brief Enum for last two bits of device address.
     */
    typedef enum
    {
        PCAL6524_A0_SCL,
        PCAL6524_A0_SDA,
        PCAL6524_A0_GND,
        PCAL6524_A0_VDD
    } pcal6524_A0_t;
    /**
     * @brief Enum for pin mode.
     */
    typedef enum
    {
        PCAL6524_Output,
        PCAL6524_Input
    } pcal6524_InOut_t;
    /**
     * @brief Enum for pin interrupt mode.
     */
    typedef enum
    {
        PCAL6524_DisableInterrupt,
        PCAL6524_EnableInterrupt
    } pcal6524_InterruptEN_t;
    /**
     * @brief Enum for pin pullup/-down mode.
     */
    typedef enum
    {
        PCAL6524_PullDown,
        PCAL6524_PullUp
    } pcal6524_PullUpDown_t;
    /**
     * @brief Enum for dis-/enable pullup/-down.
     */
    typedef enum
    {
        PCAL6524_PullUpDownDisconnected,
        PCAL6524_PullUpDownConnected
    } pcal6524_PullUpDownEN_t;
    /**
     * @brief Enum for polarity.
     */
    typedef enum
    {
        PCAL6524_PolarityNormal,
        PCAL6524_PolarityInverted
    } pcal6524_Polarity_t;
    /**
     * @brief Enum for interrupt trigger.
     */
    typedef enum
    {
        PCAL6524_TriggerOnChange,
        PCAL6524_TriggerRisingEdge,
        PCAL6524_TriggerFallingEdge,
        PCAL6524_TriggerAnyEdge
    } pcal6524_InterruptTrigger_t;
    /**
     * @brief Enum for ports.
     */
    typedef enum
    {
        PCAL6524_Port_A,
        PCAL6524_Port_B,
        PCAL6524_Port_C
    } pcal6524_Port_t;
    /**
     * @brief Enum for pins.
     */
    typedef enum
    {
        PCAL6524_Pin_0,
        PCAL6524_Pin_1,
        PCAL6524_Pin_2,
        PCAL6524_Pin_3,
        PCAL6524_Pin_4,
        PCAL6524_Pin_5,
        PCAL6524_Pin_6,
        PCAL6524_Pin_7
    } pcal6524_Pin_t;
    /**
     * @brief Enum for pin state.
     */
    typedef enum
    {
        PCAL6524_LOW,
        PCAL6524_HIGH
    } pcal6524_Value_t;

    /**
     * @brief Struct for I2C handler and address pin status.
     */
    typedef struct
    {
        I2C_HandleTypeDef *hi2c;
        pcal6524_A0_t a0;
    } pcal6524_Device_t;

    /**
     * @brief 				Defines whether a pin is an in- or output.
     *
     * @param   device      Struct with I2C handler and address pin status.
     * @param 	port 		Port of the pin.
     * @param 	pin 		Selected pin.
     * @param 	io 			Set to 1 for input and 0 for output.
     *
     * @retval 	uint8_t		Error code.
     */
    uint8_t PCAL6524_SetInOut(pcal6524_Device_t *device, pcal6524_Port_t port, pcal6524_Pin_t pin, pcal6524_InOut_t io);

    /**
     * @brief 				Gets the current in-, output configuration for selected port.
     *
     * @param   device      Struct with I2C handler and address pin status.
     * @param 	port 		Selected Port.
     * @param 	*ios 		Pointer to output variable.
     *
     * @retval	uint8_t 	Error code.
     */
    uint8_t PCAL6524_GetInOutConfig(pcal6524_Device_t *device, pcal6524_Port_t port, uint8_t *ios);

    /**
     * @brief 				Activates/deactivates the interrupt for selected pin.
     *
     * @param   device      Struct with I2C handler and address pin status.
     * @param 	port 		Port of the pin.
     * @param 	pin 		Selected pin.
     * @param 	intr 		To enable interrupt set to 1, to disable it to 0.
     *
     * @retval 	uint8_t		Error code.
     */
    uint8_t PCAL6524_SetInterrupt(pcal6524_Device_t *device, pcal6524_Port_t port, pcal6524_Pin_t pin, pcal6524_InterruptEN_t intr);

    /**
     * @brief 				Gets current interrupt mask for selected port.
     *
     * @param   device      Struct with I2C handler and address pin status.
     * @param 	port 		Selected port.
     * @param 	*intr 		Pointer to output variable.
     *
     * @retval 	uint8_t		Error code.
     */
    uint8_t PCAL6524_GetInterruptConfig(pcal6524_Device_t *device, pcal6524_Port_t port, uint8_t *intr);

    /**
     * @brief 				Activates/deactivates the pull-up or pull-down resistor for selected pin.
     *
     * @param   device      Struct with I2C handler and address pin status.
     * @param 	port 		Port of the pin.
     * @param 	pin 		Selected pin.
     * @param 	pull 		Set to 1 for pull-up and to 0 for pull-down.
     * @param 	active 		Set to 1 to connect the resistor and to 0 to disconnect it.
     *
     * @retval 	uint8_t		Error code.
     */
    uint8_t PCAL6524_SetPullupDown(pcal6524_Device_t *device, pcal6524_Port_t port, pcal6524_Pin_t pin, pcal6524_PullUpDown_t pull, pcal6524_PullUpDownEN_t active);

    /**
     * @brief 				Gets current pull-up/pull-down configuration for selected port.
     *
     * @param   device      Struct with I2C handler and address pin status.
     * @param 	port 		Selected port.
     * @param 	*pull 		Pointer to output variable for pull-up/pull-down configuration.
     * @param 	*active 	Pointer to output variable for pull-up/pull-down activation status.
     *
     * @retval 	uint8_t		Error code.
     */
    uint8_t PCAL6524_GetPullupDownConfig(pcal6524_Device_t *device, pcal6524_Port_t port, uint8_t *pull, uint8_t *active);

    /**
     * @brief				Activates/deactivates the polarity inversion for selected pin.
     *
     * @param   device      Struct with I2C handler and address pin status.
     * @param 	port 		Port of the pin.
     * @param 	pin 		Selected pin.
     * @param 	pol 		Set to 1 for inverted polarity and to 0 for normal operation.
     *
     * @retval 	uint8_t		Error code.
     */
    uint8_t PCAL6524_SetPolarity(pcal6524_Device_t *device, pcal6524_Port_t port, pcal6524_Pin_t pin, pcal6524_Polarity_t pol);

    /**
     * @brief 				Gets current polarity configuration for selected port.
     *
     * @param   device      Struct with I2C handler and address pin status.
     * @param 	port 		Selected port.
     * @param 	*pol 		Pointer to output variable for polarity configuration.
     *
     * @retval 	uint8_t		Error code.
     */
    uint8_t PCAL6524_GetPolarityConfig(pcal6524_Device_t *device, pcal6524_Port_t port, uint8_t *pol);

    /**
     * @brief 				Sets trigger for interrupt for selected pin.
     *
     * @param   device      Struct with I2C handler and address pin status.
     * @param 	port 		Port of the pin.
     * @param 	pin 		Selected pin.
     * @param 	trig 		0b00 trigger at change, 0b01 rising edge, 0b10 falling edge, 0b11 any edge.
     *
     * @retval 	uint8_t		Error code.
     */
    uint8_t PCAL6524_SetInterruptTrigger(pcal6524_Device_t *device, pcal6524_Port_t port, pcal6524_Pin_t pin, pcal6524_InterruptTrigger_t trig);

    /**
     * @brief 				Gets current interrupt trigger configuration for selected port.
     *
     * @param   device      Struct with I2C handler and address pin status.
     * @param 	port 		Selected port.
     * @param 	*trig 		Pointer to output variable for trigger configuration. 0b00 trigger at change, 0b01 rising edge, 0b10 falling edge, 0b11 any edge.
     *
     * @retval 	uint8_t		Error code.
     */
    uint8_t PCAL6524_GetInterruptTriggerConfig(pcal6524_Device_t *device, pcal6524_Port_t port, pcal6524_Pin_t pin, pcal6524_InterruptTrigger_t *trig);

    /**
     * @brief	 			Get value of selected pin.
     *
     * @param   device      Struct with I2C handler and address pin status.
     * @param 	port 		Port of the pin.
     * @param 	pin 		Selected pin.
     * @param 	*value 		Pointer to output variable.
     *
     * @retval 	uint8_t		Error code.
     */
    uint8_t PCAL6524_GetPinValue(pcal6524_Device_t *device, pcal6524_Port_t port, pcal6524_Pin_t pin, pcal6524_Value_t *value);

    /**
     * @brief 				Get value of selected port.
     *
     * @param   device      Struct with I2C handler and address pin status.
     * @param 	port 		Selected port.
     * @param 	*values 	Pointer to output variable.
     *
     * @retval 	uint8_t		Error code.
     */
    uint8_t PCAL6524_GetPortPinValues(pcal6524_Device_t *device, pcal6524_Port_t port, uint8_t *values);

    /**
     * @brief 				Gets interrupt register of selected port.
     *
     * @param   device      Struct with I2C handler and address pin status.
     * @param 	port 		Selected port.
     * @param 	*values 	Pointer to output variable.
     *
     * @retval 	uint8_t		Error code.
     */
    uint8_t PCAL6524_GetInterrupts(pcal6524_Device_t *device, pcal6524_Port_t port, uint8_t *intr);

    /**
     * @brief	 			Clears interrupt register and -pin.
     *
     * @param   device      Struct with I2C handler and address pin status.
     *
     * @retval 	uint8_t		Error code.
     */
    uint8_t PCAL6524_ClearAllInterrupts(pcal6524_Device_t *device);

    /**
     * @brief 				Outputs a value on selected pin. Only works if pin is set to output.
     *
     * @param   device      Struct with I2C handler and address pin status.
     * @param 	port 		Port of the pin.
     * @param 	pin 		Selected pin.
     * @param 	value 		Output value.
     *
     * @retval 	uint8_t		Error code.
     */
    uint8_t PCAL6524_OutputValue(pcal6524_Device_t *device, pcal6524_Port_t port, pcal6524_Pin_t pin, pcal6524_Value_t value);

    /**
     * @}
     */

    /**
     * @}
     */

#ifdef __cplusplus
}
#endif

#endif /* CUSTOM_DRIVERS_INC_PCAL6524_H_ */
