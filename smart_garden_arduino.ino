#include <LiquidCrystal.h>
#define ldrSensor A0
#define lmSensor A1
#define moistureSensor A2

const unsigned long SECOND = 1000;
const unsigned long HOUR = 3600 * SECOND;

const int ledPin = 13;
const int pumpPin = 10;
const int dry = 100; // value for dry sensor
const int wet = 1017; // value for wet sensor
int ldrValue = 0;
int lmValue = 0;
float lmVoltage = 0;
int temp = 0;
int moistureValue = 0;
int percentageHumididy = 0;

const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

void setup() {
  Serial.begin(9600);
  lcd.begin(16, 2);
  pinMode(pumpPin, OUTPUT);
  pinMode(ledPin, OUTPUT);
}

void loop() {
  getSensorsValues();
  turnLedOnOrOff();
  startWatering();
}

void startWatering() {
  if (percentageHumididy < 18) {
    //TODO: need to check if it's early morning or not to start watering.
    if (temp <= 25 && (ldrValue <= 890 && ldrValue >= 743)) {
      timesToWater(2);
    }
    else if (temp > 25 && (ldrValue <= 890 && ldrValue >= 743)) {
      timesToWater(3);
    }
  }
  else {
    digitalWrite(pumpPin, LOW);
  }
}

void timesToWater(int number) {
  for (int i = 0; i < number; i++) {
    getSensorsValues();
    //turn led on/off
    turnLedOnOrOff();

    if ((percentageHumididy >= 18 && percentageHumididy <= 20) || percentageHumididy > 20) {
      continue;
    }
    digitalWrite(pumpPin, HIGH);
    delay(0.5 * HOUR);
    digitalWrite(pumpPin, LOW);
    delay(4 * HOUR);
  }
}


void getLdrValue() {
  ldrValue = analogRead(ldrSensor);
}

void getTempValue() {
  lmValue = analogRead(lmSensor);
  lmVoltage = (5.0 / 1023.0) * lmValue;
  temp = lmVoltage / .01; // Temp = (Vout/10mv)
}

void getHumidityValue() {
  moistureValue = analogRead(moistureSensor);
  percentageHumididy = map(moistureValue, wet, dry, 0, 100);
}

void getSensorsValues() {
  getLdrValue();
  getTempValue();
  getHumidityValue();
}

void turnLedOnOrOff(){
  digitalWrite(ledPin, ldrValue >= 800 ? HIGH : LOW);
}
