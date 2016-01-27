/* USB Suncom SFS Conversion
   for Teensy 3.2 MCU
   You must select Joystick from the "Tools > USB Type" menu

   Throttle Buttons are muxed into rows and columns
   SEL1, 2, 3 (columns) on Pins 10, 5, 9
   Rows on pins 0, 1, 2, 3, 4, 6, 7, 8
   Buttons read LOW on activation, so initial state is HIGH

   Wiring from Pots as follows - Left & Right throttles from Suncom SFS Throttle to A0 & A1
   1:          AGND
   2: (wiper)  A0 & A1
   3:          3.3V

   Button Map:
     Rocker 1-2
     Rocker 3-4
     Rocker 5-6
     Rocker 7-8

     Hat         U  9| D 10| L 11| R 12
     Inverse Hat U 13| D 14| L 15| R 16
     Button 17
     Vertical Rocker U 18 | D 19
     Pinky Switch 20

*/

unsigned int throttle1, throttle2;

// 2-dimensional array of row pin numbers:
const int row[8] = { 0, 1, 2, 3, 4, 6, 7, 8 };

// 2-dimensional array of column pin numbers:
// Brown2, Blue, Red
const int col[3] = { 10, 5, 9 };

int buttonprevstate[24];

const int buttonLookup[8][3] =
{ {  1, 19, 14 },
  {  2, 20, 13 },
  { 11,  8, 0 },
  {  3, 18, 16 },
  {  4, 17, 15 },
  { 12,  7, 0 },
  { 10,  6, 0 },
  {  9,  5, 0 }
};
/*
  const int buttonlookup[8][3] = { {19,20,21,22,23,24,25,26},
                                 {27,28,29,30,,,,},
                                 {31,32

*/

/*
  int filter (int curr, int prev, float tau, float t)
  {
  float coeff = (1.0f / (tau / t));
  return prev + coeff * (curr - prev);
  }
*/

int filter (int curr, int prev)
{
  return prev + (curr - prev) / 2;
}

void setup() {
  for (int x = 0; x < 24; x++)  {
    buttonprevstate[x] = 1;
  }
  for (int x = 0; x < 3; x++) {
    pinMode(col[x], OUTPUT);
  }
  for (int x = 0; x < 8; x++) {
    pinMode(row[x], INPUT_PULLUP);
  }
  Joystick.useManualSend(true);
}


void loop() 
{
  int state;
  int btn;
  digitalWrite(col[0], 0);
  digitalWrite(col[1], 1);
  digitalWrite(col[2], 1);
  delay(15);

  for (int y = 0; y < 8; y++) 
  {
    state = !digitalRead(row[y]);
    btn = buttonLookup[y][0];
    if (state != buttonprevstate[btn]) 
    {
      Joystick.button(btn, state != 0);
      //Serial.printf("%d %d\n", btn, state);
      buttonprevstate[btn] = state;
    }
  }

  digitalWrite(col[0], 1);
  digitalWrite(col[1], 0);
  digitalWrite(col[2], 1);
  delay(15);

  for (int y = 0; y < 8; y++) 
  {
    state = !digitalRead(row[y]);
    btn = buttonLookup[y][1];
    if (state != buttonprevstate[btn]) 
    {
      Joystick.button(btn, state != 0);
      //Serial.printf("%d %d\n", btn, state);
      buttonprevstate[btn] = state;
    }
  }


  digitalWrite(col[0], 1);
  digitalWrite(col[1], 1);
  digitalWrite(col[2], 0);
  delay(15);

  for (int y = 0; y < 8; y++) 
  {
    state = !digitalRead(row[y]);
    btn = buttonLookup[y][2];
    if (state != buttonprevstate[btn]) 
    {
      Joystick.button(btn, state != 0);
      //Serial.printf("%d %d\n", btn, state);
      buttonprevstate[btn] = state;
    }
  }
  throttle1 = filter(analogRead(0), throttle1);
  throttle2 = filter(analogRead(1), throttle2);

  Joystick.sliderLeft(throttle1);
  Joystick.sliderRight(throttle2);

  Joystick.send_now();
}


