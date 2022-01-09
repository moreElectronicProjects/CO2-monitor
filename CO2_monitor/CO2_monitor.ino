// Include the needed libraries
#include <Wire.h>
#include "SparkFun_SCD4x_Arduino_Library.h"

/*
 * Define the pins which each of the digits on
 * the 7 segment display is connected to.
 *
 * Each segment of the display is hard coded to
 * be connected to PORT D on the ATMega 328p.
 *
 * Which segment is connected to which pin on the
 * Arduino UNO is as follows:
 *
 * A = pin 2
 * B = pin 3
 * C = pin 6
 * D = pin 5
 * E = pin 4
 * F = pin 1
 * G = pin 0
 * pin 7 is not connected but is defined as an output.
 */
#define DIGIT_1 8                                                                                                                                                                                                                       
#define DIGIT_2 9
#define DIGIT_3 10
#define DIGIT_4 11

// Define how long (in ms) to display a specific digit on the display
#define DISPLAY_DELAY 1

/*
 * Define the altitude (in meters) which the sensor will operate at.
 * This is to get a more accurate reading.  This value is hard coded
 * as the device is only planned to be operated in one location so
 * this is the easier option.
 *
 * Change if the device is going to be used at a different altitude.
 */
#define ALTITUDE_OFFSET 91

/*
 * Function to write the passed in number to the segments.
 * Any number not in the range of 0 - 9 inclusive has all
 * segments turned off.
 */
void write_digit(const uint8_t value_to_write)
{
    switch(value_to_write) {
        case 0:      //CDEBAFG
            PORTD = 0b01111110;
            break;
        case 1:
            PORTD = 0b01001000;
            break;
        case 2:
            PORTD = 0b00111101;
            break;
        case 3:
            PORTD = 0b01101101;
            break;
        case 4:
            PORTD = 0b01001011;
            break;
        case 5:
            PORTD = 0b01100111;
            break;
        case 6:
            PORTD = 0b01110111;
            break;
        case 7:
            PORTD = 0b01001100;
            break;
        case 8:
            PORTD = 0b01111111;
            break;
        case 9:
            PORTD = 0b01101111;
            break;
        default:
            PORTD = 0b00000000;
            break;
    }
}

/*
 * Function which takes in a number between 0 - 9999 inclusive
 * and write the number to the 7 segment display.  In the case
 * where the number is less than 1000, only light up the digits
 * which are needed.
 */
void write_number(const int number)
{
    // Variable to hold whether or not a non zero digit has been found.
    uint8_t number_found = 0;

    // Digit 1

    // Calculate the digit to write.
    uint8_t value_to_write = (number/1000)%10;

    // Check to see if the digit to write is zero.
    // If so, set the value to write to 255.  This is to not light up the digit.
    // Else, specify the number has been found.
    if(value_to_write == 0)
        value_to_write = 0xff;
    else
        number_found = 1;

    // Write the digit to the display
    digitalWrite(DIGIT_1, LOW);
    write_digit(value_to_write);
    delay(DISPLAY_DELAY);
    write_digit(0xff);
    digitalWrite(DIGIT_1, HIGH);

    // Digit 2

    // Repeat the above steps for the reaming digits.
    // The only additional check is if a previous digit has been displayed.
    value_to_write = (number/100)%10;
    if(value_to_write == 0 && number_found == 0)
        value_to_write = 0xff;
    else
        number_found = 1;

    digitalWrite(DIGIT_2, LOW);
    write_digit(value_to_write);
    delay(DISPLAY_DELAY);
    write_digit(0xff);
    digitalWrite(DIGIT_2, HIGH);

    // Digit 3
    value_to_write = (number/10)%10;
    if(value_to_write == 0 && number_found == 0)
        value_to_write = 0xff;

    digitalWrite(DIGIT_3, LOW);
    write_digit(value_to_write);
    delay(DISPLAY_DELAY);
    write_digit(0xff);
    digitalWrite(DIGIT_3, HIGH);

    // Digit 4
    value_to_write = number%10;
    digitalWrite(DIGIT_4, LOW);
    write_digit(value_to_write);
    delay(DISPLAY_DELAY);
    write_digit(0xff);
    digitalWrite(DIGIT_4, HIGH);
}

// Declare an instance of the SCD4x sensor
SCD4x CO2_sensor;

// Declare a variable to hold the CO2 reading
int CO2_value = 0;

void setup()
{
    // Declare all of the pins on PORT D as output
    DDRD = 0xff;

    // Declare the segment pins as output and set them to a HIGH state.
    pinMode(DIGIT_1, OUTPUT);
    digitalWrite(DIGIT_1, HIGH);
    pinMode(DIGIT_2, OUTPUT);
    digitalWrite(DIGIT_2, HIGH);
    pinMode(DIGIT_3, OUTPUT);
    digitalWrite(DIGIT_3, HIGH);
    pinMode(DIGIT_4, OUTPUT);
    digitalWrite(DIGIT_4, HIGH);

    // Start the I2C protocol
    Wire.begin();

    // Attempt to start the SCD4x sensor.
    // If the sensor fails to start, continuously blink an error code of 8888.
    // The program does not continue as there is no reason to with a non-initialized sensor.
    if(CO2_sensor.begin() == false) {
        while(1) {
            long current_millis = millis();
            while(current_millis+250 > millis())
                write_number(8888);
            current_millis = millis();
            // Since the display is off after each call to write_number, simply wait the needed time.
            while(current_millis+250 > millis()) {}
        }
    }

    // Attempt to stop the periodic measurements in preparation for the altitude offset.
    // If the sensor fails to stop, continuously blink an error code of 7878.
    // The program does not continue as the is no reason to with a non-responding sensor.
    if(CO2_sensor.stopPeriodicMeasurement() == false)
    {
        while(1) {
            long current_millis = millis();
            while(current_millis+250 > millis())
                write_number(7878);
            current_millis = millis();
            while(current_millis+250 > millis()) {}
        }
    }

    // Set the altitude offset.
    CO2_sensor.setSensorAltitude(ALTITUDE_OFFSET);

    // Check to see if the offset took.  If it didn't, blink and error code of 7777 10 times.
    // The program continues as the sensor is working but might not have accurate data.
    if(CO2_sensor.getSensorAltitude() != ALTITUDE_OFFSET) {
        for(int i = 0; i < 10; ++i) {
            long current_millis = millis();
            while(current_millis+250 > millis())
                write_number(7777);
            current_millis = millis();
            while(current_millis+250 > millis()) {}
        }
    }

    // Attempt to start the periodic measurements.
    // If the sensor fails to start, continuously blink an error code of 8787.
    // The program does not continue as there is no reason to with a not running sensor.
    if(CO2_sensor.startPeriodicMeasurement() == false) {
        while(1) {
            long current_millis = millis();
            while(current_millis+250 > millis())
                write_number(8787);
            current_millis = millis();
            while(current_millis+250 > millis()) {}
        }
    }
}

// Main loop
void loop()
{
    // Check to see if the CO2 sensor has new readings.
    // If so, get and store the reading.
    if(CO2_sensor.readMeasurement()) {
        CO2_value = CO2_sensor.getCO2();
    }
    // Display the reading to the 7 segment display.
    write_number(CO2_value);
}
