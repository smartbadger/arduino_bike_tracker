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

void readGSM() {
  ArduinoCloud.update();
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

void initProperties() {
#if defined(BOARD_ESP8266)
  ArduinoCloud.setBoardId(BOARD_ID);
  ArduinoCloud.setSecretDeviceKey(SECRET_DEVICE_KEY);
#endif
  ArduinoCloud.setThingId(THING_ID);
#if defined(BOARD_HAS_WIFI) || defined(BOARD_HAS_GSM) || defined(BOARD_HAS_NB)
  ArduinoCloud.addProperty(switchButton, WRITE, ON_CHANGE, onSwitchButtonChange);
  ArduinoCloud.addProperty(location, READ, ON_CHANGE);
  ArduinoCloud.addProperty(color, READWRITE, ON_CHANGE, onColorChange);
#elif defined(BOARD_HAS_LORA)
  ArduinoCloud.addProperty(switchButton, 1, WRITE, ON_CHANGE, onSwitchButtonChange);
  ArduinoCloud.addProperty(location, 2, READ, ON_CHANGE);
  ArduinoCloud.addProperty(color, 3, READWRITE, ON_CHANGE, onColorChange);
#endif
}

#if defined(BOARD_HAS_WIFI)
  WiFiConnectionHandler ArduinoIoTPreferredConnection(SECRET_SSID, SECRET_PASS);
#elif defined(BOARD_HAS_GSM)
  GSMConnectionHandler ArduinoIoTPreferredConnection(SECRET_PIN, SECRET_APN, SECRET_LOGIN, SECRET_PASS);
#elif defined(BOARD_HAS_LORA)
  LoRaConnectionHandler ArduinoIoTPreferredConnection(SECRET_APP_EUI, SECRET_APP_KEY, _lora_band::EU868, _lora_class::CLASS_A);
#elif defined(BOARD_HAS_NB)
  NBConnectionHandler ArduinoIoTPreferredConnection(SECRET_PIN, SECRET_APN, SECRET_LOGIN, SECRET_PASS);
#endif
