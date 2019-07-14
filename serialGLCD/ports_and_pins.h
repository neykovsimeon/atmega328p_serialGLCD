/*
 * ports_and_pins.h
 *
 * \author Simeon Neykov
 *
 */ 

#ifndef PORTS_AND_PINS_H_
#define PORTS_AND_PINS_H_

// AVR MCU's ports and pins MACRO definitions
#define INPUT(port,pin)			port &= ~(1<<pin)	///< Defines DDR register and pin/bit number as an input
#define OUTPUT(port,pin)		port |= (1<<pin)	///< Defines DDR register and pin/bit number as an output
#define SET(port,pin)			port |= (1<<pin)	///< Defines PORT register and set pin/bit number to HIGH
#define CLEAR(port,pin)			port &= ~(1<<pin)	///< Defines PORT register and set pin/bit number to LOW
#define TOGGLE(port,pin)		port ^= (1<<pin)	///< Defines PORT register and toggles the bit number
#define READ(port,pin)			(port & (1<<pin))	///< Defines PINx register and read the pin value at respective bit number. 
													///< Logical result: "0" if tested pin is "0" and something ">0"if the tested pin is "1".
													///< The exact return value if tested pin is 1 would depends of the pin number													

extern unsigned char read_PINB_logical_level(unsigned char pin);
extern unsigned char read_PINC_logical_level(unsigned char pin);
extern unsigned char read_PIND_logical_level(unsigned char pin);


#endif /* PORTS_AND_PINS_H_ */