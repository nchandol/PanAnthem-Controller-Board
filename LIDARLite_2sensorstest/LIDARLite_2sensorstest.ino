/* =============================================================================
  LIDAR-Lite v2: Change the I2C address of multiple sensors with PWR_EN line

  This example demonstrates how to chage the i2c address of multiple sensors.

  The library is in BETA, so subscribe to the github repo to recieve updates, or
  just check in periodically:
  https://github.com/PulsedLight3D/LIDARLite_v2_Arduino_Library

  To learn more read over lidarlite.cpp as each function is commented
=========================================================================== */

#include <Wire.h>
#include <LIDARLite.h>


//-----------------LIDARLite-------------------
int sensorPins[] = {2, A3}; // Array of pins connected to the sensor Power Enable lines
int enableSensorPins[] = {2, A3};
unsigned char addresses[] = {0x66,0x68};
//unsigned char address[] = {0x66};
int distance1 = 0;
int distance2 = 0;

LIDARLite myLidarLite;

//-----------------Buzzer-------------------
int sensorBuzzFreq[] = {
  30, 300
};
int sensorBuzzDur[] = {
  500, 100
};
int speakerPin = 9;

//-----------------Setup-------------------
boolean enableOnce = true; //false;


void setup() {
  Serial.begin(115200);
  pinMode(speakerPin, OUTPUT);
  for (int i = 0; i < 2; i++) {
    pinMode(enableSensorPins[i], OUTPUT); // Pin to first LIDAR-Lite Power Enable line
    //digitalWrite(enableSensorPins[i], HIGH);  
}
  digitalWrite(enableSensorPins[0], HIGH); 
  //digitalWrite(enableSensorPins[1], HIGH); 
  
  for (int i = 0; i < 2; i++) {
    beep(speakerPin, sensorBuzzFreq[i], sensorBuzzDur[i]);
    delay(200);
  }
  
  delay(1000);
  myLidarLite.begin();
  //myLidarLite.beginContinuous();
  myLidarLite.changeAddressMultiPwrEn(2,sensorPins,addresses,false);
  
 
}

void loop() {
   
   if ( millis() > 2000 ) 
   {
      //Serial.print("Sensor 0x66: ");
      distance1 = myLidarLite.distance(true,true,0x66);
      //Serial.print(myLidarLite.distance(true,true,0x66));
      //Serial.print(", Sensor 0x68: ");
      distance2 = myLidarLite.distance(true,true,0x68);
      //Serial.print(myLidarLite.distance(true,true,0x68));
      //Serial.println();
      checkLIDAR();
  }
  

}

void checkLIDAR(){
  for(int i=0; i<2; i++){
    if((distance1 < 200)||(distance2 < 200))
    {
       beep(speakerPin,sensorBuzzFreq[i],sensorBuzzDur[i]);
       Serial.print("Sensor 0x66: ");
       Serial.print(distance1);
       Serial.print(", Sensor 0x68: ");
       Serial.print(distance2);
       Serial.println();
     }  
  }
}
