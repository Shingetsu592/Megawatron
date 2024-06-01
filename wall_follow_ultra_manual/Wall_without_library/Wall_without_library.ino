#include <PID_v1.h>
#include <NewPing.h>


#define ENA 10
#define IN1 9
#define IN2 8
#define IN3 6
#define IN4 7
#define ENB 5
#define t_ult 3
#define l_ult 11
#define f_ult 12
#define r_ult 13
int pwm = 80;
long l_dur, f_dur, r_dur;
int l_dist, f_dist, r_dist;

void moveForward(){
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  analogWrite(ENA, pwm);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  analogWrite(ENB, pwm);
}

void turnLeft(){
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  analogWrite(ENA, pwm);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  analogWrite(ENB, pwm);
}

void turnRight(){
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  analogWrite(ENA, pwm);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  analogWrite(ENB, pwm);
}

void turnFRight(){
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  analogWrite(ENA, pwm-10);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  analogWrite(ENB, pwm);
}

void turnFLeft(){
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  analogWrite(ENA, pwm);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  analogWrite(ENB, pwm-10);
}

void Stop(){
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  analogWrite(ENA, 0);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
  analogWrite(ENB, 0);
}

void readUlt(){
  delay(50);
  digitalWrite(t_ult, LOW);
  delayMicroseconds(2);
  digitalWrite(t_ult, HIGH);
  delayMicroseconds(10);
  digitalWrite(t_ult, LOW);
  l_dur = pulseIn(l_ult, HIGH);
  l_dist = l_dur * 0.0343 / 2;
  delay(50);
  digitalWrite(t_ult, LOW);
  delayMicroseconds(2);
  digitalWrite(t_ult, HIGH);
  delayMicroseconds(10);
  digitalWrite(t_ult, LOW);
  f_dur = pulseIn(f_ult, HIGH);
  f_dist = f_dur * 0.0343 / 2;
  delay(50);
  digitalWrite(t_ult, LOW);
  delayMicroseconds(2);
  digitalWrite(t_ult, HIGH);
  delayMicroseconds(10);
  digitalWrite(t_ult, LOW);
  r_dur = pulseIn(r_ult, HIGH);
  r_dist = r_dur * 0.0343 / 2;
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
  pinMode(l_ult, INPUT);
  pinMode(f_ult, INPUT);
  pinMode(r_ult, INPUT);
  Serial.begin(9600);
}

void loop() {
  readUlt();
  if (f_dist <= 10){
    if (l_dist < r_dist && l_dist < 10){
      turnRight();
    }
    else if (l_dist > r_dist && r_dist < 10){
      turnLeft();
    }
  }
  else {
    if (l_dist > 10){
      turnFLeft();
    }
    else if (r_dist > 10){
      turnFRight();
    }
    else{
      moveForward();
    }
  }
  Serial.print(l_dist);
  Serial.print(" ");
  Serial.print(f_dist);
  Serial.print(" ");
  Serial.println(r_dist);
}
