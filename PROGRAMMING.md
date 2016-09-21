##Program the CYCLOPS and the MinimOSD Boards
Both PCBs use the same processor and Atmel Atmega 328p.
This is good since the same programming tools can be used. 
To program or "flash" the Arduino "sketch" to the receiver board you must have an ISP programmer for AVR micro controllers.
The original from Atmel is called AVR ISP mkII and is expensive.
The design is open source and copies can be had for around 20$.
But there are several other cheaper alternatives available to choose from.
An USBASP is the cheapest alternative (2-3$).
If you decide to get a USBASP, make sure to select one that can be switched between 3.3-volt and 5-volt output.
The CYCLOPS controller should be flashed using 3.3 volts while the MinimOSD is a 5 volt board.
The voltage is selected by moving a jumper often labled JP1. If there is no jumper on the USBASP, you should not buy it.
![USBASP](/images/usbasp_and_adapter.jpg)

#### Connection
Almost all USBASPs come with a 10 wire flat cable with two 10 pin connectors.
This is unfortunate since almost all boards use 6 pin connectors for the programmer.
This means that you will need an adapter between the 10 pole connector and the 6 pin ISP connector.
These adapters are also available on eBay and cost about as much as the USBASP.

#### Driver Installation
The USBASP driver is sometimes notoriously difficult to get to work on Windows.
USBASPs work without drivers in Linux and OSX.
Thomas Fischler is the original designer of the USBASP and you find the Window driver via his project page: http://www.fischl.de/usbasp/

You might just as well start off by installing a driver installation helper program called Zadig before you attempt to install the USBASP driver.
Be prepared for a bit of a fight and use Google a lot.

#### Flashing Software
The default programming tool from the processor manufacturer AVR is called AVRDUDE (seriously).
This is command line tool that is horrible to use.
There is a GUI version available that is called AVRDUDESS. 
This program is quite nice, so I recommend it instead of the bare bones original.
You find it here: http://blog.zakkemble.co.uk/avrdudess-a-gui-for-avrdude/

##Programming
- Select voltage. 3.3 volts for CYCLOPS, 5 volts for MinimOSB.
- Connect the programmer to the ISP contact. The battery should not be connected.
- Start AVRDUDESS.
- Select your ISP programmer in the Programmer box.
- Select the type of MCU (Atmega 328p).
- Select your firmware file in the Flash box.
- Select "Intel Hex" as format in the Flash box.
- Execute a write.

If everything is set up correctly a LED on the PCB will light up for a while.
When it goes black again the programming is done and the processor is programmed.

