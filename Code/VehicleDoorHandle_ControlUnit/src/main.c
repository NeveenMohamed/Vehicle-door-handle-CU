/*
 * ====================================================================================================================================
 *  Project		: Vehicle Door Handle Control Unit
 * 	File Name	: main.c
 *	Team 		: 20
 *	Members		: Abdelrahman Yasser - Omar Ahmed Anwar - Mo'men Mohamed - Neveen Mohamed
 *  ===================================================================================================================================
 */

/*---------------------- Includes -----------------------*/
#include "VehicleDoorHandle.h"
/*---------------------------------------------------------*/

/*---------------------- Global Variables -----------------------*/

uint32 first_time = 0; // variable holds first time for each event.

VDH_Flags allFlags; // An instant from all flags bit field.

TIMx_Configue Timer = {
		TIM2,
		4000,
		GPT_Up_Counting
};
/*----------------------------------------------------------------------------------------------------*/

/*---------------------- Global Variables ------------------------------------------------------------*/

/*
 * extern (From VehicleDoorHandle.c) Number of ticks incremented every 500 ms to handle blinking time
 * for end action to re-intialize it every handle button press.
 */
extern uint8 endAction_noOfTicks;

/*
 * extern (From VehicleDoorHandle.c) Number of ticks incremented every 500 ms to handle blinking time
 * for handle unlock action to re-intialize it every handle button press.
 */
extern uint8 handleUnlock_noOfTicks;
/*----------------------------------------------------------------------------------------------------*/

/*------------------------------------------ Main Function -------------------------------------------*/
int main(void) {

	VDH_init(); // Initialize buttons, LEDs, RCC, GPT, EXTI

/*------------------------------------------ Super Loop -------------------------------------------*/

	/*
	 * it is excuted periodically.
	 */
	while(1){

		/*
		 * Check if the handle button is pressed
		 * and handle door is not opened.
		 */
		if (BUTTON_ReadState(VDH_BUTTONS_PORT, VDH_HANDLE_LOCK_UNLOCK_BUTTON) == BUTTON_PRESSED
				&& allFlags.combinedFlags.door != VDH_DOOR_IS_OPENED) {

			timerIsOn = BUTTON_TimerIsOn; // mark the timer as on for button driver.

			endAction_noOfTicks = 0; // Re-intialize end action number of ticks.

			handleUnlock_noOfTicks = 0;  // Re-intialize handle unlock action number of ticks.

			/*
			 * For each button press toggle the state of the handle
			 * initial state as floating state indicates that it is the first press on the handle
			 * this state exists to avoid doing (noActionState) function (handle locked action).
			 */
			if(allFlags.combinedFlags.handle == VDH_HANDLE_FLOATING_STATE){
				allFlags.combinedFlags.handle = VDH_HANDLE_IS_LOCKED;
			}

			if (allFlags.combinedFlags.handle == VDH_HANDLE_IS_LOCKED) {
				allFlags.combinedFlags.handle = VDH_HANDLE_IS_UNLOCKED;
			}
			else {
				allFlags.combinedFlags.handle = VDH_HANDLE_IS_LOCKED;
				allFlags.combinedFlags.door_changeState = VDH_ACTION_IS_NOT_DONE;
			}

			VDH_stateAllLeds(LED_ON); // Activate all LEDs.


			allFlags.combinedFlags.end_action = VDH_ACTION_IS_NOT_DONE; // Mark end action as not done

			allFlags.combinedFlags.timer_10SecFlag = VDH_10SEC_NOT_ELAPSED; // Mark 10 Seconds timer as not elapsed

			allFlags.combinedFlags.doorUnlock_action = VDH_ACTION_IS_NOT_DONE; // Mark door unlock action as not done

			GPT_StartTimer(&Timer,VDH_SECURITY_TIME_IN_ms); // start the timer for 10 seconds

			first_time = GPT_GetElapsedTime(&Timer); // Update first time.

		}

		/*
		 * Check if the handle is unlocked
		 */
		if (allFlags.combinedFlags.handle == VDH_HANDLE_IS_UNLOCKED) {

			/*
			 * Check if the handle unlock action is done or not
			 */
			if (allFlags.combinedFlags.doorUnlock_action == VDH_ACTION_IS_NOT_DONE) {
				VDH_handleUnlock(); // periodically excute the handle unlock action until it is done.
			}

#ifndef EXTI_H_ // In case Exti.h is included

			/*
			 * Check if the door button is pressed
			 */
			if (BUTTON_ReadState(VDH_BUTTONS_PORT, VDH_DOOR_LOCK_UNLOCK_BUTTON) == BUTTON_PRESSED) {

				VDH_doorButtonPressed(); // periodically excute the door button pressed action until it is done.
			}

#endif
			/*
			 * Check if door is closed
			 * and door action is not done yet
			 */
			if ((allFlags.combinedFlags.door == VDH_DOOR_IS_CLOSED)
					&& (allFlags.combinedFlags.door_action == VDH_ACTION_IS_NOT_DONE)) {

				VDH_doorIsClosed(); // periodically excute the door is closed action until it is done.

			}

			/*
			 * check if all that true:
			 * 		--> Time is elapsed
			 * 		--> 10 seconeds flag is not elapsed
			 * 		--> door is closed
			 * 		--> door changing state is not done yet
			 */
			if (GPT_CheckTimeIsElapsed(&Timer) == GPT_Elapsed
					&& allFlags.combinedFlags.timer_10SecFlag == VDH_10SEC_NOT_ELAPSED
					&& allFlags.combinedFlags.door == VDH_DOOR_IS_CLOSED
					&& allFlags.combinedFlags.door_changeState == VDH_ACTION_IS_NOT_DONE) {

				GPT_StartTimer(&Timer,VDH_HAZZARD_HANDLE_LOCK_BLINKING_TIME); // start time for 2 seconds to excute noActionState function

				allFlags.combinedFlags.timer_10SecFlag = VDH_10SEC_ELAPSED; // Mark 10 seconds time as elapsed.

				first_time = GPT_GetElapsedTime(&Timer); // update first time.

				VDH_stateAllLeds(LED_OFF); // Switch OFF all LEDs.

				LED_Activate(VDH_LEDS_PORT, VDH_HAZARD_LIGHTS_LED, LED_ON); // Activate Hazzard LED.


			}

			/*
			 * Check if 10 seconds timer is elapsed
			 */
			if (allFlags.combinedFlags.timer_10SecFlag == VDH_10SEC_ELAPSED){

				VDH_noActionState(); // periodically excute noAction state until it is done.

			}
		}

		/*
		 * if the handle button is locked
		 * and the door is closed.
		 */
		else if(allFlags.combinedFlags.handle == VDH_HANDLE_IS_LOCKED
				&& allFlags.combinedFlags.door == VDH_DOOR_IS_CLOSED){

			LED_Activate(VDH_LEDS_PORT, VDH_AMBIENT_LIGHT_LED, LED_OFF); // Turn OFF ambient LED.

			LED_Activate(VDH_LEDS_PORT, VDH_LOCK_LED, LED_OFF); // Turn OFF vehicle LED.

			VDH_noActionState(); // periodically excute noAction state until it is done.

		}
	}
/*----------------------------------------------------------------------------------------------------*/

	return 0;
}
/*----------------------------------------------------------------------------------------------------*/




