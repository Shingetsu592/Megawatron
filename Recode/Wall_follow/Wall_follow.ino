#include <PID_v1.h>
#include <NewPing.h>

#define ENA 5
#define IN1 2
#define IN2 3
#define IN3 7
#define IN4 4
#define ENB 6
#define t_ult 8
#define l_ult 9
#define f_ult 10
#define r_ult 11
#define b_ir A0
#define f_ir A1
int pwm = 100;
float fRef = 10, rlRef = 5, ErrL = 0, ErrF = 0, ErrR = 0;
int lSpd = 0, rSpd = 0;
float left, right, front;

NewPing Lsonar(t_ult, l_ult, 200);
NewPing Fsonar(t_ult, f_ult, 200);
NewPing Rsonar(t_ult, r_ult, 200);

void readUlt(){
//  delay(50);
 // left = Lsonar.ping_cm();
 // delay(50);
  float dfront = Fsonar.ping_cm();
  delay(50);

  if(dfront != 0 ) front = dfront;
  else front = 100;
 // right = Rsonar.ping_cm();
}

int motor(int speedL, int speedR){
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  analogWrite(ENA, speedL);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  analogWrite(ENB, speedR);
}

void setup() {
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(t_ult, OUTPUT);
  pinMode(f_ir, INPUT);
  pinMode(b_ir, INPUT);
  pinMode(l_ult, INPUT);
  pinMode(f_ult, INPUT);
  pinMode(r_ult, INPUT);
  Serial.begin(9600);
}

void loop(){
  readUlt();
  // ErrL = left - rlRef;
  // float kpL = 4.5;
  // float settingL = ErrL * kpL;
  // lSpd = pwm - settingL;
  // rSpd = pwm + settingL;
  // // if (left > 30 && left == 2){
  // //   motor(-pwm, pwm);
  // // }
  // if (front > 10 || front == 0){
  //   motor(-pwm, pwm);
  // }
  // else {
  //   motor(lSpd, rSpd);
  // }
  Serial.print(left);
  Serial.print(" ");
  Serial.print(front);
  Serial.print(" ");
  Serial.println(right);
  // Serial.print(" ");
  // Serial.print(lSpd);
  // Serial.print(" ");
  // Serial.println(rSpd);
  // delay(50);
}
