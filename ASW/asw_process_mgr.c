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

#define HYSTERESIS_START_GAP  0.5f  /* Heater ON:  Target - 0.5 */
#define HYSTERESIS_STOP_GAP   0.5f  /* Heater OFF: Target + 0.5 */
#define FAN_SAFETY_GAP        1.0f  /* Fan ON:     Target + 1.0 */

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
				
				/* --- NEW CONTROL LOGIC --- */
				
				/* HEATER LOGIC */
				/* ON if drops below Target - 0.5 */
				if (g_fCurrentTemp <= (g_fTargetTemp - HYSTERESIS_START_GAP)) {
					g_bHeaterState = TRUE;
				}
				/* OFF if rises above Target + 0.5 */
				else if (g_fCurrentTemp >= (g_fTargetTemp + HYSTERESIS_STOP_GAP)) {
					g_bHeaterState = FALSE;
				}
				/* If between -0.5 and +0.5, keep previous g_bHeaterState */

				/* COOLER LOGIC */
				/* ON only if it overshoots to Target + 1.0 */
				if (g_fCurrentTemp >= (g_fTargetTemp + FAN_SAFETY_GAP)) {
					g_bCoolerState = TRUE;
				}
				/* OFF once it cools back down to Target + 0.5 */
				else if (g_fCurrentTemp <= (g_fTargetTemp + HYSTERESIS_STOP_GAP)) {
					g_bCoolerState = FALSE;
				}
				/* If between +0.5 and +1.0, keep previous g_bCoolerState */

				/* SAFETY INTERLOCK */
				/* If Fan needs to run, force Heater OFF */
				if (g_bCoolerState == TRUE) {
					g_bHeaterState = FALSE;
				}

				/* APPLY TO HARDWARE */
				if (g_bHeaterState == TRUE) {
					Hal_Relay_TurnOn(&Mcal_RelayHeater);
					} else {
					Hal_Relay_TurnOff(&Mcal_RelayHeater);
				}

				if (g_bCoolerState == TRUE) {
					Hal_Relay_TurnOn(&Mcal_RelayCooler);
					} else {
					Hal_Relay_TurnOff(&Mcal_RelayCooler);
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