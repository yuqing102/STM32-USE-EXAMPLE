/**
 ******************************************************************************
 * @file    LP5024.c
 * @version 2.0
 * @author  Till Heuer
 * @brief   Driver for NXP PCAL6524 IO expander IC.
 * @date 	Nov 22, 2023
 ******************************************************************************
 */

/** @addtogroup IC_Drivers
 * @{
 */

/** @addtogroup IO_Expander
 * @{
 */

#include "PCAL6524.h"

uint8_t PCAL6524_ReadI2C(pcal6524_Device_t *device, uint8_t regAdress, uint8_t *data)
{
    return HAL_I2C_Mem_Read(device->hi2c, (PCAL6524_ADDRESS + device->a0) << 1, regAdress, I2C_MEMADD_SIZE_8BIT, data, 1, PCAL6524_I2C_TIMEOUT);
}

uint8_t PCAL6524_WriteI2C(pcal6524_Device_t *device, uint8_t regAdress, uint8_t *data)
{
    return HAL_I2C_Mem_Write(device->hi2c, (PCAL6524_ADDRESS + device->a0) << 1, regAdress, I2C_MEMADD_SIZE_8BIT, data, 1, PCAL6524_I2C_TIMEOUT);
}

uint8_t PCAL6524_SetInOut(pcal6524_Device_t *device, pcal6524_Port_t port, pcal6524_Pin_t pin, pcal6524_InOut_t io)
{
    uint8_t data = 0;   // Holds data for i2c communication.
    uint8_t status = 0; // Holds i2c status for error catching.
    if (port > 2 || pin > 7 || io > 1)
    { // Checks for input errors.
        return PCAL6524_INPUTOUTOFRANGE;
    }
    /* Repeats i2c call, in case of busy i2c unit. */
    for (uint8_t attempt = 0; attempt <= PCAL6524_I2C_MAX_ATTEMPTS; attempt++)
    {
        switch (port)
        { // Reads current setting of register.
        case 0:
            status = PCAL6524_ReadI2C(device, PCAL6524_REG_CONF_PORT_0, &data);
            break;
        case 1:
            status = PCAL6524_ReadI2C(device, PCAL6524_REG_CONF_PORT_1, &data);
            break;
        case 2:
            status = PCAL6524_ReadI2C(device, PCAL6524_REG_CONF_PORT_2, &data);
            break;
        }
        if (status == HAL_OK)
        { // Breaks out of loop when successful.
            break;
        }
        else if (status == HAL_ERROR)
        { // Returns error when i2c unit fails.
            return HAL_ERROR;
        }
        else
        {
            /* Delays next i2c call if first attempt failed. */
            HAL_Delay(PCAL6524_I2C_ATTEMPT_DELAY);
        }
    }
    /* Catches case when all attempts failed and returns last error code. */
    if (status > HAL_OK)
    {
        return status;
    }
    /* Combines current value of register with value that has to be changed. */
    data = (data & (~(1 << pin))) | (io << pin);
    /* Repeats i2c call, in case of busy i2c unit. */
    for (uint8_t attempt = 0; attempt <= PCAL6524_I2C_MAX_ATTEMPTS; attempt++)
    {
        switch (port)
        { // Sends changed register settings to chip.
        case 0:
            status = PCAL6524_WriteI2C(device, PCAL6524_REG_CONF_PORT_0, &data);
            break;
        case 1:
            status = PCAL6524_WriteI2C(device, PCAL6524_REG_CONF_PORT_1, &data);
            break;
        case 2:
            status = PCAL6524_WriteI2C(device, PCAL6524_REG_CONF_PORT_2, &data);
            break;
        }
        if (status == HAL_OK)
        { // Breaks out of loop when successful.
            break;
        }
        else if (status == HAL_ERROR)
        { // Returns error when i2c unit fails.
            return HAL_ERROR;
        }
        else
        {
            /* Delays next i2c call if first attempt failed. */
            HAL_Delay(PCAL6524_I2C_ATTEMPT_DELAY);
        }
    }
    /* Catches case when all attempts failed and returns last error code. */
    if (status > HAL_OK)
    {
        return status;
    }
    return PCAL6524_SUCCESS; // Returns success code when transmission successful.
}
uint8_t PCAL6524_GetInOutConfig(pcal6524_Device_t *device, pcal6524_Port_t port, uint8_t *ios)
{
    uint8_t status = 0; // Holds i2c status for error catching.
    if (port > 2)
    { // Checks for input errors.
        return PCAL6524_INPUTOUTOFRANGE;
    }
    /* Repeats i2c call, in case of busy i2c unit. */
    for (uint8_t attempt = 0; attempt <= PCAL6524_I2C_MAX_ATTEMPTS; attempt++)
    {
        switch (port)
        { // Reads current setting of register.
        case 0:

            status = PCAL6524_ReadI2C(device, PCAL6524_REG_CONF_PORT_0, ios);
            break;
        case 1:
            status = PCAL6524_ReadI2C(device, PCAL6524_REG_CONF_PORT_1, ios);
            break;
        case 2:
            status = PCAL6524_ReadI2C(device, PCAL6524_REG_CONF_PORT_2, ios);
            break;
        }
        if (status == HAL_OK)
        { // Breaks out of loop when successful.
            break;
        }
        else if (status == HAL_ERROR)
        { // Returns error when i2c unit fails.
            return HAL_ERROR;
        }
        else
        {
            /* Delays next i2c call if first attempt failed. */
            HAL_Delay(PCAL6524_I2C_ATTEMPT_DELAY);
        }
    }
    /* Catches case when all attempts failed and returns last error code. */
    if (status > HAL_OK)
    {
        return status;
    }
    return PCAL6524_SUCCESS; // Returns success code when transmission successful.
}
uint8_t PCAL6524_SetInterrupt(
    pcal6524_Device_t *device, pcal6524_Port_t port,
    pcal6524_Pin_t pin, pcal6524_InterruptEN_t intr)
{
    uint8_t data = 0;   // Holds data for i2c communication.
    uint8_t status = 0; // Holds i2c status for error catching.
    /* Checks for input errors. */
    if (port > 2 || pin > 7 || intr > 1)
    {
        return PCAL6524_INPUTOUTOFRANGE;
    } /* Repeats i2c call, in case of busy i2c unit. */
    for (uint8_t attempt = 0; attempt <= PCAL6524_I2C_MAX_ATTEMPTS; attempt++)
    {
        switch (port)
        { // Reads current setting of register.
        case 0:
            status = PCAL6524_ReadI2C(device, PCAL6524_REG_INT_MASK_PORT_0, &data);
            break;
        case 1:
            status = PCAL6524_ReadI2C(device, PCAL6524_REG_INT_MASK_PORT_1, &data);
            break;
        case 2:
            status = PCAL6524_ReadI2C(device, PCAL6524_REG_INT_MASK_PORT_2, &data);
            break;
        }
        if (status == HAL_OK)
        { // Breaks out of loop when successful.
            break;
        }
        else if (status == HAL_ERROR)
        { // Returns error when i2c unit fails.
            return HAL_ERROR;
        }
        else
        {
            /* Delays next i2c call if first attempt failed. */
            HAL_Delay(PCAL6524_I2C_ATTEMPT_DELAY);
        }
    }
    /* Catches case when all attempts failed and returns last error code. */
    if (status > HAL_OK)
    {
        return status;
    }
    /* Combines current value of register with value that has to be changed. */
    data = (data & (~(0b1 << pin))) | (~(intr) << pin);
    /* Repeats i2c call, in case of busy i2c unit. */
    for (uint8_t attempt = 0; attempt <= PCAL6524_I2C_MAX_ATTEMPTS; attempt++)
    {
        switch (port)
        { // Sends changed register settings to chip.
        case 0:
            status = PCAL6524_WriteI2C(device, PCAL6524_REG_INT_MASK_PORT_0, &data);
            break;
        case 1:
            status = PCAL6524_WriteI2C(device, PCAL6524_REG_INT_MASK_PORT_1, &data);
            break;
        case 2:
            status = PCAL6524_WriteI2C(device, PCAL6524_REG_INT_MASK_PORT_2, &data);
            break;
        }
        if (status == HAL_OK)
        { // Breaks out of loop when successful.
            break;
        }
        else if (status == HAL_ERROR)
        { // Returns error when i2c unit fails.
            return HAL_ERROR;
        }
        else
        {
            /* Delays next i2c call if first attempt failed. */
            HAL_Delay(PCAL6524_I2C_ATTEMPT_DELAY);
        }
    }
    /* Catches case when all attempts failed and returns last error code. */
    if (status > HAL_OK)
    {
        return status;
    }
    return PCAL6524_SUCCESS; // Returns success code when transmission successful.
}
uint8_t PCAL6524_GetInterruptConfig(
    pcal6524_Device_t *device, pcal6524_Port_t port,
    uint8_t *intr)
{
    uint8_t status = 0; // Holds i2c status for error catching.
    if (port > 2)
    { // Checks for input errors.
        return PCAL6524_INPUTOUTOFRANGE;
    }
    /* Repeats i2c call, in case of busy i2c unit. */
    for (uint8_t attempt = 0; attempt <= PCAL6524_I2C_MAX_ATTEMPTS; attempt++)
    {
        switch (port)
        { // Reads current setting of register.
        case 0:
            status = PCAL6524_ReadI2C(device, PCAL6524_REG_INT_MASK_PORT_0, intr);
            break;
        case 1:
            status = PCAL6524_ReadI2C(device, PCAL6524_REG_INT_MASK_PORT_1, intr);
            break;
        case 2:
            status = PCAL6524_ReadI2C(device, PCAL6524_REG_INT_MASK_PORT_2, intr);
            break;
        }
        if (status == HAL_OK)
        { // Breaks out of loop when successful.
            break;
        }
        else if (status == HAL_ERROR)
        { // Returns error when i2c unit fails.
            return HAL_ERROR;
        }
        else
        {
            /* Delays next i2c call if first attempt failed. */
            HAL_Delay(PCAL6524_I2C_ATTEMPT_DELAY);
        }
    }
    /* Catches case when all attempts failed and returns last error code. */
    if (status > HAL_OK)
    {
        return status;
    }
    return PCAL6524_SUCCESS; // Returns success code when transmission successful.
}
uint8_t PCAL6524_SetPullupDown(
    pcal6524_Device_t *device, pcal6524_Port_t port,
    pcal6524_Pin_t pin, pcal6524_PullUpDown_t pull,
    pcal6524_PullUpDownEN_t active)
{
    uint8_t data = 0;   // Holds data for i2c communication.
    uint8_t status = 0; // Holds i2c status for error catching.
    /* Checks for input errors. */
    if (port > 2 || pin > 7 || pull > 1 || active > 1)
    {
        return PCAL6524_INPUTOUTOFRANGE;
    }
    /* Repeats i2c call, in case of busy i2c unit. */
    for (uint8_t attempt = 0; attempt <= PCAL6524_I2C_MAX_ATTEMPTS; attempt++)
    {
        /* Reads current setting of pull-up/pull-down select register. */
        switch (port)
        {
        case 0:
            status = PCAL6524_ReadI2C(device, PCAL6524_REG_PULL_SEL_PORT_0, &data);
            break;
        case 1:
            status = PCAL6524_ReadI2C(device, PCAL6524_REG_PULL_SEL_PORT_1, &data);
            break;
        case 2:
            status = PCAL6524_ReadI2C(device, PCAL6524_REG_PULL_SEL_PORT_2, &data);
            break;
        }
        if (status == HAL_OK)
        { // Breaks out of loop when successful.
            break;
        }
        else if (status == HAL_ERROR)
        { // Returns error when i2c unit fails.
            return HAL_ERROR;
        }
        else
        {
            /* Delays next i2c call if first attempt failed. */
            HAL_Delay(PCAL6524_I2C_ATTEMPT_DELAY);
        }
    }
    /* Catches case when all attempts failed and returns last error code. */
    if (status > HAL_OK)
    {
        return status;
    }
    /* Combines current value of register with value that has to be changed. */
    data = (data & (~(0b1 << pin))) | (pull << pin);
    /* Repeats i2c call, in case of busy i2c unit. */
    for (uint8_t attempt = 0; attempt <= PCAL6524_I2C_MAX_ATTEMPTS; attempt++)
    {
        /* Sends changed setting in pull-up/pull-down select register to chip. */
        switch (port)
        {
        case 0:
            status = PCAL6524_WriteI2C(device, PCAL6524_REG_PULL_SEL_PORT_0, &data);
            break;
        case 1:
            status = PCAL6524_WriteI2C(device, PCAL6524_REG_PULL_SEL_PORT_1, &data);
            break;
        case 2:
            status = PCAL6524_WriteI2C(device, PCAL6524_REG_PULL_SEL_PORT_2, &data);
            break;
        }
        if (status == HAL_OK)
        { // Breaks out of loop when successful.
            break;
        }
        else if (status == HAL_ERROR)
        { // Returns error when i2c unit fails.
            return HAL_ERROR;
        }
        else
        {
            /* Delays next i2c call if first attempt failed. */
            HAL_Delay(PCAL6524_I2C_ATTEMPT_DELAY);
        }
    }
    /* Catches case when all attempts failed and returns last error code. */
    if (status > HAL_OK)
    {
        return status;
    }
    /* Repeats i2c call, in case of busy i2c unit. */
    for (uint8_t attempt = 0; attempt <= PCAL6524_I2C_MAX_ATTEMPTS; attempt++)
    {
        /* Reads current setting of pull-up/pull-down enable register. */
        switch (port)
        {
        case 0:
            status = PCAL6524_ReadI2C(device, PCAL6524_REG_PULL_EN_PORT_0, &data);
            break;
        case 1:
            status = PCAL6524_ReadI2C(device, PCAL6524_REG_PULL_EN_PORT_1, &data);
            break;
        case 2:
            status = PCAL6524_ReadI2C(device, PCAL6524_REG_PULL_EN_PORT_2, &data);
            break;
        }
        if (status == HAL_OK)
        { // Breaks out of loop when successful.
            break;
        }
        else if (status == HAL_ERROR)
        { // Returns error when i2c unit fails.
            return HAL_ERROR;
        }
        else
        {
            /* Delays next i2c call if first attempt failed. */
            HAL_Delay(PCAL6524_I2C_ATTEMPT_DELAY);
        }
    }
    /* Catches case when all attempts failed and returns last error code. */
    if (status > HAL_OK)
    {
        return status;
    }
    /* Combines current value of register with value that has to be changed. */
    data = (data & (~(0b1 << pin))) | (active << pin);
    /* Repeats i2c call, in case of busy i2c unit. */
    for (uint8_t attempt = 0; attempt <= PCAL6524_I2C_MAX_ATTEMPTS; attempt++)
    {
        /* Sends changed setting in pull-up/pull-down enable register to chip. */
        switch (port)
        {
        case 0:
            status = PCAL6524_WriteI2C(device, PCAL6524_REG_PULL_EN_PORT_0, &data);
            break;
        case 1:
            status = PCAL6524_WriteI2C(device, PCAL6524_REG_PULL_EN_PORT_1, &data);
            break;
        case 2:
            status = PCAL6524_WriteI2C(device, PCAL6524_REG_PULL_EN_PORT_2, &data);
            break;
        }
        if (status == HAL_OK)
        { // Breaks out of loop when successful.
            break;
        }
        else if (status == HAL_ERROR)
        { // Returns error when i2c unit fails.
            return HAL_ERROR;
        }
        else
        {
            /* Delays next i2c call if first attempt failed. */
            HAL_Delay(PCAL6524_I2C_ATTEMPT_DELAY);
        }
    }
    /* Catches case when all attempts failed and returns last error code. */
    if (status > HAL_OK)
    {
        return status;
    }
    return PCAL6524_SUCCESS; // Returns success code when transmission successful.
}
uint8_t PCAL6524_GetPullupDownConfig(
    pcal6524_Device_t *device, pcal6524_Port_t port,
    uint8_t *pull, uint8_t *active)
{
    uint8_t status = 0; // Holds i2c status for error catching.
    if (port > 2)
    { // Checks for input errors.
        return PCAL6524_INPUTOUTOFRANGE;
    }
    /* Repeats i2c call, in case of busy i2c unit.*/
    for (uint8_t attempt = 0; attempt <= PCAL6524_I2C_MAX_ATTEMPTS; attempt++)
    {
        /* Reads current setting of pull-up/pull-down select register. */
        switch (port)
        {
        case 0:
            status = PCAL6524_ReadI2C(device, PCAL6524_REG_PULL_SEL_PORT_0, pull);
            break;
        case 1:
            status = PCAL6524_ReadI2C(device, PCAL6524_REG_PULL_SEL_PORT_1, pull);
            break;
        case 2:
            status = PCAL6524_ReadI2C(device, PCAL6524_REG_PULL_SEL_PORT_2, pull);
            break;
        }
        if (status == HAL_OK)
        { // Breaks out of loop when successful.
            break;
        }
        else if (status == HAL_ERROR)
        { // Returns error when i2c unit fails.
            return HAL_ERROR;
        }
        else
        {
            /* Delays next i2c call if first attempt failed. */
            HAL_Delay(PCAL6524_I2C_ATTEMPT_DELAY);
        }
    }
    /* Catches case when all attempts failed and returns last error code. */
    if (status > HAL_OK)
    {
        return status;
    }
    /* Repeats i2c call, in case of busy i2c unit. */
    for (uint8_t attempt = 0; attempt <= PCAL6524_I2C_MAX_ATTEMPTS; attempt++)
    {
        /* Reads current setting of pull-up/pull-down enable register. */
        switch (port)
        {
        case 0:
            status = PCAL6524_ReadI2C(device, PCAL6524_REG_PULL_EN_PORT_0, active);
            break;
        case 1:
            status = PCAL6524_ReadI2C(device, PCAL6524_REG_PULL_EN_PORT_1, active);
            break;
        case 2:
            status = PCAL6524_ReadI2C(device, PCAL6524_REG_PULL_EN_PORT_2, active);
            break;
        }
        if (status == HAL_OK)
        { // Breaks out of loop when successful.
            break;
        }
        else if (status == HAL_ERROR)
        { // Returns error when i2c unit fails.
            return HAL_ERROR;
        }
        else
        {
            /* Delays next i2c call if first attempt failed. */
            HAL_Delay(PCAL6524_I2C_ATTEMPT_DELAY);
        }
    }
    /* Catches case when all attempts failed and returns last error code. */
    if (status > HAL_OK)
    {
        return status;
    }
    return PCAL6524_SUCCESS; // Returns success code when transmission successful.
}
uint8_t PCAL6524_SetPolarity(
    pcal6524_Device_t *device, pcal6524_Port_t port,
    pcal6524_Pin_t pin, pcal6524_Polarity_t pol)
{
    uint8_t data = 0;   // Holds data for i2c communication.
    uint8_t status = 0; // Holds i2c status for error catching.
    if (port > 2 || pin > 7 || pol > 1)
    { // Checks for input errors.
        return PCAL6524_INPUTOUTOFRANGE;
    }
    /* Repeats i2c call, in case of busy i2c unit. */
    for (uint8_t attempt = 0; attempt <= PCAL6524_I2C_MAX_ATTEMPTS; attempt++)
    {
        switch (port)
        { // Reads current setting of register.
        case 0:
            status = PCAL6524_ReadI2C(device, PCAL6524_REG_POL_PORT_0, &data);
            break;
        case 1:
            status = PCAL6524_ReadI2C(device, PCAL6524_REG_POL_PORT_1, &data);
            break;
        case 2:
            status = PCAL6524_ReadI2C(device, PCAL6524_REG_POL_PORT_2, &data);
            break;
        }
        if (status == HAL_OK)
        { // Breaks out of loop when successful.
            break;
        }
        else if (status == HAL_ERROR)
        { // Returns error when i2c unit fails.
            return HAL_ERROR;
        }
        else
        {
            /* Delays next i2c call if first attempt failed. */
            HAL_Delay(PCAL6524_I2C_ATTEMPT_DELAY);
        }
    }
    /* Catches case when all attempts failed and returns last error code. */
    if (status > HAL_OK)
    {
        return status;
    }
    /* Combines current value of register with value that has to be changed. */
    data = (data & (~(0b1 << pin))) | (pol << pin);
    /* Repeats i2c call, in case of busy i2c unit. */
    for (uint8_t attempt = 0; attempt <= PCAL6524_I2C_MAX_ATTEMPTS; attempt++)
    {
        switch (port)
        { // Sends changed register settings to chip.
        case 0:
            status = PCAL6524_WriteI2C(device, PCAL6524_REG_POL_PORT_0, &data);
            break;
        case 1:
            status = PCAL6524_WriteI2C(device, PCAL6524_REG_POL_PORT_1, &data);
            break;
        case 2:
            status = PCAL6524_WriteI2C(device, PCAL6524_REG_POL_PORT_2, &data);
            break;
        }
        if (status == HAL_OK)
        { // Breaks out of loop when successful.
            break;
        }
        else if (status == HAL_ERROR)
        { // Returns error when i2c unit fails.
            return HAL_ERROR;
        }
        else
        {
            /* Delays next i2c call if first attempt failed. */
            HAL_Delay(PCAL6524_I2C_ATTEMPT_DELAY);
        }
    }
    /* Catches case when all attempts failed and returns last error code. */
    if (status > HAL_OK)
    {
        return status;
    }
    return PCAL6524_SUCCESS; // Returns success code when transmission successful.
}
uint8_t PCAL6524_GetPolarityConfig(
    pcal6524_Device_t *device, pcal6524_Port_t port,
    uint8_t *pol)
{
    uint8_t status = 0; // Holds i2c status for error catching.
    if (port > 2)
    { // Checks for input errors.
        return PCAL6524_INPUTOUTOFRANGE;
    }
    /* Repeats i2c call, in case of busy i2c unit. */
    for (uint8_t attempt = 0; attempt <= PCAL6524_I2C_MAX_ATTEMPTS; attempt++)
    {
        switch (port)
        { // Reads current setting of register.
        case 0:
            status = PCAL6524_ReadI2C(device, PCAL6524_REG_POL_PORT_0, pol);
            break;
        case 1:
            status = PCAL6524_ReadI2C(device, PCAL6524_REG_POL_PORT_1, pol);
            break;
        case 2:
            status = PCAL6524_ReadI2C(device, PCAL6524_REG_POL_PORT_2, pol);
            break;
        }
        if (status == HAL_OK)
        { // Breaks out of loop when successful.
            break;
        }
        else if (status == HAL_ERROR)
        { // Returns error when i2c unit fails.
            return HAL_ERROR;
        }
        else
        {
            /* Delays next i2c call if first attempt failed. */
            HAL_Delay(PCAL6524_I2C_ATTEMPT_DELAY);
        }
    }
    /* Catches case when all attempts failed and returns last error code. */
    if (status > HAL_OK)
    {
        return status;
    }
    return PCAL6524_SUCCESS; // Returns success code when transmission successful.
}
uint8_t PCAL6524_SetInterruptTrigger(
    pcal6524_Device_t *device, pcal6524_Port_t port,
    pcal6524_Pin_t pin, pcal6524_InterruptTrigger_t trig)
{
    uint8_t data = 0;   // Holds data for i2c communication.
    uint8_t status = 0; // Holds i2c status for error catching.
    /* Checks for input errors. */
    if (port > 2 || pin > 7 || trig > 0b11)
    {
        return PCAL6524_INPUTOUTOFRANGE;
    }
    /* Repeats i2c call, in case of busy i2c unit. */
    for (uint8_t attempt = 0; attempt <= PCAL6524_I2C_MAX_ATTEMPTS; attempt++)
    {
        switch (port)
        { // Reads current setting of register.
        case 0:
            if (pin < 4)
            {
                status = PCAL6524_ReadI2C(device, PCAL6524_REG_INT_EGDE_PORT_0A, &data);
            }
            else
            {
                status = PCAL6524_ReadI2C(device, PCAL6524_REG_INT_EGDE_PORT_0B, &data);
            }
            break;
        case 1:
            if (pin < 4)
            {
                status = PCAL6524_ReadI2C(device, PCAL6524_REG_INT_EGDE_PORT_1A, &data);
            }
            else
            {
                status = PCAL6524_ReadI2C(device, PCAL6524_REG_INT_EGDE_PORT_1B, &data);
            }
            break;
        case 2:
            if (pin < 4)
            {
                status = PCAL6524_ReadI2C(device, PCAL6524_REG_INT_EGDE_PORT_2A, &data);
            }
            else
            {
                status = PCAL6524_ReadI2C(device, PCAL6524_REG_INT_EGDE_PORT_2B, &data);
            }
            break;
        }
        if (status == HAL_OK)
        { // Breaks out of loop when successful.
            break;
        }
        else if (status == HAL_ERROR)
        { // Returns error when i2c unit fails.
            return HAL_ERROR;
        }
        else
        {
            /* Delays next i2c call if first attempt failed. */
            HAL_Delay(PCAL6524_I2C_ATTEMPT_DELAY);
        }
    }
    /* Catches case when all attempts failed and returns last error code. */
    if (status > HAL_OK)
    {
        return status;
    }
    /* Combines current value of register with value that has to be changed. */
    data = (data & (~(0b1 << (2 * (pin % 4))))) | (trig << (2 * (pin % 4)));
    /* Repeats i2c call, in case of busy i2c unit. */
    for (uint8_t attempt = 0; attempt <= PCAL6524_I2C_MAX_ATTEMPTS; attempt++)
    {
        switch (port)
        { // Sends changed register settings to chip.
        case 0:
            if (pin < 4)
            {
                status = PCAL6524_WriteI2C(device, PCAL6524_REG_INT_EGDE_PORT_0A, &data);
            }
            else
            {
                status = PCAL6524_WriteI2C(device, PCAL6524_REG_INT_EGDE_PORT_0B, &data);
            }
            break;
        case 1:
            if (pin < 4)
            {
                status = PCAL6524_WriteI2C(device, PCAL6524_REG_INT_EGDE_PORT_1A, &data);
            }
            else
            {
                status = PCAL6524_WriteI2C(device, PCAL6524_REG_INT_EGDE_PORT_1B, &data);
            }
            break;
        case 2:
            if (pin < 4)
            {
                status = PCAL6524_WriteI2C(device, PCAL6524_REG_INT_EGDE_PORT_2A, &data);
            }
            else
            {
                status = PCAL6524_WriteI2C(device, PCAL6524_REG_INT_EGDE_PORT_2B, &data);
            }
            break;
        }
        if (status == HAL_OK)
        { // Breaks out of loop when successful.
            break;
        }
        else if (status == HAL_ERROR)
        { // Returns error when i2c unit fails.
            return HAL_ERROR;
        }
        else
        {
            /* Delays next i2c call if first attempt failed. */
            HAL_Delay(PCAL6524_I2C_ATTEMPT_DELAY);
        }
    }
    /* Catches case when all attempts failed and returns last error code. */
    if (status > HAL_OK)
    {
        return status;
    }
    return PCAL6524_SUCCESS; // Returns success code when transmission successful.
}
uint8_t PCAL6524_GetInterruptTriggerConfig(
    pcal6524_Device_t *device, pcal6524_Port_t port,
    pcal6524_Pin_t pin, pcal6524_InterruptTrigger_t *trig)
{
    uint8_t status = 0; // Holds i2c status for error catching.
    if (port > 2 || pin > 7)
    { // Checks for input errors.
        return PCAL6524_INPUTOUTOFRANGE;
    }
    /* Repeats i2c call, in case of busy i2c unit. */
    for (uint8_t attempt = 0; attempt <= PCAL6524_I2C_MAX_ATTEMPTS; attempt++)
    {
        switch (port)
        { // Reads current setting of register.
        case 0:
            if (pin < 4)
            {
                status = PCAL6524_ReadI2C(device, PCAL6524_REG_INT_EGDE_PORT_0A, trig);
            }
            else
            {
                status = PCAL6524_ReadI2C(device, PCAL6524_REG_INT_EGDE_PORT_0B, trig);
            }
            break;
        case 1:
            if (pin < 4)
            {
                status = PCAL6524_ReadI2C(device, PCAL6524_REG_INT_EGDE_PORT_1A, trig);
            }
            else
            {
                status = PCAL6524_ReadI2C(device, PCAL6524_REG_INT_EGDE_PORT_1B, trig);
            }
            break;
        case 2:
            if (pin < 4)
            {
                status = PCAL6524_ReadI2C(device, PCAL6524_REG_INT_EGDE_PORT_2A, trig);
            }
            else
            {
                status = PCAL6524_ReadI2C(device, PCAL6524_REG_INT_EGDE_PORT_2B, trig);
            }
            break;
        }
        if (status == HAL_OK)
        { // Breaks out of loop when successful.
            break;
        }
        else if (status == HAL_ERROR)
        { // Returns error when i2c unit fails.
            return HAL_ERROR;
        }
        else
        {
            /* Delays next i2c call if first attempt failed. */
            HAL_Delay(PCAL6524_I2C_ATTEMPT_DELAY);
        }
    }
    /* Catches case when all attempts failed and returns last error code. */
    if (status > HAL_OK)
    {
        return status;
    }
    return PCAL6524_SUCCESS; // Returns success code when transmission successful.
}
uint8_t PCAL6524_GetPinValue(
    pcal6524_Device_t *device, pcal6524_Port_t port,
    pcal6524_Pin_t pin, pcal6524_Value_t *value)
{
    uint8_t data = 0;   // Holds data for i2c communication.
    uint8_t status = 0; // Holds i2c status for error catching.
    if (port > 2 || pin > 7)
    { // Checks for input errors.
        return PCAL6524_INPUTOUTOFRANGE;
    }
    /* Repeats i2c call, in case of busy i2c unit. */
    for (uint8_t attempt = 0; attempt <= PCAL6524_I2C_MAX_ATTEMPTS; attempt++)
    {
        /* Reads input values of selected port without resetting the interrupt. */
        switch (port)
        {
        case 0:
            status = PCAL6524_ReadI2C(device, PCAL6524_REG_IN_STATUS_PORT_0, value);
            break;
        case 1:
            status = PCAL6524_ReadI2C(device, PCAL6524_REG_IN_STATUS_PORT_1, value);
            break;
        case 2:
            status = PCAL6524_ReadI2C(device, PCAL6524_REG_IN_STATUS_PORT_2, value);
            break;
        }
        if (status == HAL_OK)
        { // Breaks out of loop when successful.
            break;
        }
        else if (status == HAL_ERROR)
        { // Returns error when i2c unit fails.
            return HAL_ERROR;
        }
        else
        {
            /* Delays next i2c call if first attempt failed. */
            HAL_Delay(PCAL6524_I2C_ATTEMPT_DELAY);
        }
    }
    /* Catches case when all attempts failed and returns last error code. */
    if (status > HAL_OK)
    {
        return status;
    }
    *value = (*value & (1 << pin)) >> pin; // Picks out wanted pin value.
    data = 1 << pin;
    /* Repeats i2c call, in case of busy i2c unit. */
    for (uint8_t attempt = 0; attempt <= PCAL6524_I2C_MAX_ATTEMPTS; attempt++)
    {
        switch (port)
        { // Resets interrupt on read pin.
        case 0:
            status = PCAL6524_WriteI2C(device, PCAL6524_REG_INT_CLEAR_PORT_0, &data);
            break;
        case 1:
            status = PCAL6524_WriteI2C(device, PCAL6524_REG_INT_CLEAR_PORT_1, &data);
            break;
        case 2:
            status = PCAL6524_WriteI2C(device, PCAL6524_REG_INT_CLEAR_PORT_2, &data);
            break;
        }
        if (status == HAL_OK)
        { // Breaks out of loop when successful.
            break;
        }
        else if (status == HAL_ERROR)
        { // Returns error when i2c unit fails.
            return HAL_ERROR;
        }
        else
        {
            /* Delays next i2c call if first attempt failed. */
            HAL_Delay(PCAL6524_I2C_ATTEMPT_DELAY);
        }
    }
    /* Catches case when all attempts failed and returns last error code. */
    if (status > HAL_OK)
    {
        return status;
    }
    return PCAL6524_SUCCESS; // Returns success code when transmission successful.
}
uint8_t PCAL6524_OutputValue(
    pcal6524_Device_t *device, pcal6524_Port_t port,
    pcal6524_Pin_t pin, pcal6524_Value_t value)
{
    uint8_t data = 0;   // Holds data for i2c communication.
    uint8_t status = 0; // Holds i2c status for error catching.
    /* Checks for input errors. */
    if (port > 2 || pin > 7 || value > 1)
    {
        return PCAL6524_INPUTOUTOFRANGE;
    }
    /* Repeats i2c call, in case of busy i2c unit. */
    for (uint8_t attempt = 0; attempt <= PCAL6524_I2C_MAX_ATTEMPTS; attempt++)
    {
        switch (port)
        { // Reads current setting of register.
        case 0:
            status = PCAL6524_ReadI2C(device, PCAL6524_REG_OUT_PORT_0, &data);
            break;
        case 1:
            status = PCAL6524_ReadI2C(device, PCAL6524_REG_OUT_PORT_1, &data);
            break;
        case 2:
            status = PCAL6524_ReadI2C(device, PCAL6524_REG_OUT_PORT_2, &data);
            break;
        }
        if (status == HAL_OK)
        { // Breaks out of loop when successful.
            break;
        }
        else if (status == HAL_ERROR)
        { // Returns error when i2c unit fails.
            return HAL_ERROR;
        }
        else
        {
            /* Delays next i2c call if first attempt failed. */
            HAL_Delay(PCAL6524_I2C_ATTEMPT_DELAY);
        }
    }
    /* Catches case when all attempts failed and returns last error code. */
    if (status > HAL_OK)
    {
        return status;
    }
    /* Combines current value of register with value that has to be changed. */
    data = (data & (~(0b1 << pin))) | (value << pin);
    /* Repeats i2c call, in case of busy i2c unit. */
    for (uint8_t attempt = 0; attempt <= PCAL6524_I2C_MAX_ATTEMPTS; attempt++)
    {
        switch (port)
        { // Sends changed register settings to chip.
        case 0:
            status = PCAL6524_WriteI2C(device, PCAL6524_REG_OUT_PORT_0, &data);
            break;
        case 1:
            status = PCAL6524_WriteI2C(device, PCAL6524_REG_OUT_PORT_1, &data);
            break;
        case 2:
            status = PCAL6524_WriteI2C(device, PCAL6524_REG_OUT_PORT_2, &data);
            break;
        }
        if (status == HAL_OK)
        { // Breaks out of loop when successful.
            break;
        }
        else if (status == HAL_ERROR)
        { // Returns error when i2c unit fails.
            return HAL_ERROR;
        }
        else
        {
            /* Delays next i2c call if first attempt failed. */
            HAL_Delay(PCAL6524_I2C_ATTEMPT_DELAY);
        }
    }
    /* Catches case when all attempts failed and returns last error code. */
    if (status > HAL_OK)
    {
        return status;
    }
    return PCAL6524_SUCCESS; // Returns success code when transmission successful.
}
uint8_t PCAL6524_GetPortPinValues(
    pcal6524_Device_t *device, pcal6524_Port_t port,
    uint8_t *values)
{
    uint8_t status = 0; // Holds i2c status for error catching.
    if (port > 2)
    { // Checks for input errors.
        return PCAL6524_INPUTOUTOFRANGE;
    }
    /* Repeats i2c call, in case of busy i2c unit. */
    for (uint8_t attempt = 0; attempt <= PCAL6524_I2C_MAX_ATTEMPTS; attempt++)
    {
        /* Reads input values of selected port without resetting the interrupt. */
        switch (port)
        {
        case 0:
            status = PCAL6524_ReadI2C(device, PCAL6524_REG_IN_PORT_0, values);
            break;
        case 1:
            status = PCAL6524_ReadI2C(device, PCAL6524_REG_IN_PORT_1, values);
            break;
        case 2:
            status = PCAL6524_ReadI2C(device, PCAL6524_REG_IN_PORT_2, values);
            break;
        }
        if (status == HAL_OK)
        { // Breaks out of loop when successful.
            break;
        }
        else if (status == HAL_ERROR)
        { // Returns error when i2c unit fails.
            return HAL_ERROR;
        }
        else
        {
            /* Delays next i2c call if first attempt failed. */
            HAL_Delay(PCAL6524_I2C_ATTEMPT_DELAY);
        }
    }
    /* Catches case when all attempts failed and returns last error code. */
    if (status > HAL_OK)
    {
        return status;
    }
    return PCAL6524_SUCCESS; // Returns success code when transmission successful.
}
uint8_t PCAL6524_GetInterrupts(
    pcal6524_Device_t *device, pcal6524_Port_t port,
    uint8_t *intr)
{
    uint8_t status = 0; // Holds i2c status for error catching.
    if (port > 2)
    { // Checks for input errors.
        return PCAL6524_INPUTOUTOFRANGE;
    }
    /* Repeats i2c call, in case of busy i2c unit. */
    for (uint8_t attempt = 0; attempt <= PCAL6524_I2C_MAX_ATTEMPTS; attempt++)
    {
        /* Reads input values of selected port without resetting the interrupt. */
        switch (port)
        {
        case 0:
            status = PCAL6524_ReadI2C(device, PCAL6524_REG_INT_STAT_PORT_0, intr);
            break;
        case 1:
            status = PCAL6524_ReadI2C(device, PCAL6524_REG_INT_STAT_PORT_1, intr);
            break;
        case 2:
            status = PCAL6524_ReadI2C(device, PCAL6524_REG_INT_STAT_PORT_2, intr);
            break;
        }
        if (status == HAL_OK)
        { // Breaks out of loop when successful.
            break;
        }
        else if (status == HAL_ERROR)
        { // Returns error when i2c unit fails.
            return HAL_ERROR;
        }
        else
        {
            /* Delays next i2c call if first attempt failed. */
            HAL_Delay(PCAL6524_I2C_ATTEMPT_DELAY);
        }
    }
    /* Catches case when all attempts failed and returns last error code. */
    if (status > HAL_OK)
    {
        return status;
    }
    return PCAL6524_SUCCESS; // Returns success code when transmission successful.
}
uint8_t PCAL6524_ClearAllInterrupts(pcal6524_Device_t *device)
{
    uint8_t data = 0b11111111; // Holds data for i2c communication.
    uint8_t status = 0;        // Holds i2c status for error catching.
    /* Repeats i2c call, in case of busy i2c unit. */
    for (uint8_t attempt = 0; attempt <= PCAL6524_I2C_MAX_ATTEMPTS; attempt++)
    {
        status = PCAL6524_WriteI2C(device, PCAL6524_REG_INT_CLEAR_PORT_0, &data);
        if (status == HAL_OK)
        { // Breaks out of loop when successful.
            break;
        }
        else if (status == HAL_ERROR)
        { // Returns error when i2c unit fails.
            return HAL_ERROR;
        }
        else
        {
            /* Delays next i2c call if first attempt failed. */
            HAL_Delay(PCAL6524_I2C_ATTEMPT_DELAY);
        }
    }
    /* Repeats i2c call, in case of busy i2c unit. */
    for (uint8_t attempt = 0; attempt <= PCAL6524_I2C_MAX_ATTEMPTS; attempt++)
    {
        status = PCAL6524_WriteI2C(device, PCAL6524_REG_INT_CLEAR_PORT_1, &data);
        if (status == HAL_OK)
        { // Breaks out of loop when successful.
            break;
        }
        else if (status == HAL_ERROR)
        { // Returns error when i2c unit fails.
            return HAL_ERROR;
        }
        else
        {
            /* Delays next i2c call if first attempt failed. */
            HAL_Delay(PCAL6524_I2C_ATTEMPT_DELAY);
        }
    }
    /* Repeats i2c call, in case of busy i2c unit. */
    for (uint8_t attempt = 0; attempt <= PCAL6524_I2C_MAX_ATTEMPTS; attempt++)
    {
        status = PCAL6524_WriteI2C(device, PCAL6524_REG_INT_CLEAR_PORT_2, &data);
        if (status == HAL_OK)
        { // Breaks out of loop when successful.
            break;
        }
        else if (status == HAL_ERROR)
        { // Returns error when i2c unit fails.
            return HAL_ERROR;
        }
        else
        {
            /* Delays next i2c call if first attempt failed. */
            HAL_Delay(PCAL6524_I2C_ATTEMPT_DELAY);
        }
    }
    /* Catches case when all attempts failed and returns last error code. */
    if (status > HAL_OK)
    {
        return status;
    }
    return PCAL6524_SUCCESS; // Returns success code when transmission successful.
}

/**
 * @}
 */

/**
 * @}
 */
