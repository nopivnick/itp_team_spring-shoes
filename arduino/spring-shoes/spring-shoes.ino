/* FSR testing sketch. 
 
Connect one end of FSR to 5V, the other end to Analog 0.
Then connect one end of a 10K resistor from Analog 0 to ground
Connect LED from pin 11 through a resistor to ground 
 
For more information see www.ladyada.net/learn/sensors/fsr.html */



int fsrAnalogPin1 = 1; // FSR is connected to analog 0
int fsrAnalogPin2 = 2; // FSR is connected to analog 0
// int LEDpin = 11;      // connect Red LED to pin 11 (PWM pin)
int fsrReading1;       // the analog reading from the 1st FSR resistor divider
int fsrReading2;       // the analog reading from the 2nd FSR resistor divider
// int LEDbrightness;
 
void setup(void) {
  Serial.begin(9600);   // We'll send debugging information via the Serial monitor
//  pinMode(LEDpin, OUTPUT);
}
 
void loop(void) {
  fsrReading1 = analogRead(fsrAnalogPin1);
  fsrReading2 = analogRead(fsrAnalogPin2);
  Serial.print("FSR 1 reading = ");
  Serial.print(fsrReading1);
  Serial.print(" | ");
  Serial.print("FSR 2 reading = ");
  Serial.println(fsrReading2);
 
  // we'll need to change the range from the analog reading (0-1023) down to the range
  // used by analogWrite (0-255) with map!
  // LEDbrightness = map(fsrReading, 0, 1023, 0, 255);
  // LED gets brighter the harder you press
  // analogWrite(LEDpin, LEDbrightness);
 
  delay(100);
}
