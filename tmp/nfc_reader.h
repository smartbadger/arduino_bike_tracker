#ifndef _NFC_H
#define _NFC_H

#include <Adafruit_PN532.h>

class NfcReader
{
  bool _nfcReady;
  Adafruit_PN532 _nfc;

public:
  NfcReader(int PN532_IRQ, int PN532_RESET) : _nfc(PN532_IRQ, PN532_RESET)
  {
    _nfcReady = false;
  }

  bool nfcAuthentication()
  {

    if (!_nfcReady)
    {
      setupNFC();
      return false;
    }
    return nfcIsAuthorized();
  }

  // NFC READER AUTHORIZATION
  //=================================
  bool nfcIsAuthorized()
  {
    uint8_t success;
    uint8_t uid[] = {0, 0, 0, 0, 0, 0, 0}; // Buffer to store the returned UID
    uint8_t uidLength;
    String tagId = "m6KbRFp9BWb75tNd";

    success = _nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength, 500);
    if (success)
    {
      // Display some basic information about the card
      Serial.println("Found an ISO14443A card");
      Serial.print("  UID Length: ");
      Serial.print(uidLength, DEC);
      Serial.println(" bytes");
      Serial.print("  UID Value: ");
      _nfc.PrintHex(uid, uidLength);
      Serial.println("");

      if (uidLength == 4)
      {
        // We probably have a Mifare Classic card ...
        Serial.println("Seems to be a Mifare Classic card (4 byte UID)");

        // Now we need to try to authenticate it for read/write access
        // Try with the factory default KeyA: 0xFF 0xFF 0xFF 0xFF 0xFF 0xFF
        Serial.println("Trying to authenticate block 4 with default KEYA value");
        uint8_t keya[6] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

        // Start with block 4 (the first block of sector 1) since sector 0
        // contains the manufacturer data and it's probably better just
        // to leave it alone unless you know what you're doing
        success = _nfc.mifareclassic_AuthenticateBlock(uid, uidLength, 4, 0, keya);

        if (success)
        {
          Serial.println("Sector 1 (Blocks 4..7) has been authenticated");
          uint8_t data[16];
          // Try to read the contents of block 4
          success = _nfc.mifareclassic_ReadDataBlock(4, data);
          if (success)
          {
            String result = "";
            for (uint8_t i = 0; i < 16; i++)
            {
              result += (char)data[i];
            }
            Serial.println(result);
            if (result == tagId)
            {
              Serial.println("tag found");
              return true;
            }
          }
        }
      }
    }
    return false;
  }

  // SETUP NFC
  //=================================
  void setupNFC(void)
  {
    Serial.println("PN53x Setup");
    _nfc.begin();

    uint32_t versiondata = _nfc.getFirmwareVersion();
    if (!versiondata)
    {
      Serial.print("Didn't find PN53x board");
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
    Serial.println("Waiting for an ISO14443A Card ...");
    _nfcReady = true;
  }
};

#endif