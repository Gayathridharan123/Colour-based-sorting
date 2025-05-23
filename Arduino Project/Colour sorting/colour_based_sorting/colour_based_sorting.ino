#include <Servo.h>

// Color sensor pins
#define S0 4
#define S1 5
#define S2 6
#define S3 7
#define OUT 8

// IR sensor pin
#define IR_SENSOR 2

// Servo pin
#define SERVO_PIN 9

Servo myServo;

int redFrequency = 0;
int greenFrequency = 0;
int blueFrequency = 0;

void setup() {
  // Color sensor pin setup
  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);
  pinMode(OUT, INPUT);

  // IR sensor pin
  pinMode(IR_SENSOR, INPUT);

  // Set frequency scaling to 20%
  digitalWrite(S0, HIGH);
  digitalWrite(S1, LOW);

  // Start serial communication
  Serial.begin(9600);

  // Attach servo
  myServo.attach(SERVO_PIN);
  myServo.write(90);  // Initial position
}

void loop() {
  if (digitalRead(IR_SENSOR) == LOW) {  // Object detected
    Serial.println("Object detected!");

    // Read red component
    digitalWrite(S2, LOW);
    digitalWrite(S3, LOW);
    redFrequency = pulseIn(OUT, LOW);

    // Read green component
    digitalWrite(S2, HIGH);
    digitalWrite(S3, HIGH);
    greenFrequency = pulseIn(OUT, LOW);

    // Read blue component
    digitalWrite(S2, LOW);
    digitalWrite(S3, HIGH);
    blueFrequency = pulseIn(OUT, LOW);

    Serial.print("R: ");
    Serial.print(redFrequency);
    Serial.print(" G: ");
    Serial.print(greenFrequency);
    Serial.print(" B: ");
    Serial.println(blueFrequency);

    // Simple color detection logic
    if (redFrequency < greenFrequency && redFrequency < blueFrequency) {
      Serial.println("Red Color Detected");
      myServo.write(0);  // Move to position 0°
    } else if (greenFrequency < redFrequency && greenFrequency < blueFrequency) {
      Serial.println("Green Color Detected");
      myServo.write(90);  // Move to position 90°
    } else if (blueFrequency < redFrequency && blueFrequency < greenFrequency) {
      Serial.println("Blue Color Detected");
      myServo.write(180);  // Move to position 180°
    } else {
      Serial.println("Unknown Color");
      myServo.write(90);  // Default position
    }

    delay(1000);  // Wait before next reading
  } else {
    myServo.write(90);  // Neutral position if no object
  }

  delay(200);
}
