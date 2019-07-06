/**
 * main.h
 *
 * \date	30.6.2019 г. 18:44:02
 * \author	Simeon nEYKOV
 */ 


#ifndef MAIN_H_
#define MAIN_H_

/** 
 * General purpose macro definitions
 */
/*@{*/
#define TRUE		1
#define FALSE		0
#define LOW			0
#define HIGH		1
/*@}*/

/** 
 * System clock, used in <util/delay.h>
 */
/*@{*/
#define F_CPU 16000000UL
/*@}*/
	
/** 
 * UART system settings: start, stop bits, parity, data length, baud (scalled x100)
 */
/*@{*/
#define UART_DOUBLE_SPEED		1
#define UART_DATA_LENGTH		8				// default needed for SparkFun serial GLCD backpack
#define UART_STOP_BITS			1				// default needed for SparkFun serial GLCD backpack
#define UART_BAUD				1152			// pre-scalled from 115200. default needed for SparkFun serial GLCD backpack
#define PARITY_EVEN				0
#define PARITY_ODD				1
#define NO_PARITY				3				// give a value different than 0 or 1
/*@}*/

/** 
 * Added delay needed serial LCD to do its stuffs
 */
/*@{*/
#define GLCD_DELAY				5
/*@}*/

/** 
 * Application specific names defined for MCU's ports and pins
 */
/*@{*/
#define dirLEDs_PORT			DDRC
#define LEDs_PORT				PORTC
#define LED_RED					5
#define dirPUSHBUTTON_PORT		DDRC
#define PUSHBUTTON_PORT			PORTC
#define readPUSHBUTTON			PINC
#define BUTTON_enter			4
#define BUTTON_down				3
#define BUTTON_up				2
/*@}*/

/*@{*/
/** 
 * AVR MCU's ports and pins MACRO definitions
 */
#define INPUT(port,pin)			port &= ~(1<<pin)	///< Defines DDR register and pin/bit number as an input
#define OUTPUT(port,pin)		port |= (1<<pin)	///< Defines DDR register and pin/bit number as an output
#define SET(port,pin)			port |= (1<<pin)	///< Defines PORT register and set pin/bit number to HIGH
#define CLEAR(port,pin)			port &= ~(1<<pin)	///< Defines PORT register and set pin/bit number to LOW
#define TOGGLE(port,pin)		port ^= (1<<pin)	///< Defines PORT register and toggles the bit number
#define READ(port,pin)			(port & (1<<pin))	///< Defines PINx register and read the pin value at respective bit number	
/*@}*/



#endif /* MAIN_H_ */