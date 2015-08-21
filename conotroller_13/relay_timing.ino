void relay_switching(){

  if(relaySwitching_stage == 0 && millis() - relayTimer < 500){
    Serial.println("stage 1 high , high");
    bHighPower_state = 1;
    bLowPower_state = 1;
    relaySwitching_stage = 1;
  }

  if(relaySwitching_stage == 1 && millis() - relayTimer > 500 && millis() - relayTimer < 3000){
    Serial.println("stage 2 high , low");
    bHighPower_state = 1;
    bLowPower_state = 0;
    relaySwitching_stage = 2;
  }
  if(relaySwitching_stage == 2 && millis() - relayTimer >= 3000 && millis() - relayTimer < 3500){
    Serial.println("stage 3 high , high");
    bHighPower_state = 1;
    bLowPower_state = 1;
    relaySwitching_stage = 3;
  }
  if(relaySwitching_stage == 3 && millis() - relayTimer > 3500){
    Serial.println("stage 4 low , high");
    bHighPower_state = 0;
    bLowPower_state = 1;
    relaySwitching_stage = 4;
  }

  if(relaySwitching_stage == 4 &&millis() - relayTimer > 6000){
    Serial.println("stage 5 end");
    relayTimer = millis();
    relaySwitching_stage = 0;
  }


  digitalWrite(highPower_pin,bHighPower_state);
    digitalWrite(lowPower_pin,bLowPower_state);

    digitalWrite(ledPin1,bHighPower_state);
  //  digitalWrite(ledPin2,bLowPower_state);
    
}




