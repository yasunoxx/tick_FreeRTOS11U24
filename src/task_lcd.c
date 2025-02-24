/*
 * task_lcd.c
 *
 *  Created on: 2025/02/19
 *      Author: Yasuno
 */

#include "board.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "FreeRTOSCommonHooks.h"
#include <string.h>

#include "iodefs_Bb-LPC11U24custom.h"
#include "glcd.h"
#include "iic.h"
#include "rtc.h"

#define LCD_ITEM_SIZE           sizeof( uint8_t )
#define LCD_QUEUE_SIZE          5

//
//
extern void SetToBufLCD( uint8_t, const char * );	// glcd.c

//
//
#define USE_SEMAPHORE
static void vLCDTask( void * );
xQueueHandle pLCDQueue;
extern xSemaphoreHandle pLCDSemaphore;	// define in freertos_blinky.c

// initialize, not thread
void Init_vLCDTask()
{
	InitGLCD();	// glcd.c, configure LCD on Bb-LPC11U24_custom
	InitIIC();	// iic.c, configure I2C

    pLCDQueue = xQueueCreate( LCD_QUEUE_SIZE, LCD_ITEM_SIZE );
	xTaskCreate( vLCDTask, ( signed char * )"vTaskLCD",
				 configMINIMAL_STACK_SIZE * 2, NULL, ( tskIDLE_PRIORITY + 1UL ),
				 ( xTaskHandle * )NULL );
	vSemaphoreCreateBinary( pLCDSemaphore );
}

/* LCD thread */
static void vLCDTask( void *pvParameters )
{
	bool State = false;
	bool F_initGLCD2 = false, F_initSSD1306 = false, F_title2 = false;
    portTickType ulLastTime;
	uint8_t qMessage;

	while( 1 )
	{
	    ulLastTime = xTaskGetTickCount();
		if( State == false )
		{
			FreeRTOSDelay( 50 );
			privGPIOSetBitValue( LED_PORT, LED2_BIT, LED_ON );
			if( F_initGLCD2 != true )
			{
				// LCD bus device initialize
				InitGLCD2();	// glcd.c
				F_initGLCD2 = true;
			}
			if( F_initSSD1306 != true )
			{
#ifdef USE_SEMAPHORE
				if( xSemaphoreTake( pLCDSemaphore, ( portTickType ) 10 ) == pdTRUE )
				{
#endif
					// I2C bus devices initialize
//					InitSSD1306();	// iic.c
					InitPCF8523();	// rtc.c
					F_initSSD1306 = true;

#ifdef USE_SEMAPHORE
					xSemaphoreGive( pLCDSemaphore );
				}
#endif
			}
			privGPIOSetBitValue( LED_PORT, LED2_BIT, LED_OFF );
			if( F_initGLCD2 == true && F_initSSD1306 == true )
			{
				State = true;
			}
		}
		else	// State == true
		{
	        if( xQueueReceive( pLCDQueue, &qMessage, 0 ) == pdPASS )
			{
				privGPIOSetBitValue( LED_PORT, LED2_BIT, LED_ON );
				FreeRTOSDelay( 50 );
				privGPIOSetBitValue( LED_PORT, LED2_BIT, LED_OFF );
				if( F_title2 != true )
				{
					// rollup LCD
					SetToBufLCD( LF, "" );
					F_title2 = true;
				}
				else
				{
					uint8_t buf[ 16 ], timeBuf[ 8 ];

#ifdef USE_SEMAPHORE
					if( xSemaphoreTake( pLCDSemaphore, ( portTickType ) 10 ) == pdTRUE )
					{
#endif
						GetTimePCF8523();
						timeBuf[ 0 ] = '0' + TimePCF8523[ Hour ][ High ];
						timeBuf[ 1 ] = '0' + TimePCF8523[ Hour ][ Low ];
						timeBuf[ 2 ] = ':';
						timeBuf[ 3 ] = '0' + TimePCF8523[ Minute ][ High ];
						timeBuf[ 4 ] = '0' + TimePCF8523[ Minute ][ Low ];
						timeBuf[ 5 ] = ':';
						timeBuf[ 6 ] = '0' + TimePCF8523[ Second ][ High ];
						timeBuf[ 7 ] = '0' + TimePCF8523[ Second ][ Low ];
						strcpy( ( char * )buf, "Elap. T+" );
						strncat( ( char * )buf, ( const char * )timeBuf, 8 );
						SetToBufLCD( 1, ( const char * )buf );
#ifdef USE_SEMAPHORE
						xSemaphoreGive( pLCDSemaphore );
					}
#endif
				}
			}
		}

		vTaskDelayUntil( &ulLastTime, configTICK_RATE_HZ );
	}
}
