/*
 * asw_ui_manager.h
 *
 * Created: 12/21/2025 3:38:37 PM
 *  Author: conti
 */ 


#ifndef ASW_UI_MANAGER_H_
#define ASW_UI_MANAGER_H_


#include "mcal_std_types.h"

/* --- Function Prototypes --- */

/** * @brief Initializes the User Interface.
 * Sets up the OLED display (clears it) and the Encoder hardware.
 * Should be called once during system startup.
 */
void Asw_Ui_Init(void);

/** * @brief Main UI Task.
 * 1. Polls the Encoder for input.
 * 2. Manages the UI State Machine (Monitor Mode vs. Settings Mode).
 * 3. Updates the OLED display based on the current state and process data.
 * * This function should be called cyclically (e.g., every 10ms) to ensure
 * fluid response to button presses.
 */
void Asw_Ui_Run(void);


#endif /* ASW_UI_MANAGER_H_ */