/** \mainpage Main
 * # Utilize serial backpack GLCD ADM12864H
 *
 * main.c
 *
 * \author	Simeon Neykov
 * 
 * [Used demos](https://www.avrfreaks.net/forum/tut-c-bit-manipulation-aka-programming-101?name=PNphpBB2&file=viewtopic&t=37871)
 *
 * [SparkFun items](https://learn.sparkfun.com/tutorials/serial-graphic-lcd-hookup/?_ga=1.12355956.1126191215.1366741676)
 *
 * ##Main function concept:
 * - Definitions and initializations
 *		- variables definition and initializations
 *		- I/O ports and pins definitions and initializations
 *		- USART (in this application UART mode used) initialization
 * - Infinite loop
 *		- show the menu
 *		- pooling the events (up, down, enter, etc)
 *		- browse the menu accordingly
 */ 

#include "main.h"
#include <avr/io.h>
#include <util/delay.h>
#include "serialGLCD.h"
#include "USART.h"
#include "charMenu.h"
//#include <time.h>
#include <stdio.h>

//char const compileDATE[] = __DATE__;	/* Mmm dd yyyy */
//char const compileTIME[] = __TIME__;	/* hh:mm:ss */
//char const compileDATE[] __attribute__((__progmem__)) = __DATE__;
//char const compileTIME[] __attribute__((__progmem__)) = __TIME__;

/** \file
 * ##Main function
 *
 * - Declare and initialize needed software variables
 *		- buttonPressed_delay, buttonReleased_delay are used for push button debouncing	
 *		- update_menu controls menu to be updated only when changes are supposed (button pressed, etc)
 *		- initialize menu item selector
 * - MCU's ports and pins definitions and initializations
 * - USART Initialization
 * - Intro screen (just for fun ;) and a bit for some debuging purposes)
 * - Infinite loop
 *		- Show menu. Since this is slow process call this function only when the menu is to be updated (e.g. button is pressed)
 *		- Check buttons and act respectively. Consider debouncing delays.
 *
 */
int main(void)
{
	
	printf("__DATE__: %s\n", __DATE__);
	printf("__TIME__: %s\n", __TIME__);	
	
	int buttonPressed = 0;
	int buttonPressed_delay = 0;
	int buttonReleased_delay = 0;	
	unsigned char update_menu = 1;
	selected = 1; // initialize first menu item after the menu header/title from main menu	
	
    /** 
	 * ##MCUs ports initializations
	 * - OUTPUT sets a particular pin as an output in the corresponding DDR register
	 * - INPUT  sets a particular pin as an input in the corresponding DDR register
	 * - CLEAR sets a particular output pin to LOW
	 * - SET sets a particular output pin to HIGH
	 * - TOGGLE changes the state of the selected otput pin
	 */
	OUTPUT(dirLEDs_PORT, LED_RED);				// port C, pin 5 as output (red LED)
	CLEAR(LEDs_PORT, LED_RED);					// set it to LOW (LED is OFF)
	INPUT(dirPUSHBUTTON_PORT, BUTTON_enter);	// set port C data direction register pin 4 as input (button "enter")
	SET(PUSHBUTTON_PORT, BUTTON_enter);         // set its latch to HIGH (not pressed)
	INPUT(dirPUSHBUTTON_PORT, BUTTON_up);		// set port C data direction register pin 4 as input (button "up")
	SET(PUSHBUTTON_PORT, BUTTON_up);			// set its latch to HIGH (not pressed)
	INPUT(dirPUSHBUTTON_PORT, BUTTON_down);		// set port C data direction register pin 4 as input (button "down")
	SET(PUSHBUTTON_PORT, BUTTON_down);          // set its latch to HIGH (not pressed)		

    /** 
	 * ##USART Initialization in asynchronous mode
	 *  - set the baud
	 *	- set the data framer (8bits, 1 stop bit, no parity)
	 *	- enable UART
	 */                                                                  
	UART0_Init (UART_BAUD, UART_DOUBLE_SPEED, UART_DATA_LENGTH, NO_PARITY);
	
    /** 
	 * ##Intro Screen
	 *  - clear screen
	 *	- after 2 sec delay put an intro screen
	 */	
	serialGLCD_clear();
	_delay_ms(2000);
	start();
	
	 /** 
	 * ##Infinite loop
	 *  - Browse the menu respectively
	 */
    while (1) 
    {
		if (update_menu == 1)
		{
			show_menu();
			update_menu = 0;	
		}
		// check button status, de-bouncing
		if (READ(readPUSHBUTTON, BUTTON_enter) == 0) 
		{
			buttonPressed_delay++;
			buttonReleased_delay = 0;
			if (buttonPressed_delay > 1)
			{
				if (buttonPressed == 0)
				{
					buttonPressed = 1;
					TOGGLE(LEDs_PORT, LED_RED);
					update_menu = 1;
					if (my_menu[selected].fp != 0) my_menu[selected].fp();
					selected  = my_menu[selected].enter;
				}
				buttonPressed_delay = 0;
			}						
		} else if  (READ(readPUSHBUTTON, BUTTON_up) == 0) {
			buttonPressed_delay++;
			buttonReleased_delay = 0;
			if (buttonPressed_delay > 1)
			{
				if (buttonPressed == 0)
				{
					buttonPressed = 1;
					TOGGLE(LEDs_PORT, LED_RED);
					selected  = my_menu[selected].up;
					update_menu = 1;
				}
				buttonPressed_delay = 0;
			}
		} else if  (READ(readPUSHBUTTON, BUTTON_down) == 0) {
			buttonPressed_delay++;
			buttonReleased_delay = 0;
			if (buttonPressed_delay > 1)
			{
				if (buttonPressed == 0)
				{
					buttonPressed = 1;
					TOGGLE(LEDs_PORT, LED_RED);
					selected  = my_menu[selected].down;
					update_menu = 1;
				}
				buttonPressed_delay = 0;
			}		
		} else {
			buttonPressed_delay = 0;
			buttonReleased_delay++;
			if (buttonReleased_delay > 1)
			{
				buttonReleased_delay = 0;
				buttonPressed = 0;
			}
		}
    }
}