// sysinit_Bb-LPC11U24custom.c -- sysinit for private use

// sysinit.c Original Copyright:
/*
 * @brief Common SystemInit function for LPC11xx chips
 *
 * @note
 * Copyright(C) NXP Semiconductors, 2012
 * All rights reserved.
 */

#include "LPC11Uxx.h"
#include "sysdefs.h"

#include "iodefs_Bb-LPC11U24custom.h"

/*****************************************************************************
 * Private types/enumerations/variables
 ****************************************************************************/

/*****************************************************************************
 * Private types/enumerations/variables
 ****************************************************************************/

/* IOCON pin definitions for pin muxing */
typedef struct {
	uint32_t port : 8;			/* Pin port */
	uint32_t pin : 8;			/* Pin number */
	uint32_t modefunc : 16;		/* Function and mode */
} PINMUX_GRP_T;


/*****************************************************************************
 * Public types/enumerations/variables
 ****************************************************************************/

/* Pin muxing table, only items that need changing from their default pin
   state are in this table. */
#define IOCON_FUNC1             0x1				/*!< Selects pin function 1 */
#define IOCON_MODE_INACT        (0x0 << 3)		/*!< No addition pin function */
#define IOCON_MODE_PULLDOWN     (0x1 << 3)		/*!< Selects pull-down function */
#define IOCON_MODE_PULLUP       (0x2 << 3)		/*!< Selects pull-up function */
#define IOCON_MODE_REPEATER     (0x3 << 3)		/*!< Selects pin repeater function */
#define IOCON_HYS_EN            (0x1 << 5)		/*!< Enables hysteresis */
#define IOCON_INV_EN            (0x1 << 6)		/*!< Enables invert function on input */
#define IOCON_ADMODE_EN         (0x0 << 7)		/*!< Enables analog input function (analog pins only) */
#define IOCON_DIGMODE_EN        (0x1 << 7)		/*!< Enables digital function (analog pins only) */
#define IOCON_SFI2C_EN          (0x0 << 8)		/*!< I2C standard mode/fast-mode */
#define IOCON_STDI2C_EN         (0x1 << 8)		/*!< I2C standard I/O functionality */
#define IOCON_FASTI2C_EN        (0x2 << 8)		/*!< I2C Fast-mode Plus */
#define IOCON_FILT_DIS          (0x1 << 8)		/*!< Disables noise pulses filtering (10nS glitch filter) */
#define IOCON_OPENDRAIN_EN      (0x1 << 10)		/*!< Enables open-drain function */
#define PINMODE_PULLUP	0b00010000
#define	PINMODE_DIGITAL	0b10000000
#define PINMODE_OPENDRAIN 0b10000000000
#define PINMODE_MOSI1	0b00000010
#define PINMODE_MISO1	0b00000001
#define	PINMODE_SCK1	0b00000011

/*****************************************************************************
 * Private functions
 ****************************************************************************/
typedef enum {
	RESET_SSP0,			/*!< SSP0 reset control */
	RESET_I2C0,			/*!< I2C0 reset control */
	RESET_SSP1			/*!< SSP1 reset control */
} SYSCON_PERIPH_RESET_T;

void SYSCON_AssertPeriphReset( SYSCON_PERIPH_RESET_T periph )
{
	LPC_SYSCON->PRESETCTRL &= ~( 1 << ( uint32_t )periph );
}

void SYSCON_DeassertPeriphReset( SYSCON_PERIPH_RESET_T periph )
{
	LPC_SYSCON->PRESETCTRL |= ( 1 << ( uint32_t )periph );
}

void SYSCON_PeriphReset( SYSCON_PERIPH_RESET_T periph )
{
	SYSCON_AssertPeriphReset( periph );
	SYSCON_DeassertPeriphReset( periph );
}

/*****************************************************************************
 * Public functions
 ****************************************************************************/

/* Sets up system pin multiplexing */
void SystemSetupMuxing(void)
{
	LPC_IOCON->PIO0_1 = (IOCON_FUNC1 | IOCON_MODE_INACT);	/* PIO0_1 used for CLKOUT */
	LPC_IOCON->PIO0_2 = (IOCON_FUNC1 | IOCON_MODE_INACT);	/* PIO0_2 used for SSEL */
	LPC_IOCON->PIO0_3 = (IOCON_FUNC1 | IOCON_MODE_INACT);	/* PIO0_3 used for USB_VBUS */
	LPC_IOCON->PIO0_4 = (IOCON_FUNC1 | IOCON_SFI2C_EN);	/* PIO0_4 used for SCL */
	LPC_IOCON->PIO0_5 = (IOCON_FUNC1 | IOCON_SFI2C_EN);	/* PIO0_5 used for SDA */
	LPC_IOCON->PIO0_6 = (IOCON_FUNC1 | IOCON_MODE_INACT);	/* PIO0_6 used for USB_CONNECT */
	LPC_IOCON->PIO0_18 = (IOCON_FUNC1 | IOCON_MODE_INACT);	/* PIO0_18 used for RXD */
	LPC_IOCON->PIO0_19 = (IOCON_FUNC1 | IOCON_MODE_INACT);	/* PIO0_19 used for TXD */

	LPC_IOCON->PIO0_20 = PINMODE_OPENDRAIN;							// T_LED2
	LPC_IOCON->PIO0_23 = ( PINMODE_DIGITAL | PINMODE_OPENDRAIN );	// T_LED1
	LPC_IOCON->PIO0_16 = PINMODE_DIGITAL;							// SSI1 CS
	LPC_IOCON->PIO1_15 = ( PINMODE_SCK1    | PINMODE_OPENDRAIN );	// SCK1
	LPC_IOCON->PIO1_21 = ( PINMODE_MOSI1   | PINMODE_OPENDRAIN );	// MOSI1
	LPC_IOCON->PIO1_22 = ( PINMODE_MISO1   | PINMODE_PULLUP );		// MISO1

}

void Bb_LPC11U24_SYSCONInit( void )
{
  /* Enable AHB clock to the GPIO domain. */
  LPC_SYSCON->SYSAHBCLKCTRL |= ( 1 << 6 );

  // enable I2C, SSP0, SSP1 and USART clock
  LPC_SYSCON->SYSAHBCLKCTRL |= ( ( 1 << 5 ) | ( 1 << 11 ) | ( 1 << 12 ) | ( 1 << 18 ) );
  LPC_SYSCON->SSP0CLKDIV = 255;
  LPC_SYSCON->UARTCLKDIV = 255;
  LPC_SYSCON->SSP1CLKDIV = 255;

  // enable SSP0, SSP1 and USART clock
  LPC_SYSCON->SYSAHBCLKCTRL |= ( ( 1 << 11 ) | ( 1 << 12 ) | ( 1 << 18 ) );

  /* Enable AHB clock to the FlexInt, GroupedInt domain. */
  LPC_SYSCON->SYSAHBCLKCTRL |= ( ( 1 << 19 ) | ( 1 << 23 ) | ( 1 << 24 ) );

//  LPC_SYSCON->PRESETCTRL |= 0b111;	// reset de-assert SSP0, SSP1, I2C
  SYSCON_PeriphReset( RESET_I2C0 );
}
