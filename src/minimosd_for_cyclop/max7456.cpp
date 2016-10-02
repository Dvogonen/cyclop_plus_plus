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
  SPI.transfer(_regVm1.whole);
  digitalWrite(_pinCS, HIGH);
}

/******************************************************************************
   Function: Max7456::setDisplayOffsets
 ******************************************************************************/
void Max7456::setDisplayOffsets(byte horizontal, byte vertical)
{
  _regHos.whole = 0;
  _regVos.whole = 0;

  _regHos.bits.horizontalPositionOffset = horizontal;
  _regVos.bits.verticalPositionOffset = vertical;

  digitalWrite(_pinCS, LOW);
  SPI.transfer(HOS_ADDRESS_WRITE);
  SPI.transfer(_regHos.whole);

  SPI.transfer(VOS_ADDRESS_WRITE);
  SPI.transfer(_regVos.whole);

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
  _regCmm = 0xA0; //To write the NVM array
  SPI.transfer(CMM_ADDRESS_WRITE);
  SPI.transfer(_regCmm);
  //while STAT[5] is not good, we wait.
  _regStat.bits.characterMemoryStatus = 1;
  while (  _regStat.bits.characterMemoryStatus == 1)
  {
    SPI.transfer(STAT_ADDRESS_READ);
    _regStat.whole = SPI.transfer(0x00);
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
  _regCmm = 0x50; //To read from the NVM array
  SPI.transfer(CMM_ADDRESS_WRITE);
  SPI.transfer(_regCmm);

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

  _regDmm.whole = 0x01;

  _regDmm.bits.INV = inv;
  _regDmm.bits.BLK = blink;

  digitalWrite(_pinCS, LOW);
  SPI.transfer(DMM_ADDRESS_WRITE);
  SPI.transfer(_regDmm.whole);

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
  /*
  	_regVm0.bits.
  	SPI.transfer(VM0_ADDRESS_WRITE);
  	SPI.transfer(0x4c);
  */
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
  _regDmm.bits.clearDisplayMemory = 1 ;

  digitalWrite(_pinCS, LOW);
  SPI.transfer(DMM_ADDRESS_WRITE);
  SPI.transfer(_regDmm.whole);

  /*wait for operation to be complete.
    while (_regDmm.bits.clearDisplayMemory == 1 )
    {
    SPI.transfer(DMM_ADDRESS_READ);
    _regDmm.whole = SPI.transfer(0x00);
    }
  */
  digitalWrite(_pinCS, HIGH); //disable device
}

/******************************************************************************
   Function: Max7456::init
 ******************************************************************************/
void Max7456::init(byte iPinCS)
{
  _pinCS = iPinCS;
  _isActivatedOsd = false;

  _regVm1.whole = 0b01000111;
  pinMode(iPinCS, OUTPUT);
  digitalWrite(iPinCS, HIGH); //disable device
  delay(100); //power up time

  digitalWrite(_pinCS, LOW);
  SPI.transfer(VM0_ADDRESS_WRITE);

  _regVm0.whole = 0x00;
  _regVm0.bits.videoSelect = 1; //1=PAL, 0=NTSC
  _regVm0.bits.softwareResetBit = 1;
  SPI.transfer(_regVm0.whole);
  digitalWrite(_pinCS, HIGH);
  delay(500);

  digitalWrite(_pinCS, LOW);
  for (int x = 0 ; x < 16 ; x++)
  {
    _regRb[x].whole = 0x00;
    // _regRb[x].bits.characterWhiteLevel = 2;  // 90%
    _regRb[x].bits.characterWhiteLevel = 1;     // 100%
    SPI.transfer(x + RB0_ADDRESS_WRITE);
    SPI.transfer(_regRb[x].whole);
  }

  _regVm0.whole = 0x00;
  _regVm0.bits.verticalSynch = 1;

  SPI.transfer(VM0_ADDRESS_WRITE);
  SPI.transfer(_regVm0.whole);

  //  digitalWrite(_pinCS,HIGH);
  //
  //  digitalWrite(_pinCS,LOW);
  //SPI.transfer(VM1_ADDRESS_WRITE);
  //SPI.transfer(0x0C);

  digitalWrite(_pinCS, HIGH);
}

/******************************************************************************
   Function: Max7456::activateOSD
 ******************************************************************************/
void Max7456::activateOSD(bool act)
{
  if (_isActivatedOsd != act)
  {
    _regVm0.bits.videoSelect = 1;
    if (act)
      _regVm0.bits.enableOSD = 1;
    else
      _regVm0.bits.enableOSD = 0;

    digitalWrite(_pinCS, LOW);
    SPI.transfer(VM0_ADDRESS_WRITE);
    SPI.transfer(_regVm0.whole);
    digitalWrite(_pinCS, HIGH);
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
    _regVm0.bits.synchSelect = 0; //0

  digitalWrite(_pinCS, LOW);
  SPI.transfer(VM0_ADDRESS_WRITE);
  SPI.transfer(_regVm0.whole);
  digitalWrite(_pinCS, HIGH);
}

/******************************************************************************
   Function: Max7456::CARACT2ByteArray
 ******************************************************************************/
byte* Max7456::CARACT2ByteArray(const CARACT car)
{
  byte *array = NULL;
  array = new charact;
  for (int i = 0 ; i < 54 ; i++)
    array[i] = car.whole[i];

  return array;
}

/******************************************************************************
   Function: Max7456::byteArray2CARACT
 ******************************************************************************/
CARACT Max7456::byteArray2CARACT(const charact array)
{
  CARACT car;
  for (int i = 0 ; i < 54 ; i++)
    car.whole[i] = array[i];

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
  _regStat.whole = SPI.transfer(0x00);
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
  _regVm0.whole = SPI.transfer(0x00);
  digitalWrite(_pinCS, HIGH);

  // Switch OSD video format
  if (((byte)_regVm0.bits.videoSelect) != newVideoFormat )
  {
    _regVm0.bits.videoSelect = newVideoFormat & 0x01;
    digitalWrite(_pinCS, LOW);
    SPI.transfer(VM0_ADDRESS_WRITE);
    SPI.transfer(_regVm0.whole);
    digitalWrite(_pinCS, HIGH);
  }
}
