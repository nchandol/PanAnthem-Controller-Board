/* 
 http://pulsedlight3d.com
 This sketch demonstrates getting distance from multiple LIDAR-Lite sensors by enabling the Power Enable line of the sensor 
 to read from, and disabling the others.
 It utilizes the 'Arduino I2C Master Library' from DSS Circuits:
 http://www.dsscircuits.com/index.php/articles/66-arduino-i2c-master-library 
 You can find more information about installing libraries here:
 http://arduino.cc/en/Guide/Libraries
 
 https://github.com/PulsedLight3D/LIDARLite_MultiSensor_using_PowerEnable_Line
 */

#include <I2C.h>
#define    LIDARLite_ADDRESS     0x62          // Default I2C Address of LIDAR-Lite.
#define    RegisterMeasure       0x00          // Register to write to initiate ranging.
#define    MeasureValue          0x04          // Value to initiate ranging.
#define    RegisterHighLowB      0x8f          // Register to get both High and Low bytes in 1 call.

int sensorPins[] = {
  A3,2}; // Array of pins connected to the sensor Power Enable lines
int sensorPinsArraySize = 2; // The length of the array
int maxDistance = 350;

int sensorBuzzFreq[] = {
  30,300};
int sensorBuzzDur[] = {
  500,100};
int speakerPin = 9;

int distancePot_pin = A6;
int old_PotRead;
int potJitter = 50;

#define potREADS 10
int potReads[potREADS];
int potReadsCnt = 0;

void setup(){
  Serial.begin(115200); //Opens serial connection at 115200bps.     
  I2c.begin(); // Opens & joins the irc bus as master
  delay(100); // Waits to make sure everything is powered up before sending or receiving data  
  I2c.timeOut(50); // Sets a timeout to ensure no locking up of sketch if I2C communication fails

  pinMode(speakerPin,OUTPUT);

  for (int i = 0; i < sensorPinsArraySize; i++){
    pinMode(sensorPins[i], OUTPUT); // Pin to first LIDAR-Lite Power Enable line
    // Serial.print(sensorPins[i]);
  }

  beep(speakerPin,sensorBuzzFreq[0],sensorBuzzDur[0]);
  delay(500);
  beep(speakerPin,sensorBuzzFreq[1],sensorBuzzDur[1]);

  Serial.println("setup done");
}

void loop(){

  potReads[potReadsCnt] = analogRead(distancePot_pin);
  potReadsCnt++;
  potReadsCnt = potReadsCnt % potREADS;

  int temp_sensorVal = 0;
  for (int i = 0; i < potREADS; i++) {
    temp_sensorVal += potReads[i];
  }
  maxDistance = temp_sensorVal / potREADS;
  maxDistance = map(maxDistance,0,1024,0,800); //max read distance can be up to 40 m = 4000 cm

  for(int i=0; i<sensorPinsArraySize; i++){
    enableDisableSensor(sensorPins[i]); // Turn on sensor attached to pin 2 and disable all others
    int dist = readDistance();
    Serial.print(dist); // Read Distance from Sensor
    Serial.print(" | ");
    Serial.print(maxDistance);

    if(dist < maxDistance){
      beep(speakerPin,sensorBuzzFreq[i],sensorBuzzDur[i]);
    }
    Serial.print("\t"); // Print "." to separate readings
  }

  Serial.println("."); // Add line return
}


void enableDisableSensor(int sensorPin){
  for (int i = 0; i < sensorPinsArraySize; i++){
    digitalWrite(sensorPins[i], LOW); // Turn off all sensors
  }
  digitalWrite(sensorPin, HIGH); // Turn on the selected sensor
  delay(1); // The sensor takes 1msec to wake
}

int readDistance(){
  uint8_t nackack = 100; // Setup variable to hold ACK/NACK resopnses     
  while (nackack != 0){ // While NACK keep going (i.e. continue polling until sucess message (ACK) is received )
    nackack = I2c.write(LIDARLite_ADDRESS,RegisterMeasure, MeasureValue); // Write 0x04 to 0x00
    delay(1); // Wait 1 ms to prevent overpolling
  }

  byte distanceArray[2]; // array to store distance bytes from read function

  // Read 2byte distance from register 0x8f
  nackack = 100; // Setup variable to hold ACK/NACK resopnses     
  while (nackack != 0){ // While NACK keep going (i.e. continue polling until sucess message (ACK) is received )
    nackack = I2c.read(LIDARLite_ADDRESS,RegisterHighLowB, 2, distanceArray); // Read 2 Bytes from LIDAR-Lite Address and store in array
    delay(1); // Wait 1 ms to prevent overpolling
  }
  int distance = (distanceArray[0] << 8) + distanceArray[1];  // Shift high byte [0] 8 to the left and add low byte [1] to create 16-bit int

  return distance;   // Print Sensor Name & Distance

}



