#ifndef serialCLCD
#define serialGLCD

#define LCD12864

#ifdef LCD12864
	#define INITIAL_pixel_MAXX	127
	#define INITIAL_pixel_MAXY	63
#endif


void serialGLCD_backlight(unsigned char backlight);
void serialGLCD_gotoPixel_XY(unsigned char pixelX, unsigned char pixelY);	// X = 0, 127; Y = 0, 63
void serialGLCD_goto21x8_XY(unsigned char refX, unsigned char refY);		// X = 1, 21; Y = 1, 8
void serialGLCD_reverse();
void serialGLCD_clear();
void serialGLCD_sendChar(unsigned char myChar);
void serialGLCD_sendString(char *myString);
void serialGLCD_drawBox(unsigned char TopLeftX, unsigned char TopLeftY, unsigned char BottomRightX, unsigned char BottomRightY, unsigned char draw);

#endif // serialGLCD