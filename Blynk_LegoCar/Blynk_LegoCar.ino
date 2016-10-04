#include <Arduino.h>

/**************************************************************
   Blynk is a platform with iOS and Android apps to control
   Arduino, Raspberry Pi and the likes over the Internet.
   You can easily build graphic interfaces for all your
   projects by simply dragging and dropping widgets.

     Downloads, docs, tutorials: http://www.blynk.cc
     Blynk community:            http://community.blynk.cc
     Social networks:            http://www.fb.com/blynkapp
                                 http://twitter.com/blynk_app

   Blynk library is licensed under MIT license
   This example code is in public domain.

 **************************************************************
   This example shows how to use Software Serial
   to connect your project to Blynk.
   Be sure to check ordinary Serial example first!!!

 **************************************************************/

#define BLYNK_PRINT Serial    // Comment this out to disable prints and save space
#define BLYNK_DEBUG
#define BLYNK_TIMEOUT_MS 5000 // Longer timeout as connection was sometimes lost

#include <BlynkSimpleStream.h>
#include <SoftwareSerial.h>

#define rxPin 10
#define txPin 11

const int forwardPin = 5;     // orange
const int backwardPin = 6;   // brown
const int leftPin = 3;    // green
const int rightPin = 2;   // yellow

SoftwareSerial SwSerial(rxPin, txPin); // RX, TX

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "YourAuthToken";

void setup()
{
  // Debug console
  Serial.begin(9600);
  Serial.print("BLYNK_TIMEOUT_MS : ");
  Serial.println(BLYNK_TIMEOUT_MS);
  Serial.print("BLYNK_MAX_SENDBYTES : ");
  Serial.println(BLYNK_MAX_SENDBYTES);
  delay(10000);

  // define pin modes for tx, rx:
  pinMode(rxPin, INPUT);
  pinMode(txPin, OUTPUT);

  // Blynk will work through SoftwareSerial
  SwSerial.begin(9600);
  Blynk.begin(auth, SwSerial);

  // define pin modes for car pins
  pinMode(forwardPin, OUTPUT);
  pinMode(backwardPin, OUTPUT);
  analogWrite(forwardPin, 0);
  analogWrite(backwardPin, 0);
  digitalWrite(leftPin, LOW);
  digitalWrite(rightPin, LOW);
}

BLYNK_WRITE(V1) {
  int x = param[0].asInt();
  int y = param[1].asInt();

  // steering wheel control
  if (x < -25) {
    // ensure movement is reset before start turning the other direction
    // power to both directions might damage H-Bridge
    digitalWrite(rightPin, LOW);
    digitalWrite(leftPin, HIGH);
    Serial.println("Turn left");
  } else {
    if (x > 25) {
      digitalWrite(leftPin, LOW);
      digitalWrite(rightPin, HIGH);
      Serial.println("Turn right");
    } else {
      digitalWrite(leftPin, LOW);
      digitalWrite(rightPin, LOW);
    }
  }

  // drive control
  if (y > 0) {
    // ensure movement is reset before start driving the other direction
    // power to both directions might damage H-Bridge
    analogWrite(backwardPin, 0);
    analogWrite(forwardPin, y * 2);
    Serial.println("Driving forward");
  } else {
    if (y < 0) {
      analogWrite(forwardPin, 0);
      analogWrite(backwardPin, -y * 2);
      Serial.println("Driving backward");
    } else {
      analogWrite(forwardPin, 0);
      analogWrite(backwardPin, 0);
    }

  }

}

void loop()
{
  Blynk.run();
}
