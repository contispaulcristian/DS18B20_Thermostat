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
#define OLED_I2C_ADDR          0x78
#define OLED_CMD_MODE          0x00
#define OLED_DATA_MODE         0x40

#define OLED_WIDTH             128
#define OLED_HEIGHT            32

/* --- Function Prototypes --- */
I2cStatus_t Hal_Oled_Init(void);
void Hal_Oled_Clear(void);

/** @brief Draws a string of text at X,Y coordinates */
void Hal_Oled_WriteString(uint8_t u8X, uint8_t u8Y, const char* pszStr);

/** * @brief Draws a large 32x32 icon (Spans all 4 pages vertically).
 * @param u8X Horizontal start position (0-96).
 * @param pu8Bitmap Pointer to the 128-byte icon array.
 */
void Hal_Oled_DrawIcon32(uint8_t u8X, const uint8_t* pu8Bitmap);

/* --- 32x32 Icon Data Externs --- */
extern const uint8_t g_au8IconFire[128];
extern const uint8_t g_au8IconFan[128];
extern const uint8_t g_au8IconSmiley[128];

#endif /* HAL_OLED_H_ */




