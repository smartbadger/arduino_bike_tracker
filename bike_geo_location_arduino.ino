// GLOBAL
#include <Wire.h>
#include <SPI.h>
#include "secrets.h"
#include "gsm_interface.h"

// setup debugger;

GSMInterface network = GSMInterface(20000);
void setup() {

    Serial.begin(115200);
    while (!Serial)
            delay(10); // will pause Zero, Leonardo, etc until serial console opens

    network.setup();

}

void loop() {
    network.loop();
}