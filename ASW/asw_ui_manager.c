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

/* UI States */
typedef enum {
	UI_SCREEN_MONITOR,
	UI_SCREEN_SETTINGS
} UiScreen_t;

static UiScreen_t g_enCurrentScreen = UI_SCREEN_MONITOR;

void Asw_Ui_Init(void) {
	Hal_Oled_Init();
	Hal_Oled_Clear();
	Hal_Encoder_Init(&Hal_Encoder_DefaultCfg);
}

/* Global Static variables to remember what is currently on screen */
static float s_fLastTemp = -99.0f;
static float s_fLastTarget = -99.0f;
static Bool_t s_bLastHeater = FALSE;
static Bool_t s_bLastCooler = FALSE;
static UiScreen_t s_enLastScreen = UI_SCREEN_MONITOR;

void Asw_Ui_Run(void) {
	SystemState_t stState;
	EncoderEvent_t enEvent;
	char au8Buffer[16];
	Bool_t bRedrawNeeded = FALSE;

	/* Get Inputs */
	enEvent = Hal_Encoder_GetEvent(&Hal_Encoder_DefaultCfg);
	Asw_Process_GetState(&stState);

	/* Handle Logic (Screen Switching) */
	if (g_enCurrentScreen == UI_SCREEN_MONITOR) {
		if (ENCODER_BTN_PRESSED == enEvent) {
			g_enCurrentScreen = UI_SCREEN_SETTINGS;
			bRedrawNeeded = TRUE; /* Force redraw on screen switch */
		}
	}
	else if (g_enCurrentScreen == UI_SCREEN_SETTINGS) {
		if (ENCODER_CW == enEvent) Asw_Process_SetTargetTemp(stState.fTargetTemp + 0.5f);
		else if (ENCODER_CCW == enEvent) Asw_Process_SetTargetTemp(stState.fTargetTemp - 0.5f);
		else if (ENCODER_BTN_PRESSED == enEvent) {
			g_enCurrentScreen = UI_SCREEN_MONITOR;
			bRedrawNeeded = TRUE;
		}
	}

	/* Check for Data Changes (Anti-Flicker Logic) */
	if (g_enCurrentScreen != s_enLastScreen) {
		bRedrawNeeded = TRUE;
		s_enLastScreen = g_enCurrentScreen;
		Hal_Oled_Clear(); /* Only Clear if changing screens! */
	}

	if (g_enCurrentScreen == UI_SCREEN_MONITOR) {
		/* Check if Temp or Relay status changed */
		if ((stState.fCurrentTemp != s_fLastTemp) ||
		(stState.bHeaterOn != s_bLastHeater) ||
		(stState.bCoolerOn != s_bLastCooler) ||
		(TRUE == bRedrawNeeded)) {
			
			/* Update Cache */
			s_fLastTemp = stState.fCurrentTemp;
			s_bLastHeater = stState.bHeaterOn;
			s_bLastCooler = stState.bCoolerOn;

			/* DRAW MONITOR SCREEN */
			sprintf(au8Buffer, "%d.%d", (int)stState.fCurrentTemp, (int)(stState.fCurrentTemp * 10) % 10);
			Hal_Oled_WriteString(0, 1, au8Buffer);
			Hal_Oled_WriteString(30, 1, "{C");

			if (TRUE == stState.bHeaterOn) Hal_Oled_DrawIcon32(96, g_au8IconFire);
			else if (TRUE == stState.bCoolerOn) Hal_Oled_DrawIcon32(96, g_au8IconFan);
			else Hal_Oled_DrawIcon32(96, g_au8IconSmiley);
		}
	}
	else if (g_enCurrentScreen == UI_SCREEN_SETTINGS) {
		/* Check if Target Temp changed */
		if ((stState.fTargetTemp != s_fLastTarget) || (TRUE == bRedrawNeeded)) {
			
			/* Update Cache */
			s_fLastTarget = stState.fTargetTemp;

			/* DRAW SETTINGS SCREEN */
			Hal_Oled_WriteString(10, 0, "Set Temperature:");
			sprintf(au8Buffer, "%d.%d {C", (int)stState.fTargetTemp, (int)(stState.fTargetTemp * 10) % 10);
			Hal_Oled_WriteString(40, 2, au8Buffer);
		}
	}
}
