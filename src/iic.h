// iic.h -- I2C and OLED driver for LPC11xx

#define SPEED_100KHZ         100000
#define SPEED_400KHZ         400000
#define I2C_DEFAULT_SPEED    SPEED_400KHZ
#define I2C_FASTPLUS_BIT     0

#if (I2C_DEFAULT_SPEED > SPEED_400KHZ)
#undef  I2C_FASTPLUS_BIT
#define I2C_FASTPLUS_BIT IOCON_FASTI2C_EN
#endif

// Target configuration
#define ADDR_SSD1306    (0x78>>1)
#include "reg_ssd1306.h"

extern void Check_BlinkLED1( uint8_t );
extern void InitIIC( void );
extern bool InitSSD1306( void );
#define send_ssd1306(p_buf,len) Chip_I2C_MasterSend(I2C0,ADDR_SSD1306,p_buf,len)
#define send_pcf8523(p_buf,len) Chip_I2C_MasterSend(I2C0,ADDR_PCF8523,p_buf,len)
