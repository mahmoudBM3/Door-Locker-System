/*
 * main.c
 *
 *  Created on: Nov 2, 2022
 *      Author: mabkr
 */

#include"uart.h"
#include"app.h"
#include<util/delay.h>
#include"buzzer.h"
#include"timer1.h"
#include"dc_motor.h"
#include"twi.h"
int main() {
	{
		Timer1_ConfigType config = { 0, ONE_SECOND_COUNT, TIMER1_FCPU_1024, TIMER1_COMPARE };
		Timer1_init(&config);
		TWI_ConfigType twiconfig = { 1, 2, TWI_FCPU_64 };
		UART_ConfigType uartconfig = { 9600, ASYNCH, EIGHTBIT, RECEIVEPOLLING,
				TRANSMITPOLLING, ONEBIT, NOPARITY };
		UART_init(&uartconfig);
		TWI_init(&twiconfig);
	}
	APP_setPassword();
	BUZZER_init();
	DCMOTOR_init();
	while (1) {
		APP_systemControl();

	}
}

