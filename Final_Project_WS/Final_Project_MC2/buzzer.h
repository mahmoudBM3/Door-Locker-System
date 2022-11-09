/*
 * buzzer.h
 *
 *  Created on: Nov 3, 2022
 *      Author: mabkr
 */
#include"std_types.h"
#ifndef BUZZER_H_
#define BUZZER_H_
#define BUZZER_PORT_ID	PORTA_ID
#define	BUZZER_PIN_ID	PIN0_ID
void BUZZER_init(void);
void BUZZER_ON(void);
void BUZZER_OFF(void);


#endif /* BUZZER_H_ */
