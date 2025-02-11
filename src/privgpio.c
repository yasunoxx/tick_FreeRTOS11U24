// priv_gpio.c -- gpio.c for private use

// Original copyright:
/****************************************************************************
 *   $Id:: gpio.c 6172 2011-01-13 18:22:51Z usb00423                        $
 *   Project: NXP LPC11Uxx GPIO example
 *
 *   Description:
 *     This file contains GPIO code example which include GPIO 
 *     initialization, GPIO interrupt handler, and related APIs for 
 *     GPIO access.
 *
****************************************************************************/
#include "LPC11Uxx.h"			/* LPC11Uxx Peripheral Registers */

#include <stdint.h>
#include "gpio.h"

/*****************************************************************************
** Function name:		GPIOGetPinValue
**
** Descriptions:		Read Current state of port pin, PIN register value
**
** parameters:			port num, bit position
** Returned value:		None
**
*****************************************************************************/
uint32_t GPIOGetPinValue( uint32_t portNum, uint32_t bitPosi )
{
  uint32_t regVal = 0;

  if( bitPosi < 0x20 )
  {
	if ( LPC_GPIO->PIN[portNum] & (0x1<<bitPosi) )
	{
	  regVal = 1;
	}
  }
  else if( bitPosi == 0xFF )
  {
	regVal = LPC_GPIO->PIN[portNum];
  }
  return ( regVal );
}

/*****************************************************************************
** Function name:		GPIOSetBitValue
**
** Descriptions:		Set/clear a bit in a specific position
**
** parameters:			port num, bit position, bit value
**
** Returned value:		None
**
*****************************************************************************/
void privGPIOSetBitValue( uint32_t portNum, uint32_t bitPosi, uint32_t bitVal )
{
  if ( bitVal )
  {
	LPC_GPIO->SET[portNum] = 1<<bitPosi;
  }
  else
  {
	LPC_GPIO->CLR[portNum] = 1<<bitPosi;
  }
  return;
}

/*****************************************************************************
** Function name:		GPIOSetDir
**
** Descriptions:		Set the direction in GPIO port
**
** parameters:			portNum, bit position, direction (1 out, 0 input)
**
** Returned value:		None
**
*****************************************************************************/
void GPIOSetDir( uint32_t portNum, uint32_t bitPosi, uint32_t dir )
{
  if( dir )
  {
	LPC_GPIO->DIR[portNum] |= (1<<bitPosi);
  }
  else
  {
	LPC_GPIO->DIR[portNum] &= ~(1<<bitPosi);
  }
  return;
}
