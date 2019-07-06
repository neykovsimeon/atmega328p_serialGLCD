/** \page pageLCD Serial GLCD ADM12864H
 *
 * ##Utilize ASCII commands and base functions for a serial graphic LCD ADM12864H
 *
 * serialGLCD.c
 *
 * \author Simeon Neykov
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
#include <util/delay.h>

/** ##Serial ASCII commands - backlight duty cycle.
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
	wait_while_UART0_is_busy(0); // check if the transmitter is busy
	UDR0 = 0x7C;
	wait_while_UART0_is_busy(0); // check if the transmitter is busy
	UDR0 = 0x02;
	wait_while_UART0_is_busy(0); // check if the transmitter is busy
	UDR0 = backlight;
}

/** ##Serial ASCII commands - toggle reverse display mode.
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
	wait_while_UART0_is_busy(0); // check if the transmitter is busy
	UDR0 = 0x7C;
	wait_while_UART0_is_busy(0); // check if the transmitter is busy
	UDR0 = 0x12;
}

/** ##Serial ASCII commands - Clear Screen.
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
	wait_while_UART0_is_busy(0); // check if the transmitter is busy
	UDR0 = 0x7C;
	wait_while_UART0_is_busy(0); // check if the transmitter is busy
	UDR0 = 0x00;
}

/** ##Serial GLCD - Send an ASCII Character.
 * 
 * This command send a character to GLCD
 *
 * [SparkFun items](https://learn.sparkfun.com/tutorials/serial-graphic-lcd-hookup/?_ga=1.12355956.1126191215.1366741676)
 *
 * - Consider UART was initialized and enabled.
 * - Consider needed X, Y coordinates have been set before.
 * - Character is being displayed at current X, Y position and it is moved forward to next position, thus the display is acting like the known type character LCD.
 * - Consider needed time for the backpack's microcontroller on LCD module to do its stuffs.
 * - Initially used added delay in _sendChar function but finally moved it into wait_while_UART0_is_busy();
 * - Consider 5ms should be sufficient delay but could depends of the complexity of the screen we want to show
 *
 */
void serialGLCD_sendChar(unsigned char myChar)
{
	wait_while_UART0_is_busy(1); // check if the transmitter is busy
	//_delay_ms(GLCD_DELAY);
	UDR0 = myChar;
}

/** ##Serial GLCD - Send A String of Characters.
 * 
 * This command send a string of characters to GLCD
 *
 * [SparkFun items](https://learn.sparkfun.com/tutorials/serial-graphic-lcd-hookup/?_ga=1.12355956.1126191215.1366741676)
 *
 * - Consider UART was initialized and enabled.
 * - Consider needed X, Y coordinates have been set before.
 * - First character of the string is being displayed at current X, Y position and it is moved forward to next position.
 * - Thus the display is acting like the known type character LCD
 *
 */
void serialGLCD_sendString(char *myString)
{
	while (*myString > 0)
	{
		serialGLCD_sendChar(*myString++);
	}	
}

/** ##Serial ASCII commands - Set X and Y pixel coordinates.
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
 * @param pixelX	range 0, 127
 * @param pixelY	range 0, 63
 *
 */
void serialGLCD_gotoPixel_XY(unsigned char pixelX, unsigned char pixelY)
{
	// check the range
	if (pixelX > INITIAL_pixel_MAXX) pixelX = 0;
	if (pixelY > INITIAL_pixel_MAXY) pixelY = 0;
	// send X
	wait_while_UART0_is_busy(0); // check if the transmitter is busy
	UDR0 = 0x7C;
	wait_while_UART0_is_busy(0); // check if the transmitter is busy
	UDR0 = 0x18;
	wait_while_UART0_is_busy(0); // check if the transmitter is busy
	UDR0 = pixelX;
	
	// send Y
	wait_while_UART0_is_busy(0); // check if the transmitter is busy
	UDR0 = 0x7C;
	wait_while_UART0_is_busy(0); // check if the transmitter is busy
	UDR0 = 0x19;
	wait_while_UART0_is_busy(0); // check if the transmitter is busy
	UDR0 = pixelY;	
	_delay_ms(5);
}

/** ##Serial ASCII commands - Set refX and refY Coordinates referred to 21x8 display format.
 * 
 * Coordinate transformation is valid for 128x64 pixels display and 6x8 character format.
 *
 * [SparkFun items](https://learn.sparkfun.com/tutorials/serial-graphic-lcd-hookup/?_ga=1.12355956.1126191215.1366741676)
 *
 * Consider UART was initialized and enabled.
 * - Details:
 *		- 21x8 format emulates  character display with 21 symbols on 8 rows
 *		- Coordinates are indexed from 0, 0
 *		- Coordinates are referred to 6x8 pixels sized characters
 * @param refX		range 0, 20
 * @param refY		range 0, 7
 * 
 * Note: These ranges are valid for 128x64 type display. These should be aligned for a different type / format LCD display.
 */
void serialGLCD_goto21x8_XY(unsigned char refX, unsigned char refY)
{
	// find pixel X
	unsigned char pixelX = (refX) * 6; ///< pixelX	range 0, 127
	// find pixel Y
	unsigned char pixelY = (refY) * 8; ///< pixelY	range 0, 63
	
	// goto pixel X, Y
	serialGLCD_gotoPixel_XY(pixelX, pixelY);
}

/** ##Serial GLCD - Send A String of Characters.
 * 
 * This command send a string of characters to GLCD
 *
 * [SparkFun items](https://learn.sparkfun.com/tutorials/serial-graphic-lcd-hookup/?_ga=1.12355956.1126191215.1366741676)
 *
 * - Consider UART was initialized and enabled.
 * - Consider needed X, Y coordinates have been set before.
 * - First character of the string is being displayed at current X, Y position.
 *
 * @param refX, refY Coordinates indexed from 0 !!
 * @param *myString Points to the elements of the string to be sent to LCD
 *
 */
void serialGLCD_sendString_at_21x8(unsigned char refX, unsigned char refY, char *myString)
{
	serialGLCD_goto21x8_XY(refX, refY);
	while (*myString > 0)
	{
		serialGLCD_sendChar(*myString++);
	}
	
}

/** ##Serial ASCII commands - drawBox.
 * 
 * Set X and Y referred to the Pixel level coordinates
 *
 * [SparkFun items](https://learn.sparkfun.com/tutorials/serial-graphic-lcd-hookup/?_ga=1.12355956.1126191215.1366741676)
 *
 * Consider UART was initialized and enabled.
 *
 * Sending hex value 0x0F followed by two sets of (x, y) coordinates defining opposite corners of the box, 
 * followed by a 0 or 1 determines whether to draw or erase the box. This command is exactly like the draw line command, 
 * but instead of drawing a line you get a box that exactly contains the line between the given coordinates.
 *
 * Coordinates are indexed from 0, 0 referred to the upper left corner of the display.
 * @param TopLeftX, TopLeftY Coordinates of the upper left corner of the box.
 * @param BottomRightX, BottomRightY Coordinates of the upper left corner of the box.
 * @param draw Defines whether we draw the box or erase the box
 *
 */
void serialGLCD_drawBox(unsigned char TopLeftX, unsigned char TopLeftY, unsigned char BottomRightX, unsigned char BottomRightY, unsigned char draw)
{
	wait_while_UART0_is_busy(0); // check if the transmitter is busy
	UDR0 = 0x7C;				// send command identificator 0x7C (tells Sparkfun's backpack MCU the next data on RxD will be command.
	wait_while_UART0_is_busy(0); // check if the transmitter is busy
	UDR0 = 0x0F;				// send drawBox actual command identificator
	wait_while_UART0_is_busy(0); // check if the transmitter is busy
	UDR0 = TopLeftX;
	wait_while_UART0_is_busy(0); // check if the transmitter is busy
	UDR0 = TopLeftY;
	wait_while_UART0_is_busy(0); // check if the transmitter is busy
	UDR0 = BottomRightX;
	wait_while_UART0_is_busy(0); // check if the transmitter is busy
	UDR0 = BottomRightY;
	wait_while_UART0_is_busy(0); // check if the transmitter is busy
	UDR0 = draw;
	_delay_ms(5);	
}
