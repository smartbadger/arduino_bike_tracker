/*
  This sketch demonstrates how to use more complex cloud data types such as a colour or coordinates.

  This sketch is compatible with:
   - MKR 1000
   - MKR WIFI 1010
   - MKR GSM 1400
   - MKR NB 1500
   - MKR WAN 1300/1310
   - ESP 8266
*/
/*
void readGSM() {
//  ArduinoCloud.update();
}

void onSwitchButtonChange() {
  if (switchButton)
  {
    location = Location(latMov, lonMov);
    color    = Color(hueRed, satRed, briRed);
  }
  else
  {
    location = Location(latArd, lonArd);
    color    = Color(hueGreen, satGreen, briGreen);
  }
}

void onColorChange() {
  Serial.print("Hue = ");
  Serial.println(color.getValue().hue);
  Serial.print("Sat = ");
  Serial.println(color.getValue().sat);
  Serial.print("Bri = ");
  Serial.println(color.getValue().bri);
}

//=================================



void onSwitchButtonChange();
void onColorChange();

bool switchButton;
CloudLocation location;
CloudColor color;


 * void initProperties() {

  ArduinoCloud.setThingId(THING_ID);
  GSMConnectionHandler ArduinoIoTPreferredConnection(SECRET_PIN, SECRET_APN, SECRET_LOGIN, SECRET_PASS);

  ArduinoCloud.addProperty(switchButton, WRITE, ON_CHANGE, onSwitchButtonChange);
  ArduinoCloud.addProperty(location, READ, ON_CHANGE);
  ArduinoCloud.addProperty(color, READWRITE, ON_CHANGE, onColorChange);

}
 * 
  */
