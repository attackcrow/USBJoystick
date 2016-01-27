/* USB Suncom SFS Conversion 
   for Teensy 3.1 MCU
   You must select Joystick from the "Tools > USB Type" menu

   Buttons are muxed into rows and columns
   SEL1, 2, 3 (columns) on Pins 17, 18, 19 - these need pull-downs
   Rows on pins 1, 2, 3, 4, 5, 6, 7, 8
   
*/


int filter (int curr, int prev)
{
  return prev + (curr - prev) / 2;
}

// 2-dimensional array of row pin numbers:
const int row[8] = { 0,1,2,3,4,6,7,8 };

// 2-dimensional array of column pin numbers:
// Brown2, Blue, Red
const int col[3] = { 10,5,9 };

int throttle0 = 0;
int throttle1 = 0;

// setup() runs once on boot
void setup() {
  Serial.begin(9600);
  delay(2000);
  Serial.println("SFS Throttle Button analyser");
  delay(1000);
  for (int x = 0; x < 3; x++) {
    pinMode(col[x], OUTPUT);
  } 
  delay(1000);
  for (int x = 0; x < 8; x++) {
    pinMode(row[x], INPUT_PULLUP);  
  }
}


// loop() runs for as long as power is applied
void loop() { 
  int t0, t1;
  digitalWrite(col[0], 0);
  digitalWrite(col[1], 1);       
  digitalWrite(col[2], 1);  
  delay(15);  
  for (int y = 0; y < 8; y++) {    // only 4 buttons on this column
     if (digitalRead(row[y])==0)  {
        Serial.printf("%d,%d\n",y,0);
      }
    }
  
  digitalWrite(col[0], 1);      
  digitalWrite(col[1], 0);       
  digitalWrite(col[2], 1);      
  delay(15);
   for (int y = 0; y < 8; y++) {
     if (digitalRead(row[y])==0)  {
        Serial.printf("%d,%d\n",y,1);
      }
    }

  digitalWrite(col[0], 1);      
  digitalWrite(col[1], 1);       
  digitalWrite(col[2], 0);      
    delay(15);
   for (int y = 0; y < 5; y++) {
     if (digitalRead(row[y])==0)  {
        Serial.printf("%d,%d\n",y,2);
      }
    }
  t0 = filter(analogRead(0), throttle0);
  t1 = filter(analogRead(1), throttle1);
/*
  if (t0 != throttle0)
  {
    Serial.printf("T0 %d\n", t0);
    throttle0 = t0;
  }
  if (t1 != throttle1)
  {
    Serial.printf("T1 %d\n", t1);
    throttle1 = t1;
  } */
  /*
  for (int x = 0; x < 3; x++) {
    for (int y = 0; y < 8; y++) {
     Serial.print("Button ");
     Serial.print(x);
     Serial.print("-");
     Serial.println(y);
     digitalWrite(row[y], HIGH);      
     delay(30);
     if (digitalRead(col[x]))  {
        Serial.println("Peww");
      }
     digitalWrite(row[y], LOW);      
    }
  }
  
  */
}
