#include <Wire.h>
#include <Adafruit_GFX.h>
#include "Adafruit_LEDBackpack.h"

const int OI_INT_PIN = 2; // interrupt pin for OI signal
const int RESET_INT_PIN = 3; // interrupt pin for reset signal


// Display mode selection
//   00 (0) - Unsigned Decimal (Default)
//   01 (1) - Signed Decimal
//   10 (2) - Hex
//   11 (3) - Text
const int MODE_SEL_0 = 13; // display mode select - LSB bit 0
const int MODE_SEL_1 = 12; // display mode select - MSB bit 1

const int BUS_0 = 4; // pin number assigned to BUS_0
const int BUS_1 = 5; // pin number assigned to BUS_1
const int BUS_2 = 6; // pin number assigned to BUS_2
const int BUS_3 = 7; // pin number assigned to BUS_3
const int BUS_4 = 8; // pin number assigned to BUS_4
const int BUS_5 = 9; // pin number assigned to BUS_5
const int BUS_6 = 10; // pin number assigned to BUS_6
const int BUS_7 = 11; // pin number assigned to BUS_7

volatile int OI_FLAG = 0;
volatile int RESET_FLAG = 0;

int DISPLAY_MODE = 0;

byte output;
char buffer[5] = {' ', ' ', ' ', ' ', ' '};
char modeBuffer[5];

Adafruit_AlphaNum4 alpha4 = Adafruit_AlphaNum4();

void setup() {


  pinMode(BUS_0, INPUT);
  pinMode(BUS_1, INPUT);
  pinMode(BUS_2, INPUT);
  pinMode(BUS_3, INPUT);
  pinMode(BUS_4, INPUT);
  pinMode(BUS_5, INPUT);
  pinMode(BUS_6, INPUT);
  pinMode(BUS_7, INPUT);
 
  pinMode(MODE_SEL_0, INPUT_PULLUP); // Mode Select - LSB
  pinMode(MODE_SEL_1, INPUT_PULLUP); // Mode Select - MSB

  pinMode(OI_INT_PIN, INPUT);
  pinMode(RESET_INT_PIN, INPUT);

  // Initialize display mode
  DISPLAY_MODE = (digitalRead(MODE_SEL_1) << 1) | digitalRead(MODE_SEL_0);
  sprintf(modeBuffer, "%4d", DISPLAY_MODE);

  // Clear display buffer
  memset(&buffer[0], ' ', sizeof(buffer));

  /*
  Serial.begin(9600);
  Serial.println("Initializing display...");
  */
  
  alpha4.begin(0x70);  // pass in the address

  // send "RDY" to the display buffer
  alpha4.writeDigitAscii(0, 'R');
  alpha4.writeDigitAscii(1, 'D');
  alpha4.writeDigitAscii(2, 'Y');
  alpha4.writeDigitAscii(3, modeBuffer[3]);
 
  // write it out!
  alpha4.writeDisplay();

  attachInterrupt(digitalPinToInterrupt(OI_INT_PIN), setOIFlag, RISING);
  attachInterrupt(digitalPinToInterrupt(RESET_INT_PIN), setResetFlag, RISING);
 
}


void loop() {

    if (RESET_FLAG) {

      // Clear reset flag
      RESET_FLAG = 0;

      // Re-Initialize display mode
      DISPLAY_MODE = (digitalRead(MODE_SEL_1) << 1) | digitalRead(MODE_SEL_0);
      sprintf(modeBuffer, "%4d", DISPLAY_MODE);

      memset(&buffer[0], ' ', sizeof(buffer));

      // send "RDY1" to the display buffer
      alpha4.writeDigitAscii(0, 'R');
      alpha4.writeDigitAscii(1, 'D');
      alpha4.writeDigitAscii(2, 'Y');
      alpha4.writeDigitAscii(3, modeBuffer[3]);
 
      // write it out!
      alpha4.writeDisplay();
      
    } else if (OI_FLAG) {
      
      OI_FLAG = 0;

      bitWrite(output, 0, digitalRead(BUS_0));
      bitWrite(output, 1, digitalRead(BUS_1));
      bitWrite(output, 2, digitalRead(BUS_2));
      bitWrite(output, 3, digitalRead(BUS_3));
      bitWrite(output, 4, digitalRead(BUS_4));
      bitWrite(output, 5, digitalRead(BUS_5));
      bitWrite(output, 6, digitalRead(BUS_6));
      bitWrite(output, 7, digitalRead(BUS_7));

      switch (DISPLAY_MODE) {

        case 0:
          // format byte to 4-digit unsigned decimal
          sprintf(buffer, "%4d", output);
          break;

        case 1:
          // format byte to 4-digit signed decimal
          sprintf(buffer, "%4d", (signed char) output);
          break;

        case 2:
          // format byte to hex
          sprintf(buffer, "%4X", output);
          break;

        case 3:
          // shift buffer text left and output ascii char
          buffer[0] = buffer[1];
          buffer[1] = buffer[2];
          buffer[2] = buffer[3];
          buffer[3] = output;
          break;
          
      }
      
      // send every digit in the buffer
      alpha4.writeDigitAscii(0, buffer[0]);
      alpha4.writeDigitAscii(1, buffer[1]);
      alpha4.writeDigitAscii(2, buffer[2]);
      alpha4.writeDigitAscii(3, buffer[3]);
      alpha4.writeDisplay();
      
    }
  
}

void setOIFlag() {

  OI_FLAG = 1;
  
}

void setResetFlag() {

  RESET_FLAG = 1;
  
}
