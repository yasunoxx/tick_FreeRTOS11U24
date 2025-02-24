/*
 ** glcd.c -- 128x64 dots LCD(SG12864) & Character LCD(HD44780) routines,
 **           for LPC11xx/mcuXpresso
 ** (C)2025 yasunoxx▼Julia <yasunoxx gmail>
 */

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <cr_section_macros.h>

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
#include "glcd.h"

// High level I/O
void OutputData( uint8_t target, uint8_t data );
void OutputData4( uint8_t target, uint8_t data );
uint8_t InputData( uint8_t target );

// Low level I/O
void OutputData4sub( uint8_t data );
void setDatabusDirection( uint8_t );
void setControlLineDirection( void );
void putBufToLCD();
uint8_t *buf0, *buf1;
uint8_t *tempbuf;
uint8_t putCharToLCD( uint8_t );

// External functions
extern void FreeRTOSDelay( uint32_t ms ); // FreeRTOSCommonHooks.c

const uint32_t bitlines[ 32 ] = {
		0x1       , 0x2       , 0x4       , 0x8       ,
		0x10      , 0x20      , 0x40      , 0x80      ,
		0x100     , 0x200     , 0x400     , 0x800     ,
		0x1000    , 0x2000    , 0x4000    , 0x8000    ,
		0x10000   , 0x20000   , 0x40000   , 0x80000   ,
		0x100000  , 0x200000  , 0x400000  , 0x800000  ,
		0x1000000 , 0x2000000 , 0x4000000 , 0x8000000 ,
		0x10000000, 0x20000000, 0x40000000, 0x80000000
};

const uint8_t bitpos[ 8 ] = {
		LCD_D0, LCD_D1, LCD_D2, LCD_D3,
		LCD_D4, LCD_D5, LCD_D6, LCD_D7
};

//
//
void SetToBufLCD( uint8_t linenum, const char *pbuf )
{
	if( linenum == LF )
	{
		// shift old line to history buffer
		strcpy( ( char * )tempbuf, ( const char * )buf0 );
		strcpy( ( char * )buf0, ( const char * )buf1 );
		// clear new line
		strcpy( ( char * )buf1, pbuf );
	}
	else if( linenum == 0 )
	{
		strcpy( ( char * )buf0, pbuf );
	}
	else if( linenum == 1 )
	{
		strcpy( ( char * )buf1, pbuf );
	}

	putBufToLCD();
}
//
//
void DelayGLCD( uint32_t cycle )
{
	FreeRTOSDelay( cycle );
}

//
//
void InitGLCD()
{
	// Databus and LCD_DBL, LCD_DCS
	setDatabusDirection( IN );
	LPC_GPIO->DIR[ LCD_DB_PORT ] |= ( DBL_BIT | DCS_BIT );
	LPC_GPIO->DIR[ LCD_DB_PORT ] |= ( DRW_BIT | DEN_BIT | DRS_BIT );

	// Control line
	setControlLineDirection();

	// Output devices disable
//	privGPIOSetBitValue( LCD_DB_PORT, LCD_BL, HIGH );	//	Backlight On
	privGPIOSetBitValue( LCD_DB_PORT, LCD_CS1, LOW );
	privGPIOSetBitValue( LCD_CB_PORT, LCD_CS2, LOW );
	privGPIOSetBitValue( LCD_DB_PORT, LCD_E, LOW );
	privGPIOSetBitValue( LCD_DB_PORT, LCD_RW, LOW );
	privGPIOSetBitValue( LCD_DB_PORT, LCD_RS, LOW );
	privGPIOSetBitValue( LCD_CB_PORT, LCD_EX_EN0, LOW );
	privGPIOSetBitValue( LCD_CB_PORT, LCD_EX_EN1, HIGH );

#ifdef CLCD
	buf0 = ( uint8_t * )malloc( 64 );
	buf1 = ( uint8_t * )malloc( 64 );
	tempbuf = ( uint8_t * )malloc( 128 );
#endif
#ifdef GLCD
	buf0 = ( uint8_t * )malloc( 512 );
	buf1 = ( uint8_t * )malloc( 512 );
	tempbuf = ( uint8_t * )malloc( 1024 );
#endif
}

void InitGLCD2( void )
{
	// LCD Initial Sequence
#ifdef CLCD
	{
		ActCmdLCD;
			OutputData4sub( 0x0 );
			DelayGLCD( 1200UL );
			OutputData4sub( 0x3 );
			DelayGLCD( 120UL );
			OutputData4sub( 0x3 );
			DelayGLCD( 2UL );
			OutputData4sub( 0x3 );
			DelayGLCD( 2UL );
			OutputData4sub( 0x2 );

			OutputData4( CMD_GLCD, LCD_CMD_FUNC | 0b01000 );	// 4bit, 2 lines, 5x8
			OutputData4( CMD_GLCD, LCD_CMD_DISPOFF );

			OutputData4( CMD_GLCD, LCD_CMD_CLEAR );

			DelayGLCD( 6UL );
			OutputData4( CMD_GLCD, LCD_CMD_ENTMODE | 0b010 );	// increment
			OutputData4( CMD_GLCD, LCD_CMD_DISPON );

//			strcpy( ( char * )buf0, "Hello, 31337." );
//			strcpy( ( char * )buf1, "Test FreeRTOT\x08S" );
//			putBufToLCD();
			SetToBufLCD( 0, "Hello, 31337." );
			SetToBufLCD( 1, "Test FreeRTOT\x08S" );
	}
#endif
#ifdef GLCD
	{
		ActCmdLCD;
		Addr0LowLCD;
			OutputData( CMD_GLCD, LCD_CMD_DISPOFF );
			DelayGLCD( 1200UL );
			OutputData( CMD_GLCD, LCD_CMD_SLINE );
			DelayGLCD( 120UL );
			OutputData( CMD_GLCD, LCD_CMD_PADDR );
			DelayGLCD( 2UL );
			OutputData( CMD_GLCD, LCD_CMD_SADDR );

			OutputData( CMD_GLCD, LCD_CMD_DISPON );

			strcpy( ( char * )buf0, "Hello, 31337." );
			putBufToLCD();
	}
#endif

	// Exit: Initalize Complete
//	privGPIOSetBitValue( LCD_DB_PORT, LCD_DBL, LOW );	//	Backlight Off
}

//
//
void OutputData( uint8_t target, uint8_t data )
{
	uint32_t databuf = 0;
	uint8_t	loop, datatmp;

	datatmp = data;
	// generate output data
	for( loop = 0; loop < 8; loop++ )
	{
		if( ( datatmp & 0x01 ) )
		{
			databuf |= bitlines[ bitpos[ loop ] ];
		}
		datatmp >>= 1;
	}

	// data output
	privGPIOSetBitValue( LCD_DB_PORT, LCD_RW, LOW );
	setDatabusDirection( OUT );
	LPC_GPIO->CLR[ LCD_DB_PORT ] = LCD_DB;
	LPC_GPIO->SET[ LCD_DB_PORT ] = databuf;

	// enable target
	switch( target )
	{
		case CMD_GLCD:
			for( loop = 0; loop < 10; loop++ );
			privGPIOSetBitValue( LCD_DB_PORT, LCD_E, HIGH );
			for( loop = 0; loop < 120; loop++ );
			privGPIOSetBitValue( LCD_DB_PORT, LCD_E, LOW );
			for( loop = 0; loop < 10; loop++ );
			privGPIOSetBitValue( LCD_DB_PORT, LCD_RW, HIGH );
			for( loop = 0; loop < 20; loop++ );
			break;
		case CMD_EX1:
			privGPIOSetBitValue( LCD_CB_PORT, LCD_EX_EN1, HIGH );
			privGPIOSetBitValue( LCD_CB_PORT, LCD_EX_EN1, LOW );
			break;
		case CMD_EX0:
			privGPIOSetBitValue( LCD_CB_PORT, LCD_EX_EN0, HIGH );
			privGPIOSetBitValue( LCD_CB_PORT, LCD_EX_EN0, LOW );
			break;
		default:
			break;
	}
	// set input direection for safety
//	setDatabusDirection( IN );
}

void OutputData4( uint8_t target, uint8_t data )
{
	uint8_t tmp;

	tmp = data >> 4;
	OutputData4sub( tmp );
	tmp = data & 0x0F;
	OutputData4sub( tmp );
}

void OutputData4sub( uint8_t data )
{
	uint32_t databuf = 0;
	uint8_t	loop, datatmp;

	datatmp = data;
	datatmp <<= 4;
	// generate output data
	for( loop = 0; loop < 8; loop++ )
	{
		if( ( datatmp & 0x01 ) )
		{
			databuf |= bitlines[ bitpos[ loop ] ];
		}
		datatmp >>= 1;
	}

	// data output
	privGPIOSetBitValue( LCD_DB_PORT, LCD_RW, LOW );
	setDatabusDirection( OUT );
	LPC_GPIO->CLR[ LCD_DB_PORT ] = LCD_DB4;
	LPC_GPIO->SET[ LCD_DB_PORT ] = ( databuf & LCD_DB4 );

	// enable target(LCD ONLY)
	for( loop = 0; loop < 10; loop++ );
	privGPIOSetBitValue( LCD_DB_PORT, LCD_E, HIGH );
	for( loop = 0; loop < 120; loop++ );
	privGPIOSetBitValue( LCD_DB_PORT, LCD_E, LOW );
	for( loop = 0; loop < 120; loop++ );
	privGPIOSetBitValue( LCD_DB_PORT, LCD_RW, HIGH );
	for( loop = 0; loop < 20; loop++ );
	// set input direection for safety
//	setDatabusDirection( IN );
}

uint8_t InputData( uint8_t target )
{
	uint32_t databuf = 0;
	uint8_t	readdata = 0;

	setDatabusDirection( IN );
	// read data
	privGPIOSetBitValue( LCD_DB_PORT, LCD_RW, HIGH );
	// enable target
	switch( target )
	{
		case CMD_GLCD:
			privGPIOSetBitValue( LCD_DB_PORT, LCD_CS1, LOW );
			privGPIOSetBitValue( LCD_DB_PORT, LCD_E, HIGH );
			privGPIOSetBitValue( LCD_DB_PORT, LCD_E, LOW );
			break;
		case CMD_EX1:
			privGPIOSetBitValue( LCD_CB_PORT, LCD_EX_EN1, HIGH );
			break;
		case CMD_EX0:
			privGPIOSetBitValue( LCD_CB_PORT, LCD_EX_EN0, HIGH );
			break;
		default:
			break;
	}

	DelayGLCD( 2 );
	databuf = LPC_GPIO->PIN[ LCD_DB_PORT ];

	// disable target
	switch( target )
	{
		case CMD_GLCD:
			privGPIOSetBitValue( LCD_DB_PORT, LCD_CS1, HIGH );
			break;
		case CMD_EX1:
			privGPIOSetBitValue( LCD_CB_PORT, LCD_EX_EN1, LOW );
			break;
		case CMD_EX0:
			privGPIOSetBitValue( LCD_CB_PORT, LCD_EX_EN0, LOW );
			break;
		default:
			break;
	}

	// generate return data
	if( ( databuf & D7_BIT ) != 0 ) readdata |= 0x80;
	if( ( databuf & D6_BIT ) != 0 ) readdata |= 0x40;
	if( ( databuf & D5_BIT ) != 0 ) readdata |= 0x20;
	if( ( databuf & D4_BIT ) != 0 ) readdata |= 0x10;
	if( ( databuf & D3_BIT ) != 0 ) readdata |= 0x08;
	if( ( databuf & D2_BIT ) != 0 ) readdata |= 0x04;
	if( ( databuf & D2_BIT ) != 0 ) readdata |= 0x02;
	if( ( databuf & D1_BIT ) != 0 ) readdata |= 0x01;

	return readdata;
}

void setDatabusDirection( uint8_t direction )
{
	switch( direction )
	{
		case IN:
			LPC_GPIO->DIR[ LCD_DB_PORT ] &= ~LCD_DB;
			break;
		case OUT:
			LPC_GPIO->DIR[ LCD_DB_PORT ] |= LCD_DB;
			break;
		default:
			break;
	}
}

void setControlLineDirection()
{
	LPC_GPIO->DIR[ LCD_CB_PORT ] |= LCD_CB;
}

//
//
void putBufToLCD()
{
#ifdef CLCD
	uint8_t loop;

	ActCmdLCD;
		OutputData4( CMD_GLCD, LCD_CMD_CLEAR );	// Screen Clear
		DelayGLCD( 10UL );
		OutputData4( CMD_GLCD, LCD_CMD_ENTMODE | 0b010 ); // increment
	ActDataLCD;
		for( loop = 0; loop < 16; loop++ )
		{
			if( CHR_NULL == putCharToLCD( buf0[ loop ] ) )
			{
				break;
			}
		}
		for( loop = 0; loop < 16; loop++ )
		{
			if( loop == 0 )
			{
	#ifdef CLCD
				ActCmdLCD;
					OutputData4( CMD_GLCD, LCD_CMD_DDADDR | 0x40 );	// 2nd line
					OutputData4( CMD_GLCD, LCD_CMD_ENTMODE | 0b010 ); // increment
	#endif
				ActDataLCD;
			}
			if( CHR_NULL == putCharToLCD( buf1[ loop ] ) )
			{
				break;
			}
		}
#endif
#ifdef GLCD
	uint8_t paddr, saddr;

	ActCmdLCD;
	Addr0LowLCD;
		OutputData( CMD_GLCD, LCD_CMD_SLINE );
		OutputData( CMD_GLCD, LCD_CMD_PADDR );
		OutputData( CMD_GLCD, LCD_CMD_SADDR );
		for( paddr = 0; paddr < 7; paddr++ )
		{
			ActCmdLCD;
				OutputData( CMD_GLCD, LCD_CMD_PADDR + saddr );
			ActDataLCD;
				for( saddr = 0; saddr < 64; saddr++ )
				{
						OutputData( CMD_GLCD, buf0[ paddr * 8 + saddr ] );
				}
		}
#endif
}

uint8_t putCharToLCD( uint8_t chr )
{
#ifdef CLCD
	if( chr == BACKSPACE || chr == DELETE )	// '\x08' or '\x7F'
	{
		ActCmdLCD;
			OutputData4( CMD_GLCD, LCD_CMD_ENTMODE | 0b000 ); // decrement
		ActDataLCD;
			OutputData4( CMD_GLCD, ' ' );
		ActCmdLCD;
			OutputData4( CMD_GLCD, LCD_CMD_ENTMODE | 0b010 ); // increment
		ActDataLCD;
	}
	else
#endif
	if( chr == CHR_NULL )
	{
		return chr;
	}
	else
	{
		// ActDataLCD;
			OutputData4( CMD_GLCD, chr );
	}

	return chr;
}
