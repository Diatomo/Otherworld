
/*

Immersive Entertainment

Project:Otherworld
Exhibit: Seaweed
Author(s): Charles Stevenson
Date: 5/1/19

Revision notes:
*/



#include "alarmClock.h"
#include <FastLED.h>


//general settings
uint32_t const BAUD = 9600;
int const nLEDS = 70;
int const NUM_LEDS = nLEDS;
uint8_t const nSTRIPS = 18;
uint8_t const START_PIN = 3;

//prototypes
void activateStrip();
void triggerMeteor();

alarmClock activeClk = alarmClock(activateStrip);
alarmClock delayClk = alarmClock(triggerMeteor);

//meteor settings
byte meteorSize = 15;
boolean meteorRandomDecay = false;
byte meteorTrailDecay = 30;
uint8_t const SPEED = 1;
uint8_t const fadeVal = 32;

byte red = 255;
byte green = 4;
byte blue = 231;

CRGB leds[nLEDS];

//strips
struct strip{	
	uint8_t counter;
	bool state;
  byte red;
  byte green;
  byte blue;
};


//pointer for strips
strip* strips = new strip[nSTRIPS];

void setup(){
 // Serial.begin(BAUD);

	uint8_t const OFFSET = 3;
	for (int i = 0; i < nSTRIPS; i++){
		strips[i].counter = 0;
		strips[i].state = false;
    strips[i].red = random(0,255);
    strips[i].green = random(0,255);
    strips[i].blue = random(0,255);
	}
	//const expressions TODO need iterator
  
  FastLED.addLeds<NEOPIXEL, 1>(leds, nLEDS); 
  FastLED.addLeds<NEOPIXEL, 2>(leds, nLEDS); 
	FastLED.addLeds<NEOPIXEL, 3>(leds, nLEDS); 
	FastLED.addLeds<NEOPIXEL, 4>(leds, nLEDS); 
	FastLED.addLeds<NEOPIXEL, 5>(leds, nLEDS); 
	FastLED.addLeds<NEOPIXEL, 6>(leds, nLEDS); 
	FastLED.addLeds<NEOPIXEL, 7>(leds, nLEDS); 
	FastLED.addLeds<NEOPIXEL, 8>(leds, nLEDS); 
	FastLED.addLeds<NEOPIXEL, 9>(leds, nLEDS);
	FastLED.addLeds<NEOPIXEL, A0>(leds, nLEDS); 
	FastLED.addLeds<NEOPIXEL, A1>(leds, nLEDS); 
	FastLED.addLeds<NEOPIXEL, A2>(leds, nLEDS); 
	FastLED.addLeds<NEOPIXEL, A3>(leds, nLEDS);  
	FastLED.addLeds<NEOPIXEL, A7>(leds, nLEDS); 
	FastLED.addLeds<NEOPIXEL, A8>(leds, nLEDS);
  FastLED.addLeds<NEOPIXEL, A9>(leds, nLEDS);
  FastLED.addLeds<NEOPIXEL, A10>(leds, nLEDS);
  FastLED.addLeds<NEOPIXEL, A11>(leds, nLEDS);
 // FastLED.addLeds<NEOPIXEL, 22>(leds, nLEDS);
  //FastLED.addLeds<NEOPIXEL, 24>(leds, nLEDS);

  FastLED.clear();
  for (int j = 0; j < nSTRIPS; j++){
    FastLED[j].showLeds();
  }
  FastLED.show();
}


void poll(){
	if (activeClk.isSet()){
		activeClk.poll();
	}
	else if (!activeClk.isSet()){
    //Serial.println("active clock");
		activeClk.setAlarm(100);
	}


	if (delayClk.isSet()){
		delayClk.poll();
	}
	else if (!delayClk.isSet()){
		delayClk.setAlarm(SPEED);
	}

}

void activateStrip(){
	bool pass = false;
  //Serial.println("activate strip");
//	while (!pass){
    /*
		for (uint8_t i = 0; i < nSTRIPS; i++){
      //Serial.println("state change");
			strips[i].state = true;
      pass = true;
		}
    */
   //Serial.println("done activating");
		uint8_t pick =  random(18);
		if (strips[pick].state == false){
			strips[pick].state = true;
			//pass = true;
		//}
	}
}

void triggerMeteor(){
	for (uint8_t i = 0; i < nSTRIPS; i++){
		if (strips[i].state == true){
			meteorRain(i);
			strips[i].counter++;
			if (strips[i].counter >= nLEDS - 1){
				strips[i].counter = 0;
				strips[i].state = false;
       // randomizeColor(i);
			}
		}
	}
}

void randomizeColor(int index){
  strips[index].red = random(0,255);
  strips[index].green = random(0,255);
  strips[index].blue = random(0,255);
}

void meteorRain(uint8_t index){

	//Serial.println("are meteor's raining??");
	FastLED.clear();
//	for(int i = 0; i < NUM_LEDS+NUM_LEDS; i++) {

		int i = strips[index].counter;
		// fade brightness all LEDs one step
		for(int j=0; j<NUM_LEDS; j++) {
			if( (!meteorRandomDecay) || (random(10)>5) ) {
				leds[strips[index].counter].fadeToBlackBy(fadeVal);        
			}
		}

		// draw meteor
		for(int j = 0; j < meteorSize; j++) {
			if( ( i-j <NUM_LEDS) && (i-j>=0) ) {
				setPixel(i-j, red, green, blue);
			} 
		}
		FastLED[index].showLeds();
		//delay(SpeedDelay);
//	}
}

void setPixel(int pixel, byte red, byte green, byte blue){
	leds[pixel].r = red;
	leds[pixel].g = green;
	leds[pixel].b = blue;
}

/*
void fadeToBlack(int ledNo, byte fadeValue, strip* s) {
	#ifdef ADAFRUIT_NEOPIXEL_H 
	// NeoPixel
	uint32_t oldColor;
	uint8_t r, g, b;
	int value;

	oldColor = strip.getPixelColor(ledNo);
	r = (oldColor & 0x00ff0000UL) >> 16;
	g = (oldColor & 0x0000ff00UL) >> 8;
	b = (oldColor & 0x000000ffUL);

	r=(r<=10)? 0 : (int) r-(r*fadeValue/256);
	g=(g<=10)? 0 : (int) g-(g*fadeValue/256);
	b=(b<=10)? 0 : (int) b-(b*fadeValue/256);

	setPixel(ledNo,s,r,g,b);
	#endif
	#ifndef ADAFRUIT_NEOPIXEL_H
	// FastLED
	s->leds[ledNo].fadeToBlackBy( fadeValue );
	#endif  
}*/

void loop() {
	poll();
  //Serial.println("polling");
}
