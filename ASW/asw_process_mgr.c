/*
 * asw_process_mgr.c
 *
 * Created: 12/21/2025 3:39:56 PM
 *  Author: conti
 */ 


/* asw_process_mgr.c */
#include "asw_process_mgr.h"
#include "hal_ds18b20.h"
#include "hal_relay.h"
#include "mcal_gpio_cfg.h"

#define ASW_TEMP_HYSTERESIS    0.5f

/* State Machine definitions */
typedef enum {
	TEMP_STATE_IDLE = 0,
	TEMP_STATE_WAITING
} TempState_t;

/* Private Variables */
static float g_fTargetTemp = 26.0f;
static float g_fCurrentTemp = 0.0f;
static Bool_t g_bHeaterState = FALSE;
static Bool_t g_bCoolerState = FALSE;

/* State Machine Variables */
static TempState_t g_enTempState = TEMP_STATE_IDLE;
static uint16_t g_u16WaitCounter = 0;

void Asw_Process_Init(void) {
	Hal_Relay_Init(&Mcal_RelayHeater);
	Hal_Relay_Init(&Mcal_RelayCooler);
	/* Start the first conversion immediately */
	Hal_Ds18b20_StartConversion(&Mcal_Ds18b20Bus);
	g_enTempState = TEMP_STATE_WAITING;
}

/* This function must be called every 10ms */
void Asw_Process_Run(void) {
	
	switch(g_enTempState) {
		case TEMP_STATE_IDLE:
		/* Start a new measurement */
		Hal_Ds18b20_StartConversion(&Mcal_Ds18b20Bus);
		g_u16WaitCounter = 0;
		g_enTempState = TEMP_STATE_WAITING;
		break;

		case TEMP_STATE_WAITING:
		g_u16WaitCounter++;
		/* Wait for 75 ticks (75 * 10ms = 750ms) */
		if (g_u16WaitCounter >= 75) {
			/* Time is up! Read the data */
			if (E_OK == Hal_Ds18b20_ReadResult(&Mcal_Ds18b20Bus, &g_fCurrentTemp)) {
				/* Data is valid, run Logic immediately */
				
				/* --- CONTROL LOGIC --- */
				if (g_fCurrentTemp < (g_fTargetTemp - ASW_TEMP_HYSTERESIS)) {
					Hal_Relay_TurnOn(&Mcal_RelayHeater);
					Hal_Relay_TurnOff(&Mcal_RelayCooler);
					g_bHeaterState = TRUE;
					g_bCoolerState = FALSE;
				}
				else if (g_fCurrentTemp > (g_fTargetTemp + ASW_TEMP_HYSTERESIS)) {
					Hal_Relay_TurnOff(&Mcal_RelayHeater);
					Hal_Relay_TurnOn(&Mcal_RelayCooler);
					g_bHeaterState = FALSE;
					g_bCoolerState = TRUE;
				}
				else {
					Hal_Relay_TurnOff(&Mcal_RelayHeater);
					Hal_Relay_TurnOff(&Mcal_RelayCooler);
					g_bHeaterState = FALSE;
					g_bCoolerState = FALSE;
				}
			}
			/* Go back to IDLE to start next loop */
			g_enTempState = TEMP_STATE_IDLE;
		}
		break;
	}
}

/* ... Keep SetTargetTemp and GetState functions the same ... */
void Asw_Process_SetTargetTemp(float fNewTarget) { g_fTargetTemp = fNewTarget; }
void Asw_Process_GetState(SystemState_t* pstState) {
	if (pstState) {
		pstState->fCurrentTemp = g_fCurrentTemp;
		pstState->fTargetTemp = g_fTargetTemp;
		pstState->bHeaterOn = g_bHeaterState;
		pstState->bCoolerOn = g_bCoolerState;
	}
}