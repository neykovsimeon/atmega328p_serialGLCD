/** \page pagePortsAndPins MCU's PORTs and PINs
 * 
 * ##Utilize MCUs port and pins macros and functions
 * 
 * ports_and_pins.c
 *
 * \author Simeon Neykov
 * 
 */ 

#include "ports_and_pins.h"
#include <avr/io.h>
#include "main.h"

int buttonPressed_delay = 0;
int buttonReleased_delay = 0;
unsigned char buttonPressed = 0;

int modeButton_pressed_delay[maxButtonNum];
int modeButton_released_delay[maxButtonNum];
unsigned char modeButton_pressed[maxButtonNum];

/** ##Digital read the state of a specified pin number of a specified port
 * - Motivation
 *     - READ macro (port & (1<<pin)), where port is PINC, PIND or PINB, would return:
 *         - 0 if the pin is clear (LOW state)
 *         - something > 0 if the pin is set (HIGH state). The exact value would depends of the pin number (position)
 *
 *     - In a particular use cases (e.g. rotary encoder handler) it is needed to check
 *         - current versus previous state of a pin (e.g. rotary clk signal)
 *         - current status of different pins (e.g. rotary clk and rotary data signals)
 * 
 * - Dedicated read_PINx_logical_level() functions return 0 if the pin is LOW (clear) and return 1 (doesn't depends of the pin position) if the pin is HIGH
 * 
 * @param pinport Identifies respective input pin port (PINB, PINC, PIND), supposed predefined with an application specific names
 * @param pin Tested input pin (bit) number from a specified port
 *
 */
unsigned char read_PINx_digital_level(unsigned char pinport, unsigned char pin)
{
	unsigned char result = 255;
	result = (pinport & (1<<pin)) ? 1 : 0;
	return result;
}

/** ##Push button delay containers initialization
 * - Predefined debounce delay, would vary depends of the timing of the main loop cycle
 *
 */
void debounceDelayInit()
{
	for (int i = 0; i < maxButtonNum; i++)
	{
		modeButton_pressed_delay[i] = 0;
		modeButton_pressed_delay[i] = 0;
		modeButton_pressed[i] = 0;
	}
}
/** ##Push button function with a functional mode definition. Active LOW.
 * - Feasible for buttons directly connected to an input pin (instead of a matrix, etc). Functional mode concept: 
 *		- onClick mode:
 *			- action performed when pressed (active low);
 *			- next action possible after release and press again (if hold pressed -> action is performed once and waits for release);
 *			- reasonable use case: button "enter", switch  on / off feature, etc
 *		- whilePressed mode:	
 *			- action performed when pressed (active low);
 *			- next action comes in next cycle while button is being kept pressed (no release performed);
 *			- reasonable use case: button "up", "down", "forward", "backward", "increment", "decrement", etc, like in a menu browse buttons "up / down'..
 *		- cycling is provided outside of the function ('main' while loop, timer interrupt at a time, etc)
 * 
 * @param mode Defines functional mode described above, onClick or whilePressed. Differs on a way whether next action comes only after button was released and pressed again
 * @param myButton_pinport This is MCU's input pin port respectively (PINB, PINC, PIND) - suppose predefined with an application specific names
 * @param myButton button number (indexed from 0). If buttons on the same port these might be directly the port's pin(bit) number.
 * @param buttonDelay Predefined debounce delay, would vary depends of the timing of the main loop cycle
 *
 */
unsigned char checkButton_withMode(unsigned char mode, unsigned char myButton_pinport, unsigned char myButton, int buttonDelay)
{
	unsigned char ret_value[maxButtonNum];
	for (int i = 0; i < maxButtonNum; i++)
	{
		ret_value[i] = 0;
	}
	
	switch (mode)
	{
		case onClick: // next button action comes only after button is released and press again
		{	
			if  (READ(myButton_pinport, myButton) == 0)
			{
				modeButton_pressed_delay[myButton]++;
				modeButton_released_delay[myButton] = 0;
				if (modeButton_pressed_delay[myButton] > buttonDelay)
				{
					if (modeButton_pressed[myButton] == 0)
					{
						modeButton_pressed[myButton] = 1;
						ret_value[myButton] = 1;
					}
					modeButton_pressed_delay[myButton] = 0;
				}
			}
			else
			{
				modeButton_pressed_delay[myButton] = 0;
				modeButton_released_delay[myButton]++;
				if (modeButton_released_delay[myButton] > buttonDelay)
				{
					modeButton_pressed[myButton] = 0;
					ret_value[myButton] = 0;
					modeButton_released_delay[myButton] = 0;
				}
			}			
		}
		break;
		
		case whilePressed: // next button action comes while button is still pressed and hold down continuously loop after loop
		{
			if (modeButton_pressed[myButton] == 0)
			{
				if  (READ(myButton_pinport, myButton) == 0)
				{
					modeButton_pressed_delay[myButton]++;
					modeButton_released_delay[myButton] = 0;
					if (modeButton_pressed_delay[myButton] > buttonDelay)
					{
						modeButton_pressed[myButton] = 1;
						ret_value[myButton] = 1;
						modeButton_pressed_delay[myButton] = 0;
					}
				}
			}
			else
			{
				modeButton_pressed_delay[myButton] = 0;
				modeButton_released_delay[myButton]++;
				if (modeButton_released_delay[myButton] > buttonDelay)
				{
					modeButton_pressed[myButton] = 0;
					ret_value[myButton] = 0;
					modeButton_released_delay[myButton] = 0;
				}
			}	
		}
		break;
		
		default:	break;
	}
	
	return ret_value[myButton];
}
