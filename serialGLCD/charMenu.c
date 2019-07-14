/** \page pageMenu Menu Handler
 * 
 * ##Utilize re-usable menu structure
 *
 * charMenu.c
 *
 * \author	Simeon Neykov.
 *
 * [Reference](https://www.youtube.com/watch?v=PFzNBtnfJ6Y&list=PLfGDzGG5F5eYNBkXs0tTdXqLB3KE5CFgx)
 *
 */

#include <math.h>
//#include <stdint.h>              // needed for uint8_t types, etc
//#include <stdbool.h>             // needed for boolean types, etc
#include <stdio.h>
//#include <stdlib.h>
#include <string.h>
#include <avr/io.h>
#include "main.h"
#include "USART.h"
#include "serialGLCD.h"
#include "charMenu.h" 
#include <util/delay.h>

unsigned char selected = 1;			///< selected is used for indexing the elements from MenuEntry defined structure

const char menu_000[] = "-<Main Menu>-------";	// 0
const char menu_001[] = "Option1";					// 1
const char menu_002[] = "Go to SubMenu";			// 2
const char menu_003[] = "Option3";					// 3
const char menu_004[] = "Option4";					// 4
const char menu_005[] = "Option5";					// 5
const char menu_006[] = "Option6";					// 6
const char menu_007[] = "NextOption7";				// 7
const char menu_008[] = "Option8";					// 8
const char menu_009[] = "START";					// 9

const char menu_010[] = "-<Sub Menu>--------";	// 10
const char menu_011[] = "SubOption1";				// 11
const char menu_012[] = "Rotary Counter";	// 12
const char menu_013[] = "SubOption3";				// 13
const char menu_014[] = "SubOption4";				// 14
const char menu_015[] = "SubOption5";				// 15
const char menu_016[] = "RETURN";					// 16

MenuEntry my_menu[] =
{
    {menu_000, 10, 0, 0, 0,  0},					// selected = 0
    {menu_001, 10, 1, 2, 1,  0},					// selected = 1
    {menu_002, 10, 1, 3, 11, 0},					// selected = 2
    {menu_003, 10, 2, 4, 3,  0},					// selected = 3
    {menu_004, 10, 3, 5, 4,  0},					// selected = 4
    {menu_005, 10, 4, 6, 5,  0},					// selected = 5
    {menu_006, 10, 5, 7, 6,  0},					// selected = 6
    {menu_007, 10, 6, 8, 7,  0},					// selected = 7
    {menu_008, 10, 7, 9, 8,  0},					// selected = 8
    {menu_009, 10, 8, 9, 9,  start},				// selected = 9. Call function start if "enter" event
    
    {menu_010, 7, 0,  0,  0,  0},					// selected = 10
    {menu_011, 7, 7,  12, 11, 0},					// selected = 11
    {menu_012, 7, 11, 13, 12, rotary_counter},					// selected = 12
    {menu_013, 7, 12, 14, 13, 0},					// selected = 13
    {menu_014, 7, 13, 15, 14, 0},					// selected = 14
    {menu_015, 7, 14, 16, 15, 0},					// selected = 15
    {menu_016, 7, 15, 16, 1,  0},					// selected = 16		
};

/** ##Menu Handler - send LCD menu string at reference location
 * 
 * Set refX and refY to the character LCD format (e.g. 21x8) indexed from 0, 0
 *
 * Consider UART was initialized and enabled.
 * @param refX, refY reference coordinates as for character LCD format (e.g. 21 x 8) indexed from 0, 0.
 * @param *lcd_menu_items a pointer to the characters in selected menu item to be displayed on the LCD screen
 * @param add_line if 1 (or just > 1) then complete the row with character given in add_char. If add_line =0 the row would not be completed till the end.
 * @param add_char character to be used to complete the row after the menu string if add_line >=1.
 *
 */
void serialGLCD_writeMenuString (unsigned char refX, unsigned char refY, const char *lcd_menu_items, unsigned char add_line, char add_char) 
{
	unsigned char lcd_i;
	unsigned char lcd_offset = 0;
	// find pixel X
	unsigned char pixelX = (refX) * 6;
	// find pixel Y
	unsigned char pixelY = (refY) * 8;
	// check the range
	if (pixelX > INITIAL_pixel_MAXX) pixelX = 0;
	if (pixelY > INITIAL_pixel_MAXY) pixelY = 0;
	
	serialGLCD_gotoPixel_XY(pixelX, pixelY);
	
	lcd_offset = strlen(lcd_menu_items);
	
	if (lcd_offset > INITIAL_MAXX) lcd_offset = INITIAL_MAXX;
	for (lcd_i = lcd_offset; lcd_i; lcd_i--)
	{
		serialGLCD_sendChar(*lcd_menu_items++);
	}
	if (add_line)
	{
		for (lcd_i = INITIAL_MAXX - lcd_offset - refX; lcd_i; lcd_i--) 
		{
			serialGLCD_sendChar(add_char);
		}
	}
}

/** ##Menu Handler - show LCD menu on the screen
 *
 * Consider UART was initialized and enabled.
 *
 * Chosen menu item is contained in global variable 'selected'
 *
 * Structure of menu handler
 * - Input base: 
 *   - how many rows has the display, this is defined in 'DISPLAY_ROWS' macro definition
 *   - how many rows on the screen has the menu itself (or sub-menu, etc), stored in 'MenuEntry' structure, field 'num_menupoints' 
 *   - the range within which the selector could be displayed without need to scroll the display
 *     - 'UPPER_SPACE' and 'LOWER_SPACE'MACROs predefined 
 *   - in case 'num_menupoints' is less than DISPLAY_ROWS, to ensure unwanted screen scrolling:
 *     - actual number of rows to be displayed is 'num_menupoints' 
 *     - 'UPPER_SPACE' is recalculated to keep 2 rows less than the number of rows to be displayed
 *     - ensure to clear the display when short menu is entered, thus to delete remains from previous menu (the use of 'enClear' flag)
 *
 * - Menu handler model: could be represented like the display is a "frame-mask" moved over the indexed menu items list
 *     - thus have to be defined: 'from' which menu item 'till' which menu item depends of the menu selector 'selected'
 *         - this should be done within the range of items from the same menu/sub-menu, means the same 'num_menupoints'
 *     - ensure correct range depends of the usage of 'VISIBLE_MENU_HEADER' and upper and lower spaces
 *     - show the menu items listed in between 'from' and 'till', show selection marks and control scrolling depending of the valid range
 * 
 */
void show_menu(void)
{
	unsigned char line_cnt = 0;
	unsigned char from = 0;		// from which row of menu points
	unsigned char till = 0;		// till which row of menu points
	unsigned char temp = 0;
	unsigned char varDisplay_rows = DISPLAY_ROWS;
	unsigned char varUpper_space = UPPER_SPACE;
	static unsigned char enClear = 1;
	char buffer[21];
	
	// define from and till spec for the menu
	if (my_menu[selected].num_menupoints < DISPLAY_ROWS) 
	{
		varDisplay_rows = my_menu[selected].num_menupoints;
		varUpper_space = varDisplay_rows - 2;
		if (enClear)
		{
			serialGLCD_clear();
			enClear = 0;
		}
	} else {
		enClear = 1;
	}
	while (till <= selected)
	{
		till += my_menu[till].num_menupoints;
	}
	from = till - my_menu[selected].num_menupoints;
	temp = from;
	till--;
	
	if ((selected >= (from +varUpper_space)) && (selected <= (till - LOWER_SPACE))) 
	{
		from = selected - varUpper_space;
		till = from + (varDisplay_rows - 1);
		if (VISIBLE_MENU_HEADER) 
		{
			serialGLCD_writeMenuString(0, 0, my_menu[temp].text, 1, ' ');
			line_cnt = 1;
			from ++;
		}
		for (from; from <= till; from++) 
		{
			if (from == selected) 
			{
				sprintf(buffer, ">%s", my_menu[from].text);
				serialGLCD_writeMenuString(0, line_cnt, buffer, 1, SELECTION_CHAR_END);
				line_cnt++;	
			} else {
				sprintf(buffer, " %s", my_menu[from].text);
				serialGLCD_writeMenuString(0, line_cnt, buffer, 1, ' ');
				line_cnt++;
			}
		}
	} else {
		if (selected < (from +varUpper_space)) 
		{
			till = from + (varDisplay_rows - 1);
			for (from; from <= till; from++) 
			{
				if (from == selected) 
				{
					sprintf(buffer, ">%s", my_menu[from].text);
					serialGLCD_writeMenuString(0, line_cnt, buffer, 1, SELECTION_CHAR_END);
					line_cnt++;					
				} else {
					if ((VISIBLE_MENU_HEADER) && (line_cnt == 0))
					{
					// if this is the header line - don't add ' ' at the beginning	
						serialGLCD_writeMenuString(0, line_cnt, my_menu[from].text, 1, ' ');
						line_cnt++;
					} else {
						sprintf(buffer, " %s", my_menu[from].text);
						serialGLCD_writeMenuString(0, line_cnt, buffer, 1, ' ');
						line_cnt++;
					}
				}
			}
		} else {
			if (selected == till)
			{
				from = till - (varDisplay_rows - 1);
				if (VISIBLE_MENU_HEADER) 
				{
					serialGLCD_writeMenuString(0, 0, my_menu[temp].text, 1, ' '); 
					line_cnt = 1; 
					from ++;
				}
				for (from; from <= till; from++) 
				{
					if (from == selected) 
					{
						sprintf(buffer, ">%s", my_menu[from].text);
						serialGLCD_writeMenuString(0, line_cnt, buffer, 1, SELECTION_CHAR_END);
						line_cnt++;				
					} else {
						sprintf(buffer, " %s", my_menu[from].text);
						serialGLCD_writeMenuString(0, line_cnt, buffer, 1, ' ');
						line_cnt++;
					}
				}
			}
		}
	}
}