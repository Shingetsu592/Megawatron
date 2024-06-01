#include <Servo.h> // Include the Servo library 
#define ENA 5
#define IN1 6
#define IN2 7
#define IN3 8
#define IN4 9
#define ENB 10
#define echo A2    //Echo pin Ultrasonic
#define trigger A3 //Trigger pin Ultrasonic
#define servo A5
int servoPin = 11; // Declare the Servo pin 
Servo Servo1; // Create a servo object 
int Set=15;
int distance_L, distance_F, distance_R; 

void setup() {
  Serial.begin(9600);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);
  Servo1.attach(servoPin); 
  pinMode(servo, OUTPUT);
  for (int angle = 70; angle <= 140; angle += 5)  {
    servoPulse(servo, angle);  }
  for (int angle = 140; angle >= 0; angle -= 5)  {
   servoPulse(servo, angle);  }

  for (int angle = 0; angle <= 70; angle += 5)  {
   servoPulse(servo, angle);  }

distance_F = Ultrasonic_read();

delay(500);
}

void loop() {  
//==============================================
//==============================================  

distance_F = Ultrasonic_read();
Serial.print("D F=");Serial.println(distance_F);


}

void servoPulse (int pin, int angle){
int pwm = (angle*11) + 500;      // Convert angle to microseconds
 digitalWrite(pin, HIGH);
 delayMicroseconds(pwm);
   digitalWrite(pin, LOW);
 delay(50); // Refresh cycle of servo
}


//*******Ultrasonic_read*********
long Ultrasonic_read(){
  digitalWrite(trigger, LOW);
  delayMicroseconds(2);
  digitalWrite(trigger, HIGH);
  delayMicroseconds(10);
  long time = pulseIn (echo, HIGH);
  return time / 29 / 2;
}
void compareDistance(){
    if(distance_L > distance_R){
  turnLeft();
  delay(500);
  forword();
  delay(600);
  turnRight();
  delay(500);
  forword();
  delay(600);
  turnRight();
  delay(400);
  }
  else{
  turnRight();
  delay(500);
  forword();
  delay(600);
  turnLeft();
  delay(500);
  forword();
  delay(600);  
  turnLeft();
  delay(400);
  }
}

void Check_side(){
    Stop();
    delay(100);
 for (int angle = 70; angle <= 140; angle += 5)  {
   servoPulse(servo, angle);  }
    delay(300);
    distance_R = Ultrasonic_read();
    Serial.print("D R=");Serial.println(distance_R);
    delay(100);
  for (int angle = 140; angle >= 0; angle -= 5)  {
   servoPulse(servo, angle);  }
    delay(500);
    distance_L = Ultrasonic_read();
    Serial.print("D L=");Serial.println(distance_L);
    delay(100);
 for (int angle = 0; angle <= 70; angle += 5)  {
   servoPulse(servo, angle);  }
    delay(300);
    compareDistance();
}

void forword(){  //forword
digitalWrite(IN1, LOW); //Left Motor backword Pin 
digitalWrite(IN2, HIGH); //Left Motor forword Pin 
digitalWrite(IN3, HIGH); //Right Motor forword Pin 
digitalWrite(IN4, LOW); //Right Motor backword Pin 
}

void backword(){ //backword
digitalWrite(IN1, HIGH); //Left Motor backword Pin 
digitalWrite(IN2, LOW); //Left Motor forword Pin 
digitalWrite(IN3, LOW); //Right Motor forword Pin 
digitalWrite(IN4, HIGH); //Right Motor backword Pin 
}

void turnRight(){ //turnRight
digitalWrite(IN1, LOW); //Left Motor backword Pin 
digitalWrite(IN2, HIGH); //Left Motor forword Pin 
digitalWrite(IN3, LOW); //Right Motor forword Pin 
digitalWrite(IN4, HIGH); //Right Motor backword Pin 
}

void turnLeft(){ //turnLeft
digitalWrite(IN1, HIGH); //Left Motor backword Pin 
digitalWrite(IN2, LOW); //Left Motor forword Pin 
digitalWrite(IN3, HIGH); //Right Motor forword Pin 
digitalWrite(IN4, LOW); //Right Motor backword Pin 
}

void Stop(){ //stop
digitalWrite(IN1, LOW); //Left Motor backword Pin 
digitalWrite(IN2, LOW); //Left Motor forword Pin 
digitalWrite(IN3, LOW); //Right Motor forword Pin 
digitalWrite(IN4, LOW); //Right Motor backword Pin 
}
