// iic.h -- I2C driver for LPC11xx

#include "i2c_11xx.h"
#include "gpio.h"

#define SPEED_100KHZ         100000
#define SPEED_400KHZ         400000
#define I2C_DEFAULT_SPEED    SPEED_100KHZ
#define I2C_FASTPLUS_BIT     0

#if (I2C_DEFAULT_SPEED > SPEED_400KHZ)
#undef  I2C_FASTPLUS_BIT
#define I2C_FASTPLUS_BIT IOCON_FASTI2C_EN
#endif

// Target configuration
#define ADDR_PCF8523    0x0D0
#define CMD_READ_PCF8523    0b00000000
#define CMD_WRITE_PCF8523    0b00000000
#include "reg_pcf8523.h"

#define ADDR_SSD1306    0x78
#include "reg_ssd1306.h"

#define send_ssd1306(p_buf,len) Chip_I2C_MasterSend(I2C0,ADDR_SSD1306,p_buf,len)
#define	start_condx	LPC_I2C->CONSET=(I2C_CON_I2EN|I2C_CON_STA|I2C_CON_AA);
#define	stop_condx	LPC_I2C->CONSET=(I2C_CON_STO|I2C_CON_AA);
void InitIIC( void );
void InitSSD1306( void );
