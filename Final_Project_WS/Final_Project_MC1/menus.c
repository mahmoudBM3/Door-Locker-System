/*******************************************************************************
 * File Description:
 * Author      : Mahmoud Sherif Mahmoud
 * Module      : Door-Locker System Application Msnus Functions
 * Level  	   : High
 * Description : This file Contains the Menus of the Door Locker Application
 * µC		   : ATMega 32 (8-BIT)
 * Date 	   : 4/11/2022(November)
 *******************************************************************************/
#include"lcd.h"
void MENUS_mainMenu(void){
	LCD_clearScreen();
	LCD_moveCursor(0,0);
	LCD_displayString("+ : Open Door");
	LCD_moveCursor(1,0);
	LCD_displayString("- : Change Pass");
}
void MENUS_enterPassMenu(void){
	LCD_clearScreen();
	LCD_moveCursor(0,0);
	LCD_displayString("Plz enter pass:");
	LCD_moveCursor(1,0);
}
void MENUS_reenterPassMenu(void){
	LCD_clearScreen();
	LCD_moveCursor(0,0);
	LCD_displayString("Plz re-enter the");
	LCD_moveCursor(1,0);
	LCD_displayString("same pass:");
}
void MENUS_enterNewPassMenu(void){
	LCD_clearScreen();
	LCD_moveCursor(0,0);
	LCD_displayString("Plz enter new");
	LCD_moveCursor(1,0);
	LCD_displayString("Pass:");
	LCD_moveCursor(1,5);

}

