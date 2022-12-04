# Microcontroller and I2C LED Output Module for Ben Eater's 8-Bit CPU

![Main project image](https://github.com/The8BitEnthusiast/microcontroller-display/blob/main/Pictures/displaymodule.jpg?raw=true)

This project is a microcontroller and i2c LED version of Ben Eater's 8-bit CPU Output Module. This version of the output module is fully compatible with the stock 8-bit CPU control signals.

## Parts Used for This Project
This project made use of the following components:
- [AdaFruit 4-Digit LED Display with I2C Backpack](https://www.adafruit.com/product/881)
- ATMEGA328P Microcontroller (or just use an Arduino)
- 16Mhz crystal
- 2 X 22pF capacitors
- Dip Switch
- 74LS00 AND Gate

## Overview

This design was aimed at exploring how microcontrollers such as the one used by Arduinos handle interrupts and I2C protocols. My learning goals for this were:

- best practices for interrupt handling on these micro-controllers
- how a controller like the ATMEGA328P can be programmed and deployed as a standalone chip
- how the I2C protocol works and how an LED controller such as the one sold by AdaFruit is internally designed

## Design

The schematics for the module is shown below. The module is designed to leverage the stock output module's control signals, i.e. CLK, OI, RESET as well as the data bus as input. The CLK and OI lines are ANDed so that a positive pulse is generated when both signals are asserted. This feeds into the microcontroller input pin configured for interrupts. The RESET line is also fed into another interrupt input pin of the microcontroller.

![Design Schematics](https://github.com/The8BitEnthusiast/microcontroller-display/blob/main/Schematics/8BitCPUOutputModule.png?raw=true)

From a code perspective, since this is the same microcontroller used on many Arduino models, the program is basically a sketch. See the code folder. A key lesson learned was that interrupt handling routines should be as short as possible. I initially had debug code in there with Serial.println() statements, which turned out to be a bad idea, since that in itself relies on interrupts to transmit. The best practice is to just set a flag in the interrupt routine and then let the Loop() section do the heavylifting.

As for displaying to the LED, Adafruit provides an Arduino library that completely shields you from the intricacies of i2c and the commands sent to the display. For me this was good and bad. Good in that using the library is simple. Bad in that I was half hoping to actually interact at the protocol level. But that's ok, I'll probably test out the Arduino's native i2c libraries at some point.

## Standalone Deployment of the Microcontroller

This project can be implemented with an Arduino. However, I wanted to test out the technique to program a standalone microcontroller chip and by doing that, preserve the "TTL Look and Feel" character of the 8 bit CPU. The procedure to do so is described in this [article](https://docs.arduino.cc/built-in-examples/arduino-isp/ArduinoISP), which also contains great information on bootloaders and the role they play.

## Performance Observations

According to measurements taken on the scope, as shown below, the microcontroller handles the interrupt and starts to transmit to the LED display in about 200 uS. With a transmission rate of 100Khz, the end-to-end sequence fits in a 2 ms timeframe. The controller on the LED display supports transmission rates of up to 400 KHz. It seems that Arduinos can be configured for higher speed on the i2c bus. It would be interesting to test that out.

![Scope Trace](https://github.com/The8BitEnthusiast/microcontroller-display/blob/main/Pictures/scope.png?raw=true)

