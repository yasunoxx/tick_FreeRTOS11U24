// premain.c -- systick, iic, gpio evaluation for LPC11U24

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

#ifdef __USE_CMSIS
#include "LPC11Uxx.h"
#else
#include "chip.h"
#endif
#include <stdint.h>
#include <cr_section_macros.h>

#include "gpio.h"
#include "glcd.h"
#include "sysdefs.h"
//#include "drivers/displays/bitmap/ssd1306/ssd1306_i2c.h"
//#include "iic.h"

// LPCXpresso eval board LED
//#define LED_PORT 0		// Port on led
//#define LED_BIT 7		// Bit on led
//#define LED_ON 1		// to set port to turn on led
//#define LED_OFF 0		// set port to turn off led

// NGX Blueboard-LPC11U24 LED
#define	LED_PORT	0
#define	LED1_BIT	20
#define	LED2_BIT	23
#define LED_ON 0		// set port to turn on led
#define LED_OFF 1		// set port to turn off led

extern void privGPIOSetBitValue( uint32_t, uint32_t, uint32_t ); // privgpio.c

/*****************************************************************************
** Function name:		Bb_LPC11U24_GPIOInit
**
** Descriptions:		Initialize GPIO, install the
**						GPIO interrupt handler
**
** parameters:			None
**
** Returned value:		true or false, return false if the VIC table
**						is full and GPIO interrupt handler can be
**						installed.
**
*****************************************************************************/
void Bb_LPC11U24_GPIOInit( void )
{
  /* Enable AHB clock to the GPIO domain. */
  LPC_SYSCON->SYSAHBCLKCTRL |= (1<<6);

  // enable SSP0, SSP1 and USART clock
  LPC_SYSCON->SYSAHBCLKCTRL |= ( ( 1<<11 ) | ( 1<<12 ) | ( 1<<18 ) );
  LPC_SYSCON->SSP0CLKDIV = 255;
  LPC_SYSCON->UARTCLKDIV = 255;
  LPC_SYSCON->SSP1CLKDIV = 255;

  /* Enable AHB clock to the FlexInt, GroupedInt domain. */
  LPC_SYSCON->SYSAHBCLKCTRL |= ((1<<19) | (1<<23) | (1<<24));

#define PINMODE_PIO 	0b00000001
#define PINMODE_PULLUP	0b00010000
#define	PINMODE_DIGITAL	0b10000000
#define PINMODE_OPENDRAIN 0b10000000000
//  LPC_IOCON->TMS_PIO0_12 is JTAG
//  LPC_IOCON->TDO_PIO0_13 is JTAG
//  LPC_IOCON->TRST_PIO0_14 is JTAG
  // T_LED1
  LPC_IOCON->PIO0_23 =	             PINMODE_DIGITAL | PINMODE_OPENDRAIN;
  // open pin(MCU_PIN9) -> T_LED2
  LPC_IOCON->PIO0_20 =	             PINMODE_DIGITAL | PINMODE_OPENDRAIN;

  // not used pins(MCU_PIN18,21,1) -> LCD RS, R/W, E
  LPC_IOCON->PIO1_23 = PINMODE_PIO | PINMODE_DIGITAL;
  LPC_IOCON->PIO1_24 = PINMODE_PIO | PINMODE_DIGITAL;
  LPC_IOCON->PIO1_25 = PINMODE_PIO | PINMODE_DIGITAL;
  // LCD *CS(PIO1.13)
  // BUZZER -> LCD CS2
  LPC_IOCON->PIO0_17 = PINMODE_DIGITAL;

  LPC_SYSCON->PRESETCTRL |= 0b111;	// reset de-assert SSP0, SSP1, I2C

  // I2C Clock is already active(SYSAHBCLLKCTRL[5])
#define PINMODE_I2C	   0b00000001
#define PINMODE_I2CSTD 0b0000000000
  // SCK, SDA( but not work ... why? )
  LPC_IOCON->PIO0_4 = PINMODE_I2C | PINMODE_I2CSTD;
  LPC_IOCON->PIO0_5 = PINMODE_I2C | PINMODE_I2CSTD;

#define PINMODE_MOSI1	0b00000010
#define PINMODE_MISO1	0b00000001
#define	PINMODE_SCK1	0b00000011
  // not used pin(MCU_PIN17) -> MOSI1
  LPC_IOCON->PIO0_21 = PINMODE_MOSI1 | PINMODE_OPENDRAIN;
  // T_LED2 -> MISO1
  LPC_IOCON->PIO0_22 = PINMODE_MISO1 | PINMODE_PULLUP;
  // PS/2 DATA -> SCK1
  LPC_IOCON->PIO1_15 = PINMODE_SCK1  | PINMODE_OPENDRAIN;
  // WAKEUP -> SSi1 CS
  LPC_IOCON->PIO0_16 = PINMODE_DIGITAL;

  return;
}

void premain( void )
{
	  /* Initialize GPIO (sets up clock) */
	  Bb_LPC11U24_GPIOInit();

	  SystemCoreClockUpdate();
	  if( SysTick_Config( SystemCoreClock / 1000 ) )	/* Setup SysTick Timer for 1 msec interrupts  */
	  {
	    while( 1 );                                  	/* Capture error */
	  }

	  /* Make sure pins are in a relevant state */
	  GPIOSetDir( LED_PORT, LED1_BIT, 1 );
	  privGPIOSetBitValue( LED_PORT, LED1_BIT, LED_OFF );
	  GPIOSetDir( LED_PORT, LED2_BIT, 1 );
	  privGPIOSetBitValue( LED_PORT, LED2_BIT, LED_OFF );

//	  InitGLCD();	// configure NGX Bb-LPC11U24 LCD
//	  Delay( 20 );
//	  InitIIC();
//	  InitSSD1306()

	  // Turn LED on, then wait
//	  privGPIOSetBitValue( LED_PORT, LED1_BIT, LED_ON );

	  return;	// here we go to FreeRTOS world.

	  while( 1 )                                			/* Loop forever */
	  {
		  /* Poll for CLI input if CFG_INTERFACE is enabled */
//		  Delay( DELAY_LEN );

		  // Turn LED off, then wait
//		  privGPIOSetBitValue( LED_PORT, LED1_BIT, LED_OFF );
//		  Delay( DELAY_LEN );
	  }
}
