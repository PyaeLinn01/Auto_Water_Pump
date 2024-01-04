#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define echoPin 9
#define triggerPin 10
#define waterPumpPin 2

long duration;
long distance;
LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
  pinMode(triggerPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(waterPumpPin, OUTPUT);

  lcd.init();
  lcd.backlight();
  lcd.clear();
}

void loop() {
  digitalWrite(triggerPin, LOW);
  delayMicroseconds(2);
  digitalWrite(triggerPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(triggerPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.034 / 2;  // Calculate distance in centimeters

  // Print distance to Serial for debugging
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  // Control water pump based on distance
  if (distance < 5) {
    digitalWrite(waterPumpPin, LOW);
    lcd.setCursor(2, 1);
    lcd.print("FULL!No refill");
  } else {
    digitalWrite(waterPumpPin, HIGH);
    
    // Display "Filling ..." with moving animation
    static const char movingDots[] = "....";
    static int dotPosition = 0;
    
    lcd.setCursor(2, 1);
    lcd.print("Filling");
    lcd.print(movingDots + dotPosition);
    dotPosition = (dotPosition + 1) % (sizeof(movingDots) - 1);
  }

  // Display distance on LCD
  lcd.setCursor(2, 0);
  lcd.print("Distance: ");
  lcd.print(distance);
  lcd.print("cm");

  delay(3000);

  // Clear LCD and turn off water pump
  lcd.clear();
  digitalWrite(waterPumpPin, LOW);
}
