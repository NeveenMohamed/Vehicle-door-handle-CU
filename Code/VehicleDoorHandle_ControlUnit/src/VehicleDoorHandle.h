/*
 * VehicleDoorHandle.h
 *
 *  Created on: May 9, 2023
 *      Author: Anwar
 */

#ifndef VEHICLEDOORHANDLE_H_
#define VEHICLEDOORHANDLE_H_

#include "Led.h"
#include "Button.h"
#include "Rcc.h"

#define VDH_BUTTONS_PORT					GPIO_B
#define VDH_HANDLE_LOCK_UNLOCK_BUTTON		5
#define VDH_DOOR_LOCK_UNLOCK_BUTTON			8

#define VDH_LEDS_PORT						GPIO_A
#define VDH_LOCK_LED						2
#define VDH_HAZARD_LIGHTS_LED				4
#define VDH_AMBIENT_LIGHT_LED				6


void VDH_defaultState(void);
void VDH_doorUnlock(void);



#endif /* VEHICLEDOORHANDLE_H_ */
