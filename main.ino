void loop() {
  // get time elapsed, replaces delay() for 'hyper-threading' 
  long elapsedTime = millis() - runningTime;
  static bool motionTrigger = false; 
  runningTime += elapsedTime;

  // put your main code here, to run repeatedly:
  if(bikeState >= 1) {
    bikeState = nfcAuthentication(elapsedTime);
    // check if bike is on, power up if not
  }
  bool motion = motionDetection(elapsedTime);

  if(motionTrigger == false && motion){
    Serial.println("MOTION DETECTED");
    // set some delay or action
    // security.triggeralarm()
  }
  
  //GPS location ping
  //recive gsm commands
  //check battery power
  //
  motionTrigger = motion;
  modemT();
  sendGSM();
}
