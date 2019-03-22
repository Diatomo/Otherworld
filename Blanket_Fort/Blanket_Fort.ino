
/*

	Project : Otherworld
	Exhibit : Dimmer Ball Aluminum Foil Room
	Author : Charles Stevenson
	Contact : wojtekenson@gmail.com
	Date : 3/18/19 

*/


#include <DmxSimple.h>

#define versionString "Otherworld - [Columbus] [Dimmer Balls]"


const uint8_t PROGMEM gamma8[] = {
	0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,
	1,  1,  1,  1,  1,  1,  1,  1,  1,  2,  2,  2,  2,  2,  2,  2,
	2,  3,  3,  3,  3,  3,  3,  3,  4,  4,  4,  4,  4,  5,  5,  5,
	5,  6,  6,  6,  6,  7,  7,  7,  7,  8,  8,  8,  9,  9,  9, 10,
	10, 10, 11, 11, 11, 12, 12, 13, 13, 13, 14, 14, 15, 15, 16, 16,
	17, 17, 18, 18, 19, 19, 20, 20, 21, 21, 22, 22, 23, 24, 24, 25,
	25, 26, 27, 27, 28, 29, 29, 30, 31, 32, 32, 33, 34, 35, 35, 36,
	37, 38, 39, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 50,
	51, 52, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 66, 67, 68,
	69, 70, 72, 73, 74, 75, 77, 78, 79, 81, 82, 83, 85, 86, 87, 89,
	90, 92, 93, 95, 96, 98, 99,101,102,104,105,107,109,110,112,114,
	115,117,119,120,122,124,126,127,129,131,133,135,137,138,140,142,
	144,146,148,150,152,154,156,158,160,162,164,167,169,171,173,175,
	177,180,182,184,186,189,191,193,196,198,200,203,205,208,210,213,
	215,218,220,223,225,228,231,233,236,239,241,244,247,249,252,255
};


uint32_t const BAUD = 9600;
uint8_t const NUM_BALLS = 6;

struct ball{
	uint32_t cTime = 0;
	uint32_t pTime = 0;
	uint32_t speed = random(10,40);
	uint8_t level = uint8_t(random(0,255));
	bool dimming = false;
	uint8_t dmxChannel;
};
typedef struct ball Ball;

uint8_t const setupPin = 2;
uint8_t const dmxPin = 4;

//objects
Ball ballA;
Ball ballB;
Ball ballC;
Ball ballD;
Ball ballE;
Ball ballF

Ball balls[NUM_BALLS] = {ballA, ballB, ballC, ballD, ballE, ballF};

void setup(){
	
	Serial.begin(BAUD);

	//setup struct objects
	uint8_t channel = 0;
	for (uint8_t ball = 0; ball < NUM_BALLS; ball++){
		balls[ball].dmxChannel = channel;
		channel += 3;
	}

	//setupDMX
	pinMode(setupPin, OUTPUT);
	digitalWrite(setupPin, HIGH);
	DmxSimple.usePin(dmxPin);
}

void runDMX(){
	for (uint8_t ball = 0; ball < NUM_BALLS; ball++){
		if (millis() - balls[ball].pTime > balls[ball].speed){
			for (uint8_t i = balls[ball].dmxChannel; i < balls[ball].dmxChannel + 3; i++){
				DmxSimple.write(i, balls[ball].level);
			}
			
			//check dimmer
			if (balls[ball].level <= 0){
				balls[ball].dimming = false;
			}
			else if (balls[ball].level >= 254){
				balls[ball].dimming = true;
			}

			//increment dimmer
			if (balls[ball].dimming){
				balls[ball].level--;
			}
			else{
				balls[ball].level++;
			}
			balls[ball].pTime = millis();
		}
	}
}

void loop(){
	runDMX();
}
