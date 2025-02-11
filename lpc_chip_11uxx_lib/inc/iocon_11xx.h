/*
 * @brief IOCON registers and control functions
 *
 * @note
 * Copyright(C) NXP Semiconductors, 2012
 * All rights reserved.
 *
 * @par
 * Software that is described herein is for illustrative purposes only
 * which provides customers with programming information regarding the
 * LPC products.  This software is supplied "AS IS" without any warranties of
 * any kind, and NXP Semiconductors and its licensor disclaim any and
 * all warranties, express or implied, including all implied warranties of
 * merchantability, fitness for a particular purpose and non-infringement of
 * intellectual property rights.  NXP Semiconductors assumes no responsibility
 * or liability for the use of the software, conveys no license or rights under any
 * patent, copyright, mask work right, or any other intellectual property rights in
 * or to any products. NXP Semiconductors reserves the right to make changes
 * in the software without notification. NXP Semiconductors also makes no
 * representation or warranty that such application will be suitable for the
 * specified use without further testing or modification.
 *
 * @par
 * Permission to use, copy, modify, and distribute this software and its
 * documentation is hereby granted, under NXP Semiconductors' and its
 * licensor's relevant copyrights in the software, without fee, provided that it
 * is used in conjunction with NXP Semiconductors microcontrollers.  This
 * copyright, permission, and disclaimer notice must appear in all copies of
 * this code.
 */

#ifndef __IOCON_11XX_H_
#define __IOCON_11XX_H_

/** @defgroup IOCON_11XX CHIP: LPC11xx IO Control driver
 * @ingroup CHIP_11XX_Drivers
 * @{
 */

/**
 * @brief IO Configuration Unit register block structure
 */
typedef struct {						/*!< LPC11AXX/LPC11UXX/LPC11EXX IOCON Structure */
	__IO uint32_t  PIO0[24];
	__IO uint32_t  PIO1[32];
} LPC_IOCON_T;

/**
 * IOCON function and mode selection definitions
 * See the User Manual for specific modes and functions supported by the
 * various LPC11xx devices. Functionality can vary per device.
 */
#define IOCON_FUNC0             0x0				/*!< Selects pin function 0 */
#define IOCON_FUNC1             0x1				/*!< Selects pin function 1 */
#define IOCON_FUNC2             0x2				/*!< Selects pin function 2 */
#define IOCON_FUNC3             0x3				/*!< Selects pin function 3 */
#define IOCON_FUNC4             0x4				/*!< Selects pin function 4 */
#define IOCON_FUNC5             0x5				/*!< Selects pin function 5 */
#define IOCON_FUNC6             0x6				/*!< Selects pin function 6 */
#define IOCON_FUNC7             0x7				/*!< Selects pin function 7 */
#define IOCON_MODE_INACT        (0x0 << 3)		/*!< No addition pin function */
#define IOCON_MODE_PULLDOWN     (0x1 << 3)		/*!< Selects pull-down function */
#define IOCON_MODE_PULLUP       (0x2 << 3)		/*!< Selects pull-up function */
#define IOCON_MODE_REPEATER     (0x3 << 3)		/*!< Selects pin repeater function */
#define IOCON_HYS_EN            (0x1 << 5)		/*!< Enables hysteresis */
#define IOCON_INV_EN            (0x1 << 6)		/*!< Enables invert function on input */
#define IOCON_ADMODE_EN         (0x0 << 7)		/*!< Enables analog input function (analog pins only) */
#define IOCON_DIGMODE_EN        (0x1 << 7)		/*!< Enables digital function (analog pins only) */
#define IOCON_SFI2C_EN          (0x0 << 8)		/*!< I2C standard mode/fast-mode */
#define IOCON_STDI2C_EN         (0x1 << 8)		/*!< I2C standard I/O functionality */
#define IOCON_FASTI2C_EN        (0x2 << 8)		/*!< I2C Fast-mode Plus */
#define IOCON_FILT_DIS          (0x1 << 8)		/*!< Disables noise pulses filtering (10nS glitch filter) */
#define IOCON_OPENDRAIN_EN      (0x1 << 10)		/*!< Enables open-drain function */

/**
 * IOCON function and mode selection definitions (old)
 * For backwards compatibility.
 */
#define MD_PLN					(0x0 << 3)		/*!< Disable pull-down and pull-up resistor at resistor at pad */
#define MD_PDN					(0x1 << 3)		/*!< Enable pull-down resistor at pad */
#define MD_PUP					(0x2 << 3)		/*!< Enable pull-up resistor at pad */
#define MD_BUK					(0x3 << 3)		/*!< Enable pull-down and pull-up resistor at resistor at pad (repeater mode) */
#define MD_HYS					(0x1 << 5)		/*!< Enable hysteresis */
#define MD_INV					(0x1 << 6)		/*!< Invert enable */
#define MD_ADMODE				(0x0 << 7)		/*!< Select analog mode */
#define MD_DIGMODE				(0x1 << 7)		/*!< Select digitial mode */
#define MD_DISFIL				(0x0 << 8)		/*!< Disable 10nS input glitch filter */
#define MD_ENFIL				(0x1 << 8)		/*!< Enable 10nS input glitch filter */
#define MD_SFI2C				(0x0 << 8)		/*!< I2C standard mode/fast-mode */
#define MD_STDI2C				(0x1 << 8)		/*!< I2C standard I/O functionality */
#define MD_FASTI2C				(0x2 << 8)		/*!< I2C Fast-mode Plus */
#define MD_OPENDRAIN			(0x1 << 10)		/*!< Open drain mode bit */
#define FUNC0 0x0
#define FUNC1 0x1
#define FUNC2 0x2
#define FUNC3 0x3
#define FUNC4 0x4
#define FUNC5 0x5
#define FUNC6 0x6
#define FUNC7 0x7

/**
 * @brief	Sets I/O Control pin mux
 * @param	pIOCON		: The base of IOCON peripheral on the chip
 * @param	port		: GPIO port to mux
 * @param	pin			: GPIO pin to mux
 * @param	modefunc	: OR'ed values or type IOCON_*
 * @return	Nothing
 */
void Chip_IOCON_PinMuxSet(LPC_IOCON_T *pIOCON, uint8_t port, uint8_t pin, uint32_t modefunc);

/**
 * @brief	I/O Control pin mux
 * @param	pIOCON	: The base of IOCON peripheral on the chip
 * @param	port	: GPIO port to mux
 * @param	pin		: GPIO pin to mux
 * @param	mode	: OR'ed values or type IOCON_*
 * @param	func	: Pin function, value of type IOCON_FUNC?
 * @return	Nothing
 */
STATIC INLINE void Chip_IOCON_PinMux(LPC_IOCON_T *pIOCON, uint8_t port, uint8_t pin, uint16_t mode, uint8_t func)
{
	Chip_IOCON_PinMuxSet(pIOCON, port, pin, (uint32_t) (mode | func));
}



#endif /* __IOCON_11XX_H_ */
