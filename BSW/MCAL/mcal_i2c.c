/*
 * mcal_i2c.c
 *
 * Created: 12/21/2025 3:21:19 PM
 *  Author: conti
 */ 

#include "mcal_i2c.h"

/**
 * @brief Initializes I2C using a configuration structure.
 */
I2cStatus_t Mcal_I2c_Init(const Mcal_I2c_Config_t* pstConfig) {
    I2cStatus_t enRetVal = I2C_ERROR;

    if (NULL_PTR != pstConfig) {
        /* Set prescaler bits (TWPS0 and TWPS1) to 0 for Prescaler 1 */
        TWSR &= ~((1 << TWPS1) | (1 << TWPS0));

        /* Formula: TWBR = ((F_CPU / SCL) - 16) / (2 * Prescaler) */
        TWBR = (uint8_t)(((MCAL_GET_CPU_FREQ() / pstConfig->u32BaudRate_bps) - I2C_HDW_OVERHEAD_CYCLES) /
               (I2C_BIT_PHASE_FACTOR * pstConfig->u8Prescaler));

        /* Enable TWI Peripheral */
        TWCR = (1 << TWEN);
        
        enRetVal = I2C_OK;
    }
    
    return enRetVal;
}

/**
 * @brief Initiates a START condition on the I2C bus and sends the slave address.
 * * This function handles the low-level hardware sequence to take control of the I2C bus 
 * and address a specific device for a Write operation. It waits for the hardware 
 * flag (TWINT) to ensure the address is transmitted before returning.
 * * @param u8SlaveAddr_in 7-bit I2C address of the target device.
 * @return I2cStatus_t I2C_OK if the start and addressing were successful, 
 * I2C_ERROR if the bus is busy or the slave did not ACK.
 */
I2cStatus_t Mcal_I2c_Start(uint8_t u8SlaveAddr_in) {
	/* Send START condition */
	TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);
	
	/* Wait for TWINT Flag set */
	while (!(TWCR & (1 << TWINT)));
	
	/* Check status (0x08 = Start, 0x10 = Repeated Start) */
	if ((TWSR & 0xF8) != 0x08) return I2C_ERROR;
	
	/* Send Slave Address + Write bit */
	TWDR = u8SlaveAddr_in;
	TWCR = (1 << TWINT) | (1 << TWEN);
	
	while (!(TWCR & (1 << TWINT)));
	
	/* Check if ACK received (0x18 = SLA+W ACK) */
	if ((TWSR & 0xF8) != 0x18) return I2C_ERROR;
	
	return I2C_OK;
}

/**
 * @brief Transmits a single byte of data over the I2C bus to the previously addressed slave.
 * * This function loads the data into the TWI data register and triggers the transmission.
 * It blocks until the transfer is complete and verifies if the slave device sent an
 * Acknowledge (ACK) bit.
 * * @param u8Data_in The 8-bit data byte to be transmitted.
 * @return I2cStatus_t I2C_OK if the byte was sent and ACK was received, 
 * I2C_ERROR if a NACK was received or a bus error occurred.
 */
I2cStatus_t Mcal_I2c_Write(uint8_t u8Data_in) {
	/* Load data into register */
	TWDR = u8Data_in;
	TWCR = (1 << TWINT) | (1 << TWEN);
	
	while (!(TWCR & (1 << TWINT)));
	
	/* Check for ACK (0x28 = Data ACK) */
	if ((TWSR & 0xF8) != 0x28) return I2C_ERROR;
	
	return I2C_OK;
}

/**
 * @brief Terminates the I2C communication by transmitting a STOP condition.
 * * This function clears the I2C bus and releases the SDA and SCL lines to their 
 * idle (high) state. It blocks until the STOP condition has been 
 * fully executed by the hardware to prevent collisions with subsequent 
 * start attempts.
 * * @note Once this function is called, the bus is considered free.
 */
void Mcal_I2c_Stop(void) {
	/* Transmit STOP condition */
	TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO);
	/* Wait for stop to finish */
	while (TWCR & (1 << TWSTO));
}