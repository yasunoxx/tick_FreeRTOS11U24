/*
 * rtc.c
 *
 *  Created on: 2025/02/23
 *      Author: Yasuno
 */

#include <stdint.h>
#include <string.h>
#include <stdlib.h>

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

#include "iodefs_Bb-LPC11U24custom.h"
#include "gpio.h"
#include "iic.h"
#include "rtc.h"

//
uint8_t TimePCF8523[ 3 ][ 2 ];

bool InitPCF8523()
{
    uint8_t cmdbuf[ 5 ];

    Check_BlinkLED1( 3 );
    cmdbuf[ 0 ] = PCF8523_CTRL1; cmdbuf[ 1 ] = 0x58;	// reset, definiton in datasheet
    if( 0 != send_pcf8523( cmdbuf, 2 ) )
    {
    	return false;
    }
    FreeRTOSDelay( 10UL );

    cmdbuf[ 0 ] = PCF8523_CTRL1; cmdbuf[ 1 ] = ( PCF8523_STOP | PCF8523_12_24 );
    send_pcf8523( cmdbuf, 2 );
    cmdbuf[ 0 ] = PCF8523_CTRL2; cmdbuf[ 1 ] = 0;	// watchdog, timer, alarms disable
    send_pcf8523( cmdbuf, 2 );
    cmdbuf[ 0 ] = PCF8523_CTRL3; cmdbuf[ 1 ] = PCF8523_PM;	// battery switch-over disable
    send_pcf8523( cmdbuf, 2 );

    cmdbuf[ 0 ] = PCF8523_DAY; cmdbuf[ 1 ] = 1, cmdbuf[ 2 ] = 0, cmdbuf[ 3 ] = 1, cmdbuf[ 4 ] = 0x70;
    // set to January 1, (19)70
    send_pcf8523( cmdbuf, 5 );

    cmdbuf[ 0 ] = PCF8523_ALM_M; cmdbuf[ 1 ] = PCF8523_AEN_M, cmdbuf[ 2 ] = PCF8523_AEN_H,
    cmdbuf[ 3 ] = PCF8523_ALM_D, cmdbuf[ 4 ] = PCF8523_ALM_W;
    // alarm disable
    send_pcf8523( cmdbuf, 5 );

    cmdbuf[ 0 ] = PCF8523_OFFSET; cmdbuf[ 1 ] = 0;	// offset 0
    send_pcf8523( cmdbuf, 2 );

    cmdbuf[ 0 ] = PCF8523_CTRL1; cmdbuf[ 1 ] = 0;	// 24 hours/day mode
    send_pcf8523( cmdbuf, 2 );

    Check_BlinkLED1( 3 );

#ifdef WITH_TIME_INIT
    TimePCF8523[ Second ][ High ] = 0;
    TimePCF8523[ Second ][ Low ] = 0;
    TimePCF8523[ Minute ][ High ] = 0;
    TimePCF8523[ Minute ][ Low ] = 0;
    TimePCF8523[ Hour ][ High ] = 0;
    TimePCF8523[ Hour ][ Low ] = 0;
    SetTimePCF8523();
    Check_BlinkLED1( 3 );
#endif
    return true;
}

extern void GetTimePCF8523()
{
    uint8_t cmdbuf[ 8 ], result[ 5 ];
    uint8_t loop;

    cmdbuf[ 0 ] = PCF8523_SECOND;
    send_pcf8523( cmdbuf, 1 );
    Chip_I2C_MasterRead( I2C0, ADDR_PCF8523, result, 3 );
    //
#ifndef NOWDEBUG
    for( loop = Second; loop < Hour + 1; loop++ )
    {
        TimePCF8523[ loop ][ High ] = ( result[ loop ] >> 4 ) & 0x07;
        TimePCF8523[ loop ][ Low ] = result[ loop ] & 0x0F;
    }
    TimePCF8523[ Hour ][ High ] &= 0x03;
#else
    TimePCF8523[ Second ][ High ] = ( result[ loop ] >> 4 ) & 0x07;
    TimePCF8523[ Second ][ Low ] = result[ Second ] & 0x0F;
    TimePCF8523[ Minute ][ High ] = ( result[ loop ] >> 4 ) & 0x07;
    TimePCF8523[ Minute ][ Low ] = result[ Minute ] & 0x0F;
    TimePCF8523[ Hour ][ High ] = ( result[ loop ] >> 4 ) & 0x03;
    TimePCF8523[ Hour ][ Low ] = result[ Hour ] & 0x0F;
#endif
}

void SetTimePCF8523()
{
    uint8_t cmdbuf[ 5 ];

    cmdbuf[ 0 ] = PCF8523_SECOND;
    cmdbuf[ 1 ] = ( TimePCF8523[ Second ][ High ] << 4 ) + TimePCF8523[ Second ][ Low ];
    cmdbuf[ 2 ] = ( TimePCF8523[ Minute ][ High ] << 4 ) + TimePCF8523[ Minute ][ Low ];
    cmdbuf[ 3 ] = ( TimePCF8523[ Hour ][ High ] << 4 ) + TimePCF8523[ Hour ][ Low ];
    // clock guaranteed, set time
    send_pcf8523( cmdbuf, 4 );
}
