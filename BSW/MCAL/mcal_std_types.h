/*
 * mcal_std_types.h
 *
 * Created: 12/21/2025 2:56:17 PM
 *  Author: conti
 */ 


#ifndef MCAL_STD_TYPES_H_
#define MCAL_STD_TYPES_H_

/* Include the compiler's device map */
#include <avr/io.h>
#include <stdint.h>

/* --- Status & Logic --- */
typedef enum {
	E_NOT_OK = 0U,
	E_OK     = 1U
} enStd_ReturnType;

typedef enum {
	FALSE = 0U,
	TRUE  = 1U
} enBool_t;

#define NULL_PTR ((void*)0)

#endif /* MCAL_STD_TYPES_H_ */





 

