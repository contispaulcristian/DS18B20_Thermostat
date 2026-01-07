/*
 * asw_process_mgr.c
 *
 * Created: 12/21/2025 3:39:56 PM
 *  Author: conti
 */ 

/*
 * asw_process_mgr.c
 */ 

#include "asw_process_mgr.h"
#include "hal_relay.h"
#include "mcal_gpio_cfg.h"

#define ASW_TEMP_HYSTERESIS    0.5f

void Asw_Process_ControlLoop(float fCurrentTemp, float fTargetTemp) {
    /* Heating Logic: Turn ON if too cold */
    if (fCurrentTemp < (fTargetTemp - ASW_TEMP_HYSTERESIS)) {
        Hal_Relay_TurnOn(&Mcal_RelayHeater);
        Hal_Relay_TurnOff(&Mcal_RelayCooler);
    } 
    /* Cooling Logic: Turn ON if too hot */
    else if (fCurrentTemp > (fTargetTemp + ASW_TEMP_HYSTERESIS)) {
        Hal_Relay_TurnOff(&Mcal_RelayHeater);
        Hal_Relay_TurnOn(&Mcal_RelayCooler);
    } 
    /* Comfort Zone: Turn both OFF once target is reached */
    else if (fCurrentTemp == fTargetTemp) {
        Hal_Relay_TurnOff(&Mcal_RelayHeater);
        Hal_Relay_TurnOff(&Mcal_RelayCooler);
    }
}