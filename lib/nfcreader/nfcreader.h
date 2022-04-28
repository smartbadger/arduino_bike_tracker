#pragma once
#ifndef _NFCREADER_H
#define _NFCREADER_H
#include <Adafruit_PN532.h>

class NfcReader: public Adafruit_PN532
{
  public:
    NfcReader(int PN532_IRQ, int PN532_RESET, const char *key);
    bool _nfcReady;
    bool isAuthorized();
    String readNFC();
    void setup();
  private: 
    const char *_securityKey;
};

#endif