#include <Wire.h>
#define NIXIE_SECOND_DISPLAY_I2C_ADDR 8

const int shiftPin = 9; //SH_CP
const int storePin = 10;//ST_CP
int dataPin = 8; //DS


int led_min_pin = 2;
int led_hour_pin = 3;
int led_on_pin = 4;

int sw_add_pin = 7;
int sw_sub_pin = 5;
int sw_ok_pin = 6;

int val = 0;





unsigned long BCDencode(unsigned long binval) {
  unsigned long bcdval = 0;
  for (int i = 0; binval && i < sizeof(bcdval) * 8; i += 4) {
    bcdval += (binval % 10) << i;
    binval /= 10;
  }
  return bcdval;
}




void update_nixie() {
  unsigned long r = BCDencode(val);
  //Serial.println(c.hour_z);
  int muster[8] = {
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
  for (int i = 0; i < 8 + 1; i++) {
    digitalWrite(shiftPin, LOW);
    digitalWrite(dataPin, muster[i]);
    digitalWrite(shiftPin, HIGH);
  }
  digitalWrite(storePin, HIGH);
}


void setup() {
  Serial.begin(9600);
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

  delay(100);
  update_nixie();
}

void loop() {
  // put your main code here, to run repeatedly:
update_nixie();
delay(30);
}


void receiveEvent(int howMany) {
  if(howMany != sizeof(val)){
    continue;
  }
  val = Wire.read();    // receive byte as an integer
  //update_nixie();
  
}
