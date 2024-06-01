#include <PID_v1.h>
#include <NewPing.h>

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
int pwm = 80;
float fRef = 10, rlRef = 10, ErrL = 0, ErrF = 0, ErrR = 0;
int lSpd = 0, rSpd = 0;
float left, right, front;

NewPing Lsonar(lrt_ult, l_ult, 100);
NewPing Fsonar(ft_ult, f_ult, 100);
NewPing Rsonar(lrt_ult, r_ult, 100);

void readUlt(){
  delay(50);
  left = Lsonar.ping_cm();
  delay(50);
  float dfront = Fsonar.ping_cm();
  delay(50);
  if(dfront != 0 ) front = dfront;
  right = Rsonar.ping_cm();
}

int motor(int speedL, int speedR){
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  analogWrite(ENA, speedR);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  analogWrite(ENB, speedL);
}

int motor_rotateR(int speedL, int speedR){
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  analogWrite(ENA, speedR);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  analogWrite(ENB, speedL);
}

int motor_rotateL(int speedL, int speedR){
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  analogWrite(ENA, speedR);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  analogWrite(ENB, speedL);
}

void setup() {
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
}

void loop(){
  readUlt();
  ErrL = left - rlRef;
  float kpL = 1.5;
  float settingL = ErrL * kpL;
  lSpd = pwm - settingL;
  rSpd = pwm + settingL;
  if (lSpd < -200){
    lSpd = -200;
  }
  if (rSpd > 255){
    rSpd = 240;
  }
  // if (left > 30 && left == 2){
  //   motor(-pwm, pwm);
  // }
  if (front > 10){
    if (left > 10) {
      motor(lSpd, rSpd);
    }
    else if (left < 10 && left != 0){
      motor(lSpd-10, rSpd+10);
    }
    else if (left == 0 || right == 0){
      motor_rotateR(pwm, pwm);
    }
  }
  else {
    motor_rotateL(pwm+20, pwm+20);
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