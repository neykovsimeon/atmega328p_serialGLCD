/*
 * main.h
 *
 * \author	Simeon nEYKOV
 */ 
#ifndef MAIN_H_
#define MAIN_H_

/*@{*/
#define TRUE		1							///< General purpose macro definitions
#define FALSE		0							///< General purpose macro definitions
#define LOW			0							///< General purpose macro definitions
#define HIGH		1							///< General purpose macro definitions
/*@}*/


/*@{*/
#define F_CPU 16000000UL						///< System clock, used in <util/delay.h>
/*@}*/

#include <avr/io.h>
#include <util/delay.h>
	
/*@{*/
#define UART_DOUBLE_SPEED		1				///< Usage would depends of F_CPU and possible combinations between F_CPU, UART desired baud rate and needed prescaler
#define UART_DATA_LENGTH		8				///< default needed for SparkFun serial GLCD backpack
#define UART_STOP_BITS			1				///< default needed for SparkFun serial GLCD backpack
#define UART_BAUD				1152			///< pre-scalled from 115200. Default needed for SparkFun serial GLCD backpack
#define PARITY_EVEN				0				///< defines used EVEN parity check feature of UART
#define PARITY_ODD				1				///< defines used ODD parity check feature of UART
#define NO_PARITY				3				///< give a value different than 0 or 1 to distinguish PARITY ODD or EVEN selections 
/*@}*/

/*@{*/
#define GLCD_DELAY				5				///< Given in ms. For use in wait_while_UART0_is_busy when it should send data to serial display
#define DEBOUNCE_DELAY			0				///< Makes a common place to define number of cycles to pass in buttonPressed_delay and buttonReleased_delay
#define ROTARY_DELAY			40				///< Used in rotary encoder handler as a number of cycles to check rotary pins state (debouncing)
/*@}*/											

/*@{*/
// LED pins and registers mapping	
#define dirLEDs_PORT			DDRC			///< Application specific names defined for MCU's ports and pins
#define LEDs_PORT				PORTC			///< Application specific names defined for MCU's ports and pins
#define LED_RED					5				///< Application specific names defined for MCU's ports and pins

// push buttons MCU's pins and registers mapping
#define dirPUSHBUTTON_PORT		DDRC			///< Application specific names defined for MCU's ports and pins
#define PUSHBUTTON_PORT			PORTC			///< Application specific names defined for MCU's ports and pins
#define readPUSHBUTTON			PINC			///< Application specific names defined for MCU's ports and pins
#define BUTTON_enter			4				///< Application specific names defined for MCU's ports and pins
#define BUTTON_down				3				///< Application specific names defined for MCU's ports and pins
#define BUTTON_up				2				///< Application specific names defined for MCU's ports and pins

// rotary encoder MCU's pins mapping
// note use the same MCU port C, thus use the same registers mapping as for push buttons
#define ROTARY_DA				1				///< Application specific names defined for MCU's ports and pins
#define ROTARY_CK				0				///< Application specific names defined for MCU's ports and pins
#define ROTARY_SW				4				///< Application specific names defined for MCU's ports and pins
/*@}*/

extern void start (void);
extern void rotary_counter (void);

#endif /* MAIN_H_ */