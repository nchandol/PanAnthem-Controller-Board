int check_button1(){

  int temp_pin = buttonPin1;
  // subtract the last reading:
  button1_total = button1_total - button1_readings[button1_index];        
  //button1_total = constrain(button1_total,0,1023) ;
  // read from the sensor:  
  button1_readings[button1_index] = analogRead(temp_pin); 
  // add the reading to the total:
  button1_total = button1_total + button1_readings[button1_index];       
  // advance to the next position in the array:  
  button1_index = button1_index + 1;                    

  // if we're at the end of the array...
  if (button1_index >= numReadings)              
    // ...wrap around to the beginning: 
    button1_index = 0;                           

  // calculate the average:
  int average = button1_total / numReadings;         
  // send it to the computer as ASCII digits
  //    Serial.println(average);   

  return average;
}


boolean checkDebugMode(){
  boolean temp_bDebugMode = false;
  
  if(millis()-buttonReadTimer > buttonReadDuration){
    buttonReadTimer = millis();
    button1_value = check_button1();
  }
  if(button1_value < 20){
    temp_bDebugMode = true;
    //beep(speakerPin,sensorBuzzFreq[0],sensorBuzzDur[0]);
  }
  
  return temp_bDebugMode;
}

