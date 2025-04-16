//

#include "board.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "FreeRTOSCommonHooks.h"

#include "iodefs_Bb-LPC11U24custom.h"
#include "glcd.h"

/*****************************************************************************
 * Private types/enumerations/variables
 ****************************************************************************/

/*****************************************************************************
 * Public types/enumerations/variables
 ****************************************************************************/
extern void SystemSetupMuxing( void );	// board_sysinit.c
extern void premain( void ); //premain.c

extern void Init_vLCDTask( void );	// task_led.c
extern void Init_vLEDTask( void );	// task_lcd.c

xSemaphoreHandle pLCDSemaphore;

/*****************************************************************************
 * Private functions
 ****************************************************************************/

/* Sets up system hardware */
static void prvSetupHardware(void)
{
	SystemCoreClockUpdate();	// chip_11xx.c
	SystemSetupMuxing();		// board_sysinit.c
	Board_Init();				// board/board.c
}

/* empty thread */
static void vTask0( void *pvParameters )
{
	bool State = false;
	while( 1 )
	{
		State = ( bool )!State;

		vTaskDelay( configTICK_RATE_HZ / 2 );
	}
}

int main( void )
{
	prvSetupHardware();
	premain();	// previous main()

	Init_vLEDTask();
	Init_vLCDTask();

	/* Null thread */
	xTaskCreate( vTask0, ( signed char * )"vTask0",
				 configMINIMAL_STACK_SIZE, NULL, ( tskIDLE_PRIORITY + 1UL ),
				 ( xTaskHandle * )NULL );

	/* Start the scheduler */
	vTaskStartScheduler();

	/* Should never arrive here */
	return 1;
}
