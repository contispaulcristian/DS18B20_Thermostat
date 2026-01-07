/*
 * hal_oled.h
 *
 * Created: 12/21/2025 3:29:09 PM
 *  Author: conti
 */ 


#ifndef HAL_OLED_H_
#define HAL_OLED_H_

#include "mcal_i2c.h"

/* --- OLED Definitions --- */
#define OLED_I2C_ADDR          0x3C
#define OLED_CMD_MODE          0x00
#define OLED_DATA_MODE         0x40

#define OLED_WIDTH             128
#define OLED_HEIGHT            32

/* --- Function Prototypes --- */
I2cStatus_t Hal_Oled_Init(void);
void Hal_Oled_Clear(void);

/** @brief Draws a string of text at X,Y coordinates */
void Hal_Oled_WriteString(uint8_t u8X, uint8_t u8Y, const char* pszStr);

/** @brief Draws a 16x16 icon at X,Y coordinates */
void Hal_Oled_DrawIcon(uint8_t u8X, uint8_t u8Y, const uint8_t* pu8Bitmap);

/* --- Icon Data Externs --- */
extern const uint8_t g_au8IconFire[32];
extern const uint8_t g_au8IconFan[32];

#endif /* HAL_OLED_H_ */