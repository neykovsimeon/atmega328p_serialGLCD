/*
 * charMenu.h
 *
 * Created: 1.7.2019 г. 21:54:22
 *  Author: Simeon Neykov
 */ 

#ifndef CHARMENU_H_
#define CHARMENU_H_

#include <avr/io.h>
#include "main.h"
#include "USART.h"
#include "serialGLCD.h"

//#define DISPLAY_16x4               /**< Define display type 16x4.  */
//#define DISPLAY_20x4             /**< Define display type 20x4.  */
//#define DISPLAY_16x2             /**< Define display type 16x2.  */
//#define DISPLAY_20x2             /**< Define display type 20x2.  */
#define DISPLAY_21x8             /**< Define display type 21x8.  */

/** \brief Define whether menu header is always visible.
 * 
 * First row of the menu could be reserved for menu header.
 *
 * Use flag TRUE to keep menu header always visible.
 * If flag FALSE is used, then menu header will shift accordingly
 * depending of the selected menu item.
*/
#define VISIBLE_MENU_HEADER TRUE

/** 
 * Define selection symbols
 */
#define SELECTION_CHAR      '>'
#define SELECTION_CHAR_END  '<'

#ifdef DISPLAY_16x4
    #define DISPLAY_ROWS    4
    #define UPPER_SPACE     2
    #define LOWER_SPACE     1
    #define INITIAL_MAXX    16
    #define INITIAL_MAXY    4
    #define START_ROW3      0x10
    #define START_ROW4      0x50
#endif

#ifdef DISPLAY_20x4
    #define DISPLAY_ROWS    4
    #define UPPER_SPACE     2
    #define LOWER_SPACE     1
    #define INITIAL_MAXX    20
    #define INITIAL_MAXY    4
    #define START_ROW3      0x14
    #define START_ROW4      0x54
#endif

#ifdef DISPLAY_21x8
	#define DISPLAY_ROWS    8	///< this is copied into a variable and handled depends of num_menupoints
	#define UPPER_SPACE     6	///< this is copied into a variable and handled depends of num_menupoints. Target is UPPER_SPACE = DISPLAY_ROWS - 2
	#define LOWER_SPACE     1
	#define INITIAL_MAXX    21
	#define INITIAL_MAXY    8	
#endif

#ifdef DISPLAY_16x2
    #define DISPLAY_ROWS    2
    #define UPPER_SPACE     1
    #define LOWER_SPACE     0
    #define INITIAL_MAXX    16
    #define INITIAL_MAXY    2
    #define START_ROW3      0x00
    #define START_ROW4      0x40
#endif

#ifdef DISPLAY_20x2
    #define DISPLAY_ROWS    2
    #define UPPER_SPACE     1
    #define LOWER_SPACE     0
    #define INITIAL_MAXX    20
    #define INITIAL_MAXY    2
    #define START_ROW3      0x00
    #define START_ROW4      0x40
#endif


/**
 * A structure to represent LCD menu 
 */
typedef const struct MenuStructure {
	/*@{*/
    const char *text;				/**< gives the menu item to be displayed */
	/*@}*/
	  /**
	   * @name num_menupoints is linked with DISPLAY_ROWS and UPPER_SPACE
       */
	/*@{*/	
    unsigned char num_menupoints;	/**< tells how many menu items within selected menu or sub-menu */
	/*@}*/
	/*@{*/
	  /**
	   * @name up, down, enter are linked with respective event handler (button, encoder, etc)
       */
    unsigned char up;				/**< tells what to be selected in case of an event "up" (button, rotary encoder, etc)		*/
    unsigned char down;				/**< tells what to be selected in case of an event "down" (button, rotary encoder, etc)		*/
    unsigned char enter;			/**< tells what to be selected in case of an event "enter" (button, rotary encoder, etc)	*/
	/*@}*/
	/*@{*/
    void ( *fp ) (void);			/**< pointer to predefined function to call in case of an event "enter" (button, rotary encoder, etc). No function is called if 0 is placed. */
	/*@}*/
}MenuEntry;

extern MenuEntry my_menu[];
extern unsigned char selected;

extern void start (void);
void show_menu(void);
void serialGLCD_writeMenuString (unsigned char refX, unsigned char refY, const char *lcd_menu_items, unsigned char add_line, char add_char);
//extern void wait_while_UART0_is_busy();
//extern void serialGLCD_gotoPixel_XY(unsigned char pixelX, unsigned char pixelY);
//extern void serialGLCD_goto21x8_XY(unsigned char refX, unsigned char refY);
//extern void serialGLCD_clear();


#endif /* CHARMENU_H_ */