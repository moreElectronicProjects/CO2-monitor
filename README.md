# CO2 Monitor

This device reads the CO2 value from the sensor and displays the parts per million reading on the seven segment display.

# Hardware

The following hardware is required.

- Sensirion SCD-41 CO2 sensor
- Perfboard
- Arduino Nano
- 4 digit, 7 segment display
- 7 220 ohm resistors

The following are the required connections.

- Arduino pin 0 &#8594; 220 ohm resistor &#8594; pin G on the display
- Arduino pin 1 &#8594; 220 ohm resistor &#8594; pin F on the display
- Arduino pin 2 &#8594; 220 ohm resistor &#8594; pin A on the display
- Arduino pin 3 &#8594; 220 ohm resistor &#8594; pin B on the display
- Arduino pin 4 &#8594; 220 ohm resistor &#8594; pin E on the display
- Arduino pin 5 &#8594; 220 ohm resistor &#8594; pin D on the display
- Arduino pin 6 &#8594; 220 ohm resistor &#8594; pin C on the display
- Arduino pin A4 &#8594; SDA on CO2 sensor
- Arduino pin A5 &#8594; SCL on CO2 sensor
- Arduino +5V &#8594; VDD on the CO2 sensor
- Arduino GND &#8594; GND on the CO2 sensor and the display

# Software

The software is written using the Arduino IDE. The Arduino IDE is needed to compile and program the board.

## Libraries

The following libraries are required for the code to work.

- Wire.h (Built into the Arduino IDE)
- SparkFun_SCD4x_Arduino_Library.h (Can be downloaded from SparkFun)

## Configuration Options

There are two configurable options in the program.

The first is the altitude of the device. Depending on the altitude, the CO2 concentration can change. To get a more accurate reading, adjust line 40 of the code to the altitude (in meters) the device will be operated at.

The second configuration is how long (in ms) each number is displayed on the seven segment display. This setting is on line 30 of the program. The longer the delay, the higher chance for flickering to be noticed on the display.

# Using the Device

After the Arduino Nano has been programmed, and the board wired up, applying power to the Arduino will start the code. After the initialization, the values displayed on the seven segment display is the parts parts million reading. This value is update approximately every five seconds. The device keeps updating the display until power is removed.

## Error Codes

During the initialization, there is a possibility for error codes to be displayed on the display. An error code is noted as a flashing number. The following table explains the error code, what it means, and what happens.

| Error Code | What Caused the Error Code | What happens |
| --- | --- | --- |
| 8888 | Failed to initialize the CO2 sensor | The device will continually flash the error code and will not continue |
| 7878 | Failed to stop the periodic measurement in order to set the altitude | The device will continually flash the error code and will not continue |
| 7777 | Failed to set the altitude | The device will flash the error code ten times then continue running. It is possible the reading could be incorrect. |
| 8787 | Failed to start the periodic measurement after setting the altitude | The device will continually flash the error code and will not continue |
