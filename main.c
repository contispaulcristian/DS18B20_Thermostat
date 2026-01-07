/*
 * Thermostat.c
 *
 * Created: 12/21/2025 12:44:41 PM
 * Author : conti
 */ 

#include "mcal_clock.h"
#include "mcal_i2c_cfg.h"
#include "mcal_gpio_cfg.h"
#include "hal_oled.h"
#include "hal_ds18b20.h"
#include "hal_encoder_cfg.h"
#include "hal_relay.h"
#include "asw_ui_manager.h"
#include "asw_process_mgr.h"
#include <util/delay.h>

int main(void) {
	float fCurrentTemp = 0.0f;
	float fTargetTemp  = 26.0f;

	/* Initializations */
	(void)Mcal_I2c_Init(&Mcal_I2c_DefaultCfg);
	(void)Hal_Oled_Init();
	Hal_Encoder_Init(&Hal_Encoder_DefaultCfg);
	(void)Hal_Ds18b20_Init(&Mcal_Ds18b20Bus);
	
	Hal_Relay_Init(&Mcal_RelayHeater);
	Hal_Relay_Init(&Mcal_RelayCooler);
	
	Hal_Oled_Clear();

	while (1) {
		/* 1. Update Temperature */
		(void)Hal_Ds18b20_ReadTemp(&Mcal_Ds18b20Bus, &fCurrentTemp);

		/* 2. Handle User Input */
		Asw_Ui_HandleInput(&fTargetTemp);

		/* 3. Run Control Logic */
		Asw_Process_ControlLoop(fCurrentTemp, fTargetTemp);

		/* 4. Refresh Screen */
		Asw_Ui_UpdateDisplay(fCurrentTemp, fTargetTemp,
		Mcal_Gpio_Read(&Mcal_RelayHeater),
		Mcal_Gpio_Read(&Mcal_RelayCooler));

		_delay_ms(100);
	}
	return 0;
}
