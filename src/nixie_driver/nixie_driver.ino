// Arduino-Pin verbunden mit SH_CP des 74HC595
int shiftPin = 9;
// Arduino-Pin erbunden mit ST_CP des 74HC595
int storePin = 10;
// Arduino-Pin verbunden mit DS des 74HC595
int dataPin = 8;

#define NIXIE_UPDATE_INTERVAL_SECONDS 20
int led_min_pin = 2;
int led_hour_pin = 3;
int led_on_pin = 4;

int sw_add_pin = 7;
int sw_sub_pin = 5;
int sw_ok_pin = 6;

int random_timer = 0;
int mode = 0; //0 =min 1==hour 2=on

#include <Wire.h>

#include "RTClib.h"


RTC_DS1307 rtc;
int hours = 0;
int mins = 0;
int secs = 0;
unsigned long time;

unsigned long BCDencode(unsigned long binval) {
  unsigned long bcdval = 0;
  for (int i = 0; binval && i < sizeof(bcdval) * 8; i += 4) {
    bcdval += (binval % 10) << i;
    binval /= 10;
  }
  return bcdval;
}

String readString;
String getValue(String data, char separator, int index) {
  int found = 0;
  int strIndex[] = {
    0,
    -1
  };
  int maxIndex = data.length() - 1;
  for (int i = 0; i <= maxIndex && found <= index; i++) {
    if (data.charAt(i) == separator || i == maxIndex) {
      found++;
      strIndex[0] = strIndex[1] + 1;
      strIndex[1] = (i == maxIndex) ? i + 1 : i;
    }
  }
  return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}

void setup() {

  Serial.begin(9600);
  delay(5000);
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
  mode = 2;

  random_timer = 0;

  if (!rtc.begin()) {
    for (int i = 0; i < 4; i++) {
      delay(100);
      digitalWrite(led_hour_pin, HIGH);
      delay(100);
      digitalWrite(led_hour_pin, LOW);
    }
    Serial.println("_rct_begin_error_");
  }

  if (!rtc.isrunning()) {
    for (int i = 0; i < 4; i++) {
      delay(100);
      digitalWrite(led_min_pin, HIGH);
      delay(100);
      digitalWrite(led_min_pin, LOW);
    }
    //  Serial.println("RTC is NOT running!");
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    Serial.println("_rct_isrunning_error_");
  }

  delay(100);
  DateTime now = rtc.now();
  hours = now.hour();
  mins = now.minute();
  secs = now.second();
  time = millis();
  delay(100);
  anti_burn_out();
}

const int shift_digits = 24;
void update_nixie() {
  unsigned long r = BCDencode(hours);
  unsigned long s = BCDencode(mins);
  unsigned long t = BCDencode(secs);

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

void update_rtc() {
  rtc.adjust(DateTime(2000, 1, 1, hours, mins, 0));
  Serial.print("_t_");
  Serial.print(hours);
  Serial.print("_");
  Serial.print(mins);
  Serial.print("_");
  Serial.print(secs);
  Serial.println("_");

}

void anti_burn_out() {
  Serial.println("_abi_begin_");

  for (int j = 0; j < 4; j++) {
    for (int i = 0; i < 10; i++) {
      int muster[shift_digits] = {
        i & 8,
        i & 4,
        i & 2,
        i & 1,
        i & 8,
        i & 4,
        i & 2,
        i & 1,
        i & 8,
        i & 4,
        i & 2,
        i & 1,
        i & 8,
        i & 4,
        i & 2,
        i & 1,
        i & 8,
        i & 4,
        i & 2,
        i & 1,
        i & 8,
        i & 4,
        i & 2,
        i & 1
      };
      digitalWrite(storePin, LOW);
      for (int i = 0; i < shift_digits; i++) {
        digitalWrite(shiftPin, LOW);
        digitalWrite(dataPin, muster[i]);
        digitalWrite(shiftPin, HIGH);
      }
      digitalWrite(storePin, HIGH);
      delay(1000);
    }
    update_nixie();
  }
  Serial.println("_abi_finished_");
}

void loop() {
    // Hier passiert nichts.
    DateTime now = rtc.now();
    hours = now.hour();
    mins = now.minute();
    secs = now.second();
 

  while (Serial.available()) {
    delay(30); //delay to allow buffer to fill
    if (Serial.available() > 0) {
      char c = Serial.read(); //gets one byte from serial buffer
      readString += c; //makes the string readString
      if (c == '\n') {
        Serial.flush();
        break;
      }
    }
  }

if (readString.length() > 0) {

  if (getValue(readString, '_', 1) == "st") {

    int tmp_hours = getValue(readString, '_', 2).toInt();
    int tmp_mins = getValue(readString, '_', 3).toInt();

    if (tmp_hours > 0 && tmp_hours < 24 && tmp_mins > 0 && tmp_mins < 60) {
      hours = tmp_hours;
      mins = tmp_mins;
      update_rtc();
    } else {
      Serial.println("_error_params.out.of.range_");
    }

  } else if (getValue(readString, '_', 1) == "abi") {
    anti_burn_out();
  }
  readString = "";
}

if (digitalRead(sw_ok_pin) == LOW) {
  delay(50);
  while (digitalRead(sw_ok_pin) == LOW) {
    delay(50);
  }
  Serial.println("_btn_ok_");

}

//TODO ADD SUB
if (digitalRead(sw_add_pin) == LOW) {
  delay(300);
  mins++;
  if (mins > 59) {
    mins = 0;
  }
  update_rtc();
  update_nixie();
  return;
}

if (digitalRead(sw_sub_pin) == LOW) {
  delay(300);
  hours++;
  if (hours > 23) {
    hours = 0;
  }
  update_rtc();
  update_nixie();
  return;
}

if (digitalRead(sw_sub_pin) == LOW && mode == 1) {
  delay(300);
  hours--;
  if (hours < 0) {
    hours = 23;
  }
  update_rtc();
  update_nixie();
  return;
}

//UPDATE THE NIXIE REGIERS EVERY X SEOCND
if ((millis() - time) > (1000 * NIXIE_UPDATE_INTERVAL_SECONDS)) {
  time = millis();
  update_nixie();
  random_timer++;
  if (random_timer > (60 * 10)) {
    random_timer = 0;
    anti_burn_out();
  }
}

//EINBRENNEN VERHINDERN

}
