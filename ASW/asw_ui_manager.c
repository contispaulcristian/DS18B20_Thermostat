/*
 * asw_ui_manager.c
 *
 * Created: 12/21/2025 3:38:25 PM
 *  Author: conti
 */ 


#include "asw_ui_manager.h"
#include "asw_process_mgr.h"
#include "hal_encoder.h"
#include "hal_encoder_cfg.h"
#include "hal_oled.h"
#include <stdio.h>

/* --- Private Defines --- */
#define UI_CYCLE_INTERVAL_TICKS  150  /* 150 * 10ms = 1.5 Seconds per sensor */

/* --- Private Variables --- */
/* Local "Clear" icon (Black Box) to erase the warning area */
static const uint8_t s_au8IconClear[128] = {0};

/* UI State Machine */
typedef enum {
    UI_SCREEN_MONITOR,
    UI_SCREEN_SETTINGS
} UiScreen_t;

static UiScreen_t g_enCurrentScreen = UI_SCREEN_MONITOR;
static UiScreen_t s_enLastScreen    = UI_SCREEN_MONITOR;

/* Cycling Variables */
static uint16_t s_u16CycleTimer = 0;
static uint8_t  s_u8CycleIndex  = 0; /* 0=S1, 1=S2, 2=S3 */

void Asw_Ui_Init(void) {
    Hal_Oled_Init();
    Hal_Oled_Clear();
    Hal_Encoder_Init(&Hal_Encoder_DefaultCfg);
}

void Asw_Ui_Run(void) {
    SystemState_t stState;
    EncoderEvent_t enEvent;
    char acBuffer[16];
    
    /* 1. INPUTS */
    enEvent = Hal_Encoder_GetEvent(&Hal_Encoder_DefaultCfg);
    Asw_Process_GetState(&stState); /* Get main state (Heater/Cooler/Target) */

    /* 2. SCREEN SWITCHING LOGIC */
    if (g_enCurrentScreen == UI_SCREEN_MONITOR) {
        if (ENCODER_BTN_PRESSED == enEvent) {
            g_enCurrentScreen = UI_SCREEN_SETTINGS;
        }
    }
    else if (g_enCurrentScreen == UI_SCREEN_SETTINGS) {
        /* Handle Temp Adjustment */
        if (ENCODER_CW == enEvent) {
            Asw_Process_SetTargetTemp(stState.fTargetTemp + 0.5f);
        }
        else if (ENCODER_CCW == enEvent) {
            Asw_Process_SetTargetTemp(stState.fTargetTemp - 0.5f);
        }
        else if (ENCODER_BTN_PRESSED == enEvent) {
            g_enCurrentScreen = UI_SCREEN_MONITOR;
        }
    }

    /* 3. CLEAR SCREEN ON CHANGE */
    if (g_enCurrentScreen != s_enLastScreen) {
        Hal_Oled_Clear(); /* Wipe everything clean */
        s_enLastScreen = g_enCurrentScreen;
        /* Reset cycling timer so S1 always appears first when returning */
        s_u8CycleIndex = 0;
        s_u16CycleTimer = 0;
    }

    /* 4. DRAWING LOGIC */
    if (g_enCurrentScreen == UI_SCREEN_MONITOR) {
        
        /* --- A. Handle Cycling Timer --- */
        s_u16CycleTimer++;
        if (s_u16CycleTimer >= UI_CYCLE_INTERVAL_TICKS) {
            s_u16CycleTimer = 0;
            s_u8CycleIndex++;
            if (s_u8CycleIndex > 2) s_u8CycleIndex = 0;
        }

        /* --- B. Get the correct Sensor Value --- */
        float fDisplayTemp = 0.0f;
        if (s_u8CycleIndex == 0)      fDisplayTemp = Asw_Process_GetTemp1();
        else if (s_u8CycleIndex == 1) fDisplayTemp = Asw_Process_GetTemp2();
        else                          fDisplayTemp = Asw_Process_GetTemp3();

        /* --- C. Draw Text (Manual Float Formatting to fix "?") --- */
        /* Logic: Split into Integer Part and Decimal Part */
        int iTempInt = (int)fDisplayTemp;
        int iTempDec = (int)(fDisplayTemp * 10) % 10;
        if (iTempDec < 0) iTempDec = -iTempDec; /* Handle negative decimals */

        /* Format: "S1: 24.5 C " */
        sprintf(acBuffer, "S%d: %d.%d C ", s_u8CycleIndex + 1, iTempInt, iTempDec);
        Hal_Oled_WriteString(0, 1, acBuffer);


        /* --- D. Draw Warning Icon (Middle) --- */
        if (Asw_Process_GetWarning() == 1) {
            Hal_Oled_DrawIcon32(64, g_au8IconWarning); /* X=64 */
        } else {
            Hal_Oled_DrawIcon32(64, s_au8IconClear);   /* Erase if safe */
        }


        /* --- E. Draw Status Icon (Right) --- */
        if (stState.bHeaterOn)      Hal_Oled_DrawIcon32(96, g_au8IconFire);
        else if (stState.bCoolerOn) Hal_Oled_DrawIcon32(96, g_au8IconFan);
        else                        Hal_Oled_DrawIcon32(96, g_au8IconSmiley);
    }
    else if (g_enCurrentScreen == UI_SCREEN_SETTINGS) {
        /* --- Settings Screen Drawing --- */
        Hal_Oled_WriteString(10, 0, "Set Temperature:");

        /* Manual Float Format for Target Temp */
        int iTgtInt = (int)stState.fTargetTemp;
        int iTgtDec = (int)(stState.fTargetTemp * 10) % 10;
        
        sprintf(acBuffer, "%d.%d C", iTgtInt, iTgtDec);
        Hal_Oled_WriteString(40, 2, acBuffer);
    }
}