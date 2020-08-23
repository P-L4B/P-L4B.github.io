/*
  IO+RTC CLOCK EXPANSION BOARD FOR APPLE-1 BY CLAUDIO PARMIGIANI (P-LAB) 2020.

  USE ONLY WITH:
  - ATMEGA32A ONLY (16MHZ EXTERNAL CRYSTAL)
  - PCB v0.94c (DO NOT USE WITH PCB VERSIONS 0.94/0.94b !)

  IMPORTANT:
  CHANGE/CHECK DEFAULT ATMEGA32 FUSES AS FOLLOWS:
  CKSEL3 = 1    TO USE EXTERNAL 16MHz CRYSTAL
  CKSEL2 = 1    TO USE EXTERNAL 16MHz CRYSTAL
  CKSEL1 = 1    TO USE EXTERNAL 16MHz CRYSTAL
  CKSEL0 = 1    
  JTAGEN = 1    TO DISABLE JTAG (IN ORDER TO USE ATMEGA'S PCx PORTS AS STANDARD I/O)

  REGISTER  CONTENT
  00        HOUR
  01        MINUTE
  02        SECOND
  03        DAY
  04        MONTH
  05        YEAR
  06        TEMPERATURE FROM RTC
  07        TEMPERATURE FROM DS18B20
  08        DECIMAL DIGITS OT THE TEMPERATURE ABOVE (register 07)
  09        *FREE*
  10        ANALOG READ CHANNEL 1
  11        ANALOG READ CHANNEL 2
  12        ANALOG READ CHANNEL 3
  13        ANALOG READ CHANNEL 4
  14        ANALOG READ CHANNEL 5
  15        ANALOG READ CHANNEL 6
  16        ANALOG READ CHANNEL 7
  17        ANALOG READ CHANNEL 8
  18        *FREE*
  19        *FREE*
  20        DIGITAL READ CHANNEL 1
  21        DIGITAL READ CHANNEL 2
  22        DIGITAL READ CHANNEL 3
  23        DIGITAL READ CHANNEL 4
  24 TO 31  *FREE*
*/

#define   DB0            0    // ATMEGA PIN  1 - VIA PB0 pin 10   DATA BUS-A     
#define   DB1            1    // ATMEGA PIN  2 - VIA PB1 pin 11
#define   DB2            2    // ATMEGA PIN  3 - VIA PB2 pin 12
#define   DB3           19    // ATMEGA PIN 25 - VIA PB3 pin 13
#define   DB4            4    // ATMEGA PIN  5 - VIA PB4 pin 14
#define   DB5            5    // ATMEGA PIN  6 - VIA PB5 pin 15
#define   DB6            6    // ATMEGA PIN  7 - VIA PB6 pin 16
#define   DB7            7    // ATMEGA PIN  8 - VIA PB7 pin 17

#define   AB0           11    // ATMEGA PIN 17 - VIA PA3 pin 5    ADDRESS BUS-A
#define   AB1           20    // ATMEGA PIN 26 - VIA PA4 pin 6
#define   AB2           21    // ATMEGA PIN 27 - VIA PA5 pin 7
#define   AB3           14    // ATMEGA PIN 20 - VIA PA6 pin 8
#define   AB4           22    // ATMEGA PIN 28 - VIA PA7 pin 9

#define   STROBE        10    // ATMEGA PIN 16 - VIA PA2 pin 4    STROBE (OUTPUT: ACTIVE HIGH WHEN DATA IS CONSOLIDATED)
#define   STROBE_T      100   // HOW MANY MICROSECONDS STROBE WILL REMAIN HIGH
#define   RW            18    // ATMEGA PIN 24 - VIA PA1 pin 3    RW (INPUT: ACTIVE HIGH, VIA WANTS TO WRITE SOMETHING INTO ATMEGA)

#define   SCL           16    // ATMEGA PIN 22 - I2C RTC
#define   SDA           17    // ATMEGA PIN 23 - I2C RTC

#define   AIN1          31    // ATMEGA PIN 40 - ANALOG INPUT 1
#define   AIN2          30    // ATMEGA PIN 39 - ANALOG INPUT 2
#define   AIN3          29    // ATMEGA PIN 38 - ANALOG INPUT 3
#define   AIN4          28    // ATMEGA PIN 37 - ANALOG INPUT 4
#define   AIN5          27    // ATMEGA PIN 36 - ANALOG INPUT 5
#define   AIN6          26    // ATMEGA PIN 35 - ANALOG INPUT 6
#define   AIN7          25    // ATMEGA PIN 34 - ANALOG INPUT 7
#define   AIN8          24    // ATMEGA PIN 44 - ANALOG INPUT 8

#define   DIN1          15    // ATMEGA PIN 21 - DIGITAL INPUT 1
#define   DIN2          13    // ATMEGA PIN 19 - DIGITAL INPUT 2
#define   DIN3          12    // ATMEGA PIN 18 - DIGITAL INPUT 3
#define   DIN4           3    // ATMEGA PIN  4 - DIGITAL INPUT 4

#define   DS18B20       23    // ATMEGA PIN 29 - 1-Wire DATA PIN

//DALLAS DS18B20
unsigned long previousMillis = 0;     // will store last time DS18B20's Temperature was updated
boolean DS18B20_ACTIVE;
int DS18B20_PRECISION = 9;            // default accuracy of DS18B20
long DS18B20_INTERVAL = 500;          // interval between DS18B20 reading (milliseconds). Will be redefined by precision (the biggest accuracy --> the highest interval). For 9 bit precision is 500ms.
#include <OneWire.h>
#include <DallasTemperature.h>
OneWire oneWire(DS18B20);             // Setup oneWire instance (digital pin 23) to communicate with any OneWire device
DallasTemperature sensors(&oneWire);  // Pass our oneWire reference to Dallas Temperature.

//MISCELLANEOUS
double T;
int decT;
int t;
int Val;
int Register;

//RTC 3231
int HOUR;
int MINUTE;
int SECOND;
int YEAR;
int MONTH;
int DAY;
int TEMP;
#include "RTClib.h"
RTC_DS3231 rtc;

void setup()
{
  sensors.begin();              //start 1-wire
  rtc.begin();                  //start RTC
  Serial.begin(9600);           //start Serial (debug only)
  Serial.println("Starting...");
  pinMode(DB0, OUTPUT);
  pinMode(DB1, OUTPUT);
  pinMode(DB2, OUTPUT);
  pinMode(DB3, OUTPUT);
  pinMode(DB4, OUTPUT);
  pinMode(DB5, OUTPUT);
  pinMode(DB6, OUTPUT);
  pinMode(DB7, OUTPUT);
  pinMode(RW, INPUT);

  pinMode(AB0, OUTPUT);
  pinMode(AB1, OUTPUT);
  pinMode(AB2, OUTPUT);
  pinMode(AB3, OUTPUT);
  pinMode(AB4, OUTPUT);

  pinMode(STROBE, OUTPUT);

  pinMode(DIN1, INPUT_PULLUP);
  pinMode(DIN2, INPUT_PULLUP);
  pinMode(DIN3, INPUT_PULLUP);
  pinMode(DIN4, INPUT_PULLUP);

  // check if DS18B20 has to be active (DIN1=LOW) and, in case, its precision (9..12 bit). Default is 9 bit.
  DS18B20_ACTIVE = digitalRead(DIN1);
  if (digitalRead (DIN2) == LOW) {
    DS18B20_PRECISION = 10;     //10 bit precision
    DS18B20_INTERVAL = 2000;    //milliseconds between readings
  }
  if (digitalRead (DIN3) == LOW) {
    DS18B20_PRECISION = 11;     //11 bit precision
    DS18B20_INTERVAL = 3000;    //milliseconds between readings
  }
  if (digitalRead (DIN4) == LOW) {
    DS18B20_PRECISION = 12;     //12 bit precision
    DS18B20_INTERVAL = 4000;    //milliseconds between readings
  }

}

void loop()
{
  unsigned long currentMillis = millis();
  //check RW
  if (digitalRead(RW) == HIGH) {
    RW_loop();
  }

  //RTC
  DateTime now = rtc.now();           //read RTC
  YEAR = now.year(), DEC;
  MONTH = now.month(), DEC;
  DAY = now.day(), DEC;
  HOUR = now.hour(), DEC;
  MINUTE = now.minute(), DEC;
  SECOND = now.second(), DEC;
  TEMP = rtc.getTemperature();

  //DS18B20
  if ((DS18B20_ACTIVE == 0) && (currentMillis - previousMillis >= DS18B20_INTERVAL)) {
    previousMillis = currentMillis;
    sensors.setResolution(DS18B20_PRECISION);
    sensors.requestTemperatures();    //read ds18b20 temperature probe
    T = sensors.getTempCByIndex(0);
    if ( T < 0 ) {
      T = 128 - T;
    }
    //compute decimal digits considering the precision (12bit-->4digits, 11bit-->3digits, 10bit--> 2digits, 9bit, 1digit)
    int manT = (int)(T);
    decT = (pow (10, (DS18B20_PRECISION - 8))) * (T - manT);
  }

  //OUTPUT REGISTER 0
  Val = HOUR;  Register = 0;  DataOut(); RegisterOut();  StrobeOut();
  //OUTPUT REGISTER 1
  Val = MINUTE;  Register = 1; DataOut(); RegisterOut();  StrobeOut();
  //OUTPUT REGISTER 2
  Val = SECOND;  Register = 2; DataOut(); RegisterOut();  StrobeOut();
  //OUTPUT REGISTER 3
  Val = DAY;  Register = 3; DataOut(); RegisterOut();  StrobeOut();
  //OUTPUT REGISTER 4
  Val = MONTH;  Register = 4; DataOut(); RegisterOut();  StrobeOut();
  //OUTPUT REGISTER 5
  Val = YEAR - 2000;  Register = 5; DataOut(); RegisterOut();  StrobeOut();
  //OUTPUT REGISTER 6
  Val = TEMP;  Register = 6; DataOut(); RegisterOut();  StrobeOut();
  //OUTPUT REGISTER 7
  Val = T;  Register = 7; DataOut(); RegisterOut();  StrobeOut();
  //OUTPUT REGISTER 8
  Val = decT;  Register = 8; DataOut(); RegisterOut();  StrobeOut();
  //NOTE: ANALOG INPUT ORDER IS REVERSED IN ORDER TO MATCH PIN NAMES ON TO THE PCB... :)
  //OUTPUT REGISTER 10
  Val = map (analogRead(AIN8), 0, 1023, 0, 255); Register = 10; DataOut(); RegisterOut();  StrobeOut();
  //OUTPUT REGISTER 11
  Val = map (analogRead(AIN7), 0, 1023, 0, 255); Register = 11; DataOut(); RegisterOut();  StrobeOut();
  //OUTPUT REGISTER 12
  Val = map (analogRead(AIN6), 0, 1023, 0, 255); Register = 12; DataOut(); RegisterOut();  StrobeOut();
  //OUTPUT REGISTER 13
  Val = map (analogRead(AIN5), 0, 1023, 0, 255); Register = 13; DataOut(); RegisterOut();  StrobeOut();
  //OUTPUT REGISTER 14
  Val = map (analogRead(AIN4), 0, 1023, 0, 255); Register = 14; DataOut(); RegisterOut();  StrobeOut();
  //OUTPUT REGISTER 15
  Val = map (analogRead(AIN3), 0, 1023, 0, 255); Register = 15; DataOut(); RegisterOut();  StrobeOut();
  //OUTPUT REGISTER 16
  Val = map (analogRead(AIN2), 0, 1023, 0, 255); Register = 16; DataOut(); RegisterOut();  StrobeOut();
  //OUTPUT REGISTER 17
  Val = map (analogRead(AIN1), 0, 1023, 0, 255); Register = 17; DataOut(); RegisterOut();  StrobeOut();
  //OUTPUT REGISTER 20
  Val = digitalRead(DIN1);  Register = 20; DataOut(); RegisterOut(); StrobeOut();
  //OUTPUT REGISTER 21
  Val = digitalRead(DIN2);  Register = 21; DataOut(); RegisterOut(); StrobeOut();
  //OUTPUT REGISTER 22
  Val = digitalRead(DIN3);  Register = 22; DataOut(); RegisterOut(); StrobeOut();
  //OUTPUT REGISTER 23
  Val = digitalRead(DIN4);  Register = 23; DataOut();  RegisterOut(); StrobeOut();



}

void DataOut() {
  digitalWrite(DB7, Val & 128);
  digitalWrite(DB6, Val & 64);
  digitalWrite(DB5, Val & 32);
  digitalWrite(DB4, Val & 16);
  digitalWrite(DB3, Val & 8);
  digitalWrite(DB2, Val & 4);
  digitalWrite(DB1, Val & 2);
  digitalWrite(DB0, Val & 1);
}

void RegisterOut() {
  digitalWrite(AB0, Register & 1);
  digitalWrite(AB1, Register & 2);
  digitalWrite(AB2, Register & 4);
  digitalWrite(AB3, Register & 8);
  digitalWrite(AB4, Register & 16);
}

void StrobeOut() {
  digitalWrite (STROBE, HIGH);
  delayMicroseconds(STROBE_T);
  digitalWrite (STROBE, LOW);
  //SerialOut();    //uncomment for debug

}

void SerialOut() {
  Serial.print("REG=");
  Serial.print(Register);
  Serial.print(" - ");
  Serial.print(Register, BIN);
  Serial.print(" - VAL=");
  Serial.println(Val);
  delay(300);
}

void RW_loop() {
  while (digitalRead(RW) == 1) {
    Serial.println("RW Detected");
    //set ADDRESS-A and DATABUS-A pin to INPUT in order to receive register and value
    pinMode(DB0, INPUT);
    pinMode(DB1, INPUT);
    pinMode(DB2, INPUT);
    pinMode(DB3, INPUT);
    pinMode(DB4, INPUT);
    pinMode(DB5, INPUT);
    pinMode(DB6, INPUT);
    pinMode(DB7, INPUT);
    pinMode(AB0, INPUT);
    pinMode(AB1, INPUT);
    pinMode(AB2, INPUT);
    pinMode(AB3, INPUT);
    pinMode(AB4, INPUT);

    Register = digitalRead(AB0) + (digitalRead(AB1) * 2) + (digitalRead(AB2) * 4) + (digitalRead(AB3) * 8) + (digitalRead(AB4) * 16);
    Val = digitalRead(DB0) + (digitalRead(DB1) * 2) + (digitalRead(DB2) * 4) + (digitalRead(DB3) * 8) + (digitalRead(DB4) * 16) + (digitalRead(DB5) * 32) + (digitalRead(DB6) * 64) + (digitalRead(DB7) * 128);
    SerialOut();

    DateTime now = rtc.now();           //read RTC
    YEAR = now.year(), DEC;
    MONTH = now.month(), DEC;
    DAY = now.day(), DEC;
    HOUR = now.hour(), DEC;
    MINUTE = now.minute(), DEC;
    SECOND = now.second(), DEC;

    // This line sets the RTC with an explicit date & time, for example to set
    // January 21, 2014 at 3am you would call:
    // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));

    switch (Register) {
      case 0:
        if (Val != 0) { //HOUR=0 is excluded in order to prevent fake writings caused by VIA's weird power-up that could trigger RW pin to HIGH.
          Serial.println("Setting HOUR");
          rtc.adjust(DateTime(YEAR, MONTH, DAY, Val, MINUTE, SECOND));
        }
        break;
      case 1:
        Serial.println("Setting MINUTE");
        rtc.adjust(DateTime(YEAR, MONTH, DAY, HOUR, Val, SECOND));
        break;
      case 2:
        Serial.println("Setting SECOND");
        rtc.adjust(DateTime(YEAR, MONTH, DAY, HOUR, MINUTE, Val));
        break;
      case 3:
        Serial.println("Setting DAY");
        rtc.adjust(DateTime(YEAR, MONTH, Val, HOUR, MINUTE, SECOND));
        break;
      case 4:
        Serial.println("Setting MONTH");
        rtc.adjust(DateTime(YEAR, Val, DAY, HOUR, MINUTE, SECOND));
        break;
      case 5:
        Serial.println("Setting YEAR");
        rtc.adjust(DateTime(Val, MONTH, DAY, HOUR, MINUTE, SECOND));
        break;

    }

  }
  Serial.println("Resuming normal operations");
  pinMode(DB0, OUTPUT);
  pinMode(DB1, OUTPUT);
  pinMode(DB2, OUTPUT);
  pinMode(DB3, OUTPUT);
  pinMode(DB4, OUTPUT);
  pinMode(DB5, OUTPUT);
  pinMode(DB6, OUTPUT);
  pinMode(DB7, OUTPUT);
  pinMode(AB0, OUTPUT);
  pinMode(AB1, OUTPUT);
  pinMode(AB2, OUTPUT);
  pinMode(AB3, OUTPUT);
  pinMode(AB4, OUTPUT);
  delay(300);
}

