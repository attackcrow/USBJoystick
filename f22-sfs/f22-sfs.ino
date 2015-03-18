/* USB Thrustmaster F22 & Suncom SFS Conversion 
   for Teensy 3.1 MCU
   You must select Joystick from the "Tools > USB Type" menu

   Throttle Buttons are muxed into rows and columns
   SEL1, 2, 3 (columns) on Pins 17, 18, 19 - these need pull-downs
   Rows on pins 1, 2, 3, 4, 5, 6, 7, 8 

   Joystick Buttons are muxed into shift registers, use the SPI protocol to read them
   F22 Shift-reg are 3 x HCF4021BE
   Wiring from Handle to SPI as follows
   Brown:  +5v (VIN)   
   Green:  GND
   Orange: SCLK (pin13)
   Red:    SS (pin10)
   Yellow: MISO (pin12)
   
   Wiring from Pots as follows - I have X & Y on A2 & A3. I have Left & Right throttles from Suncom SFS Throttle to A0 & A1
   1:          3.3v
   2: (wiper)  Analog Inputs 
   3:          GND
   
*/

#include <SPI.h>
const int ss = 10;

unsigned int buttonInputs1;   // data read from SPI
unsigned int buttonInputs2;
unsigned int buttonInputs3;
unsigned int pitch, roll, throttle1, throttle2;

// 2-dimensional array of row pin numbers:
const int row[8] = { 1,2,3,4,5,6,7,8 };

// 2-dimensional array of column pin numbers:
const int col[3] = { 18,19,20 };

/*
const int buttonlookup[8][3] = { {19,20,21,22,23,24,25,26},
                                 {27,28,29,30,,,,},
                                 {31,32

*/

#define PINKY  !(buttonInputs1 & 0x80)    /* Pinky Switch */
#define TG1    !(buttonInputs1 & 0x40)    /* Trigger 1 */
#define TG2    !(buttonInputs1 & 0x20)    /* Trigger 2 */
#define S1     !(buttonInputs1 & 0x10)    /* Nose Wheel Steering */
#define PADDLE !(buttonInputs1 & 0x08)    /* Paddle Switch */
#define THUMB  !(buttonInputs1 & 0x04)    /* Pickle */

#define H1D  !(buttonInputs2 & 0x80)    /* HAT */
#define H1R  !(buttonInputs2 & 0x40)
#define H1U  !(buttonInputs2 & 0x20)
#define H1L  !(buttonInputs2 & 0x10)
#define H4U  !(buttonInputs2 & 0x08)    /* Castle */
#define H4L  !(buttonInputs2 & 0x04)
#define H4D  !(buttonInputs2 & 0x02)
#define H4R  !(buttonInputs2 & 0x01)
#define H3D  !(buttonInputs3 & 0x80)    /* Weap */
#define H3R  !(buttonInputs3 & 0x40)
#define H3U  !(buttonInputs3 & 0x20)
#define H3L  !(buttonInputs3 & 0x10)
#define H2D  !(buttonInputs3 & 0x08)    /* Target */
#define H2R  !(buttonInputs3 & 0x04)
#define H2U  !(buttonInputs3 & 0x02)
#define H2L  !(buttonInputs3 & 0x01)

void setup() {
  Serial.begin(9600);
  Serial.println("USB Joystick analyser");
  delay(1000);
  pinMode (ss, OUTPUT);
  SPI.begin();
  for (int x = 0; x < 3; x++) {
    pinMode(col[x], INPUT);
  } 
  for (int x = 0; x < 8; x++) {
    pinMode(row[x], OUTPUT);  
  }
  Joystick.useManualSend(true);
}


void loop() {
  for (int x = 0; x < 3; x++) {
    for (int y = 0; y < 8; y++) {
     Serial.print("Button ");
     Serial.print(x);
     Serial.print("-");
     Serial.println(y);
     digitalWrite(row[y], HIGH);      
     delay(30);
     if (digitalRead(col[x]))  {
       Joystick.button(18+((8*y)+x),);
       
}
     digitalWrite(row[y], LOW);      
    }
  }
  digitalWrite(ss,LOW);
  buttonInputs1 = SPI.transfer(0x00);
  buttonInputs2 = SPI.transfer(0x00);
  buttonInputs3 = SPI.transfer(0x00);

  roll = analogRead(2);
  pitch = analogRead(3);
  throttle1 = analogRead(0);
  throttle2 = analogRead(1);

  // throttle = analogRead(3);
  digitalWrite(ss,HIGH); 
  Joystick.button(1,  TG1);
  Joystick.button(2,  THUMB);
  Joystick.button(3,  PINKY);
  Joystick.button(4,  PADDLE);
  Joystick.button(5,  S1);
  Joystick.button(6,  TG2);
  Joystick.button(7,  H2U);
  Joystick.button(8,  H2R);
  Joystick.button(9,  H2D);
  Joystick.button(10, H2L);
  Joystick.button(11, H3U);
  Joystick.button(12, H3R);
  Joystick.button(13, H3D);
  Joystick.button(14, H3L);
  Joystick.button(15, H4U);
  Joystick.button(16, H4R);
  Joystick.button(17, H4D);
  Joystick.button(18, H4L);

  Joystick.X(roll);
  Joystick.Y(pitch);
  Joystick.sliderLeft(throttle1);
  Joystick.sliderRight(throttle2);
  
  int angle = -1;

  if (H1U) {
    if (H1R) {
      angle = 45;
    } else if (H1L) {
      angle = 315;
    } else {
      angle = 0;
    }
  } else if (H1D) {
    if (H1R) {
      angle = 135;
    } else if (H1L) {
      angle = 225;
    } else {
      angle = 180;
    }
  } else if (H1R) {
    angle = 90;
  } else if (H1L) {
    angle = 270;
  }
  Joystick.hat(angle);
  Joystick.send_now();
}
  
