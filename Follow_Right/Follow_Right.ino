#include <NewPing.h>
#include <Wire.h>
#include "Adafruit_TCS34725.h"

//defining pin
#define ENA 6
#define IN1 2
#define IN2 3
#define IN3 7
#define IN4 4
#define ENB 5
#define ft_ult 8 //front ultrasonic trigger pin
#define lrt_ult 9 // left and right ultrasonic trigger pin
#define l_ult 11
#define f_ult 12
#define r_ult 13
#define LED 10
#define SDA A4
#define SCL A5

//defining ultrasonic variable
#define Sonar_Num 3
unsigned long pingTimer[Sonar_Num];
double cm[Sonar_Num];
uint8_t currentSensor = 0;

//defining variable motoR
int speed = 80;
int speedL, speedR;

//color sensor
Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_4X);

//setting pid
double Err, SetPoint = 10, SumErr, LastErr = 0, dt = 0.1;
double Kil = 0, Kpl = 3, Kdl = 0;
double P, I, D, PIDl;

NewPing sonar[Sonar_Num] = {
  NewPing(lrt_ult, r_ult, 200),
  NewPing(ft_ult, f_ult, 200),
  NewPing(lrt_ult, l_ult, 200)
};

int motor(int speedL, int speedR){
  if (speedL > 0 && speedR > 0){
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    analogWrite(ENA, speedR);
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
    analogWrite(ENB, speedL);
  }
  else if (speedL < 0 && speedR > 0){
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    analogWrite(ENA, speedR);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);
    analogWrite(ENB, (speedL*(-1)));
  }
  else if (speedL > 0 && speedR < 0){
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
    analogWrite(ENA, (speedR*(-1)));
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
    analogWrite(ENB, speedL);
  }
  else if (speedL < 0 && speedR < 0){
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
    analogWrite(ENA, (speedR*(-1)));
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);
    analogWrite(ENB, (speedL*(-1)));
  }
  else if (speedL == 0 && speedR == 0){
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, LOW);
    analogWrite(ENA, speedR);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, LOW);
    analogWrite(ENB, speedL);
  }
}

int motorR(int speedL, int speedR){
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  analogWrite(ENA, speedR);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  analogWrite(ENB, speedL);
}

int motorL(int speedL, int speedR){
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  analogWrite(ENA, speedR);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  analogWrite(ENB, speedL);
}

void motorSpeed(){
  Err = SetPoint - cm[0];
  SumErr = LastErr + Err;
  LastErr = Err;
  P = Kpl * Err;
  I = Kil * SumErr;
  D = (Kdl)*(Err - LastErr);
  PIDl = P + I + D;
  speedL = speed - PIDl;
  speedR = speed + PIDl;
  if(speedL < 0){
    if (speedL-50 < -180){
      speedL = -160;
    }
    else{
      speedL = speedL-150;
    }
  }
  else if(speedL > 150){
    speedL = 150;
  }
  if(speedR < 0){
    if (speedR-50 < -180){
      speedL = -180;
    }
    else{
      speedR = speedR-150;
    }
  }
  else if(speedR > 150){
    speedR = 150;
  }
}

void setup() {
  // put your setup code here, to run once:
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(ft_ult, OUTPUT);
  pinMode(lrt_ult, OUTPUT);
  pinMode(l_ult, INPUT);
  pinMode(f_ult, INPUT);
  pinMode(r_ult, INPUT);
  Serial.begin(9600);
  if (tcs.begin()) {
    Serial.println("Found sensor");
  } else {
    Serial.println("No TCS34725 found ... check your connections");
    while (1);
  }
}
float red, green, blue;
void loop() {
  for (uint8_t i = 0; i < Sonar_Num; i++) { // Loop through each sensor and display results.
    delay(40); // Wait 40ms between pings (about 20 pings/sec). 29ms should be the shortest delay between pings.
    cm[i] = sonar[i].ping_cm();
  }
  // tcs.setInterrupt(false);  // turn on LED
  // delay(60);  // takes 50ms to read
  // tcs.getRGB(&red, &green, &blue);
  // tcs.setInterrupt(true);  // turn off LED
  if (red > 200){
    motor(0,0);
    delay(3000);
  }
  // else if (red < 200){
  else{
    if(cm[1] < 20){
      if (cm[0] > 40){
        motorR(speed+150, speed+120);
        delay(285);
        motor(speed, speed);
        delay(30);
      }
      else {
        motorL(speed+100, speed+150);
        delay(300);
        motor(speed, speed);
        delay(30);
      }
    }
    else if (cm[0] > 40){
      delay(30);
      motorR(speed+150, speed+120);
      delay(300);
      motor(speed, speed);
      delay(50);
    }
    else{
      motorSpeed();
      motor(speedL, speedR);
    }
  }
  
  tcs.setInterrupt(false);  // turn on LED
  delay(55);  // takes 50ms to read
  tcs.getRGB(&red, &green, &blue);

  Serial.print(red);
  Serial.print(" ");
  Serial.print(green);
  Serial.print(" ");
  Serial.println(blue);
  Serial.print(cm[0]);
  Serial.print(" ");
  Serial.print(cm[1]);
  Serial.print(" ");
  Serial.println(cm[2]);
  Serial.print(speedL);
  Serial.print(" ");
  Serial.println(speedR);
}