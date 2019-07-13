/*
   LED Matrix Sprite Generator: http://embed.plnkr.co/3VUsekP3jC5xwSIQDVHx/preview
   LedControl lib: https://github.com/wayoda/LedControl

   Schema can be viewed at https://www.tinkercad.com/things/crQYEKlflZ1-turn-signal-for-bike/editel
*/
#include <LedControl.h>

const int arrowMarqueStepsCount = 16;
bool isTurningLeft = false;
bool isTurningRight = false;

const int lefTurnButtonPin = 4;
const int rightTurnButtonPin = 5;

const int buzzerPin = 3;

// (DATA IN, CLK, LOAD(CS), count of attached MAX72XX)
const int dataInPin = 12;
const int clkPin = 11;
const int csPin = 10;
const int max72XXCount = 1;
LedControl ledCtrl = LedControl(dataInPin, clkPin, csPin, max72XXCount);
byte leftArrowAnimation[arrowMarqueStepsCount][8] = {
  {B00000000, B00000000, B00000000, B00000001, B00000001, B00000000, B00000000, B00000000},
  {B00000000, B00000000, B00000001, B00000011, B00000011, B00000001, B00000000, B00000000},
  {B00000000, B00000001, B00000011, B00000111, B00000111, B00000011, B00000001, B00000000},
  {B00000001, B00000011, B00000111, B00001111, B00001111, B00000111, B00000011, B00000001},
  {B00000010, B00000110, B00001110, B00011111, B00011111, B00001110, B00000110, B00000010},
  {B00000100, B00001100, B00011100, B00111111, B00111111, B00011100, B00001100, B00000100},
  {B00001000, B00011000, B00111000, B01111111, B01111111, B00111000, B00011000, B00001000},
  {B00010000, B00110000, B01110000, B11111111, B11111111, B01110000, B00110000, B00010000},
  {B00100000, B01100000, B11100000, B11111110, B11111110, B11100000, B01100000, B00100000},
  {B01000000, B11000000, B11000000, B11111100, B11111100, B11000000, B11000000, B01000000},
  {B10000000, B10000000, B10000000, B11111000, B11111000, B10000000, B10000000, B10000000},
  {B00000000, B00000000, B00000000, B11110000, B11110000, B00000000, B00000000, B00000000},
  {B00000000, B00000000, B00000000, B11100000, B11100000, B00000000, B00000000, B00000000},
  {B00000000, B00000000, B00000000, B11000000, B11000000, B00000000, B00000000, B00000000},
  {B00000000, B00000000, B00000000, B10000000, B10000000, B00000000, B00000000, B00000000},
  {B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000},
};

void setup() {
  //Begin Serial communication at a baudrate of 9600 - for testing purpose:
  Serial.begin(9600);
  Serial.println("Started!");

  ledCtrl.shutdown(0, false);
  ledCtrl.setIntensity(0, 1); //medium brightness
  clearDisplayFor(ledCtrl);

  pinMode(lefTurnButtonPin, INPUT_PULLUP);
}

void loop() {
  if (isTurningLeft)
    displaySymbol(leftArrowAnimation);
  else if (isTurningRight)
    displaySymbol(leftArrowAnimation);//TODO: right animation goes here
  else
    clearDisplayFor(ledCtrl);

  // button precessed
  if (digitalRead(lefTurnButtonPin) == HIGH) {
    Serial.println("+++++++++++++++++++++++++++++++++++++++++");
    if (isTurningLeft) {
      clearDisplayFor(ledCtrl);
      isTurningLeft = false;
      delay(250);
    } else {
      isTurningLeft = true;
      delay(150);
    }
  }

  //TODO: add check for time. if turn signal is on more than a minute automatically turn it off
  //  delay(150);
}

/*
   Show given image. in my case given arrow
*/
void displaySymbol(byte symbols[arrowMarqueStepsCount][8]) {
  for (int s = 0; s < arrowMarqueStepsCount; s++) {
    for (int i = 0; i < 8; i++) {
      ledCtrl.setRow(0, i, symbols[s][i]);
    }

    delay(30);
    if (s%9 == 0)
      tone(buzzerPin, 50);
    delay(20);
    noTone(buzzerPin);
    
    // Clearing dots
    for (int i = 0; i < 8; i++) {
      ledCtrl.setRow(0, i, 0);
    }

    if (digitalRead(lefTurnButtonPin) == HIGH || digitalRead(rightTurnButtonPin) == HIGH) {
      delay(1);
      break;
    }
  }

  noTone(buzzerPin);
}

void clearDisplayFor(LedControl ledControl) {
  ledControl.clearDisplay(0);
}

void turnSignalVoice(){
    tone(buzzerPin, 50);
    delay(20);
    noTone(buzzerPin);
    delay(500);
    tone(buzzerPin, 50);
}
