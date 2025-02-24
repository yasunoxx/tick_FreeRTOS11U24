/*
 * rtc.h
 *
 *  Created on: 2025/02/23
 *      Author: Yasuno
 */

// Target configuration
//	FIX: R/W bitは”Write/Read内部で付けている？(つまり、ADDRは7bit)
//	→その通り、i2c_11xx.c:183でADDRを左シフトしている。ゆえに、デバイス固有のADDRは事前に右シフトしておく
#define ADDR_PCF8523    (0x0D0>>1)
#include "reg_pcf8523.h"

extern bool InitPCF8523( void );
extern void GetTimePCF8523( void );
extern void SetTimePCF8523( void );
extern uint8_t TimePCF8523[ 3 ][ 2 ];
#ifndef High
#define High	1
#endif
#ifndef Low
#define Low		0
#endif
#define Second	0
#define	Minute	1
#define	Hour	2
