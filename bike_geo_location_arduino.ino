// GLOBAL
#include <Wire.h>
#include <SPI.h>
#include "secrets.h"
#include "gsm_interface.h"
#include "debug.h"

// setup debugger;
Debug db = Debug(true);
GSMInterface network = GSMInterface(20000);
void setup() {
    db.setup();
    network.setup();

}

void loop() {
    network.loop();
}