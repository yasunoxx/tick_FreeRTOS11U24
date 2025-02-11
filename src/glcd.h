/*
 ** glcd.h -- 128x64 dots LCD(SG12864) routines, for LPC11xx/mcuXpresso
 ** (C)2025 yasunoxx▼Julia <yasunoxx gmail>
 */

// SG12864A Graph. LCD
// Commands
#define	LCD_CMD_DISPON	0x03F
#define	LCD_CMD_DISPOFF	0x03E
#define	LCD_CMD_SLINE	0b11000000
#define	LCD_CMD_PADDR	0b10111000
#define	LCD_CMD_SADDR	0b01000000

// Status
#define	LCD_STAT_BUSY		0b10000000
#define	LCD_STAT_DISPOFF	0b00100000
#define	LCD_STAT_RESET		0b00010000


// LCD bus definition: Bb-LPC11U24
#define LCD_DB_PORT 1	// LCD Data bus
//               |       |       |       |       |
#define	LCD_DB	0b10011100011110000000000000000000
#define	LCD_D7	31
#define	D7_BIT	0b10000000000000000000000000000000
#define	LCD_D6	28
#define	D6_BIT	0b00010000000000000000000000000000
#define	LCD_D5	27
#define	D5_BIT	0b00001000000000000000000000000000
#define	LCD_D4	26
#define	D4_BIT	0b00000100000000000000000000000000
#define	LCD_D3	22
#define	D3_BIT	0b00000000010000000000000000000000
#define	LCD_D2	21
#define	D2_BIT	0b00000000001000000000000000000000
#define	LCD_D1	20
#define	D1_BIT	0b00000000000100000000000000000000
#define	LCD_D0	19		// D7, D6-D4, D3-D0
#define	D0_BIT	0b00000000000010000000000000000000
//               |       |       |       |       |
#define	LCD_DRS	25
#define	DRS_BIT 0b00000010000000000000000000000000
#define	LCD_DRW	24		// R/W
#define	DRW_BIT	0b00000001000000000000000000000000
#define	LCD_DEN	23
#define	DEN_BIT	0b00000000100000000000000000000000
#define	LCD_DBL	14
#define	DBL_BIT	0b00000000000000000100000000000000
#define	LCD_DCS	13
#define	DCS_BIT	0b00000000000000000010000000000000

#define LCD_CB_PORT 0		// LCD Control line
#define	LCD_CB	0b00000000110000100000000000000000
#define	LCD_EX_A0	LCD_DRS	// Ext. A0
#define	LCD_EX_EN1	22		// Ext. E sig.
#define	LCD_EX_EN0	23		// Ext. E sig.
#define LCD_CCS     17      // BUZZER

#define	CMD_GLCD	1
#define	CMD_EX0		2
#define	CMD_EX1		3
#define	LCD_BL	LCD_DBL
#define LCD_RW  LCD_DRW
#define LCD_RS  LCD_DRS
#define LCD_E   LCD_DEN
#define LCD_CS1 LCD_DCS
#define LCD_CS2 LCD_CCS

extern uint8_t InitGLCD( void );
extern void OutputData( uint8_t target, uint8_t data );
extern uint8_t InputData( uint8_t target );
#define	DispLowLCD		privGPIOSetBitValue( LCD_DB_PORT, LCD_DCS, LOW );
#define	DispHighLCD		privGPIOSetBitValue( LCD_DB_PORT, LCD_DCS, HIGH );
#define	Addr0LowLCD		privGPIOSetBitValue( LCD_DB_PORT, LCD_DRS, LOW )
#define	Addr0HighLCD	privGPIOSetBitValue( LCD_DB_PORT, LCD_DRS, HIGH )
#define	ActCmdLCD		privGPIOSetBitValue( LCD_DB_PORT, LCD_DRS, LOW )
#define	ActDataLCD		privGPIOSetBitValue( LCD_DB_PORT, LCD_DRS, HIGH )
