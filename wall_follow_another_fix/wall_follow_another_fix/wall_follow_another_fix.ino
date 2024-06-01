#include <NewPing.h>

//defining pin
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

//defining ultrasonic variable
#define Sonar_Num 3
#define PING_INTERVAL 33
unsigned long pingTimer[Sonar_Num];
double cm[Sonar_Num];
uint8_t currentSensor = 0;

//defining variable motoR
int speed = 50;
int speedL, speedR;

//setting pid
double Err, SetPoint = 5, SumErr, LastErr = 0, dt = 0.1;
double Kil = 0.5, Kpl = 1, Kdl = 0.5;
double P, I, D, PIDl;
unsigned long currentTime, previousTime;
double elapsedTime;
double error;
double lastError;
double input, output, setPoint;
double cumError, rateError;

NewPing sonar[Sonar_Num] = {
  NewPing(lrt_ult, l_ult, 200),
  NewPing(ft_ult, f_ult, 200),
  NewPing(lrt_ult, r_ult, 200)
};

int motor(int speedL, int speedR){
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  analogWrite(ENA, speedR);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  analogWrite(ENB, speedL);
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

double computePID(double inp){     
        currentTime = millis();                //get current time
        elapsedTime = (double)(currentTime - previousTime);        //compute time elapsed from previous computation
        
        error = SetPoint - inp;                                // determine error
        cumError += error * elapsedTime;                // compute integral
        rateError = (error - lastError)/elapsedTime;   // compute derivative

        double out = Kpl*error + Kil*cumError + Kdl*rateError;                //PID output               

        lastError = error;                                //remember current error
        previousTime = currentTime;                        //remember current time

        return out;                                        //have function return the PID output
}

void wallfollow(){
  // Err = SetPoint - cm[0];
  // SumErr = LastErr + Err;
  // LastErr = Err;
  // P = Kpl * Err;
  // I = Kil * SumErr;
  // D = (Kdl)*(Err - LastErr);
  // PIDl = P + I + D;
  PIDl = computePID(cm[0]);
  speedL = speed - PIDl;
  speedR = speed + PIDl;
  motor(speedL, speedR);
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
}

void loop() {
  for (uint8_t i = 0; i < Sonar_Num; i++) { // Loop through each sensor and display results.
    delay(40); // Wait 50ms between pings (about 20 pings/sec). 29ms should be the shortest delay between pings.
    cm[i] = sonar[i].ping_cm();
  }
  if (cm[1] < 15){
    if (cm[0] > 40){
      motorL(speed+75, speed+70);
    }
    else {
      motorR(speed+70, speed+75);
    }
    delay(55);
  }
  // else if(cm[0] > 40 && cm [2] < 40){
  //   delay(20);
  //   motorL(speed+75, speed+70);
  //   delay(100);
  //   // motor(speed+30, speed+30);
  //   // delay(40);
  // }
  else if(cm[0] > 40 && cm [2] > 40){
    delay(20);
    motorL(speed+75, speed+70);
    delay(100);
    motor(speed+30, speed+30);
    delay(40);
  }
  else{
    wallfollow();
  }
  Serial.print(cm[0]);
  Serial.print(" ");
  Serial.print(cm[1]);
  Serial.print(" ");
  Serial.println(cm[2]);
  Serial.print(speedL);
  Serial.print(" ");
  Serial.println(speedR);
}

void oneSensorCycle() { // Sensor ping cycle complete, do something with the results.
  // The following code would be replaced with your code that does something with the ping results.
  for (uint8_t i = 0; i < Sonar_Num; i++) {
    Serial.print(i);
    Serial.print("=");
    Serial.print(cm[i]);
    Serial.print("cm ");
  }
  Serial.println();
}

void echoCheck() { // If ping received, set the sensor distance to array.
  if (sonar[currentSensor].check_timer()){
    if (currentSensor == 1){
      float dfront = sonar[currentSensor].ping_result / US_ROUNDTRIP_CM;
      if (dfront != 0){
        cm[currentSensor] = dfront;
      }
    }
    else{
      cm[currentSensor] = sonar[currentSensor].ping_result / US_ROUNDTRIP_CM;
    }
  }
}
