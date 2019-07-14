# Atmega328P with serial GLCD from SparkFun

----
## This is dummy project I use to start with Atmega, graphical LCD and Doxygen
> IDE used: Atmel Studio 7

> Universal Programmer MiniPro TL866A

> !! Pay attention for correct setting of MCU's fuses! In this application "Full Swing Crystal Oscillator" is used, CKSEL3..0 = 0111 respectively. Using TL866A cause one should control fuses separately with programmer, it cannot be configured to be done in Atmel Studio IDE

----
## Utilize serial graphic LCD display 128x64 with backpack
see [Sparkfun's item](https://learn.sparkfun.com/tutorials/serial-graphic-lcd-hookup/?_ga=1.12355956.1126191215.1366741676)

> The Graphic Serial LCD Backpack is designed to be controlled in real time using predefined ASCII commands. Backpack's firmware has built-in character generator, character format 6x8.

----
## Scope of this Project
1. Create a bunch of re-usable C functions to utilize the predefined LCD ASCII commands and to control the display with AVR microcontroller Atmega328p (as well as with any other MCU)
2. Create dedicated functions to send a character, send a string, goto specific pixel level coordinates (0, 127 / 0, 63), goto specific character level coordinates (0, 20 / 0, 7)
 - Note: always index the coordinates from 0, 0

3. Create dedicated function to send a string as a part of a structured menu handler:
 - at specified coordinates
 - handle the display line completion with a specified character (could be ' ' , so means we clear till the end of the display row)

4. Dedicated and re-usable menu structure, easy to handle:
 - menu item selector
 - events handlers ("up", "down", "enter") based on pushbuttons or / and rotary encoder with switch
 - specified function might be called by any of the events handlers (but it is considered event "enter" is the logical way of use)

5. MCU's USART peripheral is utilized in asynchronous serial mode at 115200kb, 8 data bits, 1 stop bit, no parity, for communication with used serial LCD

6. Version control utilized with Git, remote repository at GitHub

7. Doxygen integrated in Atmel Studio 7: Target is to get as much code documented as possible - here the scope is to get an experience with documenting code with doxygen



