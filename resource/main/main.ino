// Import library
#include <MD_Parola.h>
#include <MD_MAX72xx.h>
#include <SPI.h>
#include <DHT.h>
#include <DHT_U.h>

// Pinout Setting
#define CS_PIN    10
#define DATA_PIN  11
#define CLK_PIN   13
#define buttonPin 6 
#define infoButtonPin 8
#define pwmPin 3
#define DHTPin 4 

// Hardware Setting
#define HARDWARE_TYPE MD_MAX72XX::FC16_HW
#define MAX_DEVICES 4
#define DHTTYPE DHT22   // DHT 22

MD_Parola P = MD_Parola(HARDWARE_TYPE, CS_PIN, MAX_DEVICES);
DHT dht(DHTPin, DHTTYPE);   // Initialize DHT sensor.

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
const int ledInterval = 100;
const int buttonInterval = 50;
const int DHTInterval = 5000;
const int goToDefaultDisplayInterval = 10000;
const int countDownInterval = 5000;

char estimatedHHmmTime[BUF_SIZE] = { "" };
int goToDefaultStatus = 0;
int buttonState = 1;  // variable for reading the pushbutton status
int infoButtonState = 1;
int buttonLevel;
int lastButtonState = 1;
int lastInfoButtonState = 1;
int fanSpeed = 0;
int defaultDisplayCount = 0;
int temp;
int countEstimatedTime = 0;
float humd;
float maxHumd = 0;
float startHumd = 0;
unsigned long currentMillis = 0;
unsigned long previousButtonMillis = 0;
unsigned long previousInfoButtonMillis = 0;
unsigned long previousDHTMillis = 0;
unsigned long previousDefaultDisplayMillis = 0;
unsigned long previousCountDownMillis = 0;
unsigned long estimatedDryTime = 0;
unsigned long remainedTime = 0;
bool fanButtonWasPressed = false;
bool infoButtonWasPressed = false;

// Functions
void onPressed() {
  unsigned long currentTime = millis();
  boolean buttonIsPressed = digitalRead(buttonPin) == LOW;

  // Check for button state change and do debounce
  if (buttonIsPressed != fanButtonWasPressed &&
      currentTime  -  previousButtonMillis > buttonInterval){
    // Button state has changed
    fanButtonWasPressed = buttonIsPressed;
    previousButtonMillis = currentTime;
    if (fanButtonWasPressed){
    }
    else
    {
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
  }
}

void onInfoBtnPressed() {
  unsigned long currentTime = millis();
  boolean buttonIsPressed = digitalRead(infoButtonPin) == LOW;

  // Check for button state change and do debounce
  if (buttonIsPressed != infoButtonWasPressed &&
      currentTime  -  previousInfoButtonMillis > buttonInterval){
    // Button state has changed
    infoButtonWasPressed = buttonIsPressed;
    previousButtonMillis = currentTime;
    if (infoButtonWasPressed){
    }
    else
    {
      sprintf(newMessage, "Temperature : %d, Humidity : %s", temp, String(humd,2).c_str());
      strcpy(curMessage, newMessage);
      newMessageAvailable = true;
      previousDefaultDisplayMillis = currentMillis;
      goToDefaultStatus = 0;
      P.displayText(curMessage, scrollAlign, scrollSpeed, scrollPause, scrollRemoveEffect, scrollRemoveEffect);
    }
  }
}

void goToDefaultDisplay(){
  countEstimatedTime += 1;
  if(countEstimatedTime == 20000){
    previousDefaultDisplayMillis += goToDefaultDisplayInterval;
    P.displayClear();
    P.displayReset();
    P.displayText(estimatedHHmmTime, PA_CENTER, scrollSpeed, 2500, PA_SCROLL_LEFT, PA_SCROLL_LEFT);
    countEstimatedTime = 0;
    goToDefaultStatus += 1;
    if (goToDefaultStatus == 3){
      goToDefaultStatus = 0;
    }
  }
  else if(currentMillis - previousDefaultDisplayMillis >= goToDefaultDisplayInterval && goToDefaultStatus == 0){
    previousDefaultDisplayMillis += goToDefaultDisplayInterval;
    goToDefaultStatus = 1;
    defaultDisplayCount += 1;
    P.displayClear();
    P.displayReset();
    if (humd >= 80){
      sprintf(defaultNewMessage, "T    T");
    }
    else if (humd >= 50){
      sprintf(defaultNewMessage, "-    -");
    }
    else{
      sprintf(defaultNewMessage, "^     ^");
    }
    strcpy(defaultCurMessage, defaultNewMessage);
    P.displayText(defaultCurMessage, PA_CENTER, scrollSpeed, 5000, PA_SCROLL_UP, PA_SCROLL_DOWN);
  }
}

void readDHT() {
  if (currentMillis - previousDHTMillis >= DHTInterval){
    previousDHTMillis += DHTInterval;
    if (maxHumd < dht.readHumidity()){
      maxHumd = dht.readHumidity();
      estimatedDryTime = calculateEstimatedDryTime(maxHumd);
      Serial.print("Updated ");
      Serial.println(estimatedHHmmTime);
    }
    temp = dht.readTemperature();
    humd = dht.readHumidity();
    Serial.print(temp);
    Serial.print(" / ");
    Serial.print(humd);
    Serial.print(" / ");
    Serial.print(maxHumd);
    Serial.print(" / ");
    Serial.println(millis());
  }
}

void calculateDryTime() {
  if(humd > maxHumd){
    //
  }
}

unsigned long calculateEstimatedDryTime(float HUMIDITY) {
  unsigned long estimatedCount = 3600000 / DHTInterval;
  float estimatedHumd = 0;
  while(millis()<=3600000){
    if(millis() <= 10000){
      estimatedHumd = (0.00000003*pow(estimatedCount/16,3)) + 
      (-0.00007*pow(estimatedCount/16,2)) + 
      (0.0067*estimatedCount/16) + 
      (HUMIDITY + 4); // Simplified equation before storing data
    }else{
      estimatedHumd = (0.00000003*pow(estimatedCount/16,3)) + 
      (-0.00007*pow(estimatedCount/16,2)) + 
      (0.0067*estimatedCount/16) + 
      (startHumd + (abs(HUMIDITY - startHumd)/2));
    }
    if(abs(estimatedHumd - startHumd) < 0.01){
      return estimatedCount * DHTInterval;  // convert data row number to time
    }
    else if(estimatedCount >= 10000){
      return 0; // If cannot find, return 0
    }
    else{
      estimatedCount += 1;
    }
  }
}

void convertMillsToHHmm(unsigned long milli){
  unsigned long minutes;
  unsigned long hours;

  minutes = milli / 60000;
  hours = minutes / 60;
  minutes %= 60;
  hours %= 24;

  sprintf(estimatedHHmmTime, "Estimated Time : %lu hours %lu minutes", hours, minutes);
}

void countDownTime(){
  if (currentMillis - previousCountDownMillis >= countDownInterval){
    previousCountDownMillis += countDownInterval;
    remainedTime = estimatedDryTime - millis();
    convertMillsToHHmm(remainedTime);
  }
}

void setup() {
  // Set Serial port
  Serial.begin(9600);
  Serial.println("Activating ClothSense...");

  // DHT Init
  pinMode(DHTPin, INPUT);
  dht.begin();
  readDHT();
  temp = dht.readTemperature();
  humd = dht.readHumidity();
  startHumd = humd;
  estimatedDryTime = calculateEstimatedDryTime(humd);
  convertMillsToHHmm(estimatedDryTime);
  Serial.println(estimatedHHmmTime);
  Serial.print(temp);
  Serial.print(" / ");
  Serial.print(humd);
  Serial.print(" / ");
  Serial.println(millis());

  // Buttons Init
  pinMode (buttonPin, INPUT);
  pinMode(infoButtonPin, INPUT);

  // initialize the PWM pin as an output:
  pinMode(pwmPin, OUTPUT);

  // Display Init
  P.begin();
  P.displayText(curMessage, scrollAlign, scrollSpeed, scrollPause, scrollRemoveEffect, scrollRemoveEffect);
  strcpy(curMessage, newMessage);
}

void loop() {
  currentMillis = millis();

  onPressed();
  onInfoBtnPressed();

  goToDefaultDisplay();
  readDHT();
  countDownTime();

  fanSpeed = buttonLevel * 25.5; //buttonLevel range is 0 to 10
  analogWrite(pwmPin, fanSpeed);
  if (P.displayAnimate()){
    P.displayReset();
  }
}