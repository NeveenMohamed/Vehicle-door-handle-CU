/*
 * ====================================================================================================================================
 *  Project		: Vehicle Door Handle Control Unit
 * 	File Name	: VehicleDoorHandle.h
 *	Team 		: 20
 *	Members		: Abdelrahman Yasser - Omar Ahmed Anwar - Mo'men Mohamed - Neveen Mohamed
 *  ===================================================================================================================================
 */

#ifndef VEHICLEDOORHANDLE_H_
#define VEHICLEDOORHANDLE_H_

/*---------------------- Includes -----------------------*/
#include "Led.h"
#include "Button.h"
#include "Rcc.h"
#include "gpt.h"
/*
 * Comment (#include "Exti.h") if you want to disable interrupt
 * of the door button.
 */
#include "Exti.h"
/*---------------------------------------------------------*/

/*---------------------- Definitions ----------------------*/
// Buttons Definitions
#define VDH_BUTTONS_PORT							GPIO_B
#define VDH_HANDLE_LOCK_UNLOCK_BUTTON				5
#define VDH_DOOR_LOCK_UNLOCK_BUTTON					8

// LEDs Definitions
#define VDH_LEDS_PORT								GPIO_A
#define VDH_TEST_TIMERS_LED                         1
#define VDH_LOCK_LED								2
#define VDH_HAZARD_LIGHTS_LED						4
#define VDH_AMBIENT_LIGHT_LED						6

// Delays
#define VDH_SECURITY_TIME_IN_ms						10000
#define VDH_HAZZARD_BLINKING_TIME					500
#define VDH_AMBIENT_CLOSED_DOOR_BLINKING_TIME		1000
#define VDH_AMBIENT_HANDLE_UNLOCK_BLINKING_TIME		2000
#define VDH_HAZZARD_HANDLE_LOCK_BLINKING_TIME		2000

// Magic Numbers
#define VDH_INIT_NUM_TICKS							0
#define VDH_ONE_BIT									1
#define VDH_TWO_BITS								2
/*------------------------------------------------------*/

/*---------------------- Externs ----------------------*/

/*
 * Flag to indicate whether the timer is on or not
 * Extern (From Button.c)
 */
extern BUTTON_TimerState timerIsOn;
/*------------------------------------------------------*/

/*---------------------- Defined data types ----------------------*/
typedef enum{
	VDH_DOOR_IS_CLOSED, VDH_DOOR_IS_OPENED
}Door_State;

typedef enum{
	VDH_HANDLE_IS_LOCKED,
	VDH_HANDLE_IS_UNLOCKED,
	VDH_HANDLE_FLOATING_STATE
}Handle_State;

typedef enum{
	VDH_10SEC_NOT_ELAPSED, VDH_10SEC_ELAPSED
}Time_State;

typedef enum{
	VDH_ACTION_IS_NOT_DONE, VDH_ACTION_IS_DONE
}Action_State;

/*
 * Bit Field to contain all flags.
 */
typedef union{
	uint8 flags;
	struct{
		Handle_State handle				: VDH_TWO_BITS;
		Action_State door_action		: VDH_ONE_BIT;
		Action_State door_changeState	: VDH_ONE_BIT;
		Action_State end_action			: VDH_ONE_BIT;
		Action_State doorUnlock_action	: VDH_ONE_BIT;
		Door_State door					: VDH_ONE_BIT;
		Time_State timer_10SecFlag		: VDH_ONE_BIT;
	}combinedFlags;
}VDH_Flags;
/*------------------------------------------------------*/

/*---------------------- Functions Prototype ----------------------*/

/*
 * description:
 * 	Argument(s):
 * 		--> state: State of the LEDs ON/OFF.
 * 	Function to Activate all application's LEDs ON/OFF.
 */
void VDH_stateAllLeds(LED_States);

/*
 * description:
 * 	Argument(s):
 * 		--> No Arguments.
 * 	Function to Initialize buttons, LEDs, RCC, GPT, EXTI.
 */
void VDH_init(void);

/*
 * description:
 * 	Argument(s):
 * 		--> No Arguments.
 * 	Function is Excuted when the handle is unlocked.
 */
void VDH_handleUnlock(void);

/*
 * description:
 * 	Argument(s):
 * 		--> No Arguments.
 * 	Function is Excuted when the handle is locked or when the handle
 * 	is unlocked and no action has been done on the door button.
 */
void VDH_noActionState(void);

/*
 * description:
 * 	Argument(s):
 * 		--> No Arguments.
 * 	Function is Excuted when the handle is unlocked
 * 	and the door is closed.
 */
void VDH_doorIsClosed(void);

/*
 * description:
 * 	Argument(s):
 * 		--> No Arguments.
 * 	If ExtI.h is included:
 * 		this is the handler function excuted by the interrupt.
 * 	Function is Excuted when the door Button is pressed.
 * 	and the door is closed.
 */
void VDH_doorButtonPressed(void);

/*
 * description:
 * 	Argument(s):
 * 		--> No Arguments.
 * 	Function to initialize allFlags instance.
 * 	The initial states of the application's flags.
 */
void VDH_FlagInit(void);
/*--------------------------------------------------------------------*/

#endif /* VEHICLEDOORHANDLE_H_ */
