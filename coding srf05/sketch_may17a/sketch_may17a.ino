#include <SRF05.h>
#include <PID_v1.h>

#define ENA 6
#define IN1 2
#define IN2 3
#define IN3 7
#define IN4 4
#define ENB 5
#define ft_ult 8 //front ultrasonic trigger pin
#define lrt_ult 9 // left and right ultrasonic trigger pin
#define l_ult 10
#define f_ult 11
#define r_ult 12
#define LED 13
#define SDA A4
#define SCL A5
int pwmL = 90;
int pwmR = 30;
float fRef = 10, rlRef = 10, ErrL = 0, ErrF = 0, ErrR = 0;
int lSpd = 0, rSpd = 0;
float left, right, front;

SRF05 SRFL(lrt_ult, l_ult);
SRF05 SRFF(ft_ult, f_ult);
SRF05 SRFR(lrt_ult, r_ult);

void readUlt(){
  left = SRFL.getCentimeter();
  Serial.print(left);
  Serial.print(" ");
  front = SRFF.getCentimeter();
  Serial.print(front);
  Serial.print(" ");
  right = SRFR.getCentimeter();
  Serial.print(right);
  Serial.println(" ");
}

int motor(int speedL, int speedR){
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  analogWrite(ENA, speedL);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  analogWrite(ENB, speedR);
}

int motor_rotateR(int speedL, int speedR){
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  analogWrite(ENA, speedL);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  analogWrite(ENB, speedR);
}

int motor_rotateL(int speedL, int speedR){
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  analogWrite(ENA, speedL);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  analogWrite(ENB, speedR);
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
  SRFF.setCorrectionFactor(1.035);
  SRFL.setCorrectionFactor(1.035);
  SRFR.setCorrectionFactor(1.035);
}

void loop() {
  // put your main code here, to run repeatedly:
  readUlt();
  ErrL = left - rlRef;
  float kpL = 5;
  float settingL = ErrL * kpL;
  lSpd = pwmL - settingL;
  rSpd = pwmR + settingL;
  if (lSpd < -200){
    lSpd = -200;
  }
  if (rSpd > 255){
    rSpd = 240;
  }
  // if (left > 30 && left == 2){
  //   motor(-pwm, pwm);
  // }
  if (front > 15){
    if (left > 10) {
      motor(lSpd, rSpd);
    }
    // else if (left < 5 && left != 0 && right > 5){
    //   motor(lSpd-10, rSpd+10);
    // }
    else if (left < 10 && left != 0 && right == 0){
      motor(lSpd-10, rSpd+10);
    }
    else if (left == 0 && right == 0){
      motor(lSpd+10, rSpd-10);
    }
  }
  else {
    motor_rotateR(pwmL+20, pwmL+20);
    // else motor_rotateL(pwm, pwm);
  }
  Serial.print(left);
  Serial.print(" ");
  Serial.print(front);
  Serial.print(" ");
  Serial.println(right);
  Serial.print(" ");
  Serial.print(lSpd);
  Serial.print(" ");
  Serial.println(rSpd);
  // delay(50);
}
