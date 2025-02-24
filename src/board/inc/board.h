//

#ifndef __BOARD_H_
#define __BOARD_H_


#include "chip.h"
/* board_api.h is included at the bottom of this file after DEBUG setup */

#ifdef __cplusplus
extern "C" {
#endif

/** Define DEBUG_ENABLE to enable IO via the DEBUGSTR, DEBUGOUT, and
    DEBUGIN macros. If not defined, DEBUG* functions will be optimized
	out of the code at build time.
 */
//#define DEBUG_ENABLE

/** Define DEBUG_SEMIHOSTING along with DEBUG_ENABLE to enable IO support
    via semihosting. You may need to use a C library that supports
	semihosting with this option.
 */
//#define DEBUG_SEMIHOSTING

/** Board UART used for debug output and input using the DEBUG* macros. This
    is also the port used for Board_UARTPutChar, Board_UARTGetChar, and
	Board_UARTPutSTR functions.
 */
#define DEBUG_UART LPC_USART

/**
 * @}
 */

/* Board name */
#define BOARD_Bb_LPC11U24custom

/**
 * LED defines
 */
#define	LED_PORT	0
#define	LED1_BIT	20
#define	LED2_BIT	23
#define LED_ON 0		// set port to turn on led
#define LED_OFF 1		// set port to turn off led

/**
 * @}
 */

#include "board_api.h"

#ifdef __cplusplus
}
#endif

#endif /* __BOARD_H_ */
