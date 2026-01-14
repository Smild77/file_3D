#include <Arduino.h>
#define RXD2 16
#define TXD2 17

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

bool lock_work_m_move = false;

void serialEvent_D(void);
void serialEvent_B(void);
bool isSensor1StableLow(void);
bool isSensor2StableLow(void);
void stopMotorAndNotify(const String& message);
void setup() 
{
  Serial.begin(115200);
  Serial2.begin(115200,SERIAL_8N1);
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
  }
  // if (Serial.available() > 0) 
  if (Serial2.available() > 0)
  {
    // String command = Serial.readStringUntil('\n');
    String command = Serial2.readStringUntil('\n');
    command.trim();

    if (command == "A") 
      {
        if (isSensor1StableLow()) 
          {
            Serial2.println("AOK");
            isRunning = false;
            digitalWrite(enb_pin, HIGH);
            lock_work_m_move = false;
          } 
        else 
          {
            isRunning = true;
            lock_work_m_move = true;
            digitalWrite(enb_pin, LOW);
            digitalWrite(dirPin, HIGH);
          }
      }

    if (command == "B") 
      {
        if (isSensor2StableLow()) 
        {
          Serial2.println("BOK");
          isRunning = false;
          digitalWrite(enb_pin, HIGH);
          lock_work_m_move = false;
        } 
        else 
        {
          isRunning = true;
          lock_work_m_move = true;
          digitalWrite(enb_pin, LOW);
          digitalWrite(dirPin, LOW);
        }
      }

    else if (command == "da")
    {
      debug_to_a = true;
      debug_to_b = false;
    }
    else if (command == "db")
    {
      debug_to_b = true;
      debug_to_a = false;
    }
    else if (command == "s")
    {
      isRunning = false;
      digitalWrite(enb_pin, HIGH);
    }
    else if (command == "d")
    {
      serialEvent_D();
    }
    else if (command == "b")
    {
      serialEvent_B();
    }
    else 
    {
      // Serial.println("Unknown command");
    }
  }

  if (lock_work_m_move == true)
  {
    if (isSensor1StableLow() == 1 && digitalRead(dirPin) == HIGH)
    {
      stopMotorAndNotify("AOK");
    }
    else if (isSensor2StableLow() == 1 && digitalRead(dirPin) == LOW)
    {
      stopMotorAndNotify("BOK");
    }
  }
  //============================================= DEBGU MODE ==============================================
}

bool isSensor1StableLow() 
{
    if (digitalRead(proximity_sensorA) != LOW) 
    {
      return false;
    }
    else
    {
      return true;
    }
}
bool isSensor2StableLow() 
{
    if (digitalRead(proximity_sensorB) != LOW) 
    {
      return false;
    }
    else
    {
      return true;
    }
}

void stopMotorAndNotify(const String& message) {
  isRunning = false;
  stepState = false;

  for (int i = 0; i < 200; i++) 
  {
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(100); 
    digitalWrite(stepPin, LOW);
    delayMicroseconds(100);
  }
  digitalWrite(enb_pin, HIGH); 
  delay(300); 
  Serial2.println(message);
  lock_work_m_move = false;
}

void serialEvent_D()
{
  delay(1000);
  Serial2.println("DOK");
}

void serialEvent_B()
{
  delay(1000);
  Serial2.println("BOK");
}