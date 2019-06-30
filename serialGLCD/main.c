/** \mainpage
 * # Utilize serial GLCD ADM12864H
 *
 * serialGLCD.c
 *
 * Created: 29.6.2019 г. 15:26:56
 * Author : Simeon Neykov
 * 
 * [Used demos] (https://www.avrfreaks.net/forum/tut-c-bit-manipulation-aka-programming-101?name=PNphpBB2&file=viewtopic&t=37871)
 *
 * [SparkFun items] (https://learn.sparkfun.com/tutorials/serial-graphic-lcd-hookup/?_ga=1.12355956.1126191215.1366741676)
 *
 */ 

#include "main.h"
#include <avr/io.h>
#include <util/delay.h>
#include "serialGLCD.h"
#include "USART.h"

#define LOW 0
#define HIGH 1

#define true  1
#define false 0

#define dirLEDs_PORT			DDRC
#define LEDs_PORT				PORTC
#define LED_RED					6
#define LED_WHITE				5
#define dirPUSHBUTTON_PORT		DDRC
#define PUSHBUTTON_PORT			PORTC
#define readPUSHBUTTON			PINC
#define BUTTON_enter			4
#define BUTTON_down				3
#define BUTTON_up				2


#define INPUT(port,pin)		port &= ~(1<<pin)	///< Defines DDR register and pin/bit number as an input
#define OUTPUT(port,pin)	port |= (1<<pin)	///< Defines DDR register and pin/bit number as an output
#define SET(port,pin)		port |= (1<<pin)	///< Defines PORT register and set pin/bit number to HIGH
#define CLEAR(port,pin)		port &= ~(1<<pin)	///< Defines PORT register and set pin/bit number to LOW
#define TOGGLE(port,pin)	port ^= (1<<pin)	///< Defines PORT register and toggles the bit number
#define READ(port,pin)		(port & (1<<pin))	///< Defines PINx register and read the pin value at respective bit number
	
unsigned char sampleData[] =		{'B', 'a', 's', 'i', ' ', 'M', 'a', 'm', 'a', 't', 'a'};	

int main(void)
{
    // initialize LEDs, Button
	int buttonPressed = 0;
	int buttonPressed_delay = 0;
	int buttonReleased_delay = 0;
	
	int cmdPinter = 0;		// to be used to toggle between clear screen and any screen effect
	int cmdStepPointer = 0;	// points to the particular command step		
	
	OUTPUT(dirLEDs_PORT, LED_WHITE);			// port C, pin 6 as output
	OUTPUT(dirLEDs_PORT, LED_RED);				// port C, pin 5 as output
	SET(LEDs_PORT, LED_RED);					// set it to HIGH
	CLEAR(LEDs_PORT, LED_WHITE);				// set it to LOW
	INPUT(dirPUSHBUTTON_PORT, BUTTON_enter);	// set port C pin 4 as input
	SET(PUSHBUTTON_PORT, BUTTON_enter);         // set it to HIGH (not pressed)
	INPUT(dirPUSHBUTTON_PORT, BUTTON_up);		// set port C pin 4 as input
	SET(PUSHBUTTON_PORT, BUTTON_up);			// set it to HIGH (not pressed)
	INPUT(dirPUSHBUTTON_PORT, BUTTON_down);		// set port C pin 4 as input
	SET(PUSHBUTTON_PORT, BUTTON_down);          // set it to HIGH (not pressed)	
	
	// initialize USART
	UART0_Init (UART_BAUD, UART_DOUBLE_SPEED, UART_DATA_LENGTH, NO_PARITY);

		// no driven by interrupt
		
	serialGLCD_clear();
	_delay_ms(1000);
	
    while (1) 
    {
		// check button status, de-bouncing
		if (READ(readPUSHBUTTON, BUTTON_enter) == 0) 
		{
			buttonPressed_delay++;
			buttonReleased_delay = 0;
			if (buttonPressed_delay > 16000)
			{
				if (buttonPressed == 0)
				{
					buttonPressed = 1;
					TOGGLE(LEDs_PORT, LED_RED);
					TOGGLE(LEDs_PORT, LED_WHITE);
					serialGLCD_clear();
					
					/*
					for (unsigned char Ypos = 0; Ypos < 57; Ypos = Ypos + 8)	
					{				
						serialGLCD_clear();
						//if (Ypos > 0) serialGLCD_drawBox(0, Ypos-8, 127, Ypos, 0x01);
						serialGLCD_gotoPixel_XY(31, Ypos);
						serialGLCD_sendString("Test Display");
						serialGLCD_drawBox(0, Ypos, 127, Ypos+8, 0x00);
						_delay_ms(1000);
					}
					*/	
					for (unsigned char Ypos = 1; Ypos < 9; Ypos++)
					{
						serialGLCD_clear();
						//if (Ypos > 0) serialGLCD_drawBox(0, Ypos-8, 127, Ypos, 0x01);
						serialGLCD_goto21x8_XY(6, Ypos);
						serialGLCD_sendString("Test Display");
						serialGLCD_drawBox(0, (Ypos-1)*8, 127, (Ypos-1)*8+7, 0x01);
						_delay_ms(1000);
					}
				}
				buttonPressed_delay = 0;
			}						
		} 
		else
		{
			buttonPressed_delay = 0;
			buttonReleased_delay++;
			if (buttonReleased_delay > 16000)
			{
				buttonReleased_delay = 0;
				buttonPressed = 0;
			}
		}
    }
}




