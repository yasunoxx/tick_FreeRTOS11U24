// premain.c -- FreeRTOS, iic, gpio evaluation for LPC11U24

//
// Original File "systick.c" Copyright:
//*****************************************************************************
//   +--+
//   | ++----+
//   +-++    |
//     |     |
//   +-+--+  |
//   | +--+--+
//   +----+    Copyright (c) 2011 Code Red Technologies Ltd.
//
// LED flashing SysTick application for LPCXPresso11U14 board
//
//*****************************************************************************

#define WITH_FREERTOS
#ifndef WITH_FREERTOS
 #ifdef __USE_CMSIS
 #include "LPC11Uxx.h"
 #else
 #include "chip.h"
 #endif
#else
 #include "FreeRTOS.h"
 #include "task.h"
 #include "FreeRTOSCommonHooks.h"
#endif
#include <stdint.h>
#include <cr_section_macros.h>
#include "sysdefs.h"

#include "iodefs_Bb-LPC11U24custom.h"
#include "gpio.h"
#include "glcd.h"
//#include "drivers/displays/bitmap/ssd1306/ssd1306_i2c.h"
#include "iic.h"

void premain( void )
{
	uint16_t loop;

	/* Initialize SYSCON (sets up clock) */
	Bb_LPC11U24_SYSCONInit();

	/* Make sure pins are in a relevant state */
	GPIOSetDir( LED_PORT, LED1_BIT, 1 );
	privGPIOSetBitValue( LED_PORT, LED1_BIT, LED_OFF );
	GPIOSetDir( LED_PORT, LED2_BIT, 1 );
	privGPIOSetBitValue( LED_PORT, LED2_BIT, LED_OFF );

	for( loop = 0; loop < 4096; loop++ );

	return;	// back to FreeRTOS world.
}
