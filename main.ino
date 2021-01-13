void loop() {
  // get time elapsed, replaces delay() for 'hyper-threading' 
  long elapsedTime = millis() - runningTime; 
  runningTime += elapsedTime;

  // put your main code here, to run repeatedly:
  if(bikeState >= 1) {
    bikeState = nfcAuthentication(elapsedTime);
  }
  motionDetection(elapsedTime);
}
