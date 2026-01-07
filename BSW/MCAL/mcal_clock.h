/*
 * mcal_clock.h
 *
 * Created: 12/21/2025 2:46:19 PM
 *  Author: conti
 */ 


#ifndef MCAL_CLOCK_H_
#define MCAL_CLOCK_H_

/* Include the types */
#include "mcal_std_types.h"

/* CPU clock speed */
#ifndef F_CPU
#define F_CPU          16000000UL              		/* 16MHz processor     */
//#define F_CPU        14745000               		/* 14.745MHz processor */
//#define F_CPU        8000000               		/* 8MHz processor      */
//#define F_CPU        7372800               		/* 7.37MHz processor   */
//#define F_CPU        4000000               		/* 4MHz processor      */
//#define F_CPU        3686400               		/* 3.69MHz processor   */
//#define F_CPU        8000000               		/* 1MHz processor      */
#endif

/* Retrieves the system CPU frequency. */
#define MCAL_GET_CPU_FREQ() ((uint32_t)F_CPU)

#endif /* MCAL_CLOCK_H_ */