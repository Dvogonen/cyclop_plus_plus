#Build CYCLOP++
- The project is built using the Arduino development environment.
It was originally built on Arduino IDE 1.6.9. The latest version it has been built on is Arduino IDE 1.6.12. Download the Arduino development environment from www.arduino.cc.
- Install the development environment.
- Download the CYCLOP++ source code from GitHub (https://github.com/Dvogonen/cyclop_plus_plus/archive/master.zip)
- Install the EnableUnterrupt Arduino library. This is done from within the library manager of the Arduino IDE. 
- Unzip the code package and navigate to the cyclop_plus_plus.ino file. Open it in the Arduino development environment.
- Specify "Arduino Pro or Pro Mini" as board. Then select "Atmega 328 (3.3 volt, 8 MHz)" as processor. These settings are found in the "Tool" menu.
- Build the project by pressing the v icon in the upper left corner of the Arduino window.

#Build MinimOSD For CYCLOPS
- The project is built using the same Arduino development environment as used for CYCLOP++.
- The source code is included in the CYCLOP++ source code you downloaded earlier from GitHub.
- Navigate to the minimosd_for_cyclop.ino file and open it in the Arduino development environment.
- Specify "Arduino Pro or Pro Mini" as board. Then select "Atmega 328 (5 volt,  16 MHz)" as processor. These settings are found in the "Tool" menu.
- Build the project by pressing the v icon in the upper left corner of theArduino window.
