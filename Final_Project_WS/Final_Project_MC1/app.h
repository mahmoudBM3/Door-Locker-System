/*******************************************************************************
 * File Description:
 * Author      : Mahmoud Sherif Mahmoud
 * Module      : Door-Locker System Application Functions
 * Level  	   : High
 * Description : This file Contains the functions of the Door Locker Application
 * µC		   : ATMega 32 (8-BIT)
 * Date 	   : 4/11/2022(November)
 *******************************************************************************/
#ifndef APP_H_
#define APP_H_
/*******************************************************************************
 * 							Communication Bytes
 *******************************************************************************/
#define	WRONG_CONFIRMATION			255
#define OPEN_DOOR					'+'
#define RESET_PASSWORD				'-'
#define SYSTEM_START				255
#define PASSWORD_MATCHED			254
#define PASSWORD_UNMATCHED			253
#define PASSWORD_TRIALS_EXHAUSTED	252
/*******************************************************************************
 * 							  Common Numbers
 *******************************************************************************/
#define KEYPAD_DELAY				300
#define ENTER_KEY					' '
#define OPEN_DOOR_TIME				15
#define	CLOSE_DOOR_TIME				15
#define	HOLD_DOOR_TIME				3
#define BUZZER_TIME					60
#define ONE_SECOND_COUNT			(1000*8)
/********************************************************************************
 * 							Function Prototypes
 *******************************************************************************/
/**
 * This function is called to change the system Password
 */
void APP_setPassword(void);
/**
 * This is the main function that is used to control the system when in main menu
 */
void APP_systemControl(void);
/**
 * This function is used to get the Password as input from the used
 */
void APP_getPassword(void);
/**
 * This function is used to display that the door is unlocking and then call the
 * function that displays that it's unlocked when it's unlocked
 */
void APP_displayDoorUnlocking(void);
/**
 * This function is used to display that the door is unlocked and then call the
 * function that displays that it's Locking when it's locking
 */
void APP_displayDoorUnlocked(void);
/**
  * This function is used to display that the door is locking and stop the timer
 */
void APP_displayDoorLocking(void);
/**
 * This function is used to stop the timer when the door is unlocked
 */
void APP_displayDoorComplete(void);
#endif /* APP_H_ */
