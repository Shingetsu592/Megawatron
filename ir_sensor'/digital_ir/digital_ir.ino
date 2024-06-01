#define ir1 8
#define ir2 9
int clr1, clr2;

void setup() {
  // put your setup code here, to run once:
  pinMode(ir1, INPUT);
  pinMode(ir2, INPUT);
  Serial.begin(9600);

}

void loop() {
  // put your main code here, to run repeatedly:
  clr1 = digitalRead(ir1);
  clr2 = digitalRead(ir2);
  Serial.print("ir 1 = ");
  Serial.println (clr1);
  Serial.print("ir 2 = ");
  Serial.println (clr2);
  delay(1000);

}
