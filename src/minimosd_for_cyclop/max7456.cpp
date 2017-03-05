/******************************************************************************
   File: max7456registers.cpp

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
 ******************************************************************************/
#include "max7456.h"
#include <Arduino.h>
#include <SPI.h>

/******************************************************************************
   Function: Max7456::Max7456
 ******************************************************************************/
Max7456::Max7456(byte pinCS)
{
  this->init(pinCS);
}

/******************************************************************************
   Function: Max7456::setBlinkParams
 ******************************************************************************/
void Max7456::setBlinkParams(byte blinkBase, byte blinkDC)
{
  _regVm1.bits.blinkingTime = blinkBase;
  _regVm1.bits.blinkingDutyCycle = blinkDC;
  digitalWrite(_pinCS, LOW);
  SPI.transfer(VM1_ADDRESS_WRITE);
  SPI.transfer(_regVm1.byte);
  digitalWrite(_pinCS, HIGH);
}

/******************************************************************************
   Function: Max7456::setDisplayOffsets
 ******************************************************************************/
void Max7456::setDisplayOffsets(byte horizontal, byte vertical)
{
  digitalWrite(_pinCS, LOW);
  _regHos.byte = 0;
  _regHos.bits.horizontalPositionOffset = horizontal;
  SPI.transfer(HOS_ADDRESS_WRITE);
  SPI.transfer(_regHos.byte);
  digitalWrite(_pinCS, HIGH);

  digitalWrite(_pinCS, LOW);
  SPI.transfer(VOS_ADDRESS_WRITE);
  _regVos.byte = 0;
  _regVos.bits.verticalPositionOffset = vertical;
  SPI.transfer(_regVos.byte);
  digitalWrite(_pinCS, HIGH);
}

/******************************************************************************
   Function: Max7456::Max7456
 ******************************************************************************/
Max7456::Max7456()
{
}

/******************************************************************************
   Function: Max7456::sendCharacter
 ******************************************************************************/
void Max7456::sendCharacter(const charact chara, byte pos)
{
  activateOSD(false);
  //datasheet p38
  digitalWrite(_pinCS, LOW);
  SPI.transfer(CMAH_ADDRESS_WRITE);
  SPI.transfer(pos);

  for (byte i = 0 ; i < 54 ; i++)
  {
    SPI.transfer(CMAL_ADDRESS_WRITE);
    SPI.transfer(i);
    SPI.transfer(CMDI_ADDRESS_WRITE);
    SPI.transfer(chara[i]);
  }
  _regCmm.byte = 0xA0; //To write the NVM array
  SPI.transfer(CMM_ADDRESS_WRITE);
  SPI.transfer(_regCmm.byte);
  //while STAT[5] is not good, we wait.
  _regStat.bits.characterMemoryStatus = 1;
  while (  _regStat.bits.characterMemoryStatus == 1)
  {
    SPI.transfer(STAT_ADDRESS_READ);
    _regStat.byte = SPI.transfer(0x00);
  }
  digitalWrite(_pinCS, HIGH);
}

/******************************************************************************
   Function: Max7456::getCharacter
 ******************************************************************************/
void Max7456::getCharacter(charact chara, byte x, byte y)
{
  byte charAddress;

  if (y <= 0)
    charAddress = x;
  else
    charAddress = x + y * 16;

  activateOSD(false);
  //datasheet p38
  digitalWrite(_pinCS, LOW);
  SPI.transfer(CMAH_ADDRESS_WRITE);
  SPI.transfer(charAddress);
  _regCmm.byte = 0x50; //To read from the NVM array
  SPI.transfer(CMM_ADDRESS_WRITE);
  SPI.transfer(_regCmm.byte);

  for (byte i = 0 ; i < 54 ; i++)
  {
    SPI.transfer(CMAL_ADDRESS_WRITE);
    SPI.transfer(i);
    SPI.transfer(CMDO_ADDRESS_READ); //read from device through spi
    chara[i] = SPI.transfer(0x00);
  }
  digitalWrite(_pinCS, HIGH);
}
/******************************************************************************
   Function: Max7456::printCharacterToSerial
 ******************************************************************************/
void Max7456::printCharacterToSerial(const charact array, bool img)
{
  if (img)
  {
    CARACT car ;
    car = Max7456::byteArray2CARACT(array);

    Serial.println("------------");
    for (int i = 0 ; i < 18 ; i++)
    {
      for (int j = 0 ; j < 3 ; j++)
      {
        printPixel(car.line[i].pixels[j].pix0);
        printPixel(car.line[i].pixels[j].pix1);
        printPixel(car.line[i].pixels[j].pix2);
        printPixel(car.line[i].pixels[j].pix3);
      }
      Serial.println("");
    }
    Serial.println("------------");
  }
  else
  {
    Serial.print("{");
    for (unsigned int i = 0 ; i < 53 ; i++)
    {
      Serial.print("0x");
      Serial.print(String(array[i], HEX));
      Serial.print(", ");
    }
    Serial.print("0x");
    Serial.print(String(array[53], HEX));
    Serial.println("};");
  }
}

/******************************************************************************
   Function: Max7456::Max7456
 ******************************************************************************/
void Max7456::printPixel(byte value)
{
  switch (value )
  {
    case COLOR_BLACK :
      Serial.print("#");
      return;
    case COLOR_WHITE :
      Serial.print("*");
      return;
    default:
      Serial.print(" ");
      return;
  }
}

/******************************************************************************
   Function: Max7456::print
 ******************************************************************************/
void Max7456::print(const char string[], byte x, byte y, byte blink, byte inv)
{
  char          currentChar;
  byte          size;
  byte *chars = NULL;

  if (!string) return;

  size = 0;
  currentChar = string[0];

  while (currentChar != '\0')
  {
    currentChar = string[++size];
  }
  chars = (byte*) malloc(size * sizeof(byte));

  for (byte i = 0 ; i < size ; i++)
  {
    chars[i] = string[i];
  }
  printMax7456Chars(chars, size, x,  y,  blink , inv );
  free(chars);
}

void Max7456::printMax7456Char(const byte address, byte x, byte y, byte blink, byte inv)
{
  byte ad = address;
  printMax7456Chars(&ad, 1, x, y, blink, inv);
}

/******************************************************************************
   Function: Max7456::printMax7456Chars
 ******************************************************************************/
void Max7456::printMax7456Chars(byte chars[], byte size, byte x, byte y, byte blink , byte inv )
{
  byte          currentCharMax7456;
  byte          posAddressLO;
  byte          posAddressHI;
  unsigned int  posAddress;

  posAddress = 30 * y + x;

  posAddressHI = posAddress >> 8;
  posAddressLO = posAddress;

  _regDmm.byte = 0x01;

  _regDmm.bits.INV = inv;
  _regDmm.bits.BLK = blink;

  digitalWrite(_pinCS, LOW);
  SPI.transfer(DMM_ADDRESS_WRITE);
  SPI.transfer(_regDmm.byte);

  SPI.transfer(DMAH_ADDRESS_WRITE); // set start address high
  SPI.transfer(posAddressHI);

  SPI.transfer(DMAL_ADDRESS_WRITE); // set start address low
  SPI.transfer(posAddressLO);

  for (int i = 0; i < size ; i++)
  {
    currentCharMax7456 = chars[i];
    SPI.transfer(DMDI_ADDRESS_WRITE);
    SPI.transfer(currentCharMax7456);
  }
  //end character (we're done).
  SPI.transfer(DMDI_ADDRESS_WRITE);
  SPI.transfer(0xff);
  digitalWrite(_pinCS, HIGH);
}

/******************************************************************************
   Function: Max7456::print
 ******************************************************************************/

void Max7456::print(double value, byte x, byte y, byte before, byte after, byte blink, byte inv)
{
  char *strValue = NULL;

  strValue = (char*) malloc((before + after + 2) * sizeof(char));

  if (after == 0)
    dtostrf(value, before + after, after, strValue);
  else
    dtostrf(value, before + after + 1, after, strValue);

  for (int i = 0 ; i < before + after + 1; i++)
  {
    if (strValue[i] == ' ' || strValue[i] == '-')
      strValue[i] = '0';
  }
  if (value < 0)
    strValue[0] = '-';
  if (after == 0) //If the result is bigger, we truncate it so the OSD won't be falsed.
    strValue[before] = '\0';
  else
    strValue[before + after + 1] = '\0';

  print(strValue, x, y, blink, inv);
  free(strValue);
}

/******************************************************************************
   Function: Max7456::clearScreen
 ******************************************************************************/
void Max7456::clearScreen()
{

  digitalWrite(_pinCS, LOW);
  _regDmm.bits.clearDisplayMemory = 1 ;
  SPI.transfer(DMM_ADDRESS_WRITE);
  SPI.transfer(_regDmm.byte);
  digitalWrite(_pinCS, HIGH);

  /* wait for operation to complete */
  while (_regDmm.bits.clearDisplayMemory == 1 )
  {
    digitalWrite(_pinCS, LOW);
    SPI.transfer(DMM_ADDRESS_READ);
    _regDmm.byte = SPI.transfer(0x00);
    digitalWrite(_pinCS, HIGH);
  }
}

/******************************************************************************
   Function: Max7456::init
 ******************************************************************************/
void Max7456::init(byte iPinCS)
{
  // Setup Chip Select Pin
  _pinCS = iPinCS;
  pinMode(_pinCS, OUTPUT);

  // Hardware boot time (Really needed ?)
  delay(100);

  // Reset All registers to default and clear video memory
  digitalWrite(_pinCS, HIGH); // Safety (Needed?)
  digitalWrite(_pinCS, LOW);
  _regVm0.bits.softwareResetBit = 1;
  SPI.transfer(VM0_ADDRESS_WRITE);
  SPI.transfer(_regVm0.byte);
  digitalWrite(_pinCS, HIGH);
  _regVm0.bits.softwareResetBit = 0; // he real register bit is reset automatically

  // Wait for the reset to complete
  delay(100);  // Probably to long

  // Set class global variables
  _isActivatedOsd = false;

  // Set shadow registers to default values
  for (int x = 0 ; x < 16 ; x++)
    _regRb[x].byte = 0b00000001;

  _regVm0.byte =   0b00000000;
  _regVm1.byte =   0b01000111;
  _regHos.byte =   0b00100000;
  _regVos.byte =   0b00010000;
  _regDmm.byte =   0b00000000;
  _regDmah.byte =  0b00000000;
  _regDmal.byte =  0b00000000;
  _regDmdi.byte =  0b00000000;
  _regCmm.byte =   0b00000000;
  _regCmah.byte =  0b00000000;
  _regCmal.byte =  0b00000000;
  _regCmdi.byte =  0b00000000;
  _regOsdm.byte =  0b00011011;
  _regStat.byte =  0b00000000;  // Read only, value does not matter
  _regDmdo.byte =  0b00000000;  // Read only, value does not matter
  _regCmdo.byte =  0b00000000;  // Read only, value does not matter

  // Fetch OSDBL register value. Bits 0-3 have no default values
  digitalWrite(_pinCS, LOW);
  SPI.transfer(OSDBL_ADDRESS_READ);
  _regOsdbl.byte = SPI.transfer(0x00);
  digitalWrite(_pinCS, HIGH);

  // Set video format and vertical write synch
  digitalWrite(_pinCS, LOW);
  _regVm0.bits.videoSelect = 0;      //1=PAL, 0=NTSC
  _regVm0.bits.verticalSynch = 1;
  SPI.transfer(VM0_ADDRESS_WRITE);
  SPI.transfer(_regVm0.byte);
  digitalWrite(_pinCS, HIGH);

  // Set sharpness parameters
  digitalWrite(_pinCS, LOW);
  _regOsdm.bits.osdInsertionMuxSwitchingTime = 0b010; //000= Max sharp, 111= Least sharp
  _regOsdm.bits.osdRiseAndFallTime = 0b010;           //000= Max sharp, 111= Least sharp
  SPI.transfer(OSDM_ADDRESS_WRITE);
  SPI.transfer(_regOsdm.byte);
  digitalWrite(_pinCS, HIGH);

  // Set row white levels
  for (int x = 0 ; x < 16 ; x++)
  {
    digitalWrite(_pinCS, LOW);
    _regRb[x].bits.characterWhiteLevel = 0;    // 0=120%, 1=100%, 2=90%, 3=80%
    SPI.transfer(x + RB0_ADDRESS_WRITE);
    SPI.transfer(_regRb[x].byte);
    digitalWrite(_pinCS, HIGH);
  }
  // Enable automatic OSD black level control
  digitalWrite(_pinCS, LOW);
  _regOsdbl.bits.osdImageBlackLevelControl = 0; //0=Enable, 1=Disable
  SPI.transfer(OSDBL_ADDRESS_WRITE);
  SPI.transfer(_regOsdbl.byte);
  digitalWrite(_pinCS, HIGH);
}

/******************************************************************************
   Function: Max7456::activateOSD
 ******************************************************************************/
void Max7456::activateOSD(bool act)
{
  if (_isActivatedOsd != act)
  {
    digitalWrite(_pinCS, LOW);
    _regVm0.bits.enableOSD = act ? 1 : 0; // Enable OSD
    SPI.transfer(VM0_ADDRESS_WRITE);
    SPI.transfer(_regVm0.byte);
    digitalWrite(_pinCS, HIGH);

    // Enable automatic black level controll
    if ( act )
    {
      digitalWrite(_pinCS, LOW);
      _regOsdbl.bits.osdImageBlackLevelControl = 0;  // 0=Enable, 1=Disable
      SPI.transfer(OSDBL_ADDRESS_WRITE);
      SPI.transfer(_regOsdbl.byte);
      digitalWrite(_pinCS, HIGH);
    }
    _isActivatedOsd = act;
  }
}

/******************************************************************************
   Function: Max7456::activateExternalVideo
 ******************************************************************************/
void Max7456::activateExternalVideo(bool activExtVid)
{
  if (!activExtVid)
    _regVm0.bits.synchSelect = 3; //11
  else
    _regVm0.bits.synchSelect = 0; //00

  digitalWrite(_pinCS, LOW);
  SPI.transfer(VM0_ADDRESS_WRITE);
  SPI.transfer(_regVm0.byte);
  digitalWrite(_pinCS, HIGH);
}

/******************************************************************************
   Function: Max7456::CARACT2ByteArray
 ******************************************************************************/
void Max7456::CARACT2ByteArray(const CARACT car, byte *array)
{
  for (int i = 0 ; i < 54 ; i++)
    array[i] = car.byte[i];

  return array;
}

/******************************************************************************
   Function: Max7456::byteArray2CARACT
 ******************************************************************************/
CARACT Max7456::byteArray2CARACT(const charact array)
{
  CARACT car;
  for (int i = 0 ; i < 54 ; i++)
    car.byte[i] = array[i];

  return car;
}

/******************************************************************************
   Function: Max7456::getCARACFromProgMem
 ******************************************************************************/
void Max7456::getCARACFromProgMem(const char *table, byte i, charact car)
{
  unsigned long index;
  byte read;
  index = i * 54;
  for (unsigned long j = 0 ; j < 54 ; j++)
  {
    read = pgm_read_byte_near(table + index + j );
    car[j] = read;
    if (car[j] == 0x55)
      car[j] = 0xff;
  }
}

/******************************************************************************
   Function: Max7456::getInVideoFormat
 ******************************************************************************/
byte Max7456::getInVideoFormat( void )
{
  digitalWrite(_pinCS, LOW);
  SPI.transfer(STAT_ADDRESS_READ);
  _regStat.byte = SPI.transfer(0x00);
  digitalWrite(_pinCS, HIGH);

  if (_regStat.bits.NTSCDetected ) return 0;
  if (_regStat.bits.PALDetected ) return 1;
  return 2;
}

/******************************************************************************
   Function: Max7456::setOutVideoFormat
 ******************************************************************************/
void Max7456::setOutVideoFormat( byte newVideoFormat )
{
  // Read VM0 register
  digitalWrite(_pinCS, LOW);
  SPI.transfer(VM0_ADDRESS_READ);
  _regVm0.byte = SPI.transfer(0x00);
  digitalWrite(_pinCS, HIGH);

  // Switch OSD video format
  if (_regVm0.bits.videoSelect != newVideoFormat )
  {
    digitalWrite(_pinCS, LOW);
    _regVm0.bits.videoSelect = newVideoFormat;
    SPI.transfer(VM0_ADDRESS_WRITE);
    SPI.transfer(_regVm0.byte);
    digitalWrite(_pinCS, HIGH);
  }
}
