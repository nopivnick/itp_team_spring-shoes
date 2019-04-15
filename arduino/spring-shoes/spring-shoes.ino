/*
  Spring Shoes
  Idit Barak and Noah Pivnick
  2189.00 Intro to Wearables (Jingwen Zhu)

  This sketch borrows code from the following examples:

  - FSR simple testing sketch on AdaFruit's site
    https://learn.adafruit.com/force-sensitive-resistor-fsr/using-an-fsr
  - Analog Sensor Threshold Detection on ITP's Intro to Physical Computing Site
    https://itp.nyu.edu/physcomp/labs/labs-arduino-digital-and-analog/lab-sensor-change-detection/
  - BLE start notification example on Jingwen Zhu's ITP Intro to Wearables github repo
    https://github.com/ZhuJingwen/intro-to-wearables-2019/tree/1b629b750c70ed50bbc4415cd270ffeea3e7c945/Week%207%20Examples/Week7_Example_BLE_startNotification

  Wiring:

  Connect one lead of the first FSR to power, the other lead to Analog pin 1.
  Connect one lead of the second FSR to power, the other lead to Analog pin 2.
  Connect one end of a 10K resistor from Analog pin 1 to ground
  Connect one end of a 10K resistor from Analog pin 2 to ground
  Connect blue LED from pin 8 through a 220 resistor to ground
  Connect red LED from pin 7 through a 220 resistor to ground

*/

const int fsrAnalogPinBall = 1;  // FSR is connected to analog pin 1
const int fsrAnalogPinHeel = 2;  // FSR is connected to analog pin 2

int ledDigitalPinBall = 8;       // connect Blue LED to pin 6
int ledDigitalPinHeel = 7;       // connect Red LED to pin 7

int fsrReadingBall;              // the analog reading from the FSR sensor under the ball of the foot
int fsrReadingHeel;              // the analog reading from the FSR sensor under the heel of the foot

int lastFsrReadingBall;          // previous reading of FSR sensor under the ball of the foot
int lastFsrReadingHeel;          // previous reading of FSR sensor under the ball of the foot

int ballThreshold = 925;         // threshold sensor value used to detect Ball strike
int heelThreshold = 700;         // threshold sensor value used to detect Heel strike

int ballCount = 0;               // temporary Ball strike counter for detecting skips
int heelCount = 0;               // temporary Heel strike counter for detecting skips

int totalBallCount = 0;          // cumulative Ball strike counter
int totalHeelCount = 0;          // cumulative Heel strike counter

int totalSkipCount = 0;          // cumulative skip counter

bool skipState = false;
bool skipStateBall = false;
bool skipStateHeel = false;

const int heel = 0;              // assign a numerical value to an instance of heel FSR passing threshold
const int ball = 1;              // assign a numerical value to an instance of ball FSR passing threshold
const int wash = 2;              // assign a nemerical value to wash the fsrPattern array after a skip
int fsrPattern[3];               // declare an array to store patterns and initialize all elements to 1 (ball)

// TODO: set pin for button to manually send "SKIP!" over serial and skipState = true over BLE for troubleshooting
//const int skipStateButtonPin = 0;

/*
   Bluetooth LE stuff
*/

#include <ArduinoBLE.h>

// create a service for
BLEService shoeService("b6292c11-911a-4a51-b7f2-43fe53e62a77");

// create a characteristic for sending skipState over BLE and allow remote device to read and write
BLEBoolCharacteristic skipStateCharacteristic("b6292c11-911a-4a51-b7f2-43fe53e62a77", BLERead | BLENotify);


void setup(void) {
  Serial.begin(9600);  // send debugging information to Serial Monitor
  pinMode(ledDigitalPinBall, OUTPUT);
  pinMode(ledDigitalPinHeel, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);  // onboard LED used to indicate a skip
  // TODO: set pin for button to manually send skipState = true over serial and BLE for troubleshooting
  //  pinMode(skipStateButtonPin, INPUT); // use button pin as an input

  /*
     Bluetooth LE stuff
  */

  // comment out the line below to begin BLE w/o having to open the serial monitor
  //  while (!Serial);  // stops the code from running beyond this point w/o serial monitor (for debugging)

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
  skipStateCharacteristic.writeValue(skipState);  // TODO: skipState is a boolean so let's use it

  // start advertising BLE service
  BLE.advertise();

  Serial.println("Bluetooth device active, waiting for connections...");

}

void loop(void) {
  // read force-sensing resistor values
  // fsrReadingBall = analogRead(fsrAnalogPinBall);
  // fsrReadingHeel = analogRead(fsrAnalogPinHeel);
  // send FSR readings to serial monitor for debugging
  //  Serial.print("BALL sensor = ");
  //  Serial.print(fsrReadingBall);
  //  Serial.print(" - ");
  //  Serial.print("HEEL sensor = ");
  //  Serial.println(fsrReadingHeel);

  /*
     FSR sensor threshold stuff
  */

  // read FSR sensor under Ball of foot
  fsrReadingBall = analogRead(fsrAnalogPinBall);
  // if FSR sensor under Ball of foot is *above* the threshold:
  if (fsrReadingBall >= ballThreshold) {
    // check that the previous value was below the threshold:
    if (lastFsrReadingBall < ballThreshold) {
      // print to serial monitor when FSR sensor under Ball of foot passes the threshold
      Serial.println();
      Serial.println(" *** BALL sensor ABOVE threshold *** ");
      Serial.println();
      // turn on ball threshold LED indicator
      digitalWrite(ledDigitalPinBall, HIGH);
      // increment Ball of foot strike counter
      ballCount++;
      Serial.print("ballCount = ");
      Serial.print(ballCount);
      Serial.println();
      totalBallCount++;
      Serial.print("totalBallCount = ");
      Serial.print(totalBallCount);
      Serial.println();
      // shift existing elements in fsrPattern over one and add ball strike to the first index
      fsrPattern[2] = fsrPattern[1];
      fsrPattern[1] = fsrPattern[0];
      fsrPattern[0] = ball;
      // print fsrPattern elements to serial monitor
      Serial.print("fsrPattern = ");
      Serial.print(fsrPattern[0]);
      Serial.print(fsrPattern[1]);
      Serial.print(fsrPattern[2]);
      Serial.println();
      // print skip state to serial monitor
      Serial.print("skipState = ");
      Serial.println(skipState);
    }
  }

  // save the last sensor reading for next comparison:
  lastFsrReadingBall = fsrReadingBall;

  // read FSR sensor under Heel of foot
  fsrReadingHeel = analogRead(fsrAnalogPinHeel);
  // if FSR sensor under Heel of foot is *above* the threshold:
  if (fsrReadingHeel >= heelThreshold) {
    // check that the previous value was below the threshold:
    if (lastFsrReadingHeel < heelThreshold) {
      // print to serial monitor when FSR sensor under Heel of foot passes the threshold
      Serial.println();
      Serial.println(" *** HEEL sensor ABOVE threshold *** ");
      Serial.println();
      // turn on heel threshold LED indicator
      digitalWrite(ledDigitalPinHeel, HIGH);
      // increment Ball of foot strike counter
      heelCount++;
      Serial.print("heelCount = ");
      Serial.print(heelCount);
      Serial.println();
      totalHeelCount++;
      Serial.print("totalHeelCount = ");
      Serial.print(totalHeelCount);
      Serial.println();
      // shift existing elements in fsrPattern over one and add heel strike to the first index
      fsrPattern[2] = fsrPattern[1];
      fsrPattern[1] = fsrPattern[0];
      fsrPattern[0] = heel;
      // print fsrPattern elements to serial monitor
      Serial.print("fsrPattern = ");
      Serial.print(fsrPattern[0]);
      Serial.print(fsrPattern[1]);
      Serial.print(fsrPattern[2]);
      Serial.println();
      // print skip state to serial monitor
      Serial.print("skipState = ");
      Serial.println(skipState);
    }
  }

  // save the last sensor reading for next comparison:
  lastFsrReadingHeel = fsrReadingHeel;

  /*
    Skip detection stuff
  */

  // ball & heel counter resets for detecting a skip pattern w/o using the fsrPattern array
  // if heelCount is greater than 1
  if (heelCount > 1) {
    // reset ballCount to 0
    ballCount = 0;
    // reset heelCount to 1
    heelCount = 1;
  }

  // skip detection logic using fsrPattern array
  if (fsrPattern[0] == 1 && fsrPattern[1] == 1 && fsrPattern[2] == 0) {  // if fsrPattern is a skip
    // print "SKIP!" to serial monitor (string used for p5 sketch over p5.serialcontrol)
    Serial.println();
    Serial.println("SKIP!");
    digitalWrite(LED_BUILTIN, HIGH);
    skipState = true;
    // reset ballCount to 0
    ballCount = 0;
    // reset heelCount to 0
    heelCount = 0;
    // reset the fsrPattern array
    fsrPattern[0] = wash;
    fsrPattern[1] = wash;
    fsrPattern[2] = wash;
  }

  /*
    Stomp detection stuff
  */

  // stomp detection logic using fsrPattern array
  if (fsrPattern[0] == 1 && fsrPattern[1] == 1 && fsrPattern[2] == 1) {  // if fsrPattern is a stomp
    // print "STOMP!" to serial monitor (string used for p5 sketch over p5.serialcontrol)
    Serial.println();
    Serial.println("STOMP!");
    digitalWrite(LED_BUILTIN, LOW);
    skipState = false;
    // reset ballCount to 0
    ballCount = 0;
    // reset heelCount to 0
    heelCount = 0;
    // reset the fsrPattern array
    fsrPattern[0] = wash;
    fsrPattern[1] = wash;
    fsrPattern[2] = wash;
  }

  //  // skip detection logic w/o using fsrPattern array
  //  if (heelCount == 1 && ballCount == 2) {    // if heelCount is 1 and ballCount is 2
  //    Serial.println();
  //    Serial.println("SKIP!");
  //    skipState = true;
  //    // reset ballCount to 0
  //    ballCount = 0;
  //    // reset heelCount to 0
  //    heelCount = 0;
  //  }

  // // TODO: Skip detection logic (using states)
  // // if heelCount = one then skipStateBall = TRUE
  // // ballCount can only be iterated if skipStateHeel = TRUE
  // // if *both* skipStateHeel && skipStateBall are TRUE then skipState is true
  // // send "Skip" string over P5 serial control to P5 sketch


  // // TODO: increment the counter on each only after the threshold has risen *then* fallen

  delay(100); // reading sensor data @ 10Hz (10 readings/sec)
  digitalWrite(ledDigitalPinBall, LOW);
  digitalWrite(ledDigitalPinHeel, LOW);
  digitalWrite(LED_BUILTIN, LOW);
  //}
  // TODO: turn console logging into a function
  //void consoleStatus() {
  // TODO: print skipStateCharacteristic to serial monitor
  //  Serial.println();
  //  Serial.print("skipStateCharacteristic value: ");
  //  Serial.print(skipStateCharacteristic);

  /*
    Bluetooth LE stuff
  */

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

  skipStateCharacteristic.writeValue(skipState);

}
