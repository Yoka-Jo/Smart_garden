#include <LiquidCrystal.h>
#define ldrSensor A0
#define lmSensor A1
#define moistureSensor A2

const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

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


void setup() {
  pinMode(ledPin, OUTPUT);
  pinMode(pumpPin, OUTPUT);
  lcd.begin(16, 2);
  startingLcd();
}

void startingLcd(){
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Time: ");
  lcd.setCursor(0, 1);
  lcd.print("Pump: ");
}

void loop() {
  getSensorsValues();
  turnLedOnOrOff();
  getTime();
  startWatering();
}

void startWatering() {
  if (percentageHumididy < 18) {
    if (temp <= 25 && (ldrValue <= 890 && ldrValue >= 600)) {
      pumpOn();
      timesToWater(2);
    }
    else if (temp > 25 && (ldrValue <= 890 && ldrValue >= 600)) {
      pumpOn();
      timesToWater(3);
    }
    else {
      pumpOff();
    }
  }
  else {
    pumpOff();
    digitalWrite(pumpPin, LOW);
  }
}

void timesToWater(int number) {
  for (int i = 0; i < number; i++) {
    getSensorsValues();
    getTime();
    //turn led on/off
    turnLedOnOrOff();

   if (percentageHumididy > 18) {
      getTime();
      pumpOff();
      continue;
    }
    
    pumpOn();
    
    
    delay(2000);
    
    pumpOff();
    delay(5000);
  }
}

// Sensors Values
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
  delay(100);
}

void getSensorsValues() {
  getLdrValue();
  getTempValue();
  getHumidityValue();
}

void turnLedOnOrOff() {
  if (ldrValue >= 800)
  {
    digitalWrite(ledPin, HIGH);
  }

  else {
    digitalWrite(ledPin, LOW);
  }
}

// Time
void getTime() {
  startingLcd();
  lcd.setCursor(7 , 0);
  if (ldrValue <= 400) {
    lcd.print("Bright");
  }
  else if (ldrValue < 600) {
    lcd.print("Light");
  }
  else if (ldrValue < 1023) {
    lcd.print("Morning");
  }
}

// Pump
void pumpOn() {
  digitalWrite(pumpPin, HIGH);
  lcd.setCursor(7 , 1);
  lcd.print(" ON");
}

void pumpOff() {
  digitalWrite(pumpPin, LOW);
  lcd.setCursor(7 , 1);
  lcd.print("OFF");
}
