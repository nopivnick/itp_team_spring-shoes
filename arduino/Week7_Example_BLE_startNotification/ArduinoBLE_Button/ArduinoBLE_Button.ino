/*
  Button LED

  This example creates a BLE peripheral with service that contains a
  characteristic to represents the state of the button.

  The circuit:
  - Arduino MKR WiFi 1010
  - Button connected to pin 4

  This example code is in the public domain.
*/

#include <ArduinoBLE.h>

// TODO: set pin for button to manually send "SKIP!" over serial and skipState = true over BLE for troubleshooting
//const int skipStateButtonPin = 0;

// create a service for
BLEService shoeService("b6292c11-911a-4a51-b7f2-43fe53e62a77");

// create a characteristic for sending skipState over BLE and allow remote device to read and write
BLEBoolCharacteristic skipStateCharacteristic("b6292c11-911a-4a51-b7f2-43fe53e62a77", BLERead | BLENotify);


void setup() {
  Serial.begin(9600);
  // comment out the line below to begin BLE w/o having to open the serial monitor
  //  while (!Serial);  // stops the code from running beyond this point w/o serial monitor (for debugging)

  // set pin for button to manually send skipState = true over serial and BLE for troubleshooting
  //  pinMode(skipStateButtonPin, INPUT); // use button pin as an input

  // begin BLE initialization
  if (!BLE.begin()) {
    Serial.println("starting BLE failed!");

    while (1);
  }

  // set the local name peripheral advertises
  BLE.setLocalName("Spring Shoes");

  // set the UUID for the service this peripheral advertises:
  BLE.setAdvertisedService(shoeService);

  // add characteristic9s) to the service
  shoeService.addCharacteristic(skipStateCharacteristic);

  // add the BLE service
  BLE.addService(shoeService);

  // set initial characteristic(s) value(s)
  skipStateCharacteristic.writeValue(0);

  // start advertising BLE service
  BLE.advertise();

  Serial.println("Bluetooth device active, waiting for connections...");
}

void loop() {
  // poll for BLE events
  BLE.poll();

  //  // TODO: use button to manually send "SKIP!" over serial and skipState = true over BLE for troubleshooting
  //    bool skipStateButtonValue = digitalRead(skipStateButtonPin);
  //    if (skipStateButtonValue) {
  //      digitalWrite(LED_BUILTIN, HIGH);
  //    } else {
  //      digitalWrite(LED_BUILTIN, LOW);
  //    }
  //    skipStateCharacteristic.writeValue(skipStateButtonValue);

}
