/*
  Spring Shoes
  Idit Barak and Noah Pivnick
  2189.00 Intro to Wearables (Jingwen Zhu)

  This sketch borrows code from the following examples:

  - FSR simple testing sketch on AdaFruit's site
    https://learn.adafruit.com/force-sensitive-resistor-fsr/using-an-fsr
  - Analog Sensor Threshold Detection on ITP's Intro to Physical Computing Site
    https://itp.nyu.edu/physcomp/labs/labs-arduino-digital-and-analog/lab-sensor-change-detection/

  Wiring:

  Connect one lead of the first FSR to power, the other lead to Analog pin 1.
  Connect one lead of the second FSR to power, the other lead to Analog pin 2.
  Connect one end of a 10K resistor from Analog pin 1 to ground
  Connect one end of a 10K resistor from Analog pin 2 to ground
  Connect blue LED from pin 6 through a 220 resistor to ground
  Connect red LED from pin 7 through a 220 resistor to ground

*/

int fsrAnalogPinBall = 1;     // FSR is connected to analog pin 1
int fsrAnalogPinHeel = 2;     // FSR is connected to analog pin 2

int ledDigitalPinBall = 6;    // connect Blue LED to pin 6
int ledDigitalPinHeel = 7;    // connect Red LED to pin 7

int fsrReadingBall;           // the analog reading from the FSR sensor under the ball of the foot
int fsrReadingHeel;           // the analog reading from the FSR sensor under the heel of the foot

int lastFsrReadingBall;       // previous reading of FSR sensor under the ball of the foot
int lastFsrReadingHeel;       // previous reading of FSR sensor under the ball of the foot

int ballThreshold = 925;      // threshold sensor value used to detect Ball strike
int heelThreshold = 700;      // threshold sensor value used to detect Heel strike

int ballCount = 0;            // temporary Ball strike counter for detecting skips
int heelCount = 0;            // temporary Heel strike counter for detecting skips

int totalBallCount = 0;       // cumulative Ball strike counter
int totalHeelCount = 0;       // cumulative Heel strike counter

void setup(void) {
  Serial.begin(9600);         // send debugging information to Serial Monitor
  pinMode(ledDigitalPinBall, OUTPUT);
  pinMode(ledDigitalPinHeel, OUTPUT);
}

void loop(void) {
  // send FSR readings to serial monitor for debugging
  fsrReadingBall = analogRead(fsrAnalogPinBall);
  fsrReadingHeel = analogRead(fsrAnalogPinHeel);
  Serial.print("BALL sensor = ");
  Serial.print(fsrReadingBall);
  Serial.print(" - ");
  Serial.print("HEEL sensor = ");
  Serial.println(fsrReadingHeel);

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
      digitalWrite(ledDigitalPinBall, HIGH);
      // increment Ball of foot strike counter
      ballCount++;
      Serial.print(" * temp BALL count = ");
      Serial.print(ballCount);
      Serial.println();
      totalBallCount++;
      Serial.print(" * total BALL count = ");
      Serial.print(totalBallCount);
      Serial.println();
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
      digitalWrite(ledDigitalPinHeel, HIGH);
      // increment Ball of foot strike counter
      heelCount++;
      Serial.print(" * temp HEEL count = ");
      Serial.print(heelCount);
      Serial.println();
      totalHeelCount++;
      Serial.print(" * total HEEL count = ");
      Serial.print(totalHeelCount);
      Serial.println();
    }
  }
  // save the last sensor reading for next comparison:
  lastFsrReadingHeel = fsrReadingHeel;


  // // TODO: Skip logic w/ states
  // // declare int skipState and set it to FALSE
  // // declare skipStateBall and set it to FALSE
  // // declare skipStateHeel and set it to FALSE
  // // if heelCount = one then skipStateBall = TRUE
  // // ballCount can only be iterated if skipStateHeel = TRUE
  // // if *both* skipStateHeel && skipStateBall are TRUE then skipState is true
  // // send "Skip" string over P5 serial control to P5 sketch



  // // TODO: increment the counter on each only after the threshold has risen *then* fallen

  delay(100); // reading sensor data @ 10Hz (10 readings/sec)
}
