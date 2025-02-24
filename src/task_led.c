/*
 * task_led.c
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

#include "iodefs_Bb-LPC11U24custom.h"

static void vLEDTask( void * );
extern xQueueHandle pLCDQueue;	// message queue to task_lcd.c
extern xSemaphoreHandle pLCDSemaphore;	// define in freertos_blinky.c

// initialize, not thread
void Init_vLEDTask()
{
	xTaskCreate( vLEDTask, ( signed char * )"vTaskLED1",
				 configMINIMAL_STACK_SIZE * 2, NULL, ( tskIDLE_PRIORITY + 1UL ),
				 ( xTaskHandle * )NULL );
}

/* LED1 toggle thread */
static void vLEDTask( void *pvParameters )
{
	bool LedState = false;
    portTickType ulLastTime;
	uint8_t qMessage;

	while( 1 )
	{
	    ulLastTime = xTaskGetTickCount();
		if( LedState )
		{
			privGPIOSetBitValue( LED_PORT, LED1_BIT, LED_ON );
			xQueueSend( pLCDQueue, &qMessage, portMAX_DELAY );	// synchronous message
		}
		else
		{
			privGPIOSetBitValue( LED_PORT, LED1_BIT, LED_OFF );
		}
		LedState = ( bool )!LedState;

		vTaskDelayUntil( &ulLastTime, configTICK_RATE_HZ );
	}
}
