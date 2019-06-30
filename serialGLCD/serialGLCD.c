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

#include <avr/io.h>
#include "main.h"
#include "USART.h"
#include "serialGLCD.h"

/** \brief Serial ASCII commands - backlight duty cycle.
 * 
 * Set back light Duty Cycle.
 *
 * [SparkFun items](https://learn.sparkfun.com/tutorials/serial-graphic-lcd-hookup/?_ga=1.12355956.1126191215.1366741676)
 *
 * Consider UART was initialized and enabled.
 * Setting the value to zero turns the back light off, setting it at 100 or above turns it full on.
 * Intermediate values set it somewhere in- between.
 *
 */
void serialGLCD_backlight(unsigned char backlight)
{
	wait_while_UART0_is_busy(); // check if the transmitter is busy
	UDR0 = 0x7C;
	wait_while_UART0_is_busy(); // check if the transmitter is busy
	UDR0 = 0x02;
	wait_while_UART0_is_busy(); // check if the transmitter is busy
	UDR0 = backlight;
}

/** \brief Serial ASCII commands - toggle reverse display mode.
 * 
 * Toggle reverse display mode.
 *
 * [SparkFun items](https://learn.sparkfun.com/tutorials/serial-graphic-lcd-hookup/?_ga=1.12355956.1126191215.1366741676)
 *
 * Consider UART was initialized and enabled.
 * Setting the reverse mode causes the screen to immediately clear with the new background.
 * If the display is turned off while in reverse mode, it will next power up in reverse mode.
 *
 */
void serialGLCD_reverse()
{
	wait_while_UART0_is_busy(); // check if the transmitter is busy
	UDR0 = 0x7C;
	wait_while_UART0_is_busy(); // check if the transmitter is busy
	UDR0 = 0x12;
}

/** \brief Serial ASCII commands - Clear Screen.
 * 
 * This command clears the screen of all written pixels.
 *
 * [SparkFun items](https://learn.sparkfun.com/tutorials/serial-graphic-lcd-hookup/?_ga=1.12355956.1126191215.1366741676)
 *
 * Consider UART was initialized and enabled.
 * If you're operating in normal mode, all pixels are reset.
 * If you're operating in reverse mode, all pixels are set.
 *
 */
void serialGLCD_clear()
{
	wait_while_UART0_is_busy(); // check if the transmitter is busy
	UDR0 = 0x7C;
	wait_while_UART0_is_busy(); // check if the transmitter is busy
	UDR0 = 0x00;
}

/** \brief Serial GLCD - Send A Character.
 * 
 * This command send a character to GLCD
 *
 * [SparkFun items](https://learn.sparkfun.com/tutorials/serial-graphic-lcd-hookup/?_ga=1.12355956.1126191215.1366741676)
 *
 * - Consider UART was initialized and enabled.
 * - Consider needed X, Y coordinates have been set before.
 * - Character is being displayed at current X, Y positon.
 *
 */
void serialGLCD_sendChar(unsigned char myChar)
{
	wait_while_UART0_is_busy(); // check if the transmitter is busy
	UDR0 = myChar;
}

/** \brief Serial GLCD - Send A String of Characters.
 * 
 * This command send a string of characters to GLCD
 *
 * [SparkFun items](https://learn.sparkfun.com/tutorials/serial-graphic-lcd-hookup/?_ga=1.12355956.1126191215.1366741676)
 *
 * - Consider UART was initialized and enabled.
 * - Consider needed X, Y coordinates have been set before.
 * - First character of the string is being displayed at current X, Y position.
 *
 */
void serialGLCD_sendString(char *myString)
{
	while (*myString > 0)
	{
		serialGLCD_sendChar(*myString++);
	}
	
}

/** \brief Serial ASCII commands - Set X and Y Coordinates.
 * 
 * Set X and Y referred to the Pixel level coordinates
 *
 * [SparkFun items](https://learn.sparkfun.com/tutorials/serial-graphic-lcd-hookup/?_ga=1.12355956.1126191215.1366741676)
 *
 * Consider UART was initialized and enabled.
 *
 * Coordinates are indexed from 0, 0 - upper left corner of the display.
 * SparkFun Graphic LCD Serial Backpack has embedded firmware with character generator.
 * The X and Y reference coordinates are used by the text generator to place text at specific locations on the screen.
 * The coordinates refer to the upper left most pixel in the character space. 
 *
 * If the offsets are within 6 pixels of the right edge of the screen or 8 pixels of the bottom, 
 * the text generator will revert to the next logical line for text so as to print a whole character and not parts.
 *
 */
void serialGLCD_gotoPixel_XY(unsigned char PixelX, unsigned char PixelY)
{
	// send X
	wait_while_UART0_is_busy(); // check if the transmitter is busy
	UDR0 = 0x7C;
	wait_while_UART0_is_busy(); // check if the transmitter is busy
	UDR0 = 0x18;
	wait_while_UART0_is_busy(); // check if the transmitter is busy
	UDR0 = PixelX;
	
	// send Y
	wait_while_UART0_is_busy(); // check if the transmitter is busy
	UDR0 = 0x7C;
	wait_while_UART0_is_busy(); // check if the transmitter is busy
	UDR0 = 0x19;
	wait_while_UART0_is_busy(); // check if the transmitter is busy
	UDR0 = PixelY;	
}

/** \brief Serial ASCII commands - Set X and Y Coordinates.
 * 
 * Set X and Y referred to the Pixel level coordinates
 *
 * [SparkFun items](https://learn.sparkfun.com/tutorials/serial-graphic-lcd-hookup/?_ga=1.12355956.1126191215.1366741676)
 *
 * Consider UART was initialized and enabled.
 *
 * Coordinates are indexed from 0, 0 - upper left corner of the display.
 * SparkFun Graphic LCD Serial Backpack has embedded firmware with character generator.
 * The X and Y reference coordinates are used by the text generator to place text at specific locations on the screen.
 * The coordinates refer to the upper left most pixel in the character space. 
 *
 * If the offsets are within 6 pixels of the right edge of the screen or 8 pixels of the bottom, 
 * the text generator will revert to the next logical line for text so as to print a whole character and not parts.
 *
 */
void serialGLCD_goto21x8_XY(unsigned char refX, unsigned char refY)
{
	// find pixel X
	unsigned char pixelX = (refX - 1) * 6;
	// find pixel Y
	unsigned char pixelY = (refY - 1) * 8;
	
	// goto pixel X, Y
	serialGLCD_gotoPixel_XY(pixelX, pixelY);
	// send pixel X
	/*wait_while_UART0_is_busy(); // check if the transmitter is busy
	UDR0 = 0x7C;
	wait_while_UART0_is_busy(); // check if the transmitter is busy
	UDR0 = 0x18;
	wait_while_UART0_is_busy(); // check if the transmitter is busy
	UDR0 = PixelX;
	
	// send pixel Y
	wait_while_UART0_is_busy(); // check if the transmitter is busy
	UDR0 = 0x7C;
	wait_while_UART0_is_busy(); // check if the transmitter is busy
	UDR0 = 0x19;
	wait_while_UART0_is_busy(); // check if the transmitter is busy
	UDR0 = PixelY;	
	*/
}

/** \brief Serial ASCII commands - Set X and Y Coordinates.
 * 
 * Set X and Y referred to the Pixel level coordinates
 *
 * [SparkFun items](https://learn.sparkfun.com/tutorials/serial-graphic-lcd-hookup/?_ga=1.12355956.1126191215.1366741676)
 *
 * Consider UART was initialized and enabled.
 *
 * Coordinates are indexed from 0, 0 - upper left corner of the display.
 * SparkFun Graphic LCD Serial Backpack has embedded firmware with character generator.
 * The X and Y reference coordinates are used by the text generator to place text at specific locations on the screen.
 * The coordinates refer to the upper left most pixel in the character space. 
 *
 * If the offsets are within 6 pixels of the right edge of the screen or 8 pixels of the bottom, 
 * the text generator will revert to the next logical line for text so as to print a whole character and not parts.
 *
 */
void serialGLCD_drawBox(unsigned char TopLeftX, unsigned char TopLeftY, unsigned char BottomRightX, unsigned char BottomRightY, unsigned char draw)
{
	// send X
	wait_while_UART0_is_busy(); // check if the transmitter is busy
	UDR0 = 0x7C;
	wait_while_UART0_is_busy(); // check if the transmitter is busy
	UDR0 = 0x0F;
	wait_while_UART0_is_busy(); // check if the transmitter is busy
	UDR0 = TopLeftX;
	wait_while_UART0_is_busy(); // check if the transmitter is busy
	UDR0 = TopLeftY;
	wait_while_UART0_is_busy(); // check if the transmitter is busy
	UDR0 = BottomRightX;
	wait_while_UART0_is_busy(); // check if the transmitter is busy
	UDR0 = BottomRightY;
	wait_while_UART0_is_busy(); // check if the transmitter is busy
	UDR0 = draw;	
}