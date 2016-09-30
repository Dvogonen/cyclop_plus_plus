#CYCLOP++ (UNDER DEVELOPMENT, NOT YET FUNCTIONAL)
The Quanum Cyclops googles are limited to only the most basic functionality.
The limitation is however not dependent on the electronic hardware, which is quite powerful.
By replacing the firmware of the receiver board and adding an inexpensive MinimOSD module, the googles become fully featured. 

####Added Functions
* FPV channels can be manually selected.
* The complete 5.8 GHz band can be scanned for signal strength in order to find unused channels.
* The googles can automatically scan through all channels and let you pick which one to view.
* Channel Frequency, Channel Name, Signal Strength and Battery level are displayed on the googles video screen.
* A low battery warning buzzer warns before the battery power cuts off.

This project was forked from v1.4 of the CYCLOP+ project. The original project uses an extra OLED screen to display information instead of the google screen.

##MinimOSD PCB
The screen information is added by installing a MinimOSD (aka Mavlink OSD).
These modules are commonly used in RC FPV aircraft to add visual information to the FPV video signal.
They are available from multiple sources and are inexpensive.
Avoid buing the Mikro variants. These are very small, which is convenient, but do not have a power regulator.
I have tested using the v1.1 version of the PCB. Which looks like this:
![minimOSD](/images/minimOSD1.1.jpg)

####Mount the Programming Pins
Solder a 2x3 block of pin headers into the holes for the ISP port.
The 6 holes for the pin headers are found between the processor and the MAX7456 video circuit.
The pins should point upwards on the same PCB side as all the other components are mounted.
The bottom right pin is VCC. That is the one below the hole with the square pad.

#### Program the Processor
The firmware on the MinimOSD must be replaced to make it able to talk to the CYCLOPS board.
The firmware file is called minimosd_for_cyclop.hex and can be downloaded via this link: https://raw.githubusercontent.com/Dvogonen/cyclop_plus_plus/master/minimosd_for_cyclop.hex (right-click and download).
Check the format of the downloaded file. Each line must start with a colon character and only contain letters and numbers and look something like this:
:100000000C941F030C9447030C9447030C94470370

If you do not know how to program Atmel controllers there is a guide [here](PROGRAMMING.md)

If you want to build the firmware yourself, you can do so. Here is a link to the [description](BUILDING.md)

##CYCLOPS Receiver PCB
####Mount the Programming Pins
Solder a 2x3 block of pin headers into the holes for the ISP port.
The holes for the ISP connector is found just to the right of the button switch. In the image they are marked as (1).
The pins should point upwards on the same side as the receiver tin can is installed.
![CYCLOPS PCB](/images/cyclops_pcb.jpg)

The top left pin is VCC. That is the one above the hole with the square pad.

####Cut the Video Trace
The trace leading between (2) and (3) carries the video signal. It must be cut since the signal should go via the OSD.
Use a sharp knife and cut across the trace. 
Use a multimeter to check that there is no longer any connection between (2) and (3).

#### Program the Processor
The firmware file is called cyclop_plus_plus.hex and can be downloaded via this link: https://raw.githubusercontent.com/Dvogonen/cyclop_plus_osd/master/cyclop_plus_plus.hex (right-click and download).
Check the format of the downloaded file. Each line must start with a colon character and only contain letters and numbers and look something like this:
:100000000C941F030C9447030C9447030C94470370

If you do not know how to program Atmel controllers there is a guide [here](PROGRAMMING.md)

If you want to build the firmware yourself, you can do so. Here is a link to the [description](BUILDING.md)

##Board Connection
Keep the wires as short as possible to avoid interference on the video and serial lines.
* VIN to 2
* VOUT to 3
* GND to 4
* RX to 6
* TX to 5
* VCC to 7

##Alarm Buzzer
A 5 volt miniature piezzo buzzer is used as a low battery alarm.
Both active buzzers, and passive piezzo speakers can be used.
The later kind is often refered to as "PC Motherboard Speakers".
An example of a piezo buzzer is this:

![Buzzer Example](/images/buzzer.jpg)
####Installation
- Solder the leg marked by a + or attached to a red wire to the solder point marked D6.
- Solder the other (black) leg to a ground point. The legs of the antenna contact as well as the square through hole solder island in the middle of the PCB are ground points.
![Alarm Speaker Connection](/images/pcb_buzzer.jpg)

##End Result
I recommend mounting the minimOSD with double sided mounting tape on top of the radio shielding can. If you do, this how your finished double PCB will look like:
![Finished PCBs](/images/pcb_finished.jpg)
Here are a few images of the intermediate build steps:
[Soldering](SOLDERING.md)

##Configure CYCLOP++
- Hold down the button during power up to enter the system options screen.
- Use single click and double click to navigate in the menu.
- Use long click to select or unselect an option.
- Examples of configurable options: Battery type (2s LiPo, 3s LiPo), information at top or bottom row, video format.
- The settings are saved when the Exit option is selected. All changes are lost if the battery is disconnected before Exit has been selected.

##Use CYCLOP++
- A single click jumps up in frequency to the closest channel among the 40 available.
- A double click jumps down in frequency
- A long click (0.6 - 2 seconds) triggers a autoscan for the best channel, just like a single click does in the original firmware.
- A long-long click (> 2 seconds) triggers a manual frequency scanner. The receiver will start cycling through all channels quickly. Hold down the button again when the channel you want to use flickers onto the main display.

##Words of Warning
Use of the FW is naturally on your own risk.
It is possible to destroy the receiver electronics and the MinimOSB electronics by making faulty connections.
Carefully check that everything is connected correctly and that nothing is shorted.
It is possible to "Brick" the processors by tampering with the so called processor fuses.
There is no need to change any fuses from their original values (0xE2 0xD9 0x07 for the receiver board). Leave them alone.

##Version History
* 1.0 Initial dev version, not released
* 1.1 Functionly complete dev version, not released
* 1.2 Timing optimizations. First released version. 2016-06-20
* 1.3 Configration options added. Screensaver mode added. Battery meter added. 2016-07-15
* 1.4 SH1106 OLED support added. Button timing improved. Low battery alarm added. 2016-08-20
* 2.0 OLED code removed, MinimOSD code added. UNDER DEVOLPMENT, NOT YET RELEASED

##License
The MIT License (MIT)

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.



