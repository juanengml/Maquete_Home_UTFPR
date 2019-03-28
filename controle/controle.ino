int led1 = 13;
int led2 = 12;
int led3 = 11;
int led4 = 10;

int led5 = 9;
int led6 = 8;
int led7 = 7;
int led8 = 2;

int s_presenca = A0;
float s_luz = A1;

void setup() {
  // put your setup code here, to run once:
   pinMode(led1,OUTPUT);
   pinMode(led2,OUTPUT);
   pinMode(led3,OUTPUT);
   pinMode(led4,OUTPUT);

   pinMode(s_presenca,INPUT);
   pinMode(s_luz,INPUT);
   
   pinMode(led5,OUTPUT);
   pinMode(led6,OUTPUT);
   pinMode(led7,OUTPUT);
   pinMode(led8,OUTPUT);
   Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
//  testLeds();
  testSensors();
 
}

void testSensors(){
  float presenca;
  float luz;

  presenca = analogRead(s_presenca);
// luz      = map(analogRead(s_luz),0,1023, 0,255);
  Serial.print("Sensor de Presenca: ");
  Serial.print(presenca);
 // Serial.print("\nSensor de Luz: ");
//  Serial.print(luz);
  Serial.print("\n");
  delay(200);
}

void testLeds(){
  digitalWrite(led1,HIGH);
  delay(500);
  digitalWrite(led2,HIGH);
  delay(500);
  digitalWrite(led3,HIGH);
  delay(500);
  digitalWrite(led4,HIGH);
  delay(500);
  digitalWrite(led5,HIGH);
  delay(500);
  digitalWrite(led6,HIGH);
  delay(500);
  digitalWrite(led7,HIGH);
  delay(500);
  digitalWrite(led8,HIGH);
  delay(1000);
  digitalWrite(led8,LOW);
  delay(500);
  digitalWrite(led7,LOW);
  delay(500);
  digitalWrite(led6,LOW);
  delay(500);
  digitalWrite(led5,LOW);
  delay(500);
  digitalWrite(led4,LOW);
  delay(500);
  digitalWrite(led3,LOW);
  delay(500);
  digitalWrite(led2,LOW);
  delay(500);
  digitalWrite(led1,LOW);
  delay(1000);
}
