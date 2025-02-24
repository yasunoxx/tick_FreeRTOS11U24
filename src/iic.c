//

#include "board.h"
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
#include "i2c_11xx.h"

#include "iodefs_Bb-LPC11U24custom.h"
#include "iic.h"

//
#ifdef WITH_FREERTOS
// External functions
extern void FreeRTOSDelay( uint32_t ms ); // FreeRTOSCommonHooks.c
#define DelayIIC( dlyTicks ) FreeRTOSDelay( dlyTicks )
#else
void DelayIIC( uint32_t dlyTicks )
{
  uint32_t loop;

    for( loop = 0UL; loop < dlyTicks * 10UL; loop++ );
}
#endif

//
//
void Check_BlinkLED1( uint8_t counts )
{
    uint8_t loop;

    privGPIOSetBitValue( LED_PORT, LED1_BIT, LED_ON );
    for( loop = 0; loop < counts; loop++ )
    {
        DelayIIC( 100UL );
        privGPIOSetBitValue( LED_PORT, LED1_BIT, LED_OFF );
        DelayIIC( 100UL );
        privGPIOSetBitValue( LED_PORT, LED1_BIT, LED_ON );
        DelayIIC( 100UL );
    }
}

//
//
void InitIIC()
{
    Chip_I2C_Init( I2C0 );
    Chip_I2C_SetClockRate( I2C0, I2C_DEFAULT_SPEED );    // initial value
	NVIC_DisableIRQ( I2C0_IRQn );
	Chip_I2C_SetMasterEventHandler( I2C0, Chip_I2C_EventHandlerPolling );
}

bool InitSSD1306()
{
    uint8_t cmdbuf[ 2 ];

    // ... configuration sequence ...
    privGPIOSetBitValue( LED_PORT, LED2_BIT, LED_ON );

    Check_BlinkLED1( 3 );
    cmdbuf[ 0 ] = SSD1306_MUX_RATIO;   cmdbuf[ 1 ] = 0x3F;
    if( 0 != send_ssd1306( cmdbuf, 2 ) )
    {
    	return false;
    }
    Check_BlinkLED1( 3 );
    cmdbuf[ 0 ] = SSD1306_DISP_OFFSET; cmdbuf[ 1 ] = 0x00; send_ssd1306( cmdbuf, 2 );
    cmdbuf[ 0 ] = SSD1306_SADDR_L;                         send_ssd1306( cmdbuf, 1 );
    cmdbuf[ 0 ] = SSD1306_SEG_REMAP;                       send_ssd1306( cmdbuf, 1 );
    cmdbuf[ 0 ] = SSD1306_COMOUT_DIR;                      send_ssd1306( cmdbuf, 1 );
    cmdbuf[ 0 ] = SSD1306_COMPIN_CONF;                     send_ssd1306( cmdbuf, 1 );
    cmdbuf[ 0 ] = SSD1306_CONTRAST;                        send_ssd1306( cmdbuf, 1 );
    cmdbuf[ 0 ] = SSD1306_ENT_DISPON;                      send_ssd1306( cmdbuf, 1 );
    cmdbuf[ 0 ] = SSD1306_SET_NORMAL;                      send_ssd1306( cmdbuf, 1 );
    cmdbuf[ 0 ] = SSD1306_DIV_RATIO;   cmdbuf[ 1 ] = 0x80; send_ssd1306( cmdbuf, 2 );
    cmdbuf[ 0 ] = SSD1306_EN_CPUMP;    cmdbuf[ 1 ] = 0x14; send_ssd1306( cmdbuf, 2 );
    cmdbuf[ 0 ] = SSD1306_DISPON;                          send_ssd1306( cmdbuf, 1 );

    privGPIOSetBitValue( LED_PORT, LED1_BIT, LED_OFF );
    return true;
}
