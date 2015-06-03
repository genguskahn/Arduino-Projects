/*
 ___/'¬_         /\
{_-_ ^  \_______//
     /           \
     \ _  _____  /
     || ||     |||
     || ||     |||  

            _____
           </\|/\>
     _-_-_-_ *|* _-_-_-_
    -_-_-_-_-\@/-_-_-_-_-
_____________________________

BadTail Projects ALL RIGHTS GRANTED this is OPEN SOURCE and 
without any Implied or Otherwise "Gaurantees" of a 
Particular Part in Whole or Function
and free to use in any or all parts.........
Libraries are part of Standard GNU License........
Rod Con ..... The Road to Contentment.............

Use this sketch to set the Neopixels to display the Time from a DS1307 via i2c
on a String of 60* WS2812 on pin 3 Plus 12 on pin 2 of Arduino............

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
#define PIXELPIN 3
 
#define BRIGHTNESS 32
 
RTC_DS1307 RTC;
DateTime Clock;
 
Adafruit_NeoPixel strip0 = Adafruit_NeoPixel(12, PIXELPIN0, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel strip = Adafruit_NeoPixel(60, PIXELPIN, NEO_GRB + NEO_KHZ800);

byte dialval, hourval, minuteval, secondval;
 
byte pixelColorRed, pixelColorGreen, pixelColorBlue;
 
void setup () {
  Wire.begin();
  RTC.begin();
//  Serial.begin(9600);   // Un Comment to output serial.....
  pinMode(PIXELPIN, OUTPUT);
  pinMode(PIXELPIN0, OUTPUT);
  if (! RTC.isrunning()) {
	Serial.println("RTC is NOT running!");
	RTC.adjust(DateTime(__DATE__, __TIME__));
  }
  strip.begin();
  strip.setBrightness(BRIGHTNESS);
  delay(50);
  colorWipe(strip.Color(125, 0, 125), 10);
  colorWipe(strip.Color(0, 255, 0), 10);
  colorWipe(strip.Color(125, 0, 125), 10);
  delay(500);
  strip0.begin();
  strip0.setBrightness(BRIGHTNESS);
  delay(50);
  colorWipe0(strip0.Color(125, 0, 125), 30);
  colorWipe0(strip0.Color(0, 255, 0), 40);
  colorWipe0(strip0.Color(125, 0, 125), 20);
  colorWipe0(strip0.Color(0, 0, 0), 10);
  delay(500);
  
}
 
void loop () {
  Clock = RTC.now();
 
  secondval = Clock.second();
  minuteval = Clock.minute();
  hourval = Clock.hour();
  dialval = Clock.hour();
  if(hourval > 11) hourval -= 12; // This clock is 12 hour, if 13-23, convert to 0-11
  if(dialval > 11) dialval -= 12; // This clock is 12 hour, if 13-23, convert to 0-11
  hourval = ((((hourval*60) / 12) - 1)+(minuteval / 12));  //Represent the hour number on dial 12 minute steps.
  minuteval =   minuteval - 1; // Adjust the minute to reflect the dial position
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
	if (i <= dialval) {
  	pixelColorRed = 255;   
	}
	else {
  	pixelColorRed = 0;
	}

    	strip0.setPixelColor(i, strip0.Color(pixelColorRed, pixelColorGreen, pixelColorBlue));
  
  
  }
/*            // Un Comment to output serial.....
   Serial.print(dialval, DEC);
   Serial.print(':');
   Serial.print(hourval, DEC);
   Serial.print(':');
   Serial.print(minuteval, DEC);
   Serial.print(':');
   Serial.println(secondval, DEC);
  */
  strip.show();
  strip0.show();  
  delay(200);
}
void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
	strip.setPixelColor(i, c);
	strip.show();
	delay(wait);
  }
}
void colorWipe0(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip0.numPixels(); i++) {
	strip0.setPixelColor(i, c);
	strip0.show();
	delay(wait);
  }
}

