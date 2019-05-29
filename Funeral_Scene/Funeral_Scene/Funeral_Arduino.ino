/*

	Project : Otherworld
	Exhibit : Funeral Scene
	Author : Charles Stevenson
	Contact : wojtekenson@gmail.com
	Date : 3/14/19 (pi day)

*/


//#include <Metro.h>
//#include <AltSoftSerial.h>    // Arduino build environment requires this
//#include <wavTrigger.h>
#include <alarmClock.h>
#include <DmxSimple.h>

#define versionString "Otherworld - [Columbus] [Funeral Scene]"

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

//Objects

int fadePin = 6;

//Alarm Clocks
//constants
uint8_t const NUM_CLOCKS = 2;
uint8_t const NUM_STATES = 2;

//states
bool onState = false;
bool offState = true;
bool* states[NUM_STATES] = {&onState, &offState};

bool wavState = true;

//Function Prototypes
void fadeOn();
void fadeOff();

//timers
uint32_t timer = 15;

//alarmClocks
alarmClock clkOn = alarmClock(fadeOn);
alarmClock clkOff = alarmClock(fadeOff);

//alarmClock collection
alarmClock clocks[NUM_CLOCKS] = {clkOn, clkOff};
uint32_t timers[NUM_CLOCKS] = {timer, timer};

//Settings
uint32_t const BAUD = 9600;
uint8_t const sensorPin = 7; //digital
uint8_t const dmxPin = 4;
uint8_t const setupPin = 2;

uint8_t level = 0;
uint8_t const LEVEL_MAX = 255;
uint8_t const LEVEL_MIN = 0;


void setupSensors(){
	pinMode(sensorPin, INPUT);
	pinMode(setupPin, OUTPUT);
	digitalWrite(setupPin, HIGH);
}

void setupDMX(){
	DmxSimple.usePin(dmxPin);
	DmxSimple.maxChannel(64);
}


void setup(){
    //Open Serial && write version
    Serial.begin(BAUD);
    Serial.println(versionString);
    Serial.print(F("File: "));
    Serial.println(__FILE__);

	//configure pins && components
	setupSensors();
	setupDMX();

	pinMode(fadePin, OUTPUT);
}

/*
*
* FxN :: alarmClockRoutines
*   processes states and polling
*   
*   callBack Handler
*
*/
void alarmClockRoutine(){
    for (byte i = 0; i < NUM_CLOCKS; i++){
        if (clocks[i].isSet()){//if clock is set
            if (*(states[i])){//if corresponding state is true
                clocks[i].poll();//poll
            }
            else{
                clocks[i].unSetAlarm();//otherwise;unset alarm
            }
        }
        else if (!clocks[i].isSet()){//if clock is no set
            if (*(states[i])){//if state is true
                clocks[i].setAlarm(timers[i]);//turn on alarm
            }
        }
    }
}

//CALLBACK FUNCTION
void fadeOn(){
	Serial.write(level);
	if (level < LEVEL_MAX){
		level++;
	}
	sendDmx();
}

//CALLBACK FUNCTION
void fadeOff(){
	Serial.write(level);
	if (level > LEVEL_MIN){
		level--;
	}
	sendDmx();
}

//Send DMX
void sendDmx(){
	//Serial.println("sendDMX");
	for (uint8_t i = 0; i < 24; i++){
		if (i < 12){
			//DmxSimple.write(i+1, uint8_t(&gamma8[level]));
			DmxSimple.write(i+1, level);
		}
		else{
			if (offState){
				DmxSimple.write(i+1, 0);
			}
			else{
				DmxSimple.write(i+1, 255);
			}

		}
	}
}

bool hallRead(){
  //Serial.println(digitalRead(sensorPin));
	return digitalRead(sensorPin);
}

void loop(){
	if (!hallRead()){
		offState = true;
		onState = false;
		digitalWrite(fadePin, HIGH);
	}
	else{
		offState = false;
		onState = true;
		digitalWrite(fadePin, LOW);
	}
	alarmClockRoutine();
}


