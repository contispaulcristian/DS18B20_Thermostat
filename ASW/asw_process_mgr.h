/*
 * asw_process_mgr.h
 *
 * Created: 12/21/2025 3:40:05 PM
 *  Author: conti
 */ 


#ifndef ASW_PROCESS_MGR_H_
#define ASW_PROCESS_MGR_H_

#include "mcal_std_types.h"

/**
 * @brief Executes the thermostat logic using hysteresis.
 * @param fCurrentTemp The current temperature from the sensor.
 * @param fTargetTemp The user-defined setpoint.
 */
void Asw_Process_ControlLoop(float fCurrentTemp, float fTargetTemp);


#endif /* ASW_PROCESS_MGR_H_ */