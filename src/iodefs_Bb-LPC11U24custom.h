/*
 * iodefs_Bb-LPC11U24custom.h
 *
 *  Created on: 2025/02/20
 *      Author: Yasuno
 */

#ifndef __IODEFS_H__
#define __IODEFS_H__

extern void privGPIOSetBitValue( uint32_t, uint32_t, uint32_t ); // privgpio.c
extern void Bb_LPC11U24_SYSCONInit( void );	// sysinit_Bb-LPC11U24custom.c

// Bb-LPC11U24custom LED
#define	LED_PORT	0
#define	LED1_BIT	20
#define	LED2_BIT	23
#define LED_ON 0		// set port to turn on led
#define LED_OFF 1		// set port to turn off led

// Standard valiant
#define	IN	0
#define	OUT	1
#define LOW	0
#define HIGH 1

#define	CHR_NULL	0
#define BACKSPACE	0x08
#define	LF			0x0A
#define	CR			0x0D
#define	DELETE		0x7F

#endif	/* __IODEFS_H__ */
