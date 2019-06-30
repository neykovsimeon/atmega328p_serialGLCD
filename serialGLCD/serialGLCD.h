#ifndef serialCLCD
#define serialGLCD



void serialGLCD_backlight(unsigned char backlight);
void serialGLCD_gotoPixel_XY(unsigned char PixelX, unsigned char PixelY);	// X = 0, 127; Y = 0, 63
void serialGLCD_goto21x8_XY(unsigned char refX, unsigned char refY);		// X = 1, 21; Y = 1, 8
void serialGLCD_reverse();
void serialGLCD_clear();
void serialGLCD_sendChar(unsigned char myChar);
void serialGLCD_sendString(char *myString);
void serialGLCD_drawBox(unsigned char TopLeftX, unsigned char TopLeftY, unsigned char BottomRightX, unsigned char BottomRightY, unsigned char draw);

#endif // serialGLCD