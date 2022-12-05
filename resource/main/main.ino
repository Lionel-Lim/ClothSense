// Import library
#include <Servo.h>
#include <MD_Parola.h>
#include <MD_MAX72xx.h>
#include <SPI.h>

// Pinout Setting
#define HARDWARE_TYPE MD_MAX72XX::FC16_HW
#define MAX_DEVICES 4
#define CS_PIN    10
#define DATA_PIN  11
#define CLK_PIN   13
#define buttonPin 2  // the number of the pushbutton pin
#define pwmPin 3    // the number of the LED pin
#define servoPin 6 // the number of the servo motor pin

// Software SPI Setting
MD_Parola P = MD_Parola(HARDWARE_TYPE, DATA_PIN, CLK_PIN, CS_PIN, MAX_DEVICES);

// LED Config
uint8_t scrollSpeed = 25;    // default frame delay value
textEffect_t scrollEffect = PA_NO_EFFECT;
textPosition_t scrollAlign = PA_LEFT;
uint16_t scrollPause = 2000; // in milliseconds
  // Global message buffers shared by Serial and Scrolling functions
#define	BUF_SIZE	75
char curMessage[BUF_SIZE] = { "" };
char newMessage[BUF_SIZE] = { "Hello!!!" };
bool newMessageAvailable = true;

// variables Setting:
Servo servo;
const int servoInterval = 80;
const int ledInterval = 80;
const int buttonInterval = 500;
const int servoMinDegrees = 20;
const int servoMaxDegrees = 160;

int buttonState = 0;  // variable for reading the pushbutton status
int fanSpeed = 0;
bool servoDirection = true;
unsigned long currentMillis = 0;
unsigned long previousServoMillis = 0;
unsigned long previousButtonMillis = 0;
unsigned long previousLEDMillis = 0;
int servoDegrees = 5;
int servoPosition = 90;

void onPressed() {
  if (currentMillis - previousButtonMillis >= buttonInterval){
    if(buttonState == 10){
      buttonState = 0;
    }else{
      buttonState = buttonState + 1;
    }
    previousButtonMillis += buttonInterval;
  }
  sprintf(newMessage, "Fan %d", buttonState);
  strcpy(curMessage, newMessage);
  newMessageAvailable = true;
}

void sweepServo(){
  if (currentMillis - previousServoMillis >= servoInterval) {
      previousServoMillis += servoInterval;

      servoPosition = servoPosition + servoDegrees;

      if ((servoPosition >= servoMaxDegrees) || (servoPosition <= servoMinDegrees))  {
      servoDegrees = - servoDegrees; // reverse direction
          // and update the position to ensure it is within range
      servoPosition = servoPosition + servoDegrees; 
    }
        // make the servo move to the next position
    servo.write(servoPosition);
        // and record the time when the move happened
  }
}

void displayLED(){
  if (currentMillis - previousLEDMillis >= ledInterval) {
    // Display
    P.displayAnimate();
    if (newMessageAvailable){
      strcpy(curMessage, newMessage);
      newMessageAvailable = false;
      Serial.println("In display");
      Serial.println(curMessage);
      Serial.println(newMessage);
    }
    P.displayReset();
  }
}

void setup() {
  // Set Serial port
  Serial.begin(9600);
  // initialize the pushbutton pin as an input and External Interrupt:
  pinMode(buttonPin, INPUT_PULLUP);
  // initialize the PWM pin as an output:
  pinMode(pwmPin, OUTPUT);
  
  // Set up external interrupt. On the event, onPressed function will executed.
  // Unlike Poll, interrup will be running in between delay.
  attachInterrupt(digitalPinToInterrupt(buttonPin), onPressed, RISING);

  // Set Servo
  servo.attach(servoPin);
  servo.write(0); //Return servo to its 0 location

  // Display Init
  P.begin();
  P.displayText(curMessage, scrollAlign, scrollSpeed, scrollPause, scrollEffect, scrollEffect);
}

void loop() {
  currentMillis = millis();

  sweepServo();
  displayLED();

  fanSpeed = buttonState * 25.5;
  analogWrite(pwmPin, fanSpeed);

  /*
  // Delay the loop
  // delay(1000);
  // Show btn state in Serial Monitor (0 - 10)
  Serial.print("Button State is ");
  Serial.println(buttonState);
  // Convert btn state to fan speed(PWM) (0 - 255)
  fanSpeed = buttonState * 25.5;
  // Send the fan speed to teh fan
  analogWrite(pwmPin, fanSpeed);
  // Show the fan speed in Serial Monitor
  Serial.print("The fan speed is ");
  Serial.println(fanSpeed);

  // Control Servo
  if (servoDirection){
    for (int rot = 0; rot < 180; rot++) {
      servo.write(rot);
      delay(10);
    }
    for (int rot = 180;   rot > 0; rot--) {
      servo.write(rot);
      delay(10); 
    }
  }else{
    Serial.print("Servo Turned Off");
  };

  // Display
  if (P.displayAnimate()){
    if (newMessageAvailable){
      strcpy(curMessage, newMessage);
      newMessageAvailable = false;
    }
    P.displayReset();
  }
  */
}