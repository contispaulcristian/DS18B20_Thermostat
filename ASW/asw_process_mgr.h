/*
 * asw_process_mgr.h
 *
 * Created: 12/21/2025 3:40:05 PM
 *  Author: conti
 */ 


#ifndef ASW_PROCESS_MGR_H_
#define ASW_PROCESS_MGR_H_


#include "mcal_std_types.h"

/* Data structure to share system state with UI */
typedef struct {
	float fCurrentTemp;
	float fTargetTemp;
	Bool_t bHeaterOn;
	Bool_t bCoolerOn;
} SystemState_t;

/** @brief Initializes the process manager (default target temp) */
void Asw_Process_Init(void);

/** @brief Runs the main control logic (Hysteresis) */
void Asw_Process_Run(void);

/** @brief Setter for the User Interface to change target temp */
void Asw_Process_SetTargetTemp(float fNewTarget);

/** @brief Getter for the UI to read system status */
void Asw_Process_GetState(SystemState_t* pstState);

float Asw_Process_GetTemp1(void);
float Asw_Process_GetTemp2(void);
float Asw_Process_GetTemp3(void);
uint8_t Asw_Process_GetWarning(void);

#endif /* ASW_PROCESS_MGR_H_ */


