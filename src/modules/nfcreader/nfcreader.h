#pragma once
#ifndef _NFCREADER_H
#define _NFCREADER_H
#include "secrets.h"
#include "debugger.h"
#include "config.h"
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_PN532.h>
namespace NFC
{

  Adafruit_PN532 nfcModule(PN532_IRQ, PN532_RESET);
  uint8_t success;
  String readNFC()
  {
    uint8_t data[16];
    uint8_t uid[] = {0, 0, 0, 0, 0, 0, 0}; // Buffer to store the returned UID
    uint8_t uidLength;
    String result = "";
    uint8_t keya[6] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}; // Try with the factory default KeyA: 0xFF 0xFF 0xFF 0xFF 0xFF 0xFF
    debuglnV("Waiting for an ISO14443A Card ...");

    success = nfcModule.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength, 1500);
    if (success && uidLength == 4)
    {
      // TODO: update state
      debuglnV("Found an ISO14443A card, Trying to authenticate block 4 with default KEYA value");
      if (nfcModule.mifareclassic_AuthenticateBlock(uid, uidLength, 4, 0, keya))
      {
        debuglnV("Sector 1 (Blocks 4..7) has been authenticated");

        // Try to read the contents of block 4
        if (nfcModule.mifareclassic_ReadDataBlock(4, data))
        {
          for (uint8_t i = 0; i < 16; i++)
          {
            result += (char)data[i];
          }
        }
      }
    }
    debuglnV(success);
    debuglnV(uidLength);
    return result;
  }

  bool isAuthorized()
  {
    if (nfcModule.getFirmwareVersion())
    {
      return (String(RFID_KEY) == readNFC());
    }
    return false;
  }

  void setup()
  {

    debuglnV("PN53x Setup");
    nfcModule.begin();
    uint32_t versiondata = nfcModule.getFirmwareVersion();
    if (!versiondata)
    {
      debuglnV("Didn't find PN53x board");
      return;
    }
    nfcModule.SAMConfig();
  }
}

#endif