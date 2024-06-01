#include <PID_v1.h>
#include <NewPing.h>

#define ENA 5
#define IN1 6
#define IN2 7
#define IN3 2
#define IN4 4
#define ENB 3
#define t_ult 11
#define l_ult 10
#define f_ult 9
#define r_ult 8
#define b_ir A0
#define f_ir A1
int pwm = 120;
long f_dur;
int l_dist, f_dist, r_dist;
double Setpoint, Input, Output;
double Kp=2, Ki=0, Kd=1;

PID myPID(&Input, &Output, &Setpoint, Kp, Ki, Kd, DIRECT);

void moveForward() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  analogWrite(ENA, pwm - 5);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  analogWrite(ENB, pwm);
}

void turnLeft() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  analogWrite(ENA, pwm - 5);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  analogWrite(ENB, pwm);
}

void turnRight() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  analogWrite(ENA, pwm - 5);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  analogWrite(ENB, pwm);
}

void turnFLeft() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  analogWrite(ENA, pwm - 10);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  analogWrite(ENB, pwm);
}

void turnFRight() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  analogWrite(ENA, pwm);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  analogWrite(ENB, pwm - 10);
}

void Stop() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  analogWrite(ENA, 0);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
  analogWrite(ENB, 0);
}

NewPing Lsonar(t_ult, l_ult, 200);
NewPing Fsonar(t_ult, f_ult, 200);
NewPing Rsonar(t_ult, r_ult, 200);

void readUlt() {
  delay(50);
  l_dist = Lsonar.ping_cm();
  delay(50);
  f_dist = Fsonar.ping_cm();
  delay(50);
  r_dist = Rsonar.ping_cm();
}

void setup() {
  // put your setup code here, to run once:
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
  myPID.SetMode(AUTOMATIC);
  Serial.begin(9600);
}

void loop() {
  readUlt();
  if (f_dist <= 20 && f_dist != 0) {
    if (l_dist < r_dist && l_dist < 10 && l_dist != 0) {
      turnRight();
    }
    else if (l_dist > r_dist && r_dist < 10 && r_dist != 0) {
      turnLeft();
    }
    else{
      turnLeft();
    }
  }
  else {
    if (l_dist > 10) {
      turnFLeft();
    }
    else if (r_dist > 10) {
      turnFRight();
    }
    else {
      moveForward();
    }
  }
  Serial.print(l_dist);
  Serial.print(" ");
  Serial.print(f_dist);
  Serial.print(" ");
  Serial.println(r_dist);
}
