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

ALL RIGHTS GRANTED this is OPEN SOURCE and 
without any Implied or Otherwise "Gaurantees" of a 
Particular Part in Whole or Function
and free to use in any or all parts.........
Libraries are part of Standard GNU License........
Rod Con ..... The Road to Contentment.............
Starts with an act of Giving.............

Use this sketch to set the Neopixels to display the Time from the Web via ESP8266
on a String/Ring of 60* WS2812 on pin 12 Plus 12 on pin 13 of ESP8266............


___________________________________________________________________________
!*!*!*! Please note the Libraries Required in the Includes.......!*!*!*!|
---------------------------------------------------------------------------
*/
// include the library code:
#include <ESP8266WiFi.h>
#include <Adafruit_NeoPixel.h>
#define PIXELPIN0 13
#define PIXELPIN 12
#define BRIGHTNESS 32
 
// WiFi connection
const char* ssid = "YOUR-SSID";
const char* password = "YOUR-PASSWORD";
WiFiServer server(80);
WiFiClient client;
extern "C" 
{
#include "user_interface.h"
}
ADC_MODE(ADC_VCC);
char tDigit; 
String TiMeS,TMnow,MyNtP;                                 // The current TimeStamp from Google....
int tIMeh,tIMem,tIMes,newt,newt1,inChar,SCsa,MNsa,HRsa;   // Integer values from Gooogle time reply
int hr,mn,st,st1;                                         // Uptime Duration Counter......
String DsTable;                                           // Discarded parts if the Strings......
String theDate,theDate1,duration1,theDateTr;              // The current TimeStamp Date from Google.... 
Adafruit_NeoPixel strip0 = Adafruit_NeoPixel(12, PIXELPIN0, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel strip = Adafruit_NeoPixel(60, PIXELPIN, NEO_GRB + NEO_KHZ800);

byte dialval, hourval, minuteval, secondval;
 
byte pixelColorRed, pixelColorGreen, pixelColorBlue;
 
void setup () {
  WiFi.mode(WIFI_STA);  
  WiFi.begin(ssid, password);  
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(100);
  }
  server.begin();
    ////////////////////////////////
  // connect to Google and get time
  //////////////////////////////////
  WiFiClient client;
  while (!!!client.connect("google.com", 80)) {

  }
  client.print("HEAD / HTTP/1.1\r\n\r\n");
  while(!!!client.available()) {
     yield();
  }
  while(client.available()){
  client.readStringUntil('\n');
   theDate1 = client.readStringUntil('\r'); //Date: Tue, 10 Nov 2015 19:55:38 GMT
    if (theDate1.startsWith("Date:")) {
      TiMeS=theDate1;
      theDate = theDate1.substring(6,23);
          client.flush(); 
          client.stop();             
          }
  }
  inChar = TiMeS.charAt(30);
  newt=inChar-48;             // Convert Numerical Char to int.....
  inChar = TiMeS.charAt(29);
  newt1=inChar-48; 
  tIMes=(newt1*10)+newt;
  
  inChar = TiMeS.charAt(27);
  newt=inChar-48;
  inChar = TiMeS.charAt(26);
  newt1=inChar-48; 
  tIMem=(newt1*10)+newt;
  
  inChar = TiMeS.charAt(24);
  newt=inChar-48;
  inChar = TiMeS.charAt(23);
  newt1=inChar-48; 
  tIMeh=(newt1*10)+newt;
  MyNtP=" ";
  MyNtP+=theDate;
  if (tIMeh<10){MyNtP+="0";}
  MyNtP+=tIMeh;
  MyNtP+=":";
  if (tIMem<10){MyNtP+="0";}
  MyNtP+=tIMem;
  MyNtP+=":";
  if (tIMes<10){MyNtP+="0";}
  MyNtP+=tIMes;
  MyNtP+=" GMT";
 duration1 = " ";
   st1 = millis() / 1000;
   st = millis() / 1000;
   mn = st / 60;
   hr = st / 3600;
   st = st - mn * 60;
   mn = mn - hr * 60;
   if (hr<10) {duration1 += ("0");}
   duration1 += (hr);
   duration1 += (":");
   if (mn<10) {duration1 += ("0");}
   duration1 += (mn);
   duration1 += (":");
   if (st<10) {duration1 += ("0");}
   duration1 += (st);  
 HRsa=hr+tIMeh;
 MNsa=mn+tIMem;
 SCsa=st+tIMes;
if (SCsa>59){SCsa-=60;MNsa+=1;}
if (MNsa>59){MNsa-=60;HRsa+=1;}
if (HRsa>23){HRsa-=24;}
    TMnow = " ";
    TMnow += theDate;
    if (HRsa<10){TMnow += "0";}
    TMnow += HRsa;
    TMnow += ":";
    if (MNsa<10){TMnow += "0";}
    TMnow += MNsa;
    TMnow += ":";
    if (SCsa<10){TMnow += "0";}
    TMnow += SCsa; 
  
  
  
  
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
delay(500);
   st1 = millis() / 1000;
   st = millis() / 1000;
   mn = st / 60;
   hr = st / 3600;
   st = st - mn * 60;
   mn = mn - hr * 60;  
 HRsa=hr+tIMeh;
 MNsa=mn+tIMem;
 SCsa=st+tIMes;
if (SCsa>59){SCsa-=60;MNsa+=1;}
if (MNsa>59){MNsa-=60;HRsa+=1;}
if (HRsa>23){HRsa-=24;}
  secondval = SCsa;
  minuteval = MNsa;
  hourval = HRsa;
  dialval = HRsa;
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


