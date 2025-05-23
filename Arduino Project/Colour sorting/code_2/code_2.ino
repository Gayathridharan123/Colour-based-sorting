#include <Servo.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Define pins
#define S0 4
#define S1 5
#define S2 6
#define S3 7
#define sensorOut 2
#define IR 8
#define servoPin 9

Servo myServo;
LiquidCrystal_I2C lcd(0x27, 16, 2); // Use 0x3F if LCD doesn't work

void setup() {
  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);
  pinMode(sensorOut, INPUT);
  pinMode(IR, INPUT);

  // Set frequency scaling to 20%
  digitalWrite(S0, HIGH);
  digitalWrite(S1, LOW);

  myServo.attach(servoPin);
  myServo.write(90); // Neutral position

  lcd.init();        // <- FIXED: initialize LCD
  lcd.backlight();   // Turn on LCD backlight

  Serial.begin(9600);
}

void loop() {
  if (digitalRead(IR) == LOW) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Detecting Color");
    delay(1000);

    int red = readColor(LOW, LOW);
    int green = readColor(HIGH, HIGH);
    int blue = readColor(LOW, HIGH);

    Serial.print("R: "); Serial.print(red);
    Serial.print(" G: "); Serial.print(green);
    Serial.print(" B: "); Serial.println(blue);

    lcd.setCursor(0, 1);

    if (red < green && red < blue) {
      lcd.print("Color: RED");
      myServo.write(30); // Move to RED position
    } else if (green < red && green < blue) {
      lcd.print("Color: GREEN");
      myServo.write(150); // Move to GREEN position
    } else if (blue < red && blue < green) {
      lcd.print("Color: BLUE");
      myServo.write(90); // Move to BLUE position
    } else {
      lcd.print("Unknown Color");
      myServo.write(90);
    }

    delay(2000);
    myServo.write(90); // Reset to center
    lcd.clear();
  } else {
    lcd.setCursor(0, 0);
    lcd.print("Waiting for Obj");
    delay(200);
  }
}

int readColor(int s2State, int s3State) {
  digitalWrite(S2, s2State);
  digitalWrite(S3, s3State);
  delay(100);
  return pulseIn(sensorOut, LOW);
}
