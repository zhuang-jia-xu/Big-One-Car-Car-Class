#include "bluetooth.h"
#include "RFID.h"
#include "globals.h"

#define DEBUG

#define IR0 A0
#define IR1 A4
#define IR2 A1
#define IR3 A3
#define IR4 A2

#define ENA 5
#define IN1 2
#define IN2 3
#define IN3 4
#define IN4 7
#define ENB 6

void setup()
{
  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENB, OUTPUT);

  pinMode(IR0, INPUT);
  pinMode(IR1, INPUT);
  pinMode(IR2, INPUT);
  pinMode(IR3, INPUT);
  pinMode(IR4, INPUT);
  Serial.begin(9600);
  mfrc522 = MFRC522(SS_PIN, RST_PIN);
  SPI.begin();
  mfrc522.PCD_Init();
  BT.begin(9600);
}

void motorWrite(float Vl, float Vr)
{
  if (Vl > 255)
    Vl = 255;
  if (Vr > 255)
    Vr = 255;
  if (Vl < -255)
    Vl = -255;
  if (Vr < -255)
    Vr = -255;
  analogWrite(ENA, Vl > 0 ? Vl : -Vl);
  analogWrite(ENB, Vr > 0 ? Vr : -Vr);
  digitalWrite(IN1, Vl > 0 ? 1 : 0);
  digitalWrite(IN3, Vr > 0 ? 1 : 0);
  digitalWrite(IN2, Vl > 0 ? 0 : 1);
  digitalWrite(IN4, Vr > 0 ? 0 : 1);
}

// parameter: 0 or 1 or 2, reprenting left, right, back respectively
// return: parameter
bool drive(BT_CMD direction)
{
  static bool stop = false;
  switch (direction)
  {
    case LEFT:
      stop = false;
      motorWrite(-255, -255);
      delay(50);
      motorWrite(-180, 255);
      delay(300);
      motorWrite(200, 200);
      break;
    case RIGHT:
      stop = false;
      motorWrite(-255, -255);
      delay(50);
      motorWrite(255, -180);
      delay(450);
      motorWrite(200, 200);
      break;
    case BACK:
      stop = false;
      motorWrite(-255, -255);
      delay(50);
      motorWrite(-255, 255);
      delay(600);
      motorWrite(200, 200);
      break;
    case STOP:
      if (!stop) {
        motorWrite(-255, -255);
        delay(20);
      }
      motorWrite(0, 0);
      stop = true;
      break;
    case FORWARD:
      stop = false;
      motorWrite(200, 200);
      return false;
      break;
  }
  return true;
}

void tracking()
{
  static int error;
  int current_error = R1 + R2 - L2 - L1;
  int d_error = current_error - error;
  int left = 100 + 0.3 * error + 0.2 * d_error;
  int right = 100 - 0.5 * error - 0.2 * d_error;
  error = current_error;
  motorWrite(left, right);
}

bool checkNode()
{
  static const int threshold = 700;
  static double arr[100] {};
  static double sum = 0;
  static int ticks = 0;
  sum -= arr[ticks % 100];
  arr[ticks % 100] = double(R2 + R1 + M + L1 + L2) / 100;
  sum += arr[ticks % 100];
  ticks++;
//#ifdef DEBUG
//  //    Serial.print("sum = ");
//  //    Serial.println(sum);
//  //    Serial.println(R2 + R1 + M + L1 + L2);
//#endif
  return sum >= threshold;
}

void loop()
{
  static byte *id;
  static int time = 0;
  static int turn = 0;

  //update sensor values
  R1 = analogRead(IR0) * 0.7;
  R2 = analogRead(IR1) * 0.5;
  M = analogRead(IR2);
  L2 = analogRead(IR3) * 0.6;
  L1 = analogRead(IR4) * 0.8;
  tracking();
    bool a=checkNode();
    if (lastcheck!=a&&a==1)
    {
        drive(dir[turn]);
    }
    if (id = rfid(idSize))
      send_byte(id, idSize);
    lastcheck=a;
  }
}