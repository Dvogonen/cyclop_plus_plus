#CYCLOP++ - v2.3

The Quanum Cyclops googles are limited to only the most basic functionality.
The limitation is however not dependent on the electronic hardware, which is quite powerful.
By replacing the firmware of the receiver board and adding an inexpensive MinimOSD module, the googles become fully featured. 

####Added Functions
* One or several favourite frequency bands can be selected. Only the frequencies on the favourite bands are used when the user manually steps through the frequencies. 
* There is full 48 channel support. Both for the complete race band and the new low band.
* The complete frequency range can be scanned for signal strength in order to find unused channels.
* The googles can automatically scan through all channels and let you pick which one to view.
* Channel Frequency, Channel Name, Signal Strength and Battery level are displayed on the googles video screen.
* A low battery warning buzzer warns before the battery power cuts off.

##Prepare the MinimOSD PCB
The screen information is added by installing a MinimOSD (aka Mavlink OSD).
These modules are commonly used in RC FPV aircraft to add visual information to the FPV video signal.
They are available from multiple sources and are inexpensive.
You might e.g. use the v1.1 version of the MinimOSD PCB, which looks like this:
![minimOSD](/images/minimOSD1.1.jpg)
The minimOSD board version used in the build steps below is a bit bigger since it has the so called KV-mod. 
Either version will work just fine.

Avoid buing the Mikro MinimOSD variants. These are very small, which is convenient.
What is less convenient is that they do not have a power regulator.
Neither do they have soldering pads for an ISP interface.
It is technically possible to overcome these limitations, but how to do that is not described here.

####Solder the SJ2 bridge 
Some MinimOSD come with the two bridge points marked SJ2 soldered together while either do not.
Bridging SJ2 makes it possible to supply power from the video side of the board to the processor sie of the board.
This is what we do, so SJ2 MUST be bridged.
Without the solder bridge, the minimOSD processor will not work at all.

####Mount the Programming Pins
Solder a 2x3 block of pin headers into the holes for the ISP port.
The 6 holes for the pin headers are found between the processor and the MAX7456 video circuit.
The pins should point upwards on the same PCB side as all the other components are mounted.
The bottom right pin is VCC. That is the one below the hole with the square pad.

#### Program the Processor
The firmware on the MinimOSD must be replaced to make it able to talk to the CYCLOPS board.
The firmware file is called [minimosd_for_cyclop_v0203.hex](https://raw.githubusercontent.com/Dvogonen/cyclop_plus_plus/master/minimosd_for_cyclop_v0203.hex) (right-click and download).
Check the format of the downloaded file. Each line must start with a colon character and only contain letters and numbers and look something like this:
:100000000C941F030C9447030C9447030C94470370

If you do not know how to program Atmel controllers there is a guide [here](PROGRAMMING.md)

If you want to build the firmware yourself, you can do so. Here is a link to the [description](BUILDING.md)

##Prepare the CYCLOPS Receiver PCB
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

####Add the Alarm Buzzer
A 5 volt miniature piezzo buzzer is used as a low battery alarm.
Both active buzzers, and passive piezzo speakers can be used.
The later kind is often refered to as "PC Motherboard Speakers".
An example of a piezo buzzer is this:

![Buzzer Example](/images/buzzer.jpg)

- Solder the leg marked by a + or attached to a red wire to the solder point marked D6.
- Solder the other (black) leg to a ground point. The legs of the antenna contact as well as the square through hole solder island in the middle of the PCB are ground points.
![Alarm Speaker Connection](/images/pcb_buzzer.jpg)

#### Program the Processor
The firmware file is called [cyclop_plus_plus_v0203.hex](https://raw.githubusercontent.com/Dvogonen/cyclop_plus_osd/master/cyclop_plus_plus_v0203.hex) (right-click and download).
Check the format of the downloaded file. Each line must start with a colon character and only contain letters and numbers and look something like this:
:100000000C941F030C9447030C9447030C94470370

If you do not know how to program Atmel controllers there is a guide [here](PROGRAMMING.md)

If you want to build the firmware yourself, you can do so. Here is a link to the [description](BUILDING.md)

##Connect the two boards
Keep the wires as short as possible to avoid interference on the video and serial lines.
* VIN to 2
* VOUT to 3
* GND to 4
* RX to 6
* TX to 5
* VCC to 7

##End Result
I recommend mounting the minimOSD with double sided mounting tape on top of the radio shielding can. If you do, this how your finished double PCB will look like:
![Finished PCBs](/images/pcb_finished.jpg)
Here are a few images of the intermediate build steps:
[Soldering](SOLDERING.md)

##Use CYCLOP++
- A single click jumps up in frequency to the closest higher channel among the 48 available.
- A double click jumps down in frequency.
- A long click (longer than 0.5 seconds) brings up a menu.
- In menues: A short click increments or moves forward. A double click decrements or moves backward. A long click executes functions or is used to enter/depart.
- Use the menu to start the Graphical Scanner, the Auto Scanner or enter into the Options Menu.  
- Auto Scanner: Performs an autoscan for the best channel, just like a single click does in the original firmware.
- Graphical Scanner: Triggers a manual frequency scanner. The receiver will start cycling through all channels quickly. Click the button again to select a frequency.

##Options Menu
- Examples of configurable options: Battery type, screen saver, low level battery alarm, alarm sound level, information display characteristics.
- It is possible to turn the use of individual bands On or Off. If a band is turned Off it will not be available for manual stepping. The idea is to be able to limit frequency stepping to the band you are using and ignore all other frequencies. All frequencies are however available for both Grahical Scanning and Auto Scanning. The exception to this rule is the Low Band. This band takes up as much bandwidth as all the others combined. If the Low Band is turned Off, the scan functions for it is also turned Off. The reason is that this doubles the resolution of frequency scans. 
- The settings are saved when the Exit option is selected. All changes are lost if the battery is disconnected before Exit has been selected.

##Words of Warning
Use of the FW is naturally on your own risk.
It is possible to destroy the receiver electronics and the MinimOSB electronics by making faulty connections.
Carefully check that everything is connected correctly and that nothing is shorted.
It is possible to "Brick" the processors by tampering with the so called processor fuses.
There is no need to change any fuses from their original values (0xE2 0xD9 0x07 for the receiver board). Leave them alone.

##Version History
* 2.3 Favourite bands selection added. Button timing improved. 2017-03-14
* 2.2 LowBand support added. Alarm level configuration added. 2017-03-10
* 2.1 Battery voltage bug solved. First CYCLOP++ stable release. 2016-10-13
* 2.0 Branched off from CYCLOP+. OLED code removed, MinimOSD code added. Beta1 2016-10-03
* 1.4 (CYCLOP+)SH1106 OLED support added. Button timing improved. Low battery alarm added. 2016-08-20
* 1.3 (CYCLOP+)Configration options added. Screensaver mode added. Battery meter added. 2016-07-15
* 1.2 (CYCLOP+)Timing optimizations. First released version. 2016-06-20
* 1.1 (CYCLOP+)Functionly complete dev version, not released
* 1.0 (CYCLOP+)Initial dev version, not released

##License
This project was forked from v1.4 of the CYCLOP+ project. The original project uses an extra OLED screen to display information instead of the google screen.

The MIT License (MIT)

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.



