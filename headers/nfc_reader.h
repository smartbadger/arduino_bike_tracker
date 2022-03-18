#ifndef NFC_H
#define NFC_H

#include <Adafruit_PN532.h>

class NfcReader
{
  Adafruit_PN532 _nfc;
  bool _nfcReady;

public:
  NfcReader(int PN532_IRQ, int PN532_RESET);
  ~NfcReader();
  void setup();
  bool nfcIsAuthorized();
};


#endif