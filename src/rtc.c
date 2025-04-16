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
uint8_t TimePCF8523[ 4 ][ 2 ];

bool prvTimeComp()
{
	bool F_TimeComp = true;

	// check time format
	// and time compensation when illegal format
	{
		uint8_t tmpTime;
#define	modulo( x, y ) ( x - ( x / y ) * y ) // x % y
		tmpTime = ( TimePCF8523[ Hour ][ High ] & 0x03 ) * 10
		          + TimePCF8523[ Hour ][ Low ];
		if( tmpTime > 23 )
		{
			tmpTime = modulo( tmpTime, 24 );
			TimePCF8523[ Hour ][ High ] = tmpTime / 10;
			TimePCF8523[ Hour ][ Low ] = modulo( tmpTime, 10 );
			F_TimeComp = false;
		}
		tmpTime = ( TimePCF8523[ Minute ][ High ] & 0x03 ) * 10
		          + TimePCF8523[ Minute ][ Low ];
		if( tmpTime > 59 )
		{
			tmpTime = modulo( tmpTime, 60 );
			TimePCF8523[ Hour ][ High ] = tmpTime / 10;
			TimePCF8523[ Hour ][ Low ] = modulo( tmpTime, 10 );
			F_TimeComp = false;
		}
		tmpTime = ( TimePCF8523[ Second ][ High ] & 0x03 ) * 10
		          + TimePCF8523[ Second ][ Low ];
		if( tmpTime > 59 )
		{
			tmpTime = modulo( tmpTime, 60 );
			TimePCF8523[ Hour ][ High ] = tmpTime / 10;
			TimePCF8523[ Hour ][ Low ] = modulo( tmpTime, 10 );
			F_TimeComp = false;
		}
	}

	return F_TimeComp;
}

//
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
    cmdbuf[ 0 ] = PCF8523_CTRL3; cmdbuf[ 1 ] = 0b00000000;	// battery standard switch-over enable,
                                                            // and battery low detect
    send_pcf8523( cmdbuf, 2 );

//    cmdbuf[ 0 ] = PCF8523_DAY; cmdbuf[ 1 ] = 1, cmdbuf[ 2 ] = 0, cmdbuf[ 3 ] = 4, cmdbuf[ 4 ] = 0;
    cmdbuf[ 0 ] = PCF8523_DAY; cmdbuf[ 1 ] = 1, cmdbuf[ 2 ] = 0, cmdbuf[ 3 ] = 4;
    // set to April 1, 00
    send_pcf8523( cmdbuf, 4 );

    cmdbuf[ 0 ] = PCF8523_ALM_M; cmdbuf[ 1 ] = PCF8523_AEN_M, cmdbuf[ 2 ] = PCF8523_AEN_H,
    cmdbuf[ 3 ] = PCF8523_ALM_D, cmdbuf[ 4 ] = PCF8523_ALM_W;
    // alarm disable
    send_pcf8523( cmdbuf, 5 );

    cmdbuf[ 0 ] = PCF8523_OFFSET; cmdbuf[ 1 ] = 0;	// offset 0
    send_pcf8523( cmdbuf, 2 );

    cmdbuf[ 0 ] = PCF8523_CTRL1; cmdbuf[ 1 ] = 0;	// Clock start, 24 hours/day mode
    send_pcf8523( cmdbuf, 2 );

    Check_BlinkLED1( 3 );

	TimePCF8523[ Day ][ High ] = 8;
	TimePCF8523[ Day ][ Low ] = 7;
	TimePCF8523[ Hour ][ High ] = 1;
	TimePCF8523[ Hour ][ Low ] = 2;
	TimePCF8523[ Minute ][ High ] = 3;
	TimePCF8523[ Minute ][ Low ] = 4;
	TimePCF8523[ Second ][ High ] = 5;
	TimePCF8523[ Second ][ Low ] = 6;
	SetTimePCF8523();

    Check_BlinkLED1( 3 );

    return true;
}

bool GetTimePCF8523()
{
    uint8_t cmdbuf[ 8 ], result[ 5 ], result2[ 5 ];
    uint8_t loop;

	cmdbuf[ 0 ] = PCF8523_SECOND;
	send_pcf8523( cmdbuf, 1 );
	Chip_I2C_MasterRead( I2C0, ADDR_PCF8523, result, 4 );

	//
   	if( result[ 3 ] > 1 ) // DAY is 1 -> 2
	{
		privGPIOSetBitValue( LED_PORT, LED2_BIT, LED_OFF );
		privGPIOSetBitValue( LED_PORT, LED2_BIT, LED_ON );
   		// increment Day
   		cmdbuf[ 0 ] = PCF8523_DAY;
   		send_pcf8523( cmdbuf, 1 );
   		Chip_I2C_MasterRead( I2C0, ADDR_PCF8523, result2, 4 );
   		result2[ 0 ] = 1;	// set DAY is 1
   		if( result2[ 3 ] < 0x0A0 )
		{
   			uint8_t tmpDate;
#define	modulo( x, y ) ( x - ( x / y ) * y ) // x % y
   			tmpDate = ( result2[ 3 ] & 0x0F0 ) * 10
   			          + ( result2[ 3 ] & 0x0F ) + 1;
   			if( tmpDate < 100 )
   			{
   				tmpDate = modulo( tmpDate, 10 );
   				result2[ 3 ] = ( tmpDate / 10 ) << 4;
   				result2[ 3 ] |= modulo( tmpDate, 10 );
   			}
   		}
   		else
   		{
   			result2[ 3 ] = 0x0A0; // Year increment stop
   		}
   		cmdbuf[ 0 ] = PCF8523_DAY;
   		send_pcf8523( cmdbuf, 1 );
   	    send_pcf8523( result2, 4 );
   	    TimePCF8523[ Day ][ High ] = ( result2[ 3 ] >> 4 ) & 0x0F;
   	    TimePCF8523[ Day ][ Low ] = result2[ 3 ] & 0x0F;
   	    privGPIOSetBitValue( LED_PORT, LED2_BIT, LED_OFF );
	}

   	for( loop = Second; loop <= Hour; loop++ )
    {
        TimePCF8523[ loop ][ High ] = ( result[ loop ] >> 4 ) & 0x07;
        TimePCF8523[ loop ][ Low ] = result[ loop ] & 0x0F;
    }
    TimePCF8523[ Hour ][ High ] &= 0x03;
    TimePCF8523[ Minute ][ High ] &= 0x07;
    TimePCF8523[ Second ][ High ] &= 0x07;

	return prvTimeComp();
}

void SetTimePCF8523()
{
    uint8_t cmdbuf[ 5 ];

    prvTimeComp();

    cmdbuf[ 0 ] = PCF8523_SECOND;
    cmdbuf[ 1 ] = ( TimePCF8523[ Second ][ High ] << 4 ) + TimePCF8523[ Second ][ Low ];
    cmdbuf[ 2 ] = ( TimePCF8523[ Minute ][ High ] << 4 ) + TimePCF8523[ Minute ][ Low ];
    cmdbuf[ 3 ] = ( TimePCF8523[ Hour ][ High ] << 4 ) + TimePCF8523[ Hour ][ Low ];
    // clock guaranteed, set time
    send_pcf8523( cmdbuf, 4 );
	cmdbuf[ 0 ] = PCF8523_YEAR;
    cmdbuf[ 1 ] = ( TimePCF8523[ Day ][ High ] << 4 ) + TimePCF8523[ Day ][ Low ];
	send_pcf8523( cmdbuf, 2 );
}
