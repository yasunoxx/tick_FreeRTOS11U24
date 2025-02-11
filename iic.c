//

#include <stdint.h>
#include <cr_section_macros.h>
#ifdef __USE_CMSIS
#include "LPC11Uxx.h"
#else
#include "chip.h"
#include "clock_11xx.h"
#endif
#include "i2c_11xx.h"

#include "iic.h"

extern volatile uint32_t msTicks; /* counts 1ms timeTicks */
//
__INLINE static void DelayIIC( uint32_t dlyTicks )
{
  uint32_t curTicks;

  curTicks = msTicks;
  while( ( msTicks - curTicks ) < dlyTicks );
}

extern void BlinkLED1( uint8_t );

void InitIIC()
{
    Chip_I2C_Init( I2C0 );
    Chip_I2C_SetClockRate( I2C0, I2C_DEFAULT_SPEED );    // initial value

	LPC_I2C->CONCLR = (I2C_CON_AA | I2C_CON_I2EN);
// NOTE: SCL/SDA共に'H'となる事は確認済
}

void InitSSD1306()
{
// FIXME: start conditionが出ていない

    uint8_t cmdbuf[ 2 ];

    // ... configuration sequence ...
    #define	LED_PORT	0
    #define	LED1_BIT	23
    #define	LED2_BIT	20
    #define LED_ON 0		// set port to turn on led
    #define LED_OFF 1		// set port to turn off led
    GPIOSetBitValue( LED_PORT, LED2_BIT, LED_ON );

    start_condx;
//    stop_condx;
//    start_condx;
    BlinkLED1( 2 );
    cmdbuf[ 0 ] = SSD1306_MUX_RATIO;   cmdbuf[ 1 ] = 0x3F; send_ssd1306( cmdbuf, 2 );
    BlinkLED1( 3 );
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
    stop_condx;
    GPIOSetBitValue( LED_PORT, LED1_BIT, LED_OFF );
}
