#include "nfcreader.h"
#include "debugger.h"
NfcReader::NfcReader(int PN532_IRQ, int PN532_RESET, const char *key) : Adafruit_PN532(PN532_IRQ, PN532_RESET) {
    _nfcReady = false;
    _securityKey = key;
}

String NfcReader::readNFC()
{
    uint8_t data[16];
    uint8_t success;
    uint8_t uid[] = {0, 0, 0, 0, 0, 0, 0}; // Buffer to store the returned UID
    uint8_t uidLength;
    String result = "";
    uint8_t keya[6] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}; // Try with the factory default KeyA: 0xFF 0xFF 0xFF 0xFF 0xFF 0xFF
    debuglnV("Waiting for an ISO14443A Card ...");
    
        uint32_t versiondata = getFirmwareVersion();
            if (!versiondata || !_nfcReady)
    {
        debuglnV("Couldn't access PN532");
        _nfcReady = false;
        setup();
        return result;
    }
    success = readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength, 1500);
    if (success && uidLength == 4)
    {
        debuglnV("Found an ISO14443A card, Trying to authenticate block 4 with default KEYA value");
        // Start with block 4 (the first block of sector 1) since sector 0
        // contains the manufacturer data and it's probably better just
        // to leave it alone unless you know what you're doing
        if (mifareclassic_AuthenticateBlock(uid, uidLength, 4, 0, keya))
        {
            debuglnV("Sector 1 (Blocks 4..7) has been authenticated");

            // Try to read the contents of block 4
            if (mifareclassic_ReadDataBlock(4, data))
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

bool NfcReader::isAuthorized()
{
    return (String(_securityKey) == readNFC());
}

void NfcReader::setup()
{
    debuglnV("PN53x Setup");
    begin();
    uint32_t versiondata = getFirmwareVersion();
    if (!versiondata)
    {
        debuglnV("Didn't find PN53x board");
        // set variable to false
        return;
    }
    // Got ok data, print it out!
    Serial.print("Found chip PN5"); Serial.println((versiondata>>24) & 0xFF, HEX); 
    Serial.print("Firmware ver. "); Serial.print((versiondata>>16) & 0xFF, DEC); 
    Serial.print('.'); Serial.println((versiondata>>8) & 0xFF, DEC);

    // configure board to read RFID tags
    SAMConfig();
    _nfcReady = true;
}