/* =============================================================================
  LIDAR-Lite v2: Change the I2C address of a single sensor.

  This example demonstrates how to chage the i2c address of a single sensor.

  The library is in BETA, so subscribe to the github repo to recieve updates, or
  just check in periodically:
  https://github.com/PulsedLight3D/LIDARLite_v2_Arduino_Library

  To learn more read over lidarlite.cpp as each function is commented
=========================================================================== */

#include <Wire.h>
#include <LIDARLite.h>

//-----------------id leds-------------------
int ledPin1 = 4;            //small led on pcb
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

LIDARLite myLidarLite;


int sensorPins = A3; // Array of pins connected to the sensor Power Enable lines
int enableSensorPins = A3;
unsigned char address = 0x66;
int distance1 = 0;

void setup() {
  Serial.begin(115200);

  pinMode(speakerPin, OUTPUT); 
  pinMode(ledPin1, OUTPUT); 
  delay(1000);
  myLidarLite.begin();
  myLidarLite.changeAddress(address,false);
  
  beep(speakerPin, 30, 500);
}

void loop() {

  if ( millis() > 2000 ) 
   {
      //Serial.print("Sensor 0x66: ");
      distance1 = myLidarLite.distance(true,true,0x66);
      //Serial.print(myLidarLite.distance(true,true,0x66));
      //Serial.println();
    
      checkLIDAR();
  }
}

void checkLIDAR(){
  for(int i=0; i<1; i++){
    if(distance1 < 200)
    {
       beep(speakerPin,30,500);
       digitalWrite(ledPin1, HIGH);
       Serial.print("Sensor 0x66: ");
       distance1 = myLidarLite.distance(true,true,0x66);
       Serial.print(distance1);
       Serial.println();
     } 
     else {
     digitalWrite(ledPin1, LOW);
   } 
  }
}
