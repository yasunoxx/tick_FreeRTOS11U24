/*
 ** glcd.c -- 128x64 dots LCD(SG12864) routines, for LPC11xx/mcuXpresso
 ** (C)2025 yasunoxx▼Julia <yasunoxx gmail>
 */

#include <stdint.h>
#include <cr_section_macros.h>
#ifdef __USE_CMSIS
#include "LPC11Uxx.h"
#else
#include "chip.h"
#endif

#include "gpio.h"
#define	IN	0
#define	OUT	1
#define LOW	0
#define HIGH 1

#include "glcd.h"

// High level I/O


// Low level I/O
void OutputData( uint8_t target, uint8_t data );
void OutputData4( uint8_t target, uint8_t data );
void OutputData4sub( uint8_t data );
uint8_t InputData( uint8_t target );

extern void privGPIOSetBitValue( uint32_t, uint32_t, uint32_t ); // privgpio.c
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

void setDatabusDirection( uint8_t direction );
void setControlLineDirection( void );

void DelayGLCD( uint32_t cycle )
{
	FreeRTOSDelay( cycle );
}

void InitGLCD( void )
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
}

void InitGLCD2( void )
{
	// LCD Initial Sequence
	{
		ActCmdLCD;
			OutputData4sub( 0x00 );
			DelayGLCD( 600UL );
			OutputData4sub( 0x03 );
			DelayGLCD( 120UL );
			OutputData4sub( 0x03 );
			DelayGLCD( 2UL );
			OutputData4sub( 0x03 );
			DelayGLCD( 2UL );
			OutputData4sub( 0x02 );

			OutputData4( CMD_GLCD, 0x28 );
			OutputData4( CMD_GLCD, 0x08 );

			OutputData4( CMD_GLCD, 0x01 );

			DelayGLCD( 6UL );
			OutputData4( CMD_GLCD, 0x06 );

		ActDataLCD;
			OutputData4( CMD_GLCD, 0x0AA );
			OutputData4( CMD_GLCD, 0x55 );
	}

	// Exit: Initalize Complete
//	privGPIOSetBitValue( LCD_DB_PORT, LCD_DBL, LOW );	//	Backlight Off
}

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
//		LPC_GPIO->DIR[ LCD_CB_PORT ] &= ~LCD_CB;
	LPC_GPIO->DIR[ LCD_CB_PORT ] |= LCD_CB;
}
