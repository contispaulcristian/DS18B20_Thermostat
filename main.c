/*
 * Thermostat.c
 *
 * Created: 12/21/2025 12:44:41 PM
 * Author : conti
 */ 

#include "BSW/MCAL/mcal_std_types.h"
#include "BSW/MCAL/mcal_clock.h"    /* Provides F_CPU for delay.h */
#include "BSW/MCAL/mcal_gpio.h"
#include "BSW/MCAL/mcal_gpio_cfg.h"
#include <util/delay.h>             /* Standard AVR delay library */

int main(void) {
	/* Initialize the pin as Output */
	/* We catch the return status to ensure the pointer wasn't NULL */
	enStd_ReturnType enStatus = E_NOT_OK;
	
	enStatus = enMcal_Gpio_Init(&stMcal_stOnboardLed, GPIO_OUTPUT);

	/* Check if initialization was successful */
	if (E_OK == enStatus) {
		while (1) {
			/* Use the toggle function */
			vMcal_Gpio_Toggle(&stMcal_stOnboardLed);

			/*Delay for 500ms */
			_delay_ms(500);
		}
		} else {
		/* If we are here, there was a configuration error (NULL_PTR) */
		while(1);
	}

	return 0;
}