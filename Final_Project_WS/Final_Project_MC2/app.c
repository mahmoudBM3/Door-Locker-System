/*******************************************************************************
 * File Description:
 * Author      : Mahmoud Sherif Mahmoud
 * Module      : Door-Locker System Application Functions
 * Level  	   : High
 * Description : This file Contains the functions of the Door Locker Application
 * µC		   : ATMega 32 (8-BIT)
 * Date 	   : 4/11/2022(November)
 *******************************************************************************/
/*******************************************************************************
 * 							Included Libraries
 *******************************************************************************/
#include "uart.h"
#include "app.h"
#include"buzzer.h"
#include"timer1.h"
#include"dc_motor.h"
#include"external_eeprom.h"
#include<util/delay.h>
/**This function is used to change the password of the system and when called it
 * can't be left before a password is set and confirmed**/
void APP_setPassword(void) {
	/***/
	uint8 wrongcount = 0;
	uint8 password1[5];
	uint8 password2[5];
	uint8 count = 0;
	while (1) {
		uint8 flag = 1;
		/**Get the first Password**/
		for (count = 0; count < 5; count++) {
			password1[count] = UART_receiveByte();
		}
		/**wait for the Enter KEY**/
		while (UART_receiveByte() != ENTER_KEY)
			;
		/**Get the Second Password**/
		for (count = 0; count < 5; count++) {
			password2[count] = UART_receiveByte();
		}
		/**wait for the Enter KEY**/
		while (UART_receiveByte() != ENTER_KEY)
			;
		/**Compare the Two passwords**/
		for (count = 0; count < 5; count++) {
			if (password1[count] != password2[count]) {
				/**This flag when set means that the passwords are matched when
				 * cleared means that the passwords are not matched**/
				flag = 0;
				break;
			}
		}
		/**if the passwords match store it in the EEPROM**/
		if (flag == 1) {
			/**Tell the other µCthat the passwords are matched**/
			UART_sendByte(PASSWORD_MATCHED);
			/**PUT The Password in the EEPROM**/
			for (count = 0; count < 5; count++) {
				EEPROM_writeByte(PASSWORD_ADDRESS + count, password1[count]);
				/**This delay is required by the EEPROM**/
				_delay_ms(10);
			}
			break;
		} else if (flag == 0) {
			/**If the passwords are not matched incriment the counter and tell
			 * µC that the passwords don't match to start taking the password
			 * again**/
			wrongcount++;
			/**If number of trials limit is reached Alert**/
			if (wrongcount >= 3) {
				/**Tell the other µC to display Error**/
				UART_sendByte(PASSWORD_TRIALS_EXHAUSTED);
				/**Turning the buzzer on**/
				BUZZER_ON();
				for (count = 0; count < BUZZER_TIME; count++) {
					_delay_ms(1000);
				}

				BUZZER_OFF();
			} else {
				/**if the passwords don't match Tell the Other µC and retake the
				 * password**/
				UART_sendByte(WRONG_CONFIRMATION);
				continue;
			}
		}
	}
}
/**The main Function that controls the main system when in main menu**/
void APP_systemControl(void) {
	uint8 count = 0;
	static uint8 wrongpasscount = 0;
	/**Get the user choice**/
	uint8 choice = UART_receiveByte();
	if (choice == OPEN_DOOR) {
		/**get the password Comparison result **/
		APP_PasswordCompareType result = APP_getPassword();
		if (result == PASSWORD_CORRECT) {
			/**if the password is correct Open the Door and Reset the wrong
			 * trials counter and tell the other µC**/
			UART_sendByte(PASSWORD_MATCHED);
			wrongpasscount = 0;
			/**Open The Door**/
			APP_openDoor();
		} else if (result == PASSWORD_WRONG) {
			/**if the password is not correct reset the wrong trials counter**/
			wrongpasscount++;
			if (wrongpasscount == 3) {
				/**if the number of wrong trials reaches the limit make an ALERT**/
				wrongpasscount = 0;
				UART_sendByte(PASSWORD_TRIALS_EXHAUSTED);
				/**Ring the buzzer**/
				BUZZER_ON();
				for (count = 0; count < BUZZER_TIME; count++) {
					_delay_ms(1000);
				}

				BUZZER_OFF();
			} else {
				UART_sendByte(PASSWORD_UNMATCHED);
			}
		}
	} else if (choice == RESET_PASSWORD) {
		/**get the password Comparison result **/
		APP_PasswordCompareType result = APP_getPassword();
		if (result == PASSWORD_CORRECT) {
			/**if the password is correct start resetting the password and Reset
			 *  the wrong trials counter adn tell the other µC**/
			UART_sendByte(PASSWORD_MATCHED);
			wrongpasscount = 0;
			/**Reset the password**/
			APP_setPassword();
		} else if (result == PASSWORD_WRONG) {
			/**if the password is not correct reset the wrong trials counter**/
			wrongpasscount++;
			if (wrongpasscount == 3) {
				/**if the number of wrong trials reaches the limit make an ALERT**/
				wrongpasscount = 0;
				UART_sendByte(PASSWORD_TRIALS_EXHAUSTED);
				/**Ring the buzzer**/
				BUZZER_ON();
				for (count = 0; count < BUZZER_TIME; count++) {
					_delay_ms(1000);
				}

				BUZZER_OFF();
			} else {
				UART_sendByte(PASSWORD_UNMATCHED);
			}
		}
	}
}
APP_PasswordCompareType APP_getPassword(void) {
	/**This function gets the password from the other µC compares it with the
	 * the password stored in the EEPROM then reurns the result of this comparison**/
	uint8 count = 0;
	uint8 password[5];
	/**get the user input password**/
	for (count = 0; count < 5; count++) {
		password[count] = UART_receiveByte();
	}
	/**wait for the Enter KEY**/
	while (UART_receiveByte() != ENTER_KEY)
		;
	/**Compare the Input password with the saved password**/
	for (count = 0; count < 5; count++) {
		uint8 eepromdata;
		EEPROM_readByte(PASSWORD_ADDRESS + count, &eepromdata);
		_delay_ms(10);
		if (eepromdata != password[count]) {
			return PASSWORD_WRONG;
		}
	}
	return PASSWORD_CORRECT;
}
void APP_openDoor(void) {
	/**This function starts the motor Rotation for a Certian period of time to
	 * open it then Calls the function that holds it in place**/
	/**Start Motor Rotation**/
	DCMOTOR_rotate(CLOCKWISE, 100);
	Timer1_countSeconds(OPEN_DOOR_TIME);
	Timer1_setCallBack(APP_holdDoor);
}
void APP_holdDoor(void) {
	/**This function Stops the motor Rotation for a certain period then calls
	 * the function that Closes the Door**/
	static uint8 holdcount = 0;
	/**Stop the Motor**/
	DCMOTOR_rotate(STOP, 100);
	/**if the motor wasn't stopped before then initiate Door closing after the
	 * timer fires but if it was closed before only stop the motor**/
	if (holdcount != 0) {
		holdcount = 0;
	} else {
		/**Initiate the timer**/
		Timer1_countSeconds(HOLD_DOOR_TIME);
		Timer1_setCallBack(APP_closeDoor);
		holdcount++;
	}
}
void APP_closeDoor(void) {
	/**this function strts the motor rotation in the direction of closing then
	 * after the timer fires calls the function that stops the motor**/
	/**initiate the Timer**/
	DCMOTOR_rotate(ANTICLOCKWISE, 100);
	Timer1_countSeconds(CLOSE_DOOR_TIME);
	Timer1_setCallBack(APP_holdDoor);
}

