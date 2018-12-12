// Arduino-Pin verbunden mit SH_CP des 74HC595
int shiftPin = 9;
// Arduino-Pin erbunden mit ST_CP des 74HC595
int storePin = 10;
// Arduino-Pin verbunden mit DS des 74HC595
int dataPin = 8;


int led_min_pin = 2;
int led_hour_pin = 3;
int led_on_pin = 4;


int sw_add_pin = 7;
int sw_sub_pin = 5;
int sw_ok_pin = 6;


int random_timer = 0;
int mode = 0;//0 =min 1==hour 2=on

#include <Wire.h>
#include "RTClib.h"



RTC_DS1307 rtc;
int hours = 0;
int mins = 0;
int secs = 0;
unsigned long time;


unsigned long BCDencode (unsigned long binval) {
unsigned long bcdval = 0;
for (int i=0; binval && i < sizeof(bcdval) * 8; i+=4) {
  bcdval += (binval % 10) << i;
  binval /= 10;
}
return bcdval;
}


void update_mode_led(){
  digitalWrite(led_hour_pin, LOW);
digitalWrite(led_min_pin, LOW);
digitalWrite(led_on_pin, LOW);
if(mode == 0){
  digitalWrite(led_min_pin, HIGH);
}else if(mode == 1){
  digitalWrite(led_hour_pin, HIGH);
}if(mode == 2){
  digitalWrite(led_on_pin, HIGH);
}else{
  // digitalWrite(led_on_pin, HIGH);
 //  mode= 2;
  }
  }



#define TWI_ADDR_SOUND_MODULE 0x20
#define SOUND_MODULE_SOUND_ID 6
  void play_sound(byte _id = SOUND_MODULE_SOUND_ID){
    Wire.begin();
   Wire.beginTransmission(TWI_ADDR_SOUND_MODULE); // transmit to device #8
  Wire.write(_id);              // sends one byte
  Wire.endTransmission();    // stop transmitting  
  }


    
void setup() {


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

update_mode_led();
play_sound(20);
 
  
if (!rtc.begin()) {
    for(int i = 0;i< 4;i++){
    delay(100);
    digitalWrite(led_hour_pin, LOW);
    delay(100);
    digitalWrite(led_hour_pin, LOW);
    }
  }


   if (!rtc.isrunning()) {
        for(int i = 0;i< 4;i++){
    delay(100);
    digitalWrite(led_hour_pin, LOW);
    delay(100);
    digitalWrite(led_hour_pin, LOW);
    }
  //  Serial.println("RTC is NOT running!");
     rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));

for(int i = 0;i< 7;i++){
    delay(100);
    digitalWrite(led_hour_pin, LOW);
    delay(100);
    digitalWrite(led_hour_pin, LOW);
    }
  }

  delay(100);
  DateTime now = rtc.now();
  hours=now.hour();
  mins=now.minute();
  secs = now.second();
  time = millis();
}

const int shift_digits = 24;
void update_nixie(){
  unsigned long r = BCDencode(hours);
unsigned long s = BCDencode(mins);
unsigned long t = BCDencode(secs);

//BlINK DOTS
unsigned long tsdot = 0;
if((secs % 2) == 0){
      tsdot = 1;
}
      
//Serial.println(c.hour_z);
 int muster[shift_digits+1] = {tsdot&1,t&8,t&4,t&2,t&1,t&128,t&64,t&32,t&16,s&8,s&4,s&2,s&1,s&128,s&64,s&32,s&16,r&8,r&4,r&2,r&1,r&128,r&64,r&32,r&16};
 digitalWrite(storePin, LOW);  
 for (int i=0; i<shift_digits+1; i++) {
 digitalWrite(shiftPin, LOW);
 digitalWrite(dataPin, muster[i]);
 digitalWrite(shiftPin, HIGH);
 }
 digitalWrite(storePin, HIGH);
 }



 void update_rtc(){
  rtc.adjust(DateTime(2000, 1, 1, hours, mins, 0));
  }
 
void loop () {
 // Hier passiert nichts.
  DateTime now = rtc.now();
  hours=now.hour();
  mins=now.minute();
  secs=now.second();


/*
if (GPS.newNMEAreceived()) {  
    if (!GPS.parse(GPS.lastNMEA())){
      for(int i = 0;i< 7;i++){
    delay(100);
    digitalWrite(led_hour_pin, LOW);
    delay(100);
    digitalWrite(led_hour_pin, LOW);
    }
      }   // this also sets the newNMEAreceived() flag to false
hours =GPS.hour;
mins = GPS.minute;
secs = GPS.seconds;
update_rtc_gps();
Serial.println("GPSGOT");
play_sound(10);

 for(int i = 0;i< 4;i++){
    delay(20);
    digitalWrite(led_on_pin, LOW);
    delay(20);
    digitalWrite(led_on_pin, LOW);
    }
update_mode_led();
   
  }
  */



  
if(digitalRead(sw_ok_pin) == LOW){
delay(50);
while(digitalRead(sw_ok_pin) == LOW){
  delay(50);
 }
if(mode == 0){mode = 1;}
else if(mode == 1){mode = 2;}
  else if(mode == 2){mode = 0;}
else{
mode = 2;
}
Serial.println(mode);
update_mode_led();  
}


  //TODO ADD SUB
if(digitalRead(sw_add_pin) == LOW && mode == 0){
delay(300);
 mins++;
 if(mins >59){
  mins = 0;
  }
    Serial.println("mins"+String(mins));
    update_rtc();
    update_nixie();
    return;
}

 if(digitalRead(sw_sub_pin) == LOW && mode == 0){
delay(300);
 mins--;
 if(mins <0){
  mins = 59;
  }
  //  Serial.println("mins"+String(mins));
    update_rtc();
    update_nixie();
    return;
}
  
if(digitalRead(sw_add_pin) == LOW && mode == 1){
delay(300);
 hours++;
 if(hours >23){
  hours = 0;
  }
 //   Serial.println("hours"+String(hours));
    update_rtc();
    update_nixie();
    return;
}


 if(digitalRead(sw_sub_pin) == LOW && mode == 1){
delay(300);
 hours--;
 if(hours <0){
  hours = 23;
  }
  // Serial.println("hours"+String(hours));
    update_rtc();
    update_nixie();
    return;
}
 
 // storePin sicherheitshalber auf LOW
//nur wenn sich geändertß
if((millis()-time) > (1000)){
  time = millis();
  update_nixie();

  random_timer++;

  if(random_timer > (60*10)){
    random_timer = 0;
    int tmph = hours;
    int tmpm = mins;
    int tmps = secs;



   
    for(int j = 0; j < 4;j++){
    for(int i = 0; i < 10;i++){
int muster[shift_digits] = {i&8,i&4,i&2,i&1,i&8,i&4,i&2,i&1,i&8,i&4,i&2,i&1,i&8,i&4,i&2,i&1,i&8,i&4,i&2,i&1,i&128,i&64,i&32,i&16};
 digitalWrite(storePin, LOW);  
 for (int i=0; i<shift_digits; i++) {
 digitalWrite(shiftPin, LOW);
 digitalWrite(dataPin, muster[i]);
 digitalWrite(shiftPin, HIGH);
 }
 digitalWrite(storePin, HIGH);
      delay(300);
      }

      hours = tmph;
      mins = tmpm;
      secs = tmps;
    update_nixie();
    }
  }
  }


 //EINBRENNEN VERHINDERN
 
}
