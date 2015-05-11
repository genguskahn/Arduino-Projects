/*
 ___/'¬_         /\
{_-_ ^  \_______//
     /           \
     \ _  _____  /
     || ||     |||
     || ||     |||  
BadTail Projects ALL RIGHTS GRANTED this is OPEN SOURCE and 
without any Implied or Otherwise "Gaurantees" of a 
Particular Part in Whole or Function
and free to use in any or all parts.........
Libraries are part of Standard GNU License........
Rod Con ..... The Road to Contentment.............

Use this sketch to set the Neopixels to display the Time from a DS1307 via i2c
on a String of 60* WS2812 on pin 3 of Arduino............

________________________________________________________________________________________
!*!*!*!*!*!*!* ALL Data is sent to the Serial Console  *!*!*!*!*!*!*!*!
___________________________________________________________________________
!*!*!*! Please note the Libraries Required in the Includes.......!*!*!*!|
---------------------------------------------------------------------------
*/
#include <Wire.h>
#include <RTClib.h>
#include <Adafruit_NeoPixel.h>
 
#define PIXELPIN0 2
#define PIXELPIN1 3 
#define BRIGHTNESS 16
#define DS1307_I2C_ADDRESS 0x68
 
RTC_DS1307 RTC;
DateTime Clock;

byte decToBcd(byte val)
{
 return ( (val/10*16) + (val%10) );
}

// Convert binary coded decimal to normal decimal numbers
byte bcdToDec(byte val)
{
 return ( (val/16*10) + (val%16) );
}
 
Adafruit_NeoPixel strip = Adafruit_NeoPixel(60, PIXELPIN1, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel strip0 = Adafruit_NeoPixel(12, PIXELPIN0, NEO_GRB + NEO_KHZ800); 
byte hourvald, hourval, minuteval, secondval;
 
byte pixelColorRed, pixelColorGreen, pixelColorBlue;
 
 // Gets the date and time from the ds1307
void getDateDs1307(byte *second,
 byte *minute,
 byte *hour,
 byte *dayOfWeek,
 byte *dayOfMonth,
 byte *month,
 byte *year)
{
 // Reset the register pointer
 Wire.beginTransmission(DS1307_I2C_ADDRESS);
 Wire.write(0);
 Wire.endTransmission();

 Wire.requestFrom(DS1307_I2C_ADDRESS, 7);

 // A few of these need masks because certain bits are control bits
 *second = bcdToDec(Wire.read() & 0x7f);
 *minute = bcdToDec(Wire.read());
 *hour = bcdToDec(Wire.read() & 0x3f); // Need to change this if 12 hour am/pm
 *dayOfWeek = bcdToDec(Wire.read());
 *dayOfMonth = bcdToDec(Wire.read());
 *month = bcdToDec(Wire.read());
 *year = bcdToDec(Wire.read());
}
 
 
void setup () {
  Wire.begin();
  RTC.begin();
  Serial.begin(9600);
  pinMode(PIXELPIN0, OUTPUT);
  pinMode(PIXELPIN1, OUTPUT);
  if (! RTC.isrunning()) {
	Serial.println("RTC is NOT running!");
	RTC.adjust(DateTime(__DATE__, __TIME__));
  }
  strip.begin();  // 5 * 12 Matrix
  strip.setBrightness(BRIGHTNESS);
  delay(50);
  colorWipe(strip.Color(125, 0, 125), 10);
  colorWipe(strip.Color(0, 255, 0), 10);
  colorWipe(strip.Color(125, 0, 125), 10);
  delay(50);
  strip0.begin();  // Hours string for the Dial
  strip0.setBrightness(BRIGHTNESS);
  delay(50);
  colorWipe1(strip0.Color(125, 0, 125), 10);
  colorWipe1(strip0.Color(0, 255, 0), 10);
  colorWipe1(strip0.Color(125, 0, 125), 10);
  delay(50);
 
}
 
void loop () {
  Clock = RTC.now();
  
  byte second, minute, hour, dayOfWeek, dayOfMonth, month, year;

 getDateDs1307(&second, &minute, &hour, &dayOfWeek, &dayOfMonth, &month, &year);
 Serial.print(hour, DEC);
 Serial.print(":");
 Serial.print(minute, DEC);
 Serial.print(":");
 Serial.print(second, DEC);
 Serial.print(" ");
 Serial.print(month, DEC);
 Serial.print("/");
 Serial.print(dayOfMonth, DEC);
 Serial.print("/");
 Serial.print(year, DEC);
 Serial.print(" Day_of_week:");
 Serial.println(dayOfWeek, DEC);
  secondval = Clock.second();
  minuteval = Clock.minute();
  hourval = Clock.hour();
  hourvald = Clock.hour();  // Hold the hour number for the Circle of 12 on the Dial
     Serial.print(hourval, DEC);
   Serial.print(':');
   Serial.print(minuteval, DEC);
   Serial.print(':');
   Serial.print(secondval, DEC);
   Serial.print('  Dial Hour   :  ');
   Serial.println(hourvald, DEC);
  if(hourvald > 11) hourvald -= 12; // This clock is 12 hour, if 13-23, convert to 0-11 for the Dial
  if(hourval > 11) hourval -= 12; // This clock is 12 hour, if 13-23, convert to 0-11
  hourval = ((((hourval*60) / 12) - 1)+(minuteval / 12));  //Represent the hour number on Matrix 12 minute steps.
  minuteval =   minuteval - 1; // Adjust the minute to reflect the dial position of the Matrix
  for(uint8_t i=0; i<strip.numPixels(); i++) {
 
	if (i <= secondval) {
  	pixelColorBlue = 255;
	}
	else {
  	pixelColorBlue = 0;
	}
 
	if (i <= minuteval) {
  	pixelColorGreen = 255;  
	}
	else {
  	pixelColorGreen = 0;
	}
 
	if (i <= hourval) {
  	pixelColorRed = 255;   
	}
	else {
  	pixelColorRed = 0;
	}
	strip.setPixelColor(i, strip.Color(pixelColorRed, pixelColorGreen, pixelColorBlue));
}
  for(uint8_t i=0; i<strip0.numPixels(); i++) {
 
	if (i <= hourvald) {
  	pixelColorRed = 255;   
	}
	else {
  	pixelColorRed = 0;
	}
	strip0.setPixelColor(i, strip0.Color(pixelColorRed, 0, 0));
  }
   Serial.print(hourval, DEC);
   Serial.print(':');
   Serial.print(minuteval, DEC);
   Serial.print(':');
   Serial.print(secondval, DEC);
   Serial.print('Dial Hour   :  ');
   Serial.println(hourvald, DEC);
   strip.show();
  delay(500);
}
void colorWipe1(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip0.numPixels(); i++) {
	strip0.setPixelColor(i, c);
	strip0.show();
	delay(wait);
  }
}
void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
	strip.setPixelColor(i, c);
	strip.show();
	delay(wait);
  }
}
