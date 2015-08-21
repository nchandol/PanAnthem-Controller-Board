
void checkLIDAR(){

  // if(bDebug) Serial.println("start check LIDAR");
  // bSensorsTriggered = false;
  boolean temp_trigger = false;

  for(int i=0; i<enableSensorPinsArraySize; i++){
    if(bDebug){
      if(i == 1) Serial.print("\t\t");
      Serial.print(i);
    }
  
      int averagedDistance = getAveraged_LIDAR(i); //readDistance();
    if(bDebug){
      Serial.print(" ");
      Serial.print(averagedDistance); // Read Distance from Sensor
    }
    if(averagedDistance < maxDistance){
      temp_trigger = true;
      Serial.print(averagedDistance);
      if(bDebugMode) beep(speakerPin,sensorBuzzFreq[i],sensorBuzzDur[i]);
      if(bDebug)  Serial.print(" <");
    }
    else{
      if(bDebug)  Serial.print(" |");
    }

    if(bDebug) {
      Serial.print(" ");
      Serial.print(maxDistance);
      
    }
  }

  if(temp_trigger == true){ //averagedDistance < maxDistance){
    triggerOffTimer = millis();

    if(millis() - triggerOnTimer > triggerOnDuration){
      bSensorsTriggered = true;
      if(bDebug)  Serial.print("\t\t on ");
    }
  }
  else{
    triggerOnTimer = millis(); 
    if(millis() - triggerOffTimer > triggerOffDuration){
      bSensorsTriggered = false;
      if(bDebug)  Serial.print("\t\t off ");
    }
  }

  if(bDebug) Serial.println(); // Add line return

}


int getAveraged_LIDAR(int _id){
  //get rolling avergage

  // read from the sensor:  

  int temp_reading = myLidarLite.distance(true,true,addresses[_id]);
  if(bDebug){
    Serial.print(" ");
    Serial.print(temp_reading);

  }
  if(temp_reading > 5 && temp_reading < 999){ //sometimes sensor read value that is way too big

    // subtract the last reading:
    LIDAR_total[_id] = LIDAR_total[_id] - LIDAR_readings[_id][LIDAR_index[_id]];        
    //button1_total = constrain(button1_total,0,1023) ;


    LIDAR_readings[_id][LIDAR_index[_id]] = temp_reading; //analogRead(temp_pin); 
    // add the reading to the total:
    LIDAR_total[_id] = LIDAR_total[_id] + LIDAR_readings[_id][LIDAR_index[_id]];       
    // advance to the next position in the array:  
    LIDAR_index[_id] = LIDAR_index[_id] + 1;                    

    // if we're at the end of the array...
    if (LIDAR_index[_id] >= LIDAR_numReadings)  {            
      // ...wrap around to the beginning: 
      LIDAR_index[_id] = 0;                      
    }    
  }
  // calculate the average:
  int average = LIDAR_total[_id] / LIDAR_numReadings;         
  // send it to the computer as ASCII digits
      Serial.println(average);   

  return average;
}


/*
int readDistance(){

  LIDAR_i2cCount = 0;
  uint8_t nackack = 100; // Setup variable to hold ACK/NACK resopnses     
  while (nackack != 0){ // While NACK keep going (i.e. continue polling until sucess message (ACK) is received )
    nackack = I2c.write(LIDARLite_ADDRESS,RegisterMeasure, MeasureValue); // Write 0x04 to 0x00
    delay(1); // Wait 1 ms to prevent overpolling

    LIDAR_i2cCount++;
    if(LIDAR_i2cCount > LIDAR_i2cMax){
      i2cError = 1;
      LIDAR_i2cCount = 0;
      break; 
    }

  }


  //if(i2cError == false){
  byte distanceArray[2]; // array to store distance bytes from read function
  LIDAR_i2cCount = 0;
  // Read 2byte distance from register 0x8f
  nackack = 100; // Setup variable to hold ACK/NACK resopnses     
  while (nackack != 0){ // While NACK keep going (i.e. continue polling until sucess message (ACK) is received )
    nackack = I2c.read(LIDARLite_ADDRESS,RegisterHighLowB, 2, distanceArray); // Read 2 Bytes from LIDAR-Lite Address and store in array
    delay(1); // Wait 1 ms to prevent overpolling

    LIDAR_i2cCount++;
    if(LIDAR_i2cCount > LIDAR_i2cMax){
      i2cError = 2;
      LIDAR_i2cCount = 0;
      break; 
    }

  }
  int distance = (distanceArray[0] << 8) + distanceArray[1];  // Shift high byte [0] 8 to the left and add low byte [1] to create 16-bit int

  // }

  return distance;   // Print Sensor Name & Distance
}
*/




















