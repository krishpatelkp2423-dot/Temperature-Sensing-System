#include <LiquidCrystal.h>
#include <math.h>

int buzzer = 8;
int ThermistorPin = A0;
int setTemp=80;
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

float R1 = 10000;
float c1 = 1.009249522e-03;
float c2 = 2.378405444e-04;
float c3 = 2.019202697e-07;

int mode = 0;

float readTemperatureF() {
  int Vo = analogRead(ThermistorPin);
  if (Vo == 0) return -999;   // prevent divide-by-zero

  float R2 = R1 * (1023.0 / Vo - 1.0);
  float logR2 = log(R2);

  float T = (1.0 / (c1 + c2 * logR2 + c3 * logR2 * logR2 * logR2));
  T = T - 273.15;            // Kelvin → Celsius
  T = (T * 9.0) / 5.0 + 32;  // Celsius → Fahrenheit

  return T;
}

void setup() {
  Serial.begin(9600);
  pinMode(buzzer, OUTPUT);
  lcd.begin(16, 2);

  

  while (Serial.available() == 0) {}
  mode = Serial.parseInt();

  
}

void loop() {

  float T;
  int buzzerState = 0;

  if (mode == 1) {
    T = readTemperatureF();

    if (T > setTemp) {
      tone(buzzer, 1000);
      buzzerState = 1;
      delay (1000);
      noTone(buzzer);
      delay (1000);
    } else {
      noTone(buzzer);
      buzzerState = 0;
    }

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Temp:");
    lcd.setCursor(0, 1);
    lcd.print(T);
    lcd.print(" F");

    Serial.print("Temperature: ");
    Serial.print(T);
    Serial.println(" F");
  }

  else if (mode == 2) {
    tone(buzzer, 500);
    Serial.println("Buzzer ON");
    delay(500);
    noTone(buzzer);
  }

  else if (mode == 3) {
    T = readTemperatureF();
    if (T > setTemp) {
      tone(buzzer, 1000);
      buzzerState = 1;
      delay (1000);
      noTone(buzzer);
      delay (1000);
    } else {
      noTone(buzzer);
      buzzerState = 0;
    }

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Temp:");
    lcd.setCursor(0, 1);
    lcd.print(T);
    lcd.print(" F");

    Serial.println(buzzerState);
    Serial.println(T);
    

    /*Serial.print("Buzzer is: ");
    Serial.println(buzzerState == 1 ? "ON" : "OFF");

    Serial.print("LCD Reads: Temp ");
    Serial.print(T);
    Serial.println(" F");*/
  }

  else {
    Serial.println("Invalid Mode");
  }

  delay(1000);
}
