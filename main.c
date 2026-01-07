/*
 * Thermostat.c
 *
 * Created: 12/21/2025 12:44:41 PM
 * Author : conti
 */ 

#include "mcal_clock.h"
#include "mcal_i2c_cfg.h"
#include "mcal_gpio_cfg.h"
#include "mcal_uart.h"          /* Added UART */
#include "mcal_uart_cfg.h"      /* Added UART Config */
#include "hal_oled.h"
#include "hal_ds18b20.h"
#include "hal_encoder_cfg.h"
#include "hal_relay.h"
#include "asw_ui_manager.h"
#include "asw_process_mgr.h"
#include <util/delay.h>
#include <stdio.h> /* For sprintf */

int main(void) {
    float fCurrentTemp = 0.0f;
    float fTargetTemp  = 26.0f;
    uint16_t u16Timer1s = 0;
    char au8LogMsg[32];

    /* --- 1. Initializations --- */
    (void)Mcal_I2c_Init(&Mcal_I2c_DefaultCfg);
    (void)Mcal_Uart_Init(&Mcal_Uart_DefaultCfg); /* Initialize UART */
	
	Mcal_Uart_WriteString("System Booting...\n");

	/* Initialize OLED and check result */
	if (I2C_OK != Hal_Oled_Init()) {
		Mcal_Uart_WriteString("[ERROR] OLED Init Failed! Check connections.\n");
		} else {
		Mcal_Uart_WriteString("[OK] OLED Initialized.\n");
		Hal_Oled_Clear();
	}

    Hal_Encoder_Init(&Hal_Encoder_DefaultCfg);
    
    /* Initialize Sensor (Presence Check) */
    (void)Hal_Ds18b20_Init(&Mcal_Ds18b20Bus);
    
    Hal_Relay_Init(&Mcal_RelayHeater);
    Hal_Relay_Init(&Mcal_RelayCooler);
    
    Hal_Oled_Clear();
    
    /* Send boot message */
    Mcal_Uart_WriteString("System Booted\n"); 

    while (1) {
        /* --- 2. High Speed Loop (Runs every ~10ms) --- */
        /* This ensures the encoder feels responsive */
        Asw_Ui_HandleInput(&fTargetTemp);

        /* --- 3. Low Speed Loop (Runs every ~1 sec) --- */
        u16Timer1s++;
        if (u16Timer1s >= 100) { /* 100 * 10ms = 1000ms */
            u16Timer1s = 0;

            /* A. Update Temperature */
            /* NOTE: For this to work perfectly smooth, Hal_Ds18b20_ReadTemp 
               NEEDS to be non-blocking. For now, we accept the 750ms freeze 
               ONCE per second, which is better than freeze EVERY loop. */
            if(E_OK == Hal_Ds18b20_ReadTemp(&Mcal_Ds18b20Bus, &fCurrentTemp)) {
                 /* Log Temp to UART */
                 sprintf(au8LogMsg, "T: %d.%d C\n", (int)fCurrentTemp, (int)(fCurrentTemp*10)%10);
                 Mcal_Uart_WriteString(au8LogMsg);
            }

            /* B. Run Control Logic */
            Asw_Process_ControlLoop(fCurrentTemp, fTargetTemp);

            /* C. Refresh Screen */
            /* Note: We still use direct GPIO read here until HAL is updated */
            Asw_Ui_UpdateDisplay(fCurrentTemp, fTargetTemp,
                                 Mcal_Gpio_Read(&Mcal_RelayHeater),
                                 Mcal_Gpio_Read(&Mcal_RelayCooler));
        }

        /* Small delay to stabilize loop timing */
        _delay_ms(10);
    }
    return 0;
}