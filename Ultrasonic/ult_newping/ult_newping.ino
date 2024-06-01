#include <NewPing.h>

#define l_ult 10
#define r_ult 12
#define f_ult 11
#define t_ult 9
int l_dist, r_dist, f_dist;

NewPing Lsonar(t_ult, l_ult, 250);
NewPing Rsonar(t_ult, r_ult, 250);
NewPing Fsonar(t_ult, f_ult, 250);

void setup() {
  // put your setup code here, to run once:
  pinMode(t_ult, OUTPUT);
  pinMode(l_ult, INPUT);
  pinMode(r_ult, INPUT);
  Serial.begin(9600);
}

void loop() {
  delay(500);
  l_dist = Lsonar.ping_cm();
  delay(500);
  r_dist = Rsonar.ping_cm();
  delay(500);
  f_dist = Fsonar.ping_cm();
  Serial.print("Left: ");
  Serial.println(l_dist);
  Serial.print("Front: ");
  Serial.println(f_dist);
  Serial.print("Right: ");
  Serial.println(r_dist);
}
