/** \brief Utilize USART communication
 *
 * USART.h
 *
 * Created: 30.6.2019 г. 17:16:22
 *  Author: Simeon Neykov
 */ 


#ifndef USART_H_
#define USART_H_


void UART0_Init (int baud, char AsyncDoubleSpeed, char dataLength, char Parity);
void wait_while_UART0_is_busy(unsigned char add_delay);


#endif /* USART_H_ */