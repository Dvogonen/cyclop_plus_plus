#CYCLOP++ with OSD v2.0

###Introduction and Functions
This is the OSD variant of CYCLOP++, the upgrade firmware for the Quanum Cyclops googles.
* The major functionality of this project (CYCLOP++ with OSD) is to make it possible manually select channels using only the standard button on the receiver of the Quanum Cyclops.
* The second major function is to add support for displaying channel and battery information to the video signal by adding a minimOSD board.
* The third is to add support for a low battery buzzer.

This project was forked from v1.4 of the CYCLOP+ project. The original project uses an extra OLED screen to display information instead of the google screen.

###Version History
* 1.0 Initial dev version, not released
* 1.1 Functionly complete dev version, not released
* 1.2 Timing optimizations. First released version. 2016-06-20
* 1.3 Configration options added. Screensaver mode added. Battery meter added. 2016-07-15
* 1.4 SH1106 OLED support added. Button timing improved. Low battery alarm added. 2016-08-20
* 2.0 OLED code removed, Max7456 code added. UNDER DEVOLPMENT, NOT YET RELEASED


#Installation
###Solder programming pins to the PCB
- Solder a 2x3 block of pin headers into the holes for the ICSP port.
If you do not want to solder anything you can program the board by temporarily pushing programming pins into the PCB ICSP connector holes while you program the board.
The holes for the ICSP connector is found just to the right of the button switch.
The pins should point upwards on the same side as the receiver tin can is installed.
![ICSP pin header](/images/pcb_icsp.jpg)
The top left pin is VCC. 
 
###Program the MinimOSD board
These modules are commonly used in RC FPV aircraft to add visual information to the FPV video signal. They are available from multiple sources and are very inexpensive.
In order to use a MinimOSD in the Cyclops googles, the standard firmware has to be replaced by custom firmware for CYCLOP++ .
--- INSERT PROGRAM INFO HERE ---

###Install the MinimOSD board
If you use a Mikro variant of the board, you also need to install a small converter from battery voltage to 5 volts.
I recommend using the original minimOSD variant, since it has a onboard voltage converter and can use battery power directly.
Nothing except video in, video out, ground and battery VCC needs to be connected, which makes this an easy modification.

##Attach an alarm buzzer
This modification is intended to use a 5 volt miniature piezzo buzzer as a low battery alarm. Both active buzzers, and passive piezzo speakers can be used. The later kind is often refered to as "PC Motherboard Speakers".
An example of a piezo buzzer is this:
![Buzzer Example](/images/buzzer.jpg)

- Solder the red wire comming from your buzzer to the solder point marked D6. If your buzzer does not have a red wire instead solder the leg marked with a + to the D6 solder pad.
- Solder the other (black) wire to a ground point. The legs of the antenna contact as well as the square through hole solder island in the middle of the PCB are ground points.
- Enable the Low Battery Alarm option in the configuration menu.
![Alarm Speaker Connection](/images/pcb_buzzer.jpg)

###Build CYCLOP++ (optional)
- The project is built using the Arduino development environment. Download the Arduino development environment from www.arduino.cc.
- Install the development environment.
- Download the CYCLOP++ source code from GitHub.
- Navigate to the cyclop_plus.ino file and open it in the Arduino development environment.
- Download the two external LCD libraries (Adafruit GFX and Adafruit SSD1306). This is done within the Arduino environment.
- Select display type by editing Adafruit_SSD1306.h. Look at line 42 in the file.
- Specify "Arduino Pro or Pro Mini" as board. Then select "Atmega 328 (3.3 volt, 8 MHz)" as processor. These settings are found in the "Tool" menu.
- Build the project by pressing the v icon in the upper left corner of the Arduino window.

###Load CYCLOP++
- Build or download the latest stable version of CYCLOP++.
The firmware file is called cyclop_plus_plus.hex and can be downloaded via this link: https://raw.githubusercontent.com/Dvogonen/cyclop_plus_osd/master/cyclop_plus_plus.hex (right-click and download)

Check the format of the downloaded file. Each line should start with a colon character and only contain letters and numbers like this:

:100000000C941F030C9447030C9447030C94470370

- To upload the sketch to the receiver board you must have an ISP programmer for AVR micro controllers.
The original is called AVR ISP mkII and is expensive.
The design is open source and copies can be had for around 20$.
But there are several other cheaper alternatives available to choose from.
An USBASP is the cheapest alternative (2-3$).
If you decide to get a USBASP, make sure to select one that can be switched between 3.3-volt and 5-volt output.
This is done by moving a jumper often labled JP1. If there is no jumper on the USBASP, you should not buy it.
![USBASP](/images/usbasp_and_adapter.jpg)
Almost all USBASPs come with a 10 wire flat cable with two 10 pin connectors.
This means that you will need an adapter between the 10 pole connector and the 6 pin ISCP connector.
These adapters are also available on eBay and cost about as much as the USBASP.
- Install the drivers for the programmer.
These are sometimes difficult to get to work.
Be prepared for a bit of a fight and use google a lot.
Google is your friend.
For the USBASP driver you may have to use a helper program called Zadig to install the driver.
- Set the programmer to 3.3 volts.
- Connect the programmer to the ICSP contact on the receiver board. Neither the display nor the battery should be connected simultaneously.
- The default programming tool from the processor manufacturer AVR is called AVRDUDE (seriously).
This is command line tool and is horrible to use.
There is a GUI version available that is called AVRDUDESS. This program is quite nice, so I recommend it instead of the bare bones original.
You find it here: http://blog.zakkemble.co.uk/avrdudess-a-gui-for-avrdude/
- Start AVRDUDESS.
Select your ISP programmer in the Programmer box.
Select the type of MCU (Atmega 328p).
Select your CYCLOP++ firmware file in the Flash box.
Select "Intel Hex" as format in the Flash box.
Execute a write.
- If everything is set up correctly the LED on the receiver board next to the ISP pin header will light up for a minute or so.
When it goes black again the programming is done and the board can be mounted in the googles.

###Configure CYCLOP++
- Hold down the button during power up to enter the system options screen.
- Use single click and double click to navigate in the menu.
- Use long click to select or unselect an option.
- Examples of configurable options: 3s battery meter, 2s battery meter, screen saver, low level battery alarm, information at top or bottom, video format.
- The settings are saved when the Exit option is selected. All changes are lost if the battery is disconnected before Exit has been selected.

###Use CYCLOP++
- A single click jumps up in frequency to the closest channel among the 40 available.
- A double click jumps down in frequency
- A long click (0.6 - 2 seconds) triggers a autoscan for the best channel, just like a single click does in the original firmware.
- A long-long click (> 2 seconds) triggers a manual frequency scanner. The receiver will start cycling through all channels quickly. Hold down the button again when the channel you want to use flickers onto the main display.

###Words of Warning
Use of the FW is naturally on your own risk.
You have to dismantle the googles to program the receiver with a so called ISP (alternatively called ICSP) programmer. 
It is possible to destroy the receiver electronics by making faulty connections.
It is possible to "Brick" the processor in your receiver by tampering with the so called processor fuses. There is no need to change any fuses from their original values (0xE2 0xD9 0x07). Leave them alone.

###License
The MIT License (MIT)

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
