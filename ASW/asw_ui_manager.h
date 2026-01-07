/*
 * asw_ui_manager.h
 *
 * Created: 12/21/2025 3:38:37 PM
 *  Author: conti
 */ 


#ifndef ASW_UI_MANAGER_H_
#define ASW_UI_MANAGER_H_

#include "mcal_std_types.h"

/** @brief Handles encoder input and toggles between READ and SETTINGS mode */
void Asw_Ui_HandleInput(float* pfTargetTemp);

/** @brief Updates OLED with Blink logic, Icons, and ON/OFF status */
void Asw_Ui_UpdateDisplay(float fCurrentTemp, float fTargetTemp, Bool_t bHeaterOn, Bool_t bCoolerOn);


#endif /* ASW_UI_MANAGER_H_ */