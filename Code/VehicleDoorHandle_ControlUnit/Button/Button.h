/*
 * Button.h
 *
 *  Created on: May 8, 2023
 *      Author: Anwar
 */

#ifndef BUTTON_H_
#define BUTTON_H_

#define BUTTON_PRESSED		GPIO_DATA_FALLING
#define BUTTON_RELEASED		GPIO_DATA_RISING

#include "Gpio.h"
#include "Delay.h"

void BUTTON_Init(uint8 portName, uint8 pinNum);

uint8 BUTTON_ReadState(uint8 portName, uint8 pinNum);


#endif /* BUTTON_H_ */
