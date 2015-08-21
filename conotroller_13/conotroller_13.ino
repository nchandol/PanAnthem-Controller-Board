#include <Wire.h>
#include <LIDARLite.h>


int triggerON = 0;
String versionNum = "conotroller_13";

boolean bDebug = false; //false;//

//-------------------debug button-------------------
int buttonPin1 = A7;

boolean bDebugMode = false;

int numReadings = 10;
int button1_readings[10];
int button1_index = 0;
unsigned int button1_total = 0;
int button1_value = 1000;
unsigned long buttonReadTimer;
int buttonReadDuration = 100; //read analog button every 100ms


int powerEnable_pin = A0;
//int powerEnable_pin = 2;
//int hotSwapFault_pin = A1;


//-----------------led panel-------------------
int ledPin_display = 6;        // big display
int maxBrightness = 255; // 160; //60;
int minBrightness = 0;
int panelBrightness = minBrightness;

//-----------------id leds-------------------
int ledPin1 = A2;            //small led on pcb
boolean led1_state = 0;
unsigned long led1_timer;
//
//-----------------Buzzer-------------------
int sensorBuzzFreq[] = {
  30, 300
};
int sensorBuzzDur[] = {
  500, 100
};
int speakerPin = 9;

//-----------------LIDAR sensor-------------------
LIDARLite myLidarLite;
unsigned char addresses[] = {0x66,0x68};

int enableSensorPinsArraySize = 2; // The length of the array

int enableSensorPins[] = {
  2, A3
}; // Array of pins connected to the sensor Power Enable lines


int maxDistance = 350; //max read distance can be up to 40 m = 4000 cm

boolean bSensorsTriggered = false;
unsigned long triggerOnTimer;
int triggerOnDuration = 1000;
unsigned long triggerOffTimer;
int triggerOffDuration = 2000;

int LIDAR_numReadings = 10;
int LIDAR_readings[2][10];
int LIDAR_index[] = {0, 0};
int LIDAR_total[] = {0, 0};
int LIDAR_value[] = {1000, 1000};

//sometimes the LIDAR is blocking the loop() function with its while loop
//break while is it gets stuck
int i2cError = 0;
int LIDAR_i2cCount = 0;
int LIDAR_i2cMax = 3000;

//int minDistance = 50; //almost in cm
//int maxDistance = 270; //analog reading goes from 0 -1024

//-----------distance potentiometer
int distancePot_pin = A6;

#define potREADS 10
int potReads[potREADS];
int potReadsCnt = 0;

//boolean bPresent = false;
//#define READS 10 //50
//int sensorValues[READS];  // variable to store the value coming from the sensor
//int readsCnt;

//relays
int highPower_pin = 4; //switch 9-12v
int lowPower_pin = 8; //switch 6.5 - 5v
boolean bHighPower_state = false;
boolean bLowPower_state = false;

unsigned long relayTimer;

int relaySwitching_stage = 0;



boolean presentStage;
unsigned long presentTimer;

boolean switchToLow = true;
boolean switchToHigh = false;



void setup() {
  // declare the ledPin as an OUTPUT:
  if (bDebug) {
    Serial.begin(115200);
    Serial.println(versionNum);
  }

  //pinMode(speakerPin, OUTPUT);

  pinMode(ledPin_display, OUTPUT);
  pinMode(ledPin1, OUTPUT);

  // readsCnt = 0;
  pinMode(powerEnable_pin, OUTPUT);
  digitalWrite(powerEnable_pin, LOW);

  bHighPower_state = 0;
  bLowPower_state = 1;

  pinMode(highPower_pin, OUTPUT);
  pinMode(lowPower_pin, OUTPUT);

  digitalWrite(highPower_pin, bHighPower_state);
  digitalWrite(lowPower_pin, bLowPower_state);

  presentStage = 4;
  //delay(2000);

  relayTimer = millis();
  led1_timer = millis();

  presentTimer = millis();

  for (int i = 0; i < 2; i++) {
    pinMode(enableSensorPins[i], OUTPUT); // Pin to first LIDAR-Lite Power Enable line
  }
  //digitalWrite(enableSensorPins[0], HIGH);
  
  for (int i = 0; i < 2; i++) {
   // beep(speakerPin, sensorBuzzFreq[i], sensorBuzzDur[i]);
    delay(200);
  }
  
  delay(1000);
  myLidarLite.begin();
  myLidarLite.changeAddressMultiPwrEn(2,enableSensorPins,addresses,false);

  Serial.println("setup done");

}

void loop() {

  if (i2cError != 0) {

    Serial.print("i2cError ");
    Serial.print(i2cError);
    Serial.println();
    i2cError = 0;
  }
  bDebugMode = checkDebugMode();

  if ( millis() > 1500 ) {
    checkLIDAR();

    digitalWrite(powerEnable_pin, HIGH);
  }
  else {
    //digitalWrite(powerEnable_pin, HIGH);
    //only check at start what we want the threshold distance to be set at
    checkDistancePot();
  }



  if (bDebugMode) {

    // check all the time what we want the threshold distance to be set at
    checkDistancePot();
    //digitalWrite(powerEnable_pin, LOW);
    //   digitalWrite(ledPin1, HIGH);
  }
  else {

    //digitalWrite(powerEnable_pin, HIGH);
    //  digitalWrite(ledPin1, LOW);
  }

  //bSensorsTriggered =true;

  if (bSensorsTriggered) {
    digitalWrite(ledPin1, HIGH);
    
    if (presentStage == 4) {
      presentStage = 1;
    //ss  presentTimer = millis();
    }
    else {

    }
    //    switchToHigh = true;
    //    switchToLow = false;
    // presentStage = 1;

    //    ledBightness = map(temp_sensorVal, minDistance, maxDistance, 20 ,255);
    //    ledBightness = constrain(ledBightness, 20, 255);
    analogWrite(ledPin_display, maxBrightness);
    //digitalWrite(ledPin2,LOW);
  }
  else {
    digitalWrite(ledPin1, LOW);
    //    switchToHigh = false;
    //    switchToLow = true;
    //    presentTimer = millis();
    if (presentStage == 2) {
      presentStage = 3;
    //ss  presentTimer = millis();
    }
    else {

    }

    analogWrite(ledPin_display, minBrightness);
    //digitalWrite(ledPin2,HIGH);
  }
  // digitalWrite(ledPin_display, ledBightness);

if(bSensorsTriggered){
    bHighPower_state = 1;
}else{
  bHighPower_state = 0;
}

/*
  if (presentStage == 1) {
    //both = on
    if(bDebug){
      Serial.println("stage 1 on , on");
       Serial.println(presentTimer);
    }
    bHighPower_state = 1;
    bLowPower_state = 1;
    if (millis() - presentTimer > 500) {
      presentStage = 2;
    }
  }
  if (presentStage == 2) {
    //high = on
     if(bDebug) Serial.println("stage 2 high = on");
    bHighPower_state = 1;
    bLowPower_state = 0;
  }


  if (presentStage == 3) {
    //both = on
     if(bDebug) Serial.println("stage 3 on ,on ");
    bHighPower_state = 1;
    bLowPower_state = 1;
    if (millis() - presentTimer > 500) {
      presentStage = 4;
    }

  }
  if (presentStage == 4) {
    //low = on
     if(bDebug) Serial.println("stage 4 low is on");
    bHighPower_state = 0;
    bLowPower_state = 1;
  }
*/

  digitalWrite(highPower_pin, bHighPower_state);
  digitalWrite(lowPower_pin, HIGH); // bLowPower_state);

  // digitalWrite(ledPin1, bHighPower_state);
  //  digitalWrite(ledPin2,bLowPower_state);
  

}





