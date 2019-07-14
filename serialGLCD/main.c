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
#include "ports_and_pins.h"
#include <stdio.h>
#include <string.h>

extern unsigned char read_PINC_logical_level(unsigned char pin);

unsigned char update_menu = 1;
unsigned char temp = 0;
int rotary_delay = ROTARY_DELAY;
unsigned char lastStateROTARY_CK = 0;
unsigned char currentStateROTARY_CK = 0;
unsigned char currentStateROTARY_DA = 0;
unsigned char buttonPressed = 0;
int buttonPressed_delay = 0;
int buttonReleased_delay = 0;

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
	// initialize first menu item after the menu header/title from main menu
	selected = 1;		
	
	// MCUs ports initializations
	OUTPUT(dirLEDs_PORT, LED_RED);				// port C, pin 5 as output (red LED)
	CLEAR(LEDs_PORT, LED_RED);					// set it to LOW (LED is OFF)
	INPUT(dirPUSHBUTTON_PORT, BUTTON_enter);	// set port C data direction register pin 4 as input (button "enter")
	SET(PUSHBUTTON_PORT, BUTTON_enter);         // set its latch to HIGH (not pressed)
	INPUT(dirPUSHBUTTON_PORT, BUTTON_up);		// set port C data direction register pin 4 as input (button "up")
	SET(PUSHBUTTON_PORT, BUTTON_up);			// set its latch to HIGH (not pressed)
	INPUT(dirPUSHBUTTON_PORT, BUTTON_down);		// set port C data direction register pin 4 as input (button "down")
	SET(PUSHBUTTON_PORT, BUTTON_down);          // set its latch to HIGH (not pressed)	
	INPUT(dirPUSHBUTTON_PORT, ROTARY_DA);		// set port C data direction register pin 1 as input (ROTARY DATA)
	SET(PUSHBUTTON_PORT, ROTARY_DA);			// set its latch to HIGH (not pressed)	
	INPUT(dirPUSHBUTTON_PORT, ROTARY_CK);		// set port C data direction register pin 0 as input (ROTARY CLOCK)
	SET(PUSHBUTTON_PORT, ROTARY_CK);			// set its latch to HIGH (not pressed)	

	// USART Initialization in asynchronous mode, 8bits, 1 stop bit, no parity, 1115200kb baud rate                                                                
	UART0_Init (UART_BAUD, UART_DOUBLE_SPEED, UART_DATA_LENGTH, NO_PARITY);

	serialGLCD_clear();
	_delay_ms(1000);

	// infinite loop - show menu and polling external events (buttons, encoder) respectively
    while (1) 
    {
		if (update_menu == 1)
		{
			show_menu();
			update_menu = 0;
			
			// update lastStateROTARY_CK due to delay introduced by show_menu()
			rotary_delay = ROTARY_DELAY;
			temp = 0;
			while (rotary_delay)
			{
				lastStateROTARY_CK = read_PINC_logical_level(ROTARY_CK);
				_delay_us(100);
				temp = read_PINC_logical_level(ROTARY_CK);
				
				if (temp == lastStateROTARY_CK)
				{
					rotary_delay --;
				} else {
					rotary_delay = ROTARY_DELAY;
				}
			}				
		}
		
		// check button status, debouncing
		if (READ(readPUSHBUTTON, BUTTON_enter) == 0)
		{
			buttonPressed_delay++;
			buttonReleased_delay = 0;
			if (buttonPressed_delay > DEBOUNCE_DELAY)
			{
				if (buttonPressed == 0)
				{
					buttonPressed = 1;
					TOGGLE(LEDs_PORT, LED_RED);
					update_menu = 1;
					selected  = my_menu[selected].enter;
					if (my_menu[selected].fp != 0) my_menu[selected].fp();
				}
				buttonPressed_delay = 0;
			}						
		} else if  (READ(readPUSHBUTTON, BUTTON_up) == 0) {
			buttonPressed_delay++;
			buttonReleased_delay = 0;
			if (buttonPressed_delay > DEBOUNCE_DELAY)
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
			if (buttonPressed_delay > DEBOUNCE_DELAY)
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
			if (buttonReleased_delay > DEBOUNCE_DELAY)
			{
				buttonReleased_delay = 0;
				buttonPressed = 0;
			}
		}
		if (READ(readPUSHBUTTON, ROTARY_SW) == 0)
		{
            buttonPressed_delay++;
            buttonReleased_delay = 0;
			if (buttonPressed_delay > DEBOUNCE_DELAY)
			{
				if (buttonPressed == 0)
				{
					buttonPressed = 1;
					TOGGLE(LEDs_PORT, LED_RED);
					update_menu = 1;
					selected  = my_menu[selected].enter;
					if (my_menu[selected].fp != 0) my_menu[selected].fp();
				}
				buttonPressed_delay = 0;				
			}
		} else {
			buttonPressed_delay = 0;
			buttonReleased_delay++;
			if (buttonReleased_delay > DEBOUNCE_DELAY)
			{
				buttonReleased_delay = 0;
				buttonPressed = 0;
			}
		}
		
		// check rotary encoder
		rotary_delay = ROTARY_DELAY;
		temp = 0;
		while (rotary_delay)
		{			
			currentStateROTARY_CK = read_PINC_logical_level(ROTARY_CK);
			_delay_us(100);
			temp = read_PINC_logical_level(ROTARY_CK);
						
			if (temp == currentStateROTARY_CK)
			{
				rotary_delay --;			
				currentStateROTARY_DA = read_PINC_logical_level(ROTARY_DA);			
			} else {
				rotary_delay = ROTARY_DELAY;
			}
		}		
		if (currentStateROTARY_CK != lastStateROTARY_CK) 
		{
			if (currentStateROTARY_CK == currentStateROTARY_DA) 
			{
				selected  = my_menu[selected].up;
				update_menu = 1;
				SET(LEDs_PORT, LED_RED);
			} else {
				selected  = my_menu[selected].down;
				update_menu = 1;
				CLEAR(LEDs_PORT, LED_RED);				
			}
			lastStateROTARY_CK = currentStateROTARY_CK;
		}		
		if (!(update_menu))
		{
			rotary_delay = ROTARY_DELAY;
			temp = 0;
			while (rotary_delay)
			{
				lastStateROTARY_CK = read_PINC_logical_level(ROTARY_CK);
				_delay_us(100);
				temp = read_PINC_logical_level(ROTARY_CK);
			
				if (temp == lastStateROTARY_CK)
				{
					rotary_delay --;
				} else {
					rotary_delay = ROTARY_DELAY;
				}
			}
		}
    }
}

/** ##Menu Handler - example function linked to selected menu item
 * 
 * This function is just an example to demonstrate how to 
 * link an executable code to particular selected menu item when an 
 * event "enter" occurs (button pressed, encoder, etc).
 *
 * Consider UART was initialized and enabled if LCD operation.
 *
 */
void start (void)
{
	serialGLCD_clear();
	serialGLCD_goto21x8_XY(1, 3);
	serialGLCD_sendString("Serial GLCD trials");
	_delay_ms(2000);
	selected = 1;
	serialGLCD_clear();
	_delay_ms(2);
}


/** ##Menu Handler - example rotary encoder application function
 * 
 * This function is just an example to demonstrate how to 
 * link an executable code to particular selected menu item when an 
 * event "enter" occurs (button pressed, encoder, etc).
 *
 * Counter from 0 to 100 is controlled by rotary encoder and displayed on the screen
 *
 * Concept:
 *  - once called, this function is keeping the control loop until rotary push switch is pressed
 *  - display is cleared
 *  - rotary encoder handler:
 *      - refresh valid state of the ROTARY_CK (once at the beginning of the called function)
 *		- go into loop (exit the loop when rotary switch is pressed)
 *		- within the loop: 
 *			- check status of the ROTARY_CK (clock signal), if different than the previous it is supposed a rotation is ongoing
 *			- if a rotation is ongoing, check status of ROTARY_DA (data signal). 
 *          - if CK and DA in the same state decrement the counter, else increment
 *          - use LED output for additional outside indication of rotation direction 
 *	
 *		- each time CK or DA pin status is checked - do it in a loop until de-bounced confident result is achieved (the usage of ROTARY_DELAY)
 * 
 * Use 'sprintf(ResultString, "%d", myCounter);' to convert binary (unsigned char) counter into string for LCD display
 * - clear the remains symbols when go back to less digits range (100 -> 99, 10 -> 9, etc)
 */
void rotary_counter (void)
{
	static unsigned char myCounter = 50;
	char ResultString[5];	
	unsigned char go_further = 1;
	
	serialGLCD_clear();
	update_menu = 1;
	
	rotary_delay = ROTARY_DELAY;
	temp = 0;
	while (rotary_delay)
	{
		lastStateROTARY_CK = read_PINC_logical_level(ROTARY_CK);
		_delay_us(100);
		temp = read_PINC_logical_level(ROTARY_CK);
			
		if (temp == lastStateROTARY_CK)
		{
			rotary_delay --;
		} else {
			rotary_delay = ROTARY_DELAY;
		}
	}	
	
	while (go_further)
	{

		rotary_delay = ROTARY_DELAY;
		temp = 0;
		while (rotary_delay)
		{
			currentStateROTARY_CK = read_PINC_logical_level(ROTARY_CK);
			_delay_us(20);
			temp = read_PINC_logical_level(ROTARY_CK);
				
			if (temp == currentStateROTARY_CK)
			{
				rotary_delay --;
				currentStateROTARY_DA = read_PINC_logical_level(ROTARY_DA);
			} else {
				rotary_delay = ROTARY_DELAY;
			}
		}
		if (currentStateROTARY_CK != lastStateROTARY_CK)
		{
			if (currentStateROTARY_CK == currentStateROTARY_DA)
			{
				if (myCounter) myCounter--;			// stops at 0
				SET(LEDs_PORT, LED_RED);
			} else {
				if (myCounter < 100) myCounter++;	// stops at 100
				CLEAR(LEDs_PORT, LED_RED);
			}
			update_menu = 1;
		}
		if (update_menu) 
		{
			sprintf(ResultString, "%d", myCounter);
			serialGLCD_goto21x8_XY(0, 0);
			serialGLCD_sendString ("Count (0 - 100)");
			serialGLCD_goto21x8_XY(0, 1);
			serialGLCD_sendString (strcat(ResultString, " ")); // with cleaning the remains when change the number of digits
			update_menu = 0;
		}

		rotary_delay = ROTARY_DELAY;
		temp = 0;
		while (rotary_delay)
		{
			lastStateROTARY_CK = read_PINC_logical_level(ROTARY_CK);
			_delay_us(20);
			temp = read_PINC_logical_level(ROTARY_CK);
				
			if (temp == lastStateROTARY_CK)
			{
				rotary_delay --;
			} else {
				rotary_delay = ROTARY_DELAY;
			}
		}
	
		if (READ(readPUSHBUTTON, ROTARY_SW) == 0)
		{
			buttonPressed_delay++;
			buttonReleased_delay = 0;
			if (buttonPressed_delay > DEBOUNCE_DELAY)
			{
				if (buttonPressed == 0)
				{
					buttonPressed = 1;
					update_menu = 1;
					go_further = 0;
				}
				buttonPressed_delay = 0;
			}
		} else {
			buttonPressed_delay = 0;
			buttonReleased_delay++;
			if (buttonReleased_delay > DEBOUNCE_DELAY)
			{
				buttonReleased_delay = 0;
				buttonPressed = 0;
			}
		}		
		
	}
			
	selected = 1;
	serialGLCD_clear();
	_delay_ms(2);
}


