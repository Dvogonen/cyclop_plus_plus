/*******************************************************************************
  MinimDisplay is firmware for the MinimOSD board that turns it into a general 
  device for displaying information on a video feed.

  The MIT License (MIT)

  Copyright (c) 2016 Kjell Kernen (Dvogonen)

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
********************************************************************************/
#include <SPI.h>
#include "max7456.h"

#define redLed 3
#define greenLed 4

Max7456 osd;
unsigned long counter = 0;
byte tab[]={0xC8,0xC9};

void setup()
{
  SPI.begin();
  
  osd.init(10);
  osd.setDisplayOffsets(60,18);
  osd.setBlinkParams(_8fields, _BT_BT);
 
  osd.activateOSD();
  osd.printMax7456Char(0x01,0,1);
  osd.print("Hello world :)",1,3);
  osd.print("Current Arduino time :",1,4);

  osd.printMax7456Char(0xD1,9,6,true);
  osd.print("00'00\"",10,6);  
  osd.printMax7456Chars(tab,2,12,7);
  pinMode(redLed,OUTPUT);
  pinMode(greenLed,OUTPUT);
  
  //base time = 160ms,time on = time off.
}


void loop()
{
  if(counter%2 == 0)
  {
    digitalWrite(redLed,LOW);
    digitalWrite(greenLed,HIGH);
  }
  else
  {
    digitalWrite(redLed,HIGH);
    digitalWrite(greenLed,LOW);
  }

  counter = millis()/1000;
  
  osd.print(int(counter/60),10,6,2,0,false,true);
  osd.print(int(counter%60),13,6,2,0,false,true);
  
  delay(100);
}

