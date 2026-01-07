/*
 * mcal_i2c.h
 *
 * Created: 12/21/2025 3:21:08 PM
 *  Author: conti
 */ 


#ifndef MCAL_I2C_H_
#define MCAL_I2C_H_

#include "mcal_std_types.h"
#include "mcal_clock.h"

/* --- I2C Timing Constants --- */
/** @brief Internal TWI unit overhead in clock cycles */
#define I2C_HDW_OVERHEAD_CYCLES    16U

/** @brief Clock pulses per bit (High and Low phases) */
#define I2C_BIT_PHASE_FACTOR       2U

/** @brief Default Prescaler value (TWPS bits = 00) */
#define I2C_PRESCALER_1            1U

/* I2C Status Definitions */
typedef enum {
    I2C_OK    = 0U,
    I2C_ERROR = 1U,
    I2C_BUSY  = 2U
} I2cStatus_t;

/** * @brief Configuration structure for I2C Initialization
 */
typedef struct {
    uint32_t u32BaudRate_bps;   /**< Bitrate in bits per second     */
    uint8_t  u8Prescaler;       /**< Prescaler value (1, 4, 16, 64) */
} Mcal_I2c_Config_t;

/* --- Function Prototypes --- */

/**
 * @brief Initializes I2C using a configuration structure.
 * @param pstConfig Pointer to the configuration struct.
 * @return I2C_OK if successful.
 */
I2cStatus_t Mcal_I2c_Init(const Mcal_I2c_Config_t* pstConfig);

/**
 * @brief Starts I2C communication and sends the address.
 * @param u8SlaveAddr_in 7-bit slave address.
 * @return tenI2cStatus 
 */
I2cStatus_t Mcal_I2c_Start(uint8_t u8SlaveAddr_in);

/**
 * @brief Sends a single byte over the I2C bus.
 * @param u8Data_in Byte to send.
 */
I2cStatus_t Mcal_I2c_Write(uint8_t u8Data_in);

/**
 * @brief Stops I2C communication.
 */
void Mcal_I2c_Stop(void);


#endif /* MCAL_I2C_H_ */


