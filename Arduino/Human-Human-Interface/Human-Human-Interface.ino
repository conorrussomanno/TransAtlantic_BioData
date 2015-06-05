/* BASED ON: Arduino Code for an EMG SpikerShield to control a TENS device, LED lights, and a gripper
    - By Greg Gage
    - Modified 6/5/15-6/7/15 @ BCI Montreal / HACK THE BRAIN AMSTERDAM
        - Yannick Roy, Conor Russomanno, Pieter van Boheemen, & ...
*/

#include <Servo.h>   //Includes the Gripper Servo Library
Servo ServoGripper; //Declares the Name of the Servo to be ServoGripper -- this is all if you want to control a gripper hand as well

#define NUM_LED 6  //sets the maximum numbers of LEDs
#define MAX_Low 100   //for people with low EMG activity
#define MAX_High 254//for people with high EMG activity
#define Threshold 3 // this sets the light to activate TENS
#define threshold_degrees 10 //Number of steps the Servo will have

int reading[10];
int finalReading;
int StimPin = 3; // TENS Digital 3
int SwitchPin = 4;   // pushbutton connected to digital pin 4 -  this will momentarily turn on TENS
int SwitchThreshold = 7; //pushbutton connected to digital pin 7 - this will switch thresholds
int GripPin = 2; //Digital Out that controls Servo Motor Gripper Hand
int SwitchState = 0;
int SwitchThresholdState = 0;
int MAX = 0;
byte litLeds = 0;
byte multiplier = 1;
byte leds[] = {8, 9, 10, 11, 12, 13};
int aQ1 = 11;
int aQ2 = 13;
int aQ3 = 8;

const int UpdateTime = 200; // (number of milliseconds between updating servo position -- if too low you will burn motor out)
unsigned long OldTime = 0;
int old_degrees = 0;
int new_degrees = 0;

//-------- ADDED VALUE FOR TransAtlanticBioData hack ---------- //
int incomingValue = 0;

void setup() {
  Serial.begin(9600); //begin serial communications
  ServoGripper.attach(GripPin); //Declare the Servo to be Connected to GripPin
  pinMode(StimPin, OUTPUT); // Set TENS output to StimPin
  pinMode(SwitchPin, INPUT);      // sets the digital pin 4 as the switch input
  pinMode(SwitchThreshold, INPUT);      // sets the digital pin 7 as the threshold changer
  for (int i = 0; i < NUM_LED; i++) { //initialize LEDs as outputs
    pinMode(leds[i], OUTPUT);
    pinMode(aQ1, OUTPUT);
  }
  MAX = MAX_High; //This sets the default to people with high EMG activity.
}

void serialEvent() {
  while (Serial.available()) {
    int newData = Serial.read();
    incomingValue = (int)newData;  // should be a value from 0-100 coming from the netBridge_receiver.pde Processing Sketch
  }
}

void loop() {

  //note the serial print stuff is just debugging tools
  SwitchThresholdState = digitalRead(SwitchThreshold);

  if (SwitchThresholdState == HIGH) { // This will allow the switching between a low threshold and high threshold state on S2
    if (MAX == MAX_High) {
      MAX = MAX_Low;
      digitalWrite(aQ3, HIGH);
    }
    else {
      MAX = MAX_High; digitalWrite(aQ2, HIGH);
    }
    while (SwitchThresholdState == HIGH) { // This will pause the program while the person is touching the threshold button,
      SwitchThresholdState = digitalRead(SwitchThreshold);
      delay(10);
    }                         // so it doesn't flip back and forth while button is pushed
  }

  SwitchState = digitalRead(SwitchPin);
  
  if(incomingValue >= 75){
    SwitchState = HIGH;   //if incoming value from netBridge_receiver.pde is above 75%, trigger TENS
  }
  if(incomingValue < 75){
    SwitchState = LOW;   //if incoming value is below 75%, turn off TENS
  }
  
  //Serial.println(SwitchState);
  if (SwitchState == HIGH) { // This will activate the TENS if the PushButton is pressed on S1
    digitalWrite(StimPin, HIGH), digitalWrite(aQ1, HIGH);
    //Serial.println('Stim');
    while (SwitchState == HIGH) { // This will pause the program while the person is touching TENS test button
      SwitchState = digitalRead(SwitchPin);
      delay(10);
    }
  }

  for (int i = 0; i < 10; i++) {  //take ten readings in ~0.02 seconds
    reading[i] = analogRead(A0) * multiplier;
    delay(2);
  }
  for (int i = 0; i < 10; i++) { //average the ten readings
    finalReading += reading[i];
  }
  finalReading /= 10;
  for (int j = 0; j < NUM_LED; j++) { //write all LEDs low and stim pin low

    digitalWrite(leds[j], LOW);
    digitalWrite(StimPin, LOW);
  }
  //Serial.print(finalReading);
  //Serial.print("\t");
  finalReading = constrain(finalReading, 0, MAX);
  litLeds = map(finalReading, 0, MAX, 0, NUM_LED);
  //Serial.println(litLeds);

  for (int k = 0; k < litLeds; k++) {
    digitalWrite(leds[k], HIGH); // This turns on the LEDS
    if (k >= Threshold) {
      digitalWrite(StimPin, HIGH); // This turns on the TENS as a function of which LED is lit
    }
  }

  new_degrees = map(finalReading, 0 , MAX, 165, 0); //Translate the analog reading to degrees for the servo (from 165° to 0°).

  if (millis() - OldTime > UpdateTime) {
    if (abs(new_degrees - old_degrees) > threshold_degrees) {
      ServoGripper.write(new_degrees); //Move the servo according to the degrees calculated
    }
    OldTime = millis();
    old_degrees = new_degrees;
  }
  //delay(10);
  //for serial debugging, uncomment the next two lines.
  //Serial.println(finalReading);
  //delay(100);
  
  
  SwitchState = LOW;  //make sure SwitchState is low at end of loop to ensure TENS does not get stuck on if a data communication break happens
}

