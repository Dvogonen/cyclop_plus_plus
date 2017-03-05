/*****************************************************************************
   File: max7456.h

   Original Author: Benoit, 10 oct. 2012
   Latest Author:   Kjell Kernen, 13 oct. 2016

   Copyright (c) 2017 Kjell Kernen (Dvogonen)

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
  SOFTWARE.
 *****************************************************************************/
#ifndef MAX7456_H
#define MAX7456_H

#include "max7456registers.h"

/*  class Max7456 - Represents a max7456 device communicating through SPI port */

class Max7456
{
  public:
    /* Default constructor */
    Max7456();

    /* Constructor
       Initialize communications and device
       pinCS : pin ~CS of the arduino where max7456 is plugged. */
    Max7456(byte pinCS);

    /* Initialize communications and device
       pinCS : pin ~CS of the arduino where max7456 is plugged.
       The same function as using constructor Max7456(byte pinCS) */
    void init(byte pinCS);

    /* Set the base time for blink.
       blinkBase : the base time (see datasheet)
       blinkDC : blink Duty cycle (see datasheet). */
    void setBlinkParams(byte blinkBase, byte blinkDC);

    /* Set Horizontal and Vertical display offset
       horizontal : the horizontal offset in pixels (between 0 and 63).
       vertical : the vertical offset in pixels (between 0 and 31). */
    void setDisplayOffsets(byte horizontal, byte vertical);

    /* Erase Display Memory */
    void clearScreen();

    /* Activate osd on screen
       act :
        if true OSD is activated
        if false OSD is deactivated */
    void activateOSD(bool act = true);

    /* Activate input video as a background
       activExtVid :
         if true external video is displayed
         if false external video is not displayed (bakground = grey) */
    void activateExternalVideo(bool activExtVid = true);

    /* Put a character in the memory character of max7456
       array : the byte array representing the character (54 bytes long)
       pos : the ascii table char position. */
    void sendCharacter(const charact array, byte pos);

    /* Get a character from the character memory of max7456
       array : the byte array representing the character (54 bytes long)
       x : the horizontal position of character in character memory
       y : the vertical position of character in character memory */
    void getCharacter(charact array, byte x, byte y);

    /* Put a string in the display memory of max7456
       string : The string to be displayed
       x : the horizontal position of the string on screen
       y : the vertical position of the string on screen
       blink : if 1 then character will blink
       inv : if 1 then color character will be inverted */
    void print(const char string[], byte x, byte y, byte blink = 0, byte inv = 0);

    /* Put a float in the display memory of max7456
       value : The value to be displayed
       x : the horizontal position of the value on screen
       y : the vertical position of the value on screen
       before : number of digits before comma
       after : number of digits after comma
       blink : if 1 then character will blink
       inv : if 1 then color character will be inverted
       The number of printed characters will be : before + after + 1.
       Be aware of having a coherence between the number of digits you enter
       in the function and the max. value of your double. If not may result
       in unpredictable printed string.
       Example:
         max.print(3.14,x,y,3,4);
         Will print "003.1400" on screen */
    void print(double value, byte x, byte y, byte before, byte after, byte blink = 0, byte inv = 0);

    /* Put some characters in the display memory of max7456.
       The characters are given by their address in the
       character memory of the max7456.
       chars : The characters address array to display
       size : the array size
       x : the horizontal position of the value on screen
       y : the vertical position of the value on screen
       blink : if 1 then character will blink
       inv : if 1 then color character will be inverted */
    void printMax7456Chars(byte chars[], byte size, byte x, byte y, byte blink = 0, byte inv = 0);

    /* Put one character from the character memory in the display memory of max7456
       address : The address in character memory of the character to be displayed
       x : the horizontal position of the string on screen
       y : the vertical position of the string on screen
       blink : if 1 then character will blink
       inv : if 1 then color character will be inverted */
    void printMax7456Char(const byte address, byte x, byte y, byte blink = 0, byte inv = 0);

    /* Print a character to Serial port
       array : the byte array representing the character (54 bytes long)
       img :
         true : the character will be displayed as a picture
         false : the character will be displayed as a byte array */
    static void printCharacterToSerial(const charact array, bool img = true);

    /* Converts a CARACT character to a byte array representation.
       car : the CARACT character
       array :  the byte array representing the character (54 bytes long) */
    void CARACT2ByteArray(const CARACT car, byte *array);

    /* Converts a byte array to a CARACT character.
       array : the byte array representing the character (54 bytes long)
       return :  the CARACT character */
    static CARACT byteArray2CARACT(const charact array);

    /* Get the ith character from program memory
       table the address of the array in prog memory
       i the index of character.
       c the returned character.
       The table must be in program memory as an array of bytes (a multiple of 54).
       When accessing this array, 0x55 are interpreted as 0xFF (you can't have 0xFF in program memory.
       See file example for more informations. */
    static void getCARACFromProgMem(const char *table, byte i, charact c);

    /* Retrieve current invideo format
       return: 0=NTSC, 1=PAL, 3= No format detected */
    byte getInVideoFormat( void );

    /* Sets video format
       Operation is only done if the video format differs from the one in use
       newVideoFormat: 0=NTSC, 1=PAL */
    void setOutVideoFormat( byte newVideoFormat );

  private:
    static void printPixel(byte value);

    byte _pinCS;
    bool _isActivatedOsd;
    
    // Shadow Registers
    // Keep in synch with internal registers at all times to avoid unnecessary reads
    REG_VM0   _regVm0;
    REG_VM1   _regVm1;
    REG_HOS   _regHos;
    REG_VOS   _regVos;
    REG_DMM   _regDmm;
    REG_DMAH  _regDmah;
    REG_DMAL  _regDmal;
    REG_DMDI  _regDmdi;
    REG_CMM   _regCmm;
    REG_CMAH  _regCmah;
    REG_CMAL  _regCmal;
    REG_CMDI  _regCmdi;
    REG_OSDM  _regOsdm;
    REG_RBN   _regRb[16];
    REG_OSDBL _regOsdbl;
    REG_STAT  _regStat;
    REG_DMDO  _regDmdo;
    REG_CMDO  _regCmdo;
};
#endif /* MAX7456_H_ */

