/*
 * hal_ds18b20.c
 *
 * Created: 12/21/2025 3:13:29 PM
 *  Author: conti
 */ 

#include "mcal_clock.h"    
#include "hal_ds18b20.h"
#include <util/delay.h>

/* --- Private 1-Wire Timing Constants --- */
#define DS18B20_RESET_PULSE_US        480U
#define DS18B20_PRESENCE_WAIT_US      80U
#define DS18B20_PRESENCE_FINISH_US    400U
#define DS18B20_WRITE_1_LOW_US        10U
#define DS18B20_WRITE_1_HIGH_US       55U
#define DS18B20_WRITE_0_LOW_US        65U
#define DS18B20_READ_SAMPLE_US        10U
#define DS18B20_CONVERSION_TIME_MS    750U

/* --- Static Private Function Prototypes --- */
static Std_ReturnType_t Hal_Ds18b20_Reset(const Mcal_Gpio_Config_t* pstPin);
static void Hal_Ds18b20_WriteBit(const Mcal_Gpio_Config_t* pstPin, uint8_t u8Bit_in);
static uint8_t Hal_Ds18b20_ReadBit(const Mcal_Gpio_Config_t* pstPin);
static void Hal_Ds18b20_WriteByte(const Mcal_Gpio_Config_t* pstPin, uint8_t u8Data_in);
static uint8_t Hal_Ds18b20_ReadByte(const Mcal_Gpio_Config_t* pstPin);

/**
 * @brief Performs a hardware check to see if the DS18B20 is present on the bus.
 * @param pstPin Pointer to the GPIO configuration for the 1-Wire bus.
 * @return E_OK if the sensor responded with a presence pulse, E_NOT_OK otherwise.
 */
Std_ReturnType_t Hal_Ds18b20_Init(const Mcal_Gpio_Config_t* pstPin) {
    /* In 1-Wire, Initialization is a Reset pulse to detect the device */
    return Hal_Ds18b20_Reset(pstPin);
}

/**
 * @brief Resets the DS18B20 sensor and checks for the Presence Pulse.
 * @param pstPin Pointer to the GPIO configuration.
 * @return E_OK if presence pulse detected.
 */
static Std_ReturnType_t Hal_Ds18b20_Reset(const Mcal_Gpio_Config_t* pstPin) {
    Std_ReturnType_t enRetVal = E_NOT_OK;

    if (NULL_PTR != pstPin) {
        /* Master pulls bus low for reset pulse */
        Mcal_Gpio_Init(pstPin, GPIO_OUTPUT);
        Mcal_Gpio_Write(pstPin, GPIO_LOW);
        _delay_us(DS18B20_RESET_PULSE_US);

        /* Release bus and switch to input to listen for sensor */
        Mcal_Gpio_Init(pstPin, GPIO_INPUT);
        _delay_us(DS18B20_PRESENCE_WAIT_US);

        /* Sensor pulls bus low if present */
        if (GPIO_LOW == Mcal_Gpio_Read(pstPin)) {
            enRetVal = E_OK;
        }
        
        _delay_us(DS18B20_PRESENCE_FINISH_US);
    }
    return enRetVal;
}

/**
 * @brief Writes a single bit to the DS18B20 using 1-Wire time slots.
 * @param pstPin Pointer to GPIO config.
 * @param u8Bit_in Bit value (0 or 1).
 */
static void Hal_Ds18b20_WriteBit(const Mcal_Gpio_Config_t* pstPin, uint8_t u8Bit_in) {
    Mcal_Gpio_Init(pstPin, GPIO_OUTPUT);
    Mcal_Gpio_Write(pstPin, GPIO_LOW);
    
    if (u8Bit_in) {
        /* Write 1 Slot */
        _delay_us(DS18B20_WRITE_1_LOW_US);
        Mcal_Gpio_Init(pstPin, GPIO_INPUT);
        _delay_us(DS18B20_WRITE_1_HIGH_US);
    } else {
        /* Write 0 Slot */
        _delay_us(DS18B20_WRITE_0_LOW_US);
        Mcal_Gpio_Init(pstPin, GPIO_INPUT);
        _delay_us(5);
    }
}

/**
 * @brief Reads a single bit from the DS18B20.
 * @return uint8_t Sampled bit (0 or 1).
 */
static uint8_t Hal_Ds18b20_ReadBit(const Mcal_Gpio_Config_t* pstPin) {
    uint8_t u8BitValue = 0;

    Mcal_Gpio_Init(pstPin, GPIO_OUTPUT);
    Mcal_Gpio_Write(pstPin, GPIO_LOW);
    _delay_us(2); /* Initiation pulse */

    Mcal_Gpio_Init(pstPin, GPIO_INPUT);
    _delay_us(DS18B20_READ_SAMPLE_US);

    if (GPIO_HIGH == Mcal_Gpio_Read(pstPin)) {
        u8BitValue = 1;
    }
    
    _delay_us(50); /* Complete 60us slot */
    return u8BitValue;
}

/**
 * @brief Writes a full byte to the DS18B20 (LSB first).
 */
static void Hal_Ds18b20_WriteByte(const Mcal_Gpio_Config_t* pstPin, uint8_t u8Data_in) {
    for (uint8_t u8Idx = 0; u8Idx < 8; u8Idx++) {
        Hal_Ds18b20_WriteBit(pstPin, (u8Data_in >> u8Idx) & 0x01);
    }
}

/**
 * @brief Reads a full byte from the DS18B20.
 */
static uint8_t Hal_Ds18b20_ReadByte(const Mcal_Gpio_Config_t* pstPin) {
    uint8_t u8Result = 0;
    for (uint8_t u8Idx = 0; u8Idx < 8; u8Idx++) {
        if (Hal_Ds18b20_ReadBit(pstPin)) {
            u8Result |= (1 << u8Idx);
        }
    }
    return u8Result;
}

/**
 * @brief Reads the temperature and converts it to Celsius.
 * @param pstPin Pointer to the 1-Wire bus pin.
 * @param pfTemp_out Pointer to float result.
 * @return E_OK if communication succeeded.
 */
Std_ReturnType_t Hal_Ds18b20_ReadTemp(const Mcal_Gpio_Config_t* pstPin, float* pfTemp_out) {
    uint8_t u8Lsb, u8Msb;
    int16_t i16RawTemp;
    Std_ReturnType_t enStatus = E_NOT_OK;

    if ((NULL_PTR != pstPin) && (NULL_PTR != pfTemp_out)) {
        /* Start Conversion Process */
        if (E_OK == Hal_Ds18b20_Reset(pstPin)) {
            Hal_Ds18b20_WriteByte(pstPin, DS18B20_CMD_SKIP_ROM);
            Hal_Ds18b20_WriteByte(pstPin, DS18B20_CMD_CONVERT_T);
            
            _delay_ms(DS18B20_CONVERSION_TIME_MS);

            /* Read Data from Scratchpad */
            if (E_OK == Hal_Ds18b20_Reset(pstPin)) {
                Hal_Ds18b20_WriteByte(pstPin, DS18B20_CMD_SKIP_ROM);
                Hal_Ds18b20_WriteByte(pstPin, DS18B20_CMD_READ_SCRATCHPAD);

                u8Lsb = Hal_Ds18b20_ReadByte(pstPin);
                u8Msb = Hal_Ds18b20_ReadByte(pstPin);

                /* Combine and calculate Celsius (0.0625 deg per bit) */
                i16RawTemp = (int16_t)((u8Msb << 8) | u8Lsb);
                *pfTemp_out = (float)i16RawTemp * 0.0625f;
                
                enStatus = E_OK;
            }
        }
    }
    return enStatus;
}