/*
 * main.h
 *
 * Created: 30.6.2019 г. 18:44:02
 *  Author: PC
 */ 


#ifndef MAIN_H_
#define MAIN_H_

#define F_CPU 16000000UL
#define UART_DOUBLE_SPEED	1
#define UART_DATA_LENGTH	8				// default needed for SparkFun serial GLCD backpack
#define UART_STOP_BITS		1				// default needed for SparkFun serial GLCD backpack
#define UART_BAUD			1152			// default needed for SparkFun serial GLCD backpack
#define PARITY_EVEN			0
#define PARITY_ODD			1
#define NO_PARITY			3				// give a value different than 0 or 1



#endif /* MAIN_H_ */