/*******************************************************************************
 * File Description:
 * Author      : Mahmoud Sherif Mahmoud
 * Module      : Door-Locker System Application Functions
 * Level  	   : High
 * Description : This file Contains the functions of the Door Locker Application
 * µC		   : ATMega 32 (8-BIT)
 * Date 	   : 4/11/2022(November)
 *******************************************************************************/
#include"lcd.h"
#include"keypad.h"
#include"menus.h"
#include"uart.h"
#include"app.h"
#include<util/delay.h>
#include"timer1.h"
uint8 doordisplaycompleteflag = 1;
/**This function is used to set the password if it's not set **/
void APP_setPassword(void) {
	while (1) {
		/**Status Byte is sent in the beginning to synchronize the two µCs**/
		uint8 status;
		/**Print the Main menu**/
		uint8 count = 0;
		MENUS_enterNewPassMenu();
		/**Get the First Password**/
		for (count = 0; count < 5; count++) {
			/**Get the pressed key on the keypad and send it to the other µC**/
			_delay_ms(KEYPAD_DELAY);
			UART_sendByte(KEYPAD_getPressedKey());
			/**Display the * on the LCD**/
			LCD_displayCharacter('*');
		}
		/**wait for the user to press enter**/
		while (KEYPAD_getPressedKey() != ENTER_KEY)
			;
		/**Sending the Enter Key**/
		UART_sendByte(' ');
		/**Display the Re-Enter the Password LCD**/
		MENUS_reenterPassMenu();
		/**Get the Second Password**/
		for (count = 0; count < 5; count++) {
			/**Get the pressed key on the keypad and send it to the other µC**/
			_delay_ms(KEYPAD_DELAY);
			UART_sendByte(KEYPAD_getPressedKey());
			/**Display the * on the LCD**/
			LCD_displayCharacter('*');
		}
		/**wait for the user to press enter**/
		while (KEYPAD_getPressedKey() != ENTER_KEY)
			;
		/**Sending the Enter Key**/
		UART_sendByte(' ');
		/**Wait for the result from the other µC to know if the passwords match
		 * or not**/
		status = UART_receiveByte();
		/**if the Passwords don't match then tell the user and Re-set the Whole
		 * process of setting the Password**/
		if (status == WRONG_CONFIRMATION) {
			LCD_clearScreen();
			LCD_moveCursor(0, 5);
			LCD_displayString("Wrong");
			LCD_moveCursor(1, 2);
			LCD_displayString("Confirmation");
			_delay_ms(1000);
			continue;
		} else if (status == PASSWORD_MATCHED) {
			LCD_clearScreen();
			LCD_displayString("PASSWORD IS SET");
			_delay_ms(1000);
			return;
		} else if (status == PASSWORD_TRIALS_EXHAUSTED) {
			LCD_clearScreen();
			LCD_moveCursor(0, 5);
			LCD_displayString("Wrong");
			LCD_moveCursor(1, 2);
			LCD_displayString("Confirmation");
			_delay_ms(1000);
			LCD_clearScreen();
			LCD_displayString("   TRESPASSER");
			for (count = 0; count < BUZZER_TIME - 1; count++)
				_delay_ms(1000);
		}
	}
}
/**This function controls the system when the system is on the main menu**/
void APP_systemControl(void) {
	uint8 count = 0;
	/**Print the main menu then wait and get the user selection**/
	MENUS_mainMenu();
	uint8 pressedkey = KEYPAD_getPressedKey();
	if (pressedkey == OPEN_DOOR) {
		/**If the user chooses to open the door then get the password**/
		UART_sendByte(OPEN_DOOR);
		APP_getPassword();
		/**wait for the result from the other µC**/
		uint8 status = UART_receiveByte();
		if (status == PASSWORD_MATCHED) {
			APP_displayDoorUnlocking();
			doordisplaycompleteflag = 1;
			while (doordisplaycompleteflag == 1)
				;

		} else if (status == PASSWORD_UNMATCHED) {
			/**if the passwords are not matched tell the user**/
			LCD_clearScreen();
			LCD_displayString("Wrong Password");
			_delay_ms(1000);
			return;
		} else if (status == PASSWORD_TRIALS_EXHAUSTED) {
			/**if the password trials are exhausted tell the user**/
			LCD_clearScreen();
			LCD_displayString("Wrong Password");
			_delay_ms(1000);
			LCD_clearScreen();
			LCD_displayString("   TRESPASSER");
			for (count = 0; count < BUZZER_TIME - 1; count++)
				_delay_ms(1000);
		}
	} else if (pressedkey == RESET_PASSWORD) {
		/**if the user chooses to reset the password call the feunction that
		 * changes the password**/
		UART_sendByte(RESET_PASSWORD);
		APP_getPassword();
		uint8 status = UART_receiveByte();
		if (status == PASSWORD_MATCHED) {
			APP_setPassword();
		} else if (status == PASSWORD_UNMATCHED) {
			/**if the passwords are not matched tell the user**/
			LCD_clearScreen();
			LCD_displayString("Wrong Password");
			_delay_ms(1000);
			return;
		} else if (status == PASSWORD_TRIALS_EXHAUSTED) {
			/**if the password trials are exhausted tell the user**/
			LCD_clearScreen();
			LCD_displayString("Wrong Password");
			_delay_ms(1000);
			LCD_clearScreen();
			LCD_displayString("   TRESPASSER");
			for (count = 0; count < BUZZER_TIME - 1; count++)
				_delay_ms(1000);
		}
	}
}
/**This function gets the password from the user then sends it to the other µC
 * that checks it and returns the result**/
void APP_getPassword(void) {
	/**Print the menu of entering the Pass**/
	uint8 count = 0;
	MENUS_enterPassMenu();
	for (count = 0; count < 5; count++) {
		/**get the user input and send it to the other µC**/
		_delay_ms(KEYPAD_DELAY);
		UART_sendByte(KEYPAD_getPressedKey());
		LCD_displayCharacter('*');
	}
	/**wait for the user to press enter**/
	while (KEYPAD_getPressedKey() != ENTER_KEY)
		;
	/**Sending the Enter key to the other µC**/
	UART_sendByte(' ');
}
void APP_displayDoorUnlocking(void) {
	LCD_clearScreen();
	LCD_displayString("Door is Unlocking");
	Timer1_countSeconds(OPEN_DOOR_TIME);
	Timer1_setCallBack(APP_displayDoorUnlocked);
}
void APP_displayDoorUnlocked(void) {
	LCD_clearScreen();
	LCD_displayString("Door is Unlocked");
	Timer1_countSeconds(HOLD_DOOR_TIME);
	Timer1_setCallBack(APP_displayDoorLocking);
}
void APP_displayDoorLocking(void) {
	LCD_clearScreen();
	LCD_displayString("Door is Locking");
	Timer1_countSeconds(CLOSE_DOOR_TIME);
	Timer1_setCallBack(APP_displayDoorComplete);
}
void APP_displayDoorComplete(void) {
	doordisplaycompleteflag = 0;
}
