#include <Wire.h>
#define NIXIE_SECOND_DISPLAY_I2C_ADDR 0x42

const int shiftPin = 9; //SH_CP
const int storePin = 10;//ST_CP
int dataPin = 8; //DS


int led_min_pin = 2;
int led_hour_pin = 3;
int led_on_pin = 4;

int sw_add_pin = 7;
int sw_sub_pin = 5;
int sw_ok_pin = 6;

int hours = 0;
int mins = 0;
int secs = 0;

const int shift_digits = 24;


unsigned long BCDencode(unsigned long binval) {
  unsigned long bcdval = 0;
  for (int i = 0; binval && i < sizeof(bcdval) * 8; i += 4) {
    bcdval += (binval % 10) << i;
    binval /= 10;
  }
  return bcdval;
}




void update_nixie() {
  unsigned long r = BCDencode(hours);
  unsigned long s = BCDencode(mins);
  unsigned long t = BCDencode(secs);


  Wire.beginTransmission(NIXIE_SECOND_DISPLAY_I2C_ADDR); // transmit to device #8
  Wire.write((byte)secs);              // sends one byte
  Wire.endTransmission();    // stop transmitting


  //BlINK DOTS
  unsigned long tsdot = 0;
  if ((secs % 2) == 0) {
    tsdot = 1;
  }

  //Serial.println(c.hour_z);
  int muster[shift_digits + 1] = {
    tsdot & 1,
    t & 8,
    t & 4,
    t & 2,
    t & 1,
    t & 128,
    t & 64,
    t & 32,
    t & 16,
    s & 8,
    s & 4,
    s & 2,
    s & 1,
    s & 128,
    s & 64,
    s & 32,
    s & 16,
    r & 8,
    r & 4,
    r & 2,
    r & 1,
    r & 128,
    r & 64,
    r & 32,
    r & 16
  };
  digitalWrite(storePin, LOW);
  for (int i = 0; i < shift_digits + 1; i++) {
    digitalWrite(shiftPin, LOW);
    digitalWrite(dataPin, muster[i]);
    digitalWrite(shiftPin, HIGH);
  }
  digitalWrite(storePin, HIGH);

  Serial.print("_t_");
  Serial.print(hours);
  Serial.print("_");
  Serial.print(mins);
  Serial.print("_");
  Serial.print(secs);
  Serial.println("_");

}


void setup() {

    pinMode(storePin, OUTPUT);
  pinMode(shiftPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  pinMode(sw_add_pin, INPUT);
  pinMode(sw_sub_pin, INPUT);
  pinMode(sw_ok_pin, INPUT);

  pinMode(led_on_pin, OUTPUT);
  digitalWrite(led_on_pin, LOW);
  pinMode(led_min_pin, OUTPUT);
  digitalWrite(led_min_pin, LOW);
  pinMode(led_hour_pin, OUTPUT);
  digitalWrite(led_hour_pin, LOW);
  
  Wire.begin(NIXIE_SECOND_DISPLAY_I2C_ADDR);               
  Wire.onReceive(receiveEvent);
}

void loop() {
  // put your main code here, to run repeatedly:

}


void receiveEvent(int howMany) {
  
  int x = Wire.read();    // receive byte as an integer
  mins = x;
  update_nixie();
}
