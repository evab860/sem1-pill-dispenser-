#include <Servo.h>
#include <LiquidCrystal.h>
#include <RTClib.h>

// Servo object
Servo servo;

// Pin constants
const int servoPin = 6;
const int ledPin = 7;
const int buzzerPin = 8;

// Servo variables
int angle = 0;
const int angleIncrement = 45; // Adjust for different compartments
const int movementDelay = 50;

// LCD pins
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

// RTC object
RTC_DS3231 rtc;

// Time variables
int year, month, day, hour, minutes, seconds;

void setup() {
  pinMode(ledPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);

  digitalWrite(ledPin, LOW);
  digitalWrite(buzzerPin, LOW);

  servo.attach(servoPin);
  servo.write(angle);

  lcd.begin(16, 2);
  lcd.print("Initializing...");
  delay(2000);
  lcd.clear();
}

void loop() {
  updateLCD();

  // Perform an action every 10 seconds
  if (seconds % 10 == 0) {
    rotateServo();
    flashLED(4, 150); // Flash LED and buzzer
  }
}

void rotateServo() {
  int newAngle = angle + angleIncrement;
  if (newAngle > 180) {
    newAngle = 0; // Reset to 0 if angle exceeds 180
  }

  // Smoothly move the servo to the new angle
  while (angle != newAngle) {
    angle += (newAngle > angle) ? 1 : -1;
    servo.write(angle);
    delay(movementDelay);
  }
}

void flashLED(int numFlashes, int flashDelay) {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Take medicine!");

  for (int i = 0; i < numFlashes; i++) {
    digitalWrite(ledPin, HIGH);
    digitalWrite(buzzerPin, HIGH);
    delay(flashDelay);
    digitalWrite(ledPin, LOW);
    digitalWrite(buzzerPin, LOW);
    delay(flashDelay);
  }

  delay(2500); // Non-blocking delay can replace this
  lcd.clear();
}

void updateLCD() {
  DateTime now = rtc.now();

  year = now.year();
  month = now.month();
  day = now.day();
  hour = now.hour();
  minutes = now.minute();
  seconds = now.second();

  // Update LCD only if time changes
  static int lastSecond = -1;
  if (seconds != lastSecond) {
    lastSecond = seconds;

    lcd.setCursor(0, 0);
    if (day < 10) lcd.print("0");
    lcd.print(day);
    lcd.print("/");
    if (month < 10) lcd.print("0");
    lcd.print(month);
    lcd.print("/");
    lcd.print(year);

    lcd.setCursor(0, 1);
    if (hour < 10) lcd.print("0");
    lcd.print(hour);
    lcd.print(":");
    if (minutes < 10) lcd.print("0");
    lcd.print(minutes);
    lcd.print(":");
    if (seconds < 10) lcd.print("0");
    lcd.print(seconds);
  }
}
