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
 * 							Included Libraries
 *******************************************************************************/
#include "std_types.h"
typedef enum {
	PASSWORD_WRONG, PASSWORD_CORRECT
} APP_PasswordCompareType;
/*******************************************************************************
 * 							Communications Bytes
 *******************************************************************************/
#define	WRONG_CONFIRMATION	255
#define OPEN_DOOR			'+'
#define RESET_PASSWORD		'-'
#define SET_PASSWORD		255
#define SYSTEM_START		255
#define PASSWORD_MATCHED	254
#define PASSWORD_UNMATCHED	253
#define GET_PASSWORD		253
#define PASSWORD_TRIALS_EXHAUSTED 252
#define ENTER_KEY			' '
/*******************************************************************************
 * 							Common system Numbers
 *******************************************************************************/
#define OPEN_DOOR_TIME		15
#define	CLOSE_DOOR_TIME		15
#define	HOLD_DOOR_TIME		3
#define	BUZZER_TIME			60
#define ONE_SECOND_COUNT	(1000*8)
#define	PASSWORD_ADDRESS	0x0040
/*******************************************************************************
 * 							Function Definitions
 *******************************************************************************/
/**This function is used to change the password of the system and when called it
 * can't be left before a password is set and confirmed**/
void APP_setPassword(void);
/**The main Function that controls the main system when in main menu**/
void APP_systemControl(void);
/**This function gets the password from the other µC compares it with the
 * the password stored in the EEPROM then reurns the result of this comparison**/
APP_PasswordCompareType APP_getPassword(void);
/**This function starts the motor Rotation for a Certian period of time to
 * open it then Calls the function that holds it in place**/
void APP_openDoor(void);
/**This function Stops the motor Rotation for a certain period then calls
 * the function that Closes the Door**/
void APP_holdDoor(void);
/**this function strts the motor rotation in the direction of closing then
 * after the timer fires calls the function that stops the motor**/
void APP_closeDoor(void);
#endif /* APP_H_ */
