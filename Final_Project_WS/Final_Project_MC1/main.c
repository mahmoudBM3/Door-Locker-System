/*
 * main.c
 *
 *  Created on: Nov 2, 2022
 *      Author: mabkr
 */

#include"lcd.h"
#include"keypad.h"
#include"menus.h"
#include"uart.h"
#include"app.h"
#include"timer1.h"
#include<util/delay.h>
#define	WRONG_CONFIRMATION	255

int main() {
	{
		Timer1_ConfigType config = { 0,ONE_SECOND_COUNT,
					TIMER1_FCPU_1024, TIMER1_COMPARE };
			Timer1_init(&config);
		UART_ConfigType uartconfig = { 9600, ASYNCH, EIGHTBIT, RECEIVEPOLLING,
				TRANSMITPOLLING, ONEBIT, NOPARITY };
		UART_init(&uartconfig);
	}
	LCD_init();
	LCD_clearScreen();
	APP_setPassword();
	while (1) {
		APP_systemControl();
	}
}
