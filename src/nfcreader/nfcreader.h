#pragma once
#ifndef _NFCREADER_H
#define _NFCREADER_H
#include "secrets.h"
#include "debugger.h"
#include "config.h"
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_PN532.h>
namespace nfcReaderpn
{
  bool _nfcReady = false;
  Adafruit_PN532 _nfc(PN532_IRQ, PN532_RESET);

  String readNFC()
  {
    uint8_t data[16];
    uint8_t success;
    uint8_t uid[] = {0, 0, 0, 0, 0, 0, 0}; // Buffer to store the returned UID
    uint8_t uidLength;
    String result = "";
    uint8_t keya[6] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}; // Try with the factory default KeyA: 0xFF 0xFF 0xFF 0xFF 0xFF 0xFF
    debuglnV("Waiting for an ISO14443A Card ...");

    uint32_t versiondata = _nfc.getFirmwareVersion();
    if (!versiondata || !_nfcReady)
    {
      debuglnV("Couldn't access PN532");
      _nfcReady = false;
      // setup();
      return result;
    }
    success = _nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength, 1500);
    if (success && uidLength == 4)
    {
      debuglnV("Found an ISO14443A card, Trying to authenticate block 4 with default KEYA value");
      // Start with block 4 (the first block of sector 1) since sector 0
      // contains the manufacturer data and it's probably better just
      // to leave it alone unless you know what you're doing
      if (_nfc.mifareclassic_AuthenticateBlock(uid, uidLength, 4, 0, keya))
      {
        debuglnV("Sector 1 (Blocks 4..7) has been authenticated");

        // Try to read the contents of block 4
        if (_nfc.mifareclassic_ReadDataBlock(4, data))
        {
          for (uint8_t i = 0; i < 16; i++)
          {
            result += (char)data[i];
          }
        }
      }
    }
    Serial.println(success);
    Serial.println(uidLength);
    return result;
  }

  bool isAuthorized()
  {
    return (String(RFID_KEY) == readNFC());
  }

  void setup()
  {

    debuglnV("PN53x Setup");
    _nfc.begin();
    uint32_t versiondata = _nfc.getFirmwareVersion();
    if (!versiondata)
    {
      debuglnV("Didn't find PN53x board");
      // set variable to false
      return;
    }
    // Got ok data, print it out!
    Serial.print("Found chip PN5");
    Serial.println((versiondata >> 24) & 0xFF, HEX);
    Serial.print("Firmware ver. ");
    Serial.print((versiondata >> 16) & 0xFF, DEC);
    Serial.print('.');
    Serial.println((versiondata >> 8) & 0xFF, DEC);

    // configure board to read RFID tags
    _nfc.SAMConfig();
    _nfcReady = true;
  }
}

#endif