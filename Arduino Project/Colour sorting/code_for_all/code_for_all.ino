#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Servo.h>

// LCD Address 0x27 is common, change if yours is different
LiquidCrystal_I2C lcd(0x27, 16, 2);

Servo sortingServo;

// TCS3200 Pins
int s0 = 4;
int s1 = 5;
int s2 = 6;
int s3 = 7;
int outPin = 2;

// IR sensor pin
int irPin = 8;

// Servo positions for sorting (adjust based on your setup)
int posNeutral = 90;
int posRed = 45;
int posGreen = 135;
int posBlue = 0;

void setup()
 {
  pinMode(s0, OUTPUT);
  pinMode(s1, OUTPUT);
  pinMode(s2, OUTPUT);
  pinMode(s3, OUTPUT);
  pinMode(outPin, INPUT);
  pinMode(irPin, INPUT);

  Serial.begin(9600);
  lcd.init();
  lcd.backlight();

  // Frequency scaling 20%
  digitalWrite(s0, HIGH);
  digitalWrite(s1, LOW);

  sortingServo.attach(9);
  sortingServo.write(posNeutral);

  lcd.setCursor(0,0);
  lcd.print("Color Sorting");
  delay(5000);
  lcd.clear();
}

void loop() 
{
  if (digitalRead(irPin) == LOW)
   {  // Object detected by IR sensor
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Object detected");

    // Read Red
    digitalWrite(s2, LOW);
    digitalWrite(s3, LOW);
    int red = pulseIn(outPin, LOW);

    // Read Green
    digitalWrite(s2, HIGH);
    digitalWrite(s3, HIGH);
    int green = pulseIn(outPin, LOW);

    // Read Blue
    digitalWrite(s2, LOW);
    digitalWrite(s3, HIGH);
    int blue = pulseIn(outPin, LOW);

    Serial.print("R:");
    Serial.print(red);
    Serial.print(" G:");
    Serial.print(green);
    Serial.print(" B:");
    Serial.println(blue);

    lcd.setCursor(0,1);

    // Decide color - lower pulse = stronger color intensity
    if (red < green && red < blue)
     {
      Serial.println("Red object detected");
      lcd.print("Red Detected");
      sortingServo.write(posRed);
    } 

    else if (green < red && green < blue) 
    {
      Serial.println("Green object detected");
      lcd.print("Green Detected");
      sortingServo.write(posGreen);
    } 

    else if (blue < red && blue < green)
     {
      Serial.println("Blue object detected");
      lcd.print("Blue Detected");
      sortingServo.write(posBlue);
    } 
  } 


  else
   {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Waiting for Obj");
    sortingServo.write(posNeutral);
  }

  delay(500);
}
