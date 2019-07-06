/** \page pageUSART MCU's USART
 * 
 * ##Utilize USART communication with atmega328p 
 *
 * USART.c
 *
 * \author	Simeon Neykov.
 *
 */ 

#include <avr/io.h>
#include <math.h>
#include "main.h"
#include <util/delay.h>

/** ##UART Initialization for Asynchronous serial communication. 
 * 
 * UART specifications (baud rate, data length , parity, stop).
 *
 * Some values are predefined in main.h for GLCD with backpack specifically:
 * - baud rate 115200, use double speed prescaller
 * - 8bits, no parity
 * - F_CPU
 *
 * - Setting the baud rate
 *	- K is used to take the effect on the prescaller UBRR when double speed is considered
 *  - uint16_t baud is also prescalled to avoid overflow when high speed bit rate is needed, as per example of SparkFun's serial LCD
 * - Setting frame format, 8bit, no parity, 1 stop
 *	- 1 stop bit is by default. Still better to have setting implemented
 *	- Parity should consider odd, even as well no parity
 * - Enable transmit or/and receive operation
 *	- Transmitter is enabled by setting the Transmit Enable (TXEN) bit in the UCSRnB Register
 *  - Receiver is enabled by setting the Receive Enable (RXEN) bit in the UCSRnB Register
 *
 */
void UART0_Init (uint16_t baud, char AsyncDoubleSpeed, char dataLength, char Parity, char stopbits)
{
	char K = 1;
	if (AsyncDoubleSpeed == 1) 
	{
		K = 2;
		UCSR0A |= (1 << U2X0);
	}
	//uint16_t UBRRvalue = 16; //lrint((K*F_CPU/16*100*baud) -1);
	uint16_t UBRRvalue = lrint((K*F_CPU/16/100/baud) -1);
	UBRR0H = (unsigned char) (UBRRvalue >> 8);
	UBRR0L = (unsigned char) (UBRRvalue);

	// setting frame format, 8bit, no parity, 1 stop
	if (Parity == PARITY_EVEN)	UCSR0C |= (1 << UPM01); // set parity for EVEN
	if (Parity == PARITY_ODD)	UCSR0C |= (3 << UPM00); // set parity for ODD
	
	// frame format used by the USART is set by the UCSZn2:0, UPMn1:0 and USBSn bits in UCSRnB and UCSRnC
	if (stopbits == 1) UCSR0C &= ~(1 << USBS0);		// 1 stop bit, this is default. We need to clear if it was previously set
	if (stopbits == 2) UCSR0C |= (1 << USBS0);		// 2 stop bits
	
	if (dataLength == 6) UCSR0C |= (1 << UCSZ00);	// 6 bits UART data length
	if (dataLength == 7) UCSR0C |= (2 << UCSZ00);	// 7 bits UART data length
	if (dataLength == 8) UCSR0C |= (3 << UCSZ00);	// 8 bits UART data length
	if (dataLength == 9) UCSR0C |= (7 << UCSZ00);	// 9 bits UART data length
		
	// Enable transmit or/and receive operation
	// Transmitter is enabled by setting the Transmit Enable (TXEN) bit in the UCSRnB Register
	UCSR0B |= (1 << TXEN0) | (1 << RXEN0);
}


/** ##Wait if USART is busy
 * 
 * This function checks and wait until USART has finished previous transmission.
 * One should use this each time before loading the transmit buffer UDRn with new data.
 * @param add_delay One may consider an additional delay depends of the receiver needs (e.g. serial graphic LCD)
 *
 */
void wait_while_UART0_is_busy(unsigned char add_delay)
{
	while (!(UCSR0A & (1 << UDRE0))); // check if the transmitter is busy
	if (add_delay) _delay_ms(GLCD_DELAY);
}
