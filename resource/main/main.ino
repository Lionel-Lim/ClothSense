// Import library
#include <MD_Parola.h>
#include <MD_MAX72xx.h>
#include <SPI.h>
#include <DHT.h>
#include <DHT_U.h>
// #include <Adafruit_Sensor.h>
// #include "Adafruit_BME680.h"NF
// #include "Zanshin_BME680.h"  // Include the BME680 Sensor library

#define HARDWARE_TYPE MD_MAX72XX::FC16_HW
#define MAX_DEVICES 4

// Pinout Setting
#define CS_PIN    10
#define DATA_PIN  11
#define CLK_PIN   13
#define buttonPin 6  // the number of the pushbutton pin
#define infoButtonPin 8
#define pwmPin 3    // the number of the LED pin
#define DHTPin 4        // on Pin 2 of the Huzzah
// #define servoPin 6 // the number of the servo motor pin

// #define SEALEVELPRESSURE_HPA (1013.25)
#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321

// Software SPI Setting
MD_Parola P = MD_Parola(HARDWARE_TYPE, CS_PIN, MAX_DEVICES);
// MD_Parola P = MD_Parola(HARDWARE_TYPE, DATA_PIN, CLK_PIN, CS_PIN, MAX_DEVICES);
DHT dht(DHTPin, DHTTYPE);   // Initialize DHT sensor.
// BME680_Class BME680;  ///< Create an instance of the BME680 class
// Adafruit_BME680 bme; // I2C

// LED Config
uint8_t scrollSpeed = 25;    // default frame delay value
textEffect_t scrollEffect = PA_PRINT;
textEffect_t scrollRemoveEffect = PA_SCROLL_LEFT;
textPosition_t scrollAlign = PA_LEFT;
uint16_t scrollPause = 2000; // in milliseconds
  // Global message buffers shared by Serial and Scrolling functions
#define	BUF_SIZE	70
char curMessage[BUF_SIZE] = { "" };
char newMessage[BUF_SIZE] = { "Hello!!!" };
#define	Default_SIZE	50
char defaultCurMessage[Default_SIZE] = { "" };
char defaultNewMessage[Default_SIZE] = { "^  -" };
bool newMessageAvailable = true;

// variables Setting:
// Servo servo;
// const int servoInterval = 80;
const int ledInterval = 100;
const int buttonInterval = 50;
// const int servoMinDegrees = 20;
// const int servoMaxDegrees = 160;
const int bmeInterval = 5000;
const int goToDefaultDisplayInterval = 10000;

int goToDefaultStatus = 0;
int buttonState = 1;  // variable for reading the pushbutton status
int infoButtonState = 1;
int buttonLevel;
int lastButtonState = 1;
int lastInfoButtonState = 1;
int fanSpeed = 0;
int temp;
int humd;
// bool servoDirection = true;
unsigned long currentMillis = 0;
// unsigned long previousServoMillis = 0;
unsigned long previousButtonMillis = 0;
unsigned long infoButtonMillis = 0;
unsigned long previousLEDMillis = 0;
unsigned long previousBMEMillis = 0;
unsigned long previousDefaultDisplayMillis = 0;
// int servoDegrees = 5;
// int servoPosition = 90;

void onPressed() {
  delay(30);
  int reading = digitalRead(buttonPin);
  // Serial.println(reading);
  if (reading != lastButtonState) {
    // reset the debouncing timer
    previousButtonMillis = millis();
    // Serial.print("Time is:");
    // Serial.println(previousButtonMillis);
  }
  if ((millis() - previousButtonMillis) > buttonInterval) {
    // Serial.print("inside");
    // Serial.print("Diif is:");
    // Serial.println(millis() - previousButtonMillis);
    // if the button state has changed:
    Serial.print("reading is ");
    Serial.print(reading);
    Serial.print("state is ");
    Serial.println(buttonState);
    // only toggle the LED if the new button state is HIGH
    if (reading == buttonState) {
     buttonState = reading;
      if (reading == 1) {
        if(buttonLevel == 10){
          buttonLevel = 0;
        }else{
          buttonLevel = buttonLevel + 1;
        }
        Serial.print("Level is ");
        Serial.println(buttonLevel);
        sprintf(newMessage, "Fan %d", buttonLevel);
        strcpy(curMessage, newMessage);
        newMessageAvailable = true;
        previousDefaultDisplayMillis = currentMillis;
        goToDefaultStatus = 0;
        P.displayText(curMessage, scrollAlign, scrollSpeed, scrollPause, scrollEffect, scrollRemoveEffect);
      }
    lastButtonState = reading;
    }
  }
}

void onInfoBtnPressed() {
  delay(30);
  int reading = digitalRead(infoButtonPin);
  if (reading != lastInfoButtonState) {
    // reset the debouncing timer
    infoButtonMillis = millis();

  }
  if ((millis() - infoButtonMillis) > buttonInterval) {
    // only toggle the LED if the new button state is HIGH
    if (reading == infoButtonState) {
     infoButtonState = reading;
      if (reading == 1) {
        sprintf(newMessage, "Temperature : %d, Humidity : %d", temp, humd);
        strcpy(curMessage, newMessage);
        newMessageAvailable = true;
        previousDefaultDisplayMillis = currentMillis;
        goToDefaultStatus = 0;
        P.displayText(curMessage, scrollAlign, scrollSpeed, scrollPause, scrollRemoveEffect, scrollRemoveEffect);
      }
    lastInfoButtonState = reading;
    }
  }
}

// void displayLED(){
//   if (currentMillis - previousLEDMillis >= ledInterval) {
//     // Display
//     previousLEDMillis += ledInterval;
//     P.displayAnimate();
//     if (newMessageAvailable){
//       strcpy(curMessage, newMessage);
//       newMessageAvailable = false;
//       // Serial.println("In display");
//       // Serial.println(curMessage);
//       // Serial.println(newMessage);
//     }
//     P.displayReset();
//   }
// }

void goToDefaultDisplay(){
  if(currentMillis - previousDefaultDisplayMillis >= goToDefaultDisplayInterval && goToDefaultStatus == 0){
    Serial.println("Going Back to Default Screen.");
    goToDefaultStatus = 1;
    P.displayClear();
    P.displayReset();
    if (humd >= 80){
      // char defaultNewMessage[Default_SIZE] = { "T   T" };
      sprintf(defaultNewMessage, "T    T");
      // defaultNewMessage = "T    T";
    }
    else if (humd >= 50){
      // char defaultNewMessage[Default_SIZE] = { "-   -" };
      sprintf(defaultNewMessage, "-    -");
    }
    else{
      // char defaultNewMessage[Default_SIZE] = { "^   ^" };
      sprintf(defaultNewMessage, "^     ^");
    }
    strcpy(defaultCurMessage, defaultNewMessage);
    P.displayText(defaultCurMessage, PA_CENTER, scrollSpeed, 5000, PA_SCROLL_UP, PA_SCROLL_DOWN);
  }
  // if (currentMillis - previousDefaultDisplayMillis >= goToDefaultDisplayInterval) {
    // Display
    // previousDefaultDisplayMillis += goToDefaultDisplayInterval;
}

void readDHT() {
  if (currentMillis - previousBMEMillis >= bmeInterval){
    previousBMEMillis += bmeInterval;
    temp = dht.readTemperature();
    humd = dht.readHumidity();
    Serial.print(temp);
    Serial.print("Hum is");
    Serial.println(humd);
  }
}

void setup() {
  // Set Serial port
  Serial.begin(9600);
  pinMode(DHTPin, INPUT);
  dht.begin();
  readDHT();
  // initialize the pushbutton pin as an input and External Interrupt:
  // pinMode(buttonPin, INPUT_PULLUP);
  pinMode(buttonPin, INPUT);
  pinMode(infoButtonPin, INPUT);
  // initialize the PWM pin as an output:
  pinMode(pwmPin, OUTPUT);
  
  // Set up external interrupt. On the event, onPressed function will executed.
  // Unlike Poll, interrup will be running in between delay.
  // attachInterrupt(digitalPinToInterrupt(buttonPin), onPressed, RISING);

  // Set Servo
  // servo.attach(servoPin);
  // servo.write(0); //Return servo to its 0 location

  // Display Init
  P.begin();
  P.displayText(curMessage, scrollAlign, scrollSpeed, scrollPause, scrollRemoveEffect, scrollRemoveEffect);
  strcpy(curMessage, newMessage);
}

void loop() {
  // Serial.println(buttonState);
  currentMillis = millis();

  // Serial.println(digitalRead(buttonPin));
  // if(digitalRead(buttonPin) == 1){
  //   onPressed();    
  // }
  // sweepServo();
  onPressed();
  // displayLED();
  goToDefaultDisplay();
  onInfoBtnPressed();
  readDHT();

  fanSpeed = buttonLevel * 25.5;
  analogWrite(pwmPin, fanSpeed);
  if (P.displayAnimate()){
    P.displayReset();
  }
}