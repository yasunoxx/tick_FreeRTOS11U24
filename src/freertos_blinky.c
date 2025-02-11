/*
 * @brief FreeRTOS Blinky example
 *
 * @note
 * Copyright(C) NXP Semiconductors, 2012
 * All rights reserved.
 *
 * @par
 * Software that is described herein is for illustrative purposes only
 * which provides customers with programming information regarding the
 * LPC products.  This software is supplied "AS IS" without any warranties of
 * any kind, and NXP Semiconductors and its licensor disclaim any and
 * all warranties, express or implied, including all implied warranties of
 * merchantability, fitness for a particular purpose and non-infringement of
 * intellectual property rights.  NXP Semiconductors assumes no responsibility
 * or liability for the use of the software, conveys no license or rights under any
 * patent, copyright, mask work right, or any other intellectual property rights in
 * or to any products. NXP Semiconductors reserves the right to make changes
 * in the software without notification. NXP Semiconductors also makes no
 * representation or warranty that such application will be suitable for the
 * specified use without further testing or modification.
 *
 * @par
 * Permission to use, copy, modify, and distribute this software and its
 * documentation is hereby granted, under NXP Semiconductors' and its
 * licensor's relevant copyrights in the software, without fee, provided that it
 * is used in conjunction with NXP Semiconductors microcontrollers.  This
 * copyright, permission, and disclaimer notice must appear in all copies of
 * this code.
 */

#include "board.h"
#include "FreeRTOS.h"
#include "task.h"

#include "glcd.h"

/*****************************************************************************
 * Private types/enumerations/variables
 ****************************************************************************/

/*****************************************************************************
 * Public types/enumerations/variables
 ****************************************************************************/

/*****************************************************************************
 * Private functions
 ****************************************************************************/

/* Sets up system hardware */
static void prvSetupHardware(void)
{
	SystemCoreClockUpdate();	// chip_11xx.c
	Board_Init();				// board/board.c
}

// NGX Blueboard-LPC11U24 LED
#define	LED_PORT	0
#define	LED1_BIT	20
#define	LED2_BIT	23
#define LED_ON 0		// set port to turn on led
#define LED_OFF 1		// set port to turn off led
extern void privGPIOSetBitValue( uint32_t, uint32_t, uint32_t );	// privgpio.c
extern void premain( void ); //premain.c


/* empty thread */
static void vTask0( void *pvParameters )
{
	bool State = false;
	while( 1 )
	{
		State = ( bool )!State;

		vTaskDelay(configTICK_RATE_HZ/2);
	}
}

/* LED1 toggle thread */
static void vLEDTask( void *pvParameters )
{
	bool LedState = false;
	while( 1 )
	{
		if( LedState )
		{
			privGPIOSetBitValue( LED_PORT, LED1_BIT, LED_ON );
		}
		else
		{
			privGPIOSetBitValue( LED_PORT, LED1_BIT, LED_OFF );
		}
		LedState = ( bool )!LedState;

		vTaskDelay( configTICK_RATE_HZ * 2 );
	}
}

/* LCD thread */
static void vLCDTask( void *pvParameters )
{
	bool State = false;
	while( 1 )
	{
		if( State == false )
		{
			FreeRTOSDelay( 50 );
			privGPIOSetBitValue( LED_PORT, LED2_BIT, LED_ON );
			InitGLCD2();
			privGPIOSetBitValue( LED_PORT, LED2_BIT, LED_OFF );
			State = ( bool )!State;
		}

		vTaskDelay( configTICK_RATE_HZ );
	}
}

/*****************************************************************************
 * Public functions
 ****************************************************************************/

/**
 * @brief	main routine for FreeRTOS blinky example
 * @return	Nothing, function should not exit
 */
int main( void )
{
	prvSetupHardware();
	premain();	// previous main()

	/* LED1 toggle thread */
	xTaskCreate( vLEDTask, ( signed char * )"vTaskLED1",
				 configMINIMAL_STACK_SIZE, NULL, ( tskIDLE_PRIORITY + 1UL ),
				 ( xTaskHandle * )NULL );

	/* LCD thread */
	xTaskCreate( vLCDTask, ( signed char * )"vTaskLCD",
				 configMINIMAL_STACK_SIZE, NULL, ( tskIDLE_PRIORITY + 1UL ),
				 ( xTaskHandle * )NULL );

	/* Null thread */
	xTaskCreate( vTask0, ( signed char * )"vTask0",
				 configMINIMAL_STACK_SIZE, NULL, ( tskIDLE_PRIORITY + 1UL ),
				 ( xTaskHandle * )NULL );

	/* Start the scheduler */
	vTaskStartScheduler();

	/* Should never arrive here */
	return 1;
}
