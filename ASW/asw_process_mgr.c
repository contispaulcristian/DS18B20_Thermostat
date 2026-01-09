/*
 * asw_process_mgr.c
 *
 * Created: 12/21/2025 3:39:56 PM
 *  Author: conti
 */ 


#include "asw_process_mgr.h"
#include "hal_ds18b20.h"
#include "hal_relay.h"
#include "mcal_gpio_cfg.h"

#define HYSTERESIS_START_GAP  0.5f  
#define HYSTERESIS_STOP_GAP   0.5f 
#define FAN_SAFETY_GAP        1.0f  

/* Sensor IDs */
const uint8_t u8Sensor1_ID[8] = {0x28, 0x31, 0x45, 0x47, 0x00, 0x00, 0x00, 0x99};
const uint8_t u8Sensor2_ID[8] = {0x28, 0xB7, 0x3D, 0x47, 0x00, 0x00, 0x00, 0x92};
const uint8_t u8Sensor3_ID[8] = {0x28, 0x7C, 0x20, 0x47, 0x00, 0x00, 0x00, 0x65};

/* State Variables */
typedef enum { TEMP_STATE_IDLE = 0, TEMP_STATE_WAITING } TempState_t;
static TempState_t g_enTempState = TEMP_STATE_IDLE;
static uint16_t    g_u16WaitCounter = 0;

static float   g_fTargetTemp = 26.0f;
static float   g_fCurrentAvgTemp = 0.0f; 
static uint8_t g_bHeaterState = 0;    
static uint8_t g_bCoolerState = 0;
static uint8_t g_bWarningActive = 0;

/* Store individual temps so UI can read them */
static float g_fTemp1 = 0.0f;
static float g_fTemp2 = 0.0f;
static float g_fTemp3 = 0.0f;

static float GetDiff(float a, float b) {
    return (a > b) ? (a - b) : (b - a);
}

void Asw_Process_Init(void) {
    Hal_Relay_Init(&Mcal_RelayHeater);
    Hal_Relay_Init(&Mcal_RelayCooler);
    /* Note: Hal_Oled_Init is moved to UI Manager usually, or Main */
    
    Hal_Ds18b20_StartConversion(&Mcal_Ds18b20Bus);
    g_enTempState = TEMP_STATE_WAITING;
}

void Asw_Process_Run(void) {
    switch(g_enTempState) {
        case TEMP_STATE_IDLE:
            Hal_Ds18b20_StartConversion(&Mcal_Ds18b20Bus);
            g_u16WaitCounter = 0;
            g_enTempState = TEMP_STATE_WAITING;
            break;

        case TEMP_STATE_WAITING:
            g_u16WaitCounter++;
            if (g_u16WaitCounter >= 75) {
                /* READ SENSORS */
                Hal_Ds18b20_ReadResult(&Mcal_Ds18b20Bus, u8Sensor1_ID, &g_fTemp1);
                Hal_Ds18b20_ReadResult(&Mcal_Ds18b20Bus, u8Sensor2_ID, &g_fTemp2);
                Hal_Ds18b20_ReadResult(&Mcal_Ds18b20Bus, u8Sensor3_ID, &g_fTemp3);

                /* AVERAGE */
                g_fCurrentAvgTemp = (g_fTemp1 + g_fTemp2 + g_fTemp3) / 3.0f;

                /* WARNING CHECK */
                if (GetDiff(g_fTemp1, g_fTemp2) > 0.5f || 
                    GetDiff(g_fTemp1, g_fTemp3) > 0.5f || 
                    GetDiff(g_fTemp2, g_fTemp3) > 0.5f) {
                    g_bWarningActive = 1;
                } else {
                    g_bWarningActive = 0;
                }

                /* LOGIC */
                if (g_fCurrentAvgTemp <= (g_fTargetTemp - HYSTERESIS_START_GAP)) {
                    g_bHeaterState = 1;
                } else if (g_fCurrentAvgTemp >= (g_fTargetTemp + HYSTERESIS_STOP_GAP)) {
                    g_bHeaterState = 0;
                }

                if (g_fCurrentAvgTemp >= (g_fTargetTemp + FAN_SAFETY_GAP)) {
                    g_bCoolerState = 1;
                } else if (g_fCurrentAvgTemp <= (g_fTargetTemp + HYSTERESIS_STOP_GAP)) {
                    g_bCoolerState = 0;
                }

                if (g_bCoolerState == 1) g_bHeaterState = 0;

                /* HARDWARE */
                if (g_bHeaterState) Hal_Relay_TurnOn(&Mcal_RelayHeater);
                else Hal_Relay_TurnOff(&Mcal_RelayHeater);

                if (g_bCoolerState) Hal_Relay_TurnOn(&Mcal_RelayCooler);
                else Hal_Relay_TurnOff(&Mcal_RelayCooler);

                g_enTempState = TEMP_STATE_IDLE;
            }
            break;
    }
    /* NO DRAWING CODE HERE! */
}

void Asw_Process_SetTargetTemp(float fNewTarget) { g_fTargetTemp = fNewTarget; }

/* Expanded this function to give all data to the UI */
void Asw_Process_GetState(SystemState_t* pstState) {
    if (pstState) {
        pstState->fCurrentTemp = g_fCurrentAvgTemp; /* Still the main average */
        pstState->fTargetTemp  = g_fTargetTemp;
        pstState->bHeaterOn    = (g_bHeaterState == 1);
        pstState->bCoolerOn    = (g_bCoolerState == 1);
        
        /* New fields you likely need to add to SystemState_t in asw_process_mgr.h */
        /* If SystemState_t doesn't have these, UI will use fCurrentTemp, 
           but we need a way to pass the 3 separate temps. */
    }
}

/* Helper getters so UI can read individual sensors */
float Asw_Process_GetTemp1(void) { return g_fTemp1; }
float Asw_Process_GetTemp2(void) { return g_fTemp2; }
float Asw_Process_GetTemp3(void) { return g_fTemp3; }
uint8_t Asw_Process_GetWarning(void) { return g_bWarningActive; }