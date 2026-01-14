#include <Arduino.h>

const int stepPin = 11;    // Pin for stepper motor step
const int dirPin = 7;     // Pin for stepper motor direction
const int enb_pin = 15;    // Pin for stepper motor enable
const int proximity_sensorA = 46; // Pin for proximity sensor
const int proximity_sensorB = 47; // Pin for proximity sensor

bool isRunning = false;
unsigned long lastPulseTime = 0;
unsigned long pulseInterval = 100; // Default to lowest speed 150 

bool stepState = false;
bool debug_to_a = false;
bool debug_to_b = false;

void setup() 
{
  Serial.begin(115200);

  pinMode(proximity_sensorA, INPUT_PULLUP);
  pinMode(proximity_sensorB, INPUT_PULLUP);
  pinMode(stepPin, OUTPUT);
  pinMode(dirPin, OUTPUT);
  pinMode(enb_pin, OUTPUT);
  digitalWrite(enb_pin,HIGH);
}

void loop() 
{
  if (isRunning && (micros() - lastPulseTime >= pulseInterval))
  {
    lastPulseTime = micros();
    stepState = !stepState;
    digitalWrite(stepPin, stepState);
    // Serial.println("Pulse");
  }
  if (Serial.available() > 0) 
  {
    String command = Serial.readStringUntil('\n');
    command.trim();
    
    if (command == "A") 
    {
      isRunning = true;
      digitalWrite(enb_pin, LOW);
      digitalWrite(dirPin, LOW);
      // Serial.println("move to a");
    } 
    else if (command == "B") 
    {
      isRunning = true;
      digitalWrite(enb_pin, LOW);
      digitalWrite(dirPin, HIGH);
      // Serial.println("move to b");
    } 
    else if (command == "da")
    {
      debug_to_a = true;
      debug_to_b = false;
      // Serial.println("Debug to a");
    }
    else if (command == "db")
    {
      debug_to_b = true;
      debug_to_a = false;
      // Serial.println("Debug to b");
    }
    else if (command == "s")
    {
      isRunning = false;
      digitalWrite(enb_pin, HIGH);
      // Serial.println("Motor stopped");
    }
    else 
    {
      Serial.println("Unknown command");
    }
  }
  //============================================= DEBGU MODE ==============================================

  int sensorValue1 = digitalRead(proximity_sensorA);
  int sensorValue2 = digitalRead(proximity_sensorB);
  if ((sensorValue1 == LOW && digitalRead(dirPin) == LOW)) 
  // if ((debug_to_a == true && digitalRead(dirPin) == HIGH) || (debug_to_b == true && digitalRead(dirPin) == LOW)) // is debug mode
  {
    isRunning = false;
    digitalWrite(enb_pin, HIGH);
  }
  else if ((sensorValue2 == LOW && digitalRead(dirPin) == HIGH))
  {
    isRunning = false;
    digitalWrite(enb_pin, HIGH);
  }
  //============================================= DEBGU MODE ==============================================
}