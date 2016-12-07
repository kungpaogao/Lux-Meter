# Lux-Meter
An Arduino-based lux meter that uses a BH1750 as well as a data logging shield.
Built by the Algonquin Science Research Club (SRC) for solar panel research.

## Features
- Utilises BH1750 sensor to get lux readouts
- Logs and saves data to .CSV files on an SD card
- Easily customizable log and sync intervals

## Hardware
- Arduino Uno R3 (+ USB cable)
- BH1750 light sensor
- Data logging shield (w/ RTC-DS1307)
- Breadboard and wires
- SD card (formatted to FAT16 or FAT32)

## Software
- [Arduino IDE](https://www.arduino.cc/en/Main/Software)
- [Modified Print.h file](http://playground.arduino.cc/Main/Printf)
  - might create batch file to install modified Print.h file
- [BH1750 libraries](https://github.com/claws/BH1750)
- [RTC libraries](https://github.com/adafruit/RTClib)
- Other libraries should be installed by default

## Known Issues
- Sometimes more than one logger file (usually 2 or 3) will be created
  - This can sometimes be fixed by setting WAIT_TO_START to 1
- Currently, this code only allows the creation of 100 log files (00-99)
  - This can easily be fixed 

## References and Notes
The inspiration for this project came from this [video](https://www.youtube.com/watch?v=XhUUKY8xm2A), and the initial code to test the BH1750 was taken from [here](http://www.homautomation.org/2014/06/15/measure-light-with-arduino-and-bh1750-module/).

This code is more or less an adaptation of Adafruit's data logger [sample code](https://github.com/adafruit/Light-and-Temp-logger/blob/master/lighttemplogger.ino) from their [amazing walkthrough](https://learn.adafruit.com/adafruit-data-logger-shield/overview) of the data logging shield. If you want to modify this code to work for your particular setup, you should definitely check out the tutorial.
