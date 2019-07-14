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

/** ##Digital read the state of a specified pin number of port C
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
 * @param pin tested pin number from port C
 *
 */
unsigned char read_PINC_logical_level(unsigned char pin)
{
	unsigned char result = 255;
	result = (PINC & (1<<pin)) ? 1 : 0;
	return result;
	
	// alternative method
	//if (PINC & (1<<pin)) == 0)
	//{
	//	result = 0;
	//} else {
	//	result = 1;
	//}
}

/** ##Digital read the state of a specified pin number of port D
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
 * @param pin tested pin number from port D
 *
 */
unsigned char read_PIND_logical_level(unsigned char pin)
{
	unsigned char result = 255;
	result = (PIND & (1<<pin)) ? 1 : 0;
	return result;
}


/** ##Digital read the state of a specified pin number of port B
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
 * @param pin tested pin number from port B
 *
 */
unsigned char read_PINB_logical_level(unsigned char pin)
{
	unsigned char result = 255;
	result = (PINB & (1<<pin)) ? 1 : 0;
	return result;
}

