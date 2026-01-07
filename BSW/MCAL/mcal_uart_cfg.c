/*
 * mcal_uart_cfg.c
 *
 * Created: 12/22/2025 4:32:08 PM
 *  Author: conti
 */ 

#include "mcal_uart_cfg.h"

/** @brief Uart cfg struct */
const Mcal_Uart_Config_t Mcal_Uart_DefaultCfg = {
	.u32BaudRate_bps = 9600UL,  /* Standard serial speed */
	.u8DataBits      = 8U,
	.u8StopBits      = 1U
};