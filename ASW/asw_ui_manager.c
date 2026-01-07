/*
 * asw_ui_manager.c
 *
 * Created: 12/21/2025 3:38:25 PM
 *  Author: conti
 */ 

#include "asw_ui_manager.h"
#include "hal_encoder.h"
#include "hal_encoder_cfg.h"
#include "hal_oled.h"
#include "mcal_gpio_cfg.h"
#include <stdio.h>

/* --- Private Variables --- */
static Bool_t bIsSettingsMode = FALSE;
static uint8_t u8BlinkCounter = 0;

void Asw_Ui_HandleInput(float* pfTargetTemp) {
    if (NULL_PTR != pfTargetTemp) {
        EncoderEvent_t enEvent = Hal_Encoder_GetEvent(&Hal_Encoder_DefaultCfg);

        /* Toggle Settings Mode on Button Press */
        if (ENCODER_BTN_PRESSED == enEvent) {
            bIsSettingsMode = !bIsSettingsMode;
        }

        /* Adjust Threshold only if in Settings Mode */
        if (TRUE == bIsSettingsMode) {
            if (ENCODER_CW == enEvent) {
                *pfTargetTemp += 0.5f; /* Increment by 0.5C */
            } 
            else if (ENCODER_CCW == enEvent) {
                *pfTargetTemp -= 0.5f; /* Decrement by 0.5C */
            }
        }
    }
}

void Asw_Ui_UpdateDisplay(float fCurrentTemp, float fTargetTemp, Bool_t bHeaterOn, Bool_t bCoolerOn) {
    char au8Buffer[16];
    u8BlinkCounter++;

    /* --- TOP ROW: CURRENT TEMPERATURE --- */
    /* Blink "Temp" word if in Settings Mode */
    if ((FALSE == bIsSettingsMode) || (u8BlinkCounter % 10 < 5)) {
        Hal_Oled_WriteString(0, 0, "Temp:"); 
    } else {
        Hal_Oled_WriteString(0, 0, "     "); 
    }

    /* Print Current Temp: e.g., "22.5 *C" */
    sprintf(au8Buffer, "%d.%d *C", (int)fCurrentTemp, (int)(fCurrentTemp * 10) % 10);
    Hal_Oled_WriteString(35, 0, au8Buffer);

    /* --- MIDDLE ROWS: STATUS & ICONS --- */
    
    /* Heater Section (Fire Icon + Status) */
    if (TRUE == bHeaterOn) {
        Hal_Oled_DrawIcon(90, 0, g_au8IconFire);
        Hal_Oled_WriteString(110, 0, "ON ");
    } else {
        Hal_Oled_WriteString(110, 0, "OFF");
    }

    /* Fan Section (Fan Icon + Status) */
    if (TRUE == bCoolerOn) {
        Hal_Oled_DrawIcon(90, 2, g_au8IconFan);
        Hal_Oled_WriteString(110, 2, "ON ");
    } else {
        Hal_Oled_WriteString(110, 2, "OFF");
    }

    /* --- BOTTOM ROW: SETTINGS FEEDBACK --- */
    if (TRUE == bIsSettingsMode) {
        /* Show the current threshold being set */
        sprintf(au8Buffer, "SET: %d.%d", (int)fTargetTemp, (int)(fTargetTemp * 10) % 10);
        Hal_Oled_WriteString(0, 3, au8Buffer);
    } else {
        /* Clear the settings line when in Read Mode */
        Hal_Oled_WriteString(0, 3, "          ");
    }
}