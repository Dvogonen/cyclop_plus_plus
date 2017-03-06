/*******************************************************************************
  CYCLOP++ with OSD brings manual channel selection and onscreen information to
  the HobbyKing Quanum Cyclops FPV googles.

  The rx5808-pro and rx5808-pro-diversity projects served as a starting
  point for the code base, even if little of the actual code remains.
  Without those projects CYCLOP++ would not have been created. All possible
  credit goes to the two mentioned projects and their contributors.

  The max7456 library by theboredengineers with a few changes by Dvogonen is
  used to control the video circuit.

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
********************************************************************************/
#ifndef cyclop_plus_osd_h
#define cyclop_plus_osd_h

// User Configuration Options
#define BATTERY_ALARM_OPTION      0
#define ALARM_LEVEL_OPTION        1
#define BATTERY_TYPE_OPTION       2
#define BATTERY_CALIB_OPTION      3 
#define SHOW_STARTSCREEN_OPTION   4
#define INFO_LINE_OPTION          5
#define INFO_LINE_POS_OPTION      6
#define LOW_BAND_OPTION           7

#define BATTERY_ALARM_DEFAULT     1   /* On    */
#define ALARM_LEVEL_DEFAULT       5   /* value 1-8   */
#define BATTERY_TYPE_DEFAULT      0   /* 3s    */
#define BATTERY_CALIB_DEFAULT     128 /* 0 */
#define SHOW_STARTSCREEN_DEFAULT  1   /* Yes   */
#define INFO_LINE_DEFAULT         1   /* On    */
#define INFO_LINE_POS_DEFAULT     0   /* Right */
#define LOW_BAND_DEFAULT          1   /* Yes */

#define MAX_OPTIONS               8

// User Configuration Commands
#define TEST_ALARM_COMMAND        8
#define RESET_SETTINGS_COMMAND    9
#define EXIT_COMMAND              10
#define MAX_COMMANDS              3

// Number of lines in configuration menu
#define MAX_OPTION_LINES          (MAX_OPTIONS + MAX_COMMANDS)

// Minimum time info is shown on screen
#define FORCED_SCREEN_UPDATE_MS   6000

// Alarm timing constants (in milli seconds)
#define ALARM_MAX_ON              50
#define ALARM_MAX_OFF             200
#define ALARM_MED_ON              100
#define ALARM_MED_OFF             1000
#define ALARM_MIN_ON              200
#define ALARM_MIN_OFF             3000

// Digital pin definitions
#define SPI_CLOCK_PIN             2
#define SLAVE_SELECT_PIN          3
#define SPI_DATA_PIN              4
#define BUTTON_PIN                5
#define ALARM_PIN                 6
#define LED_PIN                   13

// Analog pin definitions
#define VOLTAGE_METER_PIN         A1
#define RSSI_PIN                  A6

// Minimum delay between setting a channel and trusting the RSSI values
#define RSSI_STABILITY_DELAY_MS   25

// RSSI threshold for accepting a channel
#define RSSI_TRESHOLD             250

// Channels in use 
#define CHANNEL_MIN               (options[LOW_BAND_OPTION] ? 0 : 8)
#define CHANNEL_MAX               47

// Max and Min frequencies
#define FREQUENCY_MIN             (options[LOW_BAND_OPTION] ? 5345 : 5645)
#define FREQUENCY_MAX             5945

//******************************************************************************
//* Frequency resolutions
#define SCANNING_STEP             (options[LOW_BAND_OPTION] ? 10 : 5)
#define FREQUENCY_DIVIDER         (options[LOW_BAND_OPTION] ? 20 : 10)

//EEPROM addresses
#define EEPROM_CHANNEL            0
#define EEPROM_OPTIONS            1
#define EEPROM_CHECK              (EEPROM_OPTIONS + MAX_OPTIONS)

// click types
#define NO_CLICK                  0
#define SINGLE_CLICK              1
#define DOUBLE_CLICK              2
#define LONG_CLICK                3
#define LONG_LONG_CLICK           4
#define WAKEUP_CLICK              5

// Button pins go low or high on button clicks
#define BUTTON_PRESSED            LOW

// LED state defines
#define LED_OFF                   LOW
#define LED_ON                    HIGH

// Release information
#define VER_DATE_STRING           "2017-03-10"
#define VER_INFO_STRING           "v2.2 by Dvogonen"

#endif // cyclop_plus_osd_h
