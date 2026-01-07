/*
 * Thermostat.c
 *
 * Created: 12/21/2025 12:44:41 PM
 * Author : conti
 */ 

#include "mcal_clock.h"
#include "mcal_i2c_cfg.h"
#include "mcal_uart_cfg.h"
#include "mcal_gpio_cfg.h"
#include "hal_ds18b20.h"  
#include "asw_process_mgr.h"
#include "asw_ui_manager.h"
#include <util/delay.h>



void System_Init(void) {
    /* MCAL Init */
    (void)Mcal_I2c_Init(&Mcal_I2c_DefaultCfg);
    (void)Mcal_Uart_Init(&Mcal_Uart_DefaultCfg);
    
    /* HAL Init (Sensor bus) */
    (void)Hal_Ds18b20_Init(&Mcal_Ds18b20Bus);

    /* ASW Init */
    Asw_Process_Init();
    Asw_Ui_Init();
}


int main(void) {
    /* ... Init functions ... */
    (void)Mcal_I2c_Init(&Mcal_I2c_DefaultCfg);
    (void)Mcal_Uart_Init(&Mcal_Uart_DefaultCfg);
    (void)Hal_Ds18b20_Init(&Mcal_Ds18b20Bus);
    Asw_Process_Init();
    Asw_Ui_Init();

    while (1) {
        /* Task 1: UI (Encoder/Screen) */
        Asw_Ui_Run();

        /* Task 2: Process (Temperature/Relays) */
        Asw_Process_Run();

        /* System Tick: 10ms */
        /* This keeps the loop running at ~100Hz */
        _delay_ms(10);
    }
    return 0;
}