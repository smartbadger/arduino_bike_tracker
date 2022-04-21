#ifndef _NFC_H
#define _NFC_H
#include <Adafruit_PN532.h>

class NfcReader
{
  bool _nfcReady;
  const char *_key;
  Adafruit_PN532 _nfc;

public:
  NfcReader(int PN532_IRQ, int PN532_RESET, const char *key);
  bool isAuthorized();
  String readNFC();
  void setup();
};

#endif