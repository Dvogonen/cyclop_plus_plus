/******************************************************************************
   File: max7456registers.h

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
#ifndef MAX7456REGISTERS_H_
#define MAX7456REGISTERS_H_

#include <Arduino.h>

// Typedef of a character as stored in max7456 character memory.
typedef byte charact[54];

/******************************************************************************
  Video Mode 0 register
 ******************************************************************************/
#define VM0_ADDRESS_WRITE 0x00
#define VM0_ADDRESS_READ 0x80

union REG_VM0
{
  /* access to the whole register at once*/
  unsigned char byte ;
  
  /* individual bit access to the register */
  struct
  {
    /* Video BUffer Enable
       0 = Enable
       1 = Disable(VOUT is high impedance) */
    unsigned char videoBuffer : 1;

    /* Software Reset Bit
       When bit set all register are set to default. */
    unsigned char softwareResetBit : 1;

    /* Vertical Synchronization of On-Screen Data
       0 = Enable osd immediately
       1 = Enable osd at the next ~VSYNC  */
    unsigned char verticalSynch : 1;

    /* Enable Display of OSD Image
       0 = Off
       1 = On  */
    unsigned char enableOSD : 1;

    /* Synch Select Mode
       0x (0 or 1) = Internal when LOS, external otherwise
       10 (2)      = external
       11 (3)      = internal  */
    unsigned char synchSelect : 2;

    /* Video Standard Select
       0 = NTSC
       1 = PAL  */
    unsigned char videoSelect : 1;

    /* don't care*/
    unsigned char unused : 1;
  }
  bits;
};

/******************************************************************************
  Video Mode 1 register
 ******************************************************************************/
#define VM1_ADDRESS_WRITE 0x01
#define VM1_ADDRESS_READ 0x81

union REG_VM1
{
  /* access to the whole register at once*/
  unsigned char byte;
  
  /* individual bit access to the register */
  struct
  {
    /* Blinking Duty Cycle (On:Off)
       b00 (0) = BT:BT
       b01 (1) = BT:(2*BT)
       b10 (2) = BT:(3*BT)
       b11 (3) = (3*BT):BT  */
    unsigned char blinkingDutyCycle : 2;

    /* Blinking Time (BT)
       b00 (0) = 2 fields (NTSC = 33ms ; PAL = 40ms)
       b01 (1) = 4 fields (NTSC = 67ms ; PAL = 80ms)
       b10 (2) = 6 fields (NTSC = 100ms ; PAL = 120ms)
       b11 (3) = 8 fields (NTSC = 133ms ; PAL = 160ms) */
    unsigned char blinkingTime : 2;

    /* Background Mode Brightness
       b000 (0) = 0%
       b001 (1) = 7%
       b010 (2) = 14%
       b011 (3) = 21%
       b100 (4) = 28%
       b101 (5) = 35%
       b110 (6) = 43%
       b111 (7) = 49% */
    unsigned char backgroundModeBrightness : 3;

    /* Background Mode
       0 = DMM[5] & DMM[7] sets the state of each character background
       1 = Sets all displayed background pixel to gray */
    unsigned char backgroundMode : 1;
  }
  bits;
};

/******************************************************************************
  Horizontal Offset register
 ******************************************************************************/
#define HOS_ADDRESS_WRITE 0x02
#define HOS_ADDRESS_READ 0x82

union REG_HOS
{
  /* access to the whole register at once*/
  unsigned char byte;
  
  /* individual bit access to the register */
  struct
  {
    /* Vertical Position Offset
       b00 0000 (0) = Farthest left (-32 pixels)
       .
       b10 0000 (32) = No horizontal offset
       .
       b11 1111 (31) = Farthest right (+31pixels)  */
    unsigned char horizontalPositionOffset : 6;

    /* Don't care */
    unsigned char unsused : 2;
  }
  bits;
};

/******************************************************************************
  Vertical OffSet register
 ******************************************************************************/
#define VOS_ADDRESS_WRITE 0x03
#define VOS_ADDRESS_READ 0x83

union REG_VOS
{
  /* access to the whole register at once */
  unsigned char byte;
  
  /* individual bit access to the register */
  struct
  {
    /* Vertical Position Offset
       b0 0000 (0) = Farthest up (+16 pixels)
       .
       b1 0000 (16) = No vertical offset
       .
       b1 1111 (31) = Farthest down (-15 pixels) */
    unsigned char verticalPositionOffset : 5;
    
    /* Don't care */
    unsigned char unsused : 3;
  }
  bits;
};

/******************************************************************************
  Display Memory Mode register
 ******************************************************************************/
#define DMM_ADDRESS_WRITE 0x04
#define DMM_ADDRESS_READ 0x84

union REG_DMM
{
  /* access to the whole register at once */
  unsigned char byte;
  
  /* individual bit access to the register */
  struct
  {
    /* Auto-Increment Mode
       0 = Disabled
       1 = Enabled
       @note When this bit is enabled for the first time, data in the Display Memory Address (DMAH[0] and
      DMAL[7:0]) registers are used as the starting location to which the data is written. When performing
      the auto-increment write for the display memory, the 8-bit address is internally generated, and
      therefore only 8-bit data is required by the SPI-compatible interface (Figure 21). The content is to
      be interpreted as a Character Address byte if DMAH[1] = 0 or a Character Attribute byte if
      DMAH[1] = 1. This mode is disabled by writing the escape character 1111 1111.
      If the Clear Display Memory bit is set, this bit is reset internally. */
    unsigned char autoIncrementMode : 1;
    
    /* Vertical Sync Clear (Valid only when clear display memory = 1, (DMM[2] = 1) )
       0 = Immediately applies the clear display-memory command, DMM[2] = 1
       1 = Applies the clear display-memory command, DMM[2] = 1, at the next VSYNC time */
    unsigned char verticalSynchClear : 1;
    
    /* Clear Display Memory
       0 = Inactive
       1 = Clear (fill all display memories with zeros)
       @note This bit is automatically cleared after the operation is completed (the operation requires
       20us). The user does not need to write a 0 afterwards. The status of the bit can be checked by
       reading this register.
       This operation is automatically performed:
       a) On power-up
       b) Immediately following the rising edge of RESET
       c) Immediately following the rising edge of CS after VM0[1] has been set to 1 */
    unsigned char clearDisplayMemory : 1;
    
    /* Invert Bit (applies to characters written in 16-bit operating mode)
       0 = Normal (white pixels display white, black pixels display black)
       1 = Invert (white pixels display black, black pixels display white) */
    unsigned char INV : 1;
    
    /* Blink Bit (applies to characters written in 16-bit operating mode)
       0 = Blinking off
       1 = Blinking on
       @note Blinking rate and blinking duty cycle data in the Video Mode 1 (VM1) register are used for blinking control */
    unsigned char BLK : 1;
    
    /* Local Background Control Bit (applies to characters written in 16-bit operating mode)
       0 = sets the background pixels of the character to the video input (VIN) when in external sync mode.
       1 = sets the background pixels of the character to the background mode brightness level defined by VM1[6:4] in external or internal sync mode.
       @note In internal sync mode, the local background control bit behaves as if it is set to 1 */
    unsigned char LBC : 1;
    
    /* Operation Mode Selection
       0 = 16-bit operation mode
       1 = 8-bit operation mode */
    unsigned char operationModeSelection : 1;

    /* Don't care */
    unsigned char unsused : 1;
  }
  bits;
};

/******************************************************************************
  Display Memory Address High register
 ******************************************************************************/
#define DMAH_ADDRESS_WRITE 0x05
#define DMAH_ADDRESS_READ 0x85

union REG_DMAH
{
  /* access to the whole register at once*/
  unsigned char byte;
  
  /* individual bit access to the register */
  struct
  {
    /* 8th bit for Display Memory Address. */
    unsigned char DisplayMemoryAdressBit8 : 1;
    
    /* */
     unsigned char byteSelectionBit : 1;
    
    /* Don't care*/
    unsigned char unsused : 6;
  }
  bits;
};

/******************************************************************************
  Display Memory Address Low register
 ******************************************************************************/
#define DMAL_ADDRESS_WRITE 0x06
#define DMAL_ADDRESS_READ 0x86

union REG_DMAL
{
  /* access to the whole register at once*/
  unsigned char byte;
};

/******************************************************************************
  Display Memory Data In register
 ******************************************************************************/
#define DMDI_ADDRESS_WRITE 0x07
#define DMDI_ADDRESS_READ 0x87

union REG_DMDI
{
  /* access to the whole register at once*/
  unsigned char byte;
};

/******************************************************************************
  Character Memory Mode register
 ******************************************************************************/
#define CMM_ADDRESS_WRITE 0x08
#define CMM_ADDRESS_READ 0x88

union REG_CMM
{
  /* access to the whole register at once*/
  unsigned char byte;
};

/******************************************************************************
  Character Memory Address High register
 ******************************************************************************/
#define CMAH_ADDRESS_WRITE 0x09
#define CMAH_ADDRESS_READ 0x89

typedef union REG_CMAH
{
  /* access to the whole register at once*/
  unsigned char byte;
};

/******************************************************************************
  Character Memory Address Low register
 ******************************************************************************/
#define CMAL_ADDRESS_WRITE 0x0A
#define CMAL_ADDRESS_READ 0x8A

union REG_CMAL
{
  /* access to the whole register at once*/
  unsigned char byte;
};

/******************************************************************************
  Character Memory Data In register
 ******************************************************************************/
#define CMDI_ADDRESS_WRITE 0x0B
#define CMDI_ADDRESS_READ 0x8B

union REG_CMDI
{
  /* access to the whole register at once*/
  unsigned char byte;

  /* individual bit access to the register */
  struct
  {
    /* value of the right most pixel*/
    unsigned char rightMostPixel : 2;

    /* value of the right center pixel*/
    unsigned char rightCenterPixel : 2;

    /* value of the left center pixel*/
    unsigned char leftCenterPixel : 2;

    /* value of the left most pixel*/
    unsigned char leftMostPixel : 2;
  }
  bits;
};

/******************************************************************************
  OSD Insersion Mux register
 ******************************************************************************/
#define OSDM_ADDRESS_WRITE 0x0C
#define OSDM_ADDRESS_READ 0x8C

union REG_OSDM
{
  /* access to the whole register at once*/
  unsigned char byte;
  
  /* individual bit access to the register */
  struct
  {
    /* OSD Insersion Mux Switching Time
       b000 (0) : 30ns (maximum sharpness/maximum crosscolor artifacts )
       b001 (1) : 35ns
       b010 (2) : 50ns
       b011 (3) : 75ns
       b100 (4) : 100ns
       b101 (5) : 120ns (minimum sharpness/minimum crosscolor artifacts) */
    unsigned char osdInsertionMuxSwitchingTime : 3;

    /* OSD Rise And Fall Time
       b000 (0) : 20ns (maximum sharpness/maximum crosscolor artifacts )
       b001 (1) : 30ns
       b010 (2): 35ns
       b011 (3) : 60ns
       b100 (4) : 80ns
       b101 (5) : 110ns (minimum sharpness/minimum crosscolor artifacts) */
    unsigned char osdRiseAndFallTime : 3;

    /* don't care */
    unsigned char unused : 2;
  }
  bits;
};

/******************************************************************************
  Row Brithness registers (15 of them)
 ******************************************************************************/
#define RB0_ADDRESS_WRITE 0x10
#define RB0_ADDRESS_READ 0x90

#define RB1_ADDRESS_WRITE 0x11
#define RB1_ADDRESS_READ 0x91

#define RB2_ADDRESS_WRITE 0x12
#define RB2_ADDRESS_READ 0x92

#define RB3_ADDRESS_WRITE 0x13
#define RB3_ADDRESS_READ 0x93

#define RB4_ADDRESS_WRITE 0x14
#define RB4_ADDRESS_READ 0x94

#define RB5_ADDRESS_WRITE 0x15
#define RB5_ADDRESS_READ 0x95

#define RB6_ADDRESS_WRITE 0x16
#define RB6_ADDRESS_READ 0x96

#define RB7_ADDRESS_WRITE 0x17
#define RB7_ADDRESS_READ 0x97

#define RB8_ADDRESS_WRITE 0x18
#define RB8_ADDRESS_READ 0x98

#define RB9_ADDRESS_WRITE 0x19
#define RB9_ADDRESS_READ 0x99

#define RBA_ADDRESS_WRITE 0x1A
#define RBA_ADDRESS_READ 0x9A

#define RBB_ADDRESS_WRITE 0x1B
#define RBB_ADDRESS_READ 0x9B

#define RBC_ADDRESS_WRITE 0x1C
#define RBC_ADDRESS_READ 0x9C

#define RBD_ADDRESS_WRITE 0x1D
#define RBD_ADDRESS_READ 0x9D

#define RBE_ADDRESS_WRITE 0x1E
#define RBE_ADDRESS_READ 0x9E

#define RBF_ADDRESS_WRITE 0x1F
#define RBF_ADDRESS_READ 0x9F

union REG_RBN
{
  /* access to the whole register at once*/
  unsigned char byte;
  
  /* individual bit access to the register */
  struct
  {
    /* Character white level
       b00 (0) = 120%
       b01 (1) = 100%
       b10 (2) = 90%
       b11 (3) = 80% */
    unsigned char characterWhiteLevel : 2;

    /* Character black level
       b00 (0) = 0%
       b01 (1) = 10%
       b10 (2) = 20%
       b11 (3) = 20% */
    unsigned char characterBlackLevel : 2;

    /* don't care*/
    unsigned char unused : 4;
  }
  bits;
};

/******************************************************************************
  OSD Black Level register
 ******************************************************************************/
#define OSDBL_ADDRESS_WRITE 0x6C
#define OSDBL_ADDRESS_READ 0xEC

union REG_OSDBL
{
  /* access to the whole register at once */
  unsigned char byte;
  
  /* individual bit access to the register */
  struct
  {
    /* do not change those bits : factory preset */
    unsigned char doNotChange : 4;
    /* OSD Image Black Level Control.
       0 = automatic.
       1 = manual. */
    unsigned char osdImageBlackLevelControl : 1;

    /* don't care */
    unsigned char unused : 3;
  }
  bits;
};

/******************************************************************************
  Status Register
 ******************************************************************************/
#define STAT_ADDRESS_READ 0xA0

union REG_STAT
{
  /* access to the whole register at once */
  unsigned char byte;
  
  /* individual bit access to the register */
  struct
  {
    /* Detected PAL.
       0 = PAL signal is not detected ad VIN.
       1 = PAL signal is detected at VIN. */
    unsigned char PALDetected : 1;
    
    /* Detected NTSC.
       0 = NTSC signal is not detected ad VIN.
       1 = NTSC signal is detected at VIN. */
    unsigned char NTSCDetected : 1;
    
    /* Loss-Of-Sync.
       0 = Sync Active. Asserted after 32 consecutive input video lines.
       1 = No Sync. Asserted after 32 consecutive missing input video lines. */
    unsigned char LOS : 1;
    
    /* ~HSYNC Ouput Level.
       0 = Active during horizontal synch time.
       1 = inactive otherwise. */
    unsigned char NOTHsynchOutputLevel : 1;
    
    /* ~VSYNC Ouput Level.
       0 = Active during vertical synch time.
       1 = Inactive otherwise. */
    unsigned char NOTVsynchOutputLevel : 1;
    
    /* Character Memory Status.
       0 = Available access.
       1 = Unavailable access. */
    unsigned char characterMemoryStatus : 1;
    
    /* Reset Mode.
       0 = Clear when power up reset mode is complete.
       1 = Set when in power-up reset mode. */
    unsigned char resetMode : 1;
    
    /* don't care. */
    unsigned char unused : 1;
  }
  bits;
};

/******************************************************************************
  Display Memory Data Out register
 ******************************************************************************/
#define DMDO_ADDRESS_READ 0xB0

union REG_DMDO
{
  /* access to the whole register at once*/
  unsigned char byte;
};

/******************************************************************************
  Character Memory Data Out register
 ******************************************************************************/
#define CMDO_ADDRESS_READ 0xC0

union REG_CMDO
{
  /* access to the whole register at once*/
  unsigned char byte;
  
  /* individual bit access to the register */
  struct
  {
    /* right most pixel*/
    unsigned char rightMostPowel : 2;
    
    /* right center pixel*/
    unsigned char rightCenterPixel : 2;
    
    /* left center pixel*/
    unsigned char leftCenterPixel : 2;
    
    /* left most pixel*/
    unsigned char leftMostPixel : 2;
  }
  bits;
};

/******************************************************************************
 ******************************************************************************/
/* COLOR_BLACK - Black value for a pixel (2bits) */
#define COLOR_BLACK       0

/* COLOR_WHITE - White value for a pixel (2bits) */
#define COLOR_WHITE       2

/* COLOR_TRANSPARENT - Transparent value for a pixel (2bits) */
#define COLOR_TRANSPARENT 1

/* COLOR_GREY - Grey value for a pixel (2bits) */
#define COLOR_GREY COLOR_TRANSPARENT

/* PIXEL - represent a 4-pixels value */
struct PIXEL
{
  /* 4th pixel*/
  byte pix3 : 2;
  /* 3rd pixel*/
  byte pix2 : 2;
  /* 2nd pixel*/
  byte pix1 : 2;
  /* 1st pixel*/
  byte pix0 : 2;
};

/* LINE - Represents a line in a max7456 character ie. 12 pixels */
union LINE
{
  /* the whole line*/
  byte byte[3];
  
  /* individual 4-pixels access*/
  struct PIXEL pixels[3];
};

/* CARACT - Represents a character with lines and pixels.
  	example : myCarac.line[3].pixels[2].pix2 = COLOR_TRANSPARENT ; */
union CARACT
{
  /* the whole CARACT as in max7456 Character Memory*/
  charact byte;
  
  /* acces with lines*/
  union LINE line[18];
};

enum {
  _BT_BT = 0,
  _BT_2BT,
  _BT_3BT,
  _3BT_BT
};

enum {
  _2fields = 0,
  _4fields,
  _6fields,
  _8fields
};

#endif /* MAX7456REGISTERS_H_ */
