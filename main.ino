void loop() {
  // get time elapsed, replaces delay() for 'hyper-threading' 
  long elapsedTime = micros() - runningTime; 
  runningTime += elapsedTime;

  // put your main code here, to run repeatedly:
  nfcAuthentication(elapsedTime);
  readGyro(elapsedTime);
}