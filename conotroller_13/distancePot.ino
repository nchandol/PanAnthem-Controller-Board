void setup_distancePot() {

}

void checkDistancePot() {

  potReads[potReadsCnt] = analogRead(distancePot_pin);
  potReadsCnt++;
  potReadsCnt = potReadsCnt % potREADS;

  int temp_sensorVal = 0;
  for (int i = 0; i < potREADS; i++) {
    temp_sensorVal += potReads[i];
  }
  maxDistance = temp_sensorVal / potREADS;
  maxDistance = map(maxDistance, 0, 1024, 0, 800); //max read distance can be up to 40 m = 4000 cm

  if (bDebug || millis() < 2000) {
    Serial.print("maxDistance ");
    Serial.println(maxDistance);
  }
}

