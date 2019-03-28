int led1 = 13;

int leds[7] =   {12,11,10,9,8,7,2};
int status[7] = {0 ,0 ,0 ,0,0,0,0};

int s_presenca = A0;
float s_luz = A1;

void controle(int pino,int opcao);

void setup() {
  // put your setup code here, to run once:
   Serial.begin(9600);
   pinMode(led1,OUTPUT);
   int i = 0;
   for(i = 0; i < 7; i++){
    Serial.println("%d %d\n",leds[i],status[i]);
    pinMode(leds[i],OUTPUT);
   }
  
   pinMode(s_presenca,INPUT);
   pinMode(s_luz,INPUT);
   
   
}

void loop() {
  // checa presensa e mudar status led
  Serial.print("Sensor de Presenca: ");
  Serial.print(status_presenca());
  Serial.print("\n");
  
  // Controle de leds
  controle(7,1);
  delay(1000);
  controle(7,0);
  delay(1000);
}

void controle(int pino,int opcao){
   int i = 0;
   for(i = 0; i < 7;i++){
     if (leds[i] == pino){
      status[i] = opcao;
      digitalWrite(leds[i],opcao);
     }
     printf("%d %d\n",leds[i],status[i]);
   }
}


int status_presenca()
 {if (analogRead(s_presenca) == 1){
     digitalWrite(led1,HIGH);
     return 1;
  }else{
    digitalWrite(led1,LOW);
    return 0;
  }
 }