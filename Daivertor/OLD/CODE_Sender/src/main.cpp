#include <Arduino.h>
#define RXD2 16
#define TXD2 17

const int A_OK = 9; //plc pin X021
const int B_OK = 8; // plc pin X020

const int start_move_motor = 4; //plc pin y015
const int end_process = 5; //plc pin y017

const int reset_pin = 34; 

const int test_pin = 12;

bool status_move_motor_to_A = false;
bool status_move_motor_to_B = false;

int move_m = 0;
int end_pro = 0;


// static unsigned long previousMillis = 0;
const unsigned long interval = 3000; // 1.5 seconds
unsigned long currentMillis = 0;

void setup() {
  delay(3000);
  Serial.begin(115200);    
  Serial2.begin(115200, SERIAL_8N1);

  pinMode(A_OK, OUTPUT);
  pinMode(B_OK, OUTPUT);
  pinMode(reset_pin, OUTPUT);
  pinMode(test_pin, OUTPUT);

  pinMode(start_move_motor, INPUT_PULLUP);
  pinMode(end_process, INPUT_PULLUP);
  digitalWrite(A_OK, LOW);
  digitalWrite(B_OK, LOW);
  digitalWrite(reset_pin, LOW);
}

void loop() 
{
  move_m = digitalRead(start_move_motor);
  end_pro = digitalRead(end_process);
  if (move_m == 1) 
  {
    digitalWrite(B_OK, LOW);
    Serial2.println("B");
    while (true) {
      if (Serial2.available()) {
        String response = Serial2.readStringUntil('\n');
        response.trim();
        if (response == "BOK") {
          digitalWrite(A_OK, HIGH);
          response = "";
          break;
        }
      }
      delay(3500);
    }
  }
  if (end_pro == 1) 
  {
    digitalWrite(A_OK, LOW);
    Serial2.println("A");
    while (true) {
      if (Serial2.available()) {
        String response = Serial2.readStringUntil('\n');
        response.trim();
        if (response == "AOK") {
          digitalWrite(B_OK, HIGH);
          response = "";
          break;
        }
      }
      delay(3500);
    }
  }
  

}

