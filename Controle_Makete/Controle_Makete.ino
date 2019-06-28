#include <ArduinoJson.h>
#include <PubSubClient.h>
#include <Ethernet.h>
#include <Servo.h>

Servo Garagem; 


#define TOKEN "dTIeNux208W2uxRzcwme" // ThingsBoard Device Auth Token
#define ID_DEVICE "8e2ed310-7bf8-11e9-be51-21f72e58f847"

// PINOS DO ARDUINO 

#define GPIO14 14
#define GPIO15 15
#define GPIO16 16
#define GPIO17 17
#define GPIO18 18
#define GPIO19 19
#define GPIO20 20
#define GPIO21 21
#define GPIO22 22

 // PINOS DA THINGSBOARD 

#define GPIO14_PIN 1
#define GPIO15_PIN 2
#define GPIO16_PIN 3
#define GPIO17_PIN 4
#define GPIO18_PIN 5
#define GPIO19_PIN 6
#define GPIO20_PIN 7
#define GPIO21_PIN 8
#define GPIO22_PIN 9

long ultimo_valor = 0;

void  printIPAddress();
void    reconnect();
void Pega_Envia_Sensores();
void on_message(const char* topic, byte* payload, unsigned int length);

// configurações de servidor e mac E
byte mac[]    = {  0xDE, 0xED, 0xBA, 0xFE, 0xFE, 0xED };
IPAddress server(200, 134, 31, 225);

// IMPORTANTE: para cada led um novo deve ser adicionado, caso queira addicionar novos leds 

boolean gpioState[] = {false, false,false,false,false,false,false,false,false};
EthernetClient ethClient;
PubSubClient client(server, 1883, on_message, ethClient);

// setup 

void setup() {
  Serial.begin(9600);
  Garagem.attach(7); 
  Garagem.write(0);
  pinMode(A0,INPUT);
  pinMode(A1,INPUT);
  pinMode(8,OUTPUT);    // banheiro suite
  pinMode(9,OUTPUT);    // corredor

  
  pinMode(13,OUTPUT);
  pinMode(GPIO14, OUTPUT);
  pinMode(GPIO15, OUTPUT);
  pinMode(GPIO16, OUTPUT);
  pinMode(GPIO17, OUTPUT);
  pinMode(GPIO18, OUTPUT);
  pinMode(GPIO19, OUTPUT);
  pinMode(GPIO20, OUTPUT);
  pinMode(GPIO21, OUTPUT);
  pinMode(GPIO22, OUTPUT);

  delay(10);

  if (Ethernet.begin(mac) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");
  }

  delay(1500);
  printIPAddress();

  client.setServer( server, 1883 );


  client.setCallback(on_message);
}

// LOOP 

void loop() {
  if ( !client.connected() ) {
    reconnect();
  }
  long atual = millis();
  if (millis() - ultimo_valor  > 5000) { 
    Pega_Envia_Sensores();
    ultimo_valor = millis();
  }

  digitalWrite(9,digitalRead(A0) ? HIGH:LOW);
  digitalWrite(8,digitalRead(A1) ? HIGH:LOW);
  

  client.loop();
}

/*
  recebe mensagens e trata para controle nas portas do arduino
*/
void on_message(const char* topic, byte* payload, unsigned int length) {

  Serial.println("On message");

  char json[length + 1];
  strncpy (json, (char*)payload, length);
  json[length] = '\0';

  Serial.print("Topic: ");
  Serial.println(topic);
  Serial.print("Message: ");
  Serial.println(json);

  // Decode JSON request
  StaticJsonBuffer<200> jsonBuffer;
  JsonObject& data = jsonBuffer.parseObject((char*)json);

  if (!data.success())
  {
    Serial.println("parseObject() failed");
    return;
  }

  // Check request method
  String methodName = String((const char*)data["method"]);

  if (methodName.equals("getGpioStatus")) {
    // Reply with GPIO status
    String responseTopic = String(topic);
    responseTopic.replace("request", "response");
    client.publish(responseTopic.c_str(), get_gpio_status().c_str());
    client.publish("v1/devices/me/attributes", get_gpio_status().c_str());

} else if (methodName.equals("setGpioStatus")) {
    // Update GPIO status and reply
    set_gpio_status(data["params"]["pin"], data["params"]["enabled"]);
    String responseTopic = String(topic);
    responseTopic.replace("request", "response");
    client.publish(responseTopic.c_str(), get_gpio_status().c_str());
    client.publish("v1/devices/me/attributes", get_gpio_status().c_str());
  }
}


/*
  Atualiza data string para valores dos pinos 
*/
String get_gpio_status() {
  // Prepare gpios JSON payload string
  StaticJsonBuffer<200> jsonBuffer;
  JsonObject& data = jsonBuffer.createObject();
  data[String(GPIO14_PIN)] = gpioState[0] ? true : false;
  data[String(GPIO15_PIN)] = gpioState[1] ? true : false;
  data[String(GPIO16_PIN)] = gpioState[2] ? true : false;
  data[String(GPIO17_PIN)] = gpioState[3] ? true : false;
  data[String(GPIO18_PIN)] = gpioState[4] ? true : false;
  data[String(GPIO19_PIN)] = gpioState[5] ? true : false;
  data[String(GPIO20_PIN)] = gpioState[6] ? true : false;
  data[String(GPIO21_PIN)] = gpioState[7] ? true : false;
  data[String(GPIO22_PIN)] = gpioState[8] ? true : false;
  char payload[256];
  data.printTo(payload, sizeof(payload));
  String strPayload = String(payload);
  Serial.print("Get gpio status: ");
  Serial.println(strPayload);
  return strPayload;
}


/*
faz controle de leds e servo motor
 GPIO0_PIN = pino na thingsboard 
 GPIOO = pino no arduinp
 boolean = valor boleano
 
*/ 

void set_gpio_status(int pin, boolean enabled) {
  switch(pin){
  case GPIO14_PIN:
    digitalWrite(GPIO14, enabled ? HIGH : LOW);
    gpioState[0] = enabled;
    break;
  case GPIO15_PIN:
    digitalWrite(GPIO15, enabled ? HIGH : LOW);
    gpioState[1] = enabled;
    break;

  case GPIO16_PIN:
    digitalWrite(GPIO16, enabled ? HIGH : LOW);
    gpioState[2] = enabled;
    break;
  case GPIO17_PIN:
    digitalWrite(GPIO17, enabled ? HIGH : LOW);
    gpioState[3] = enabled;
    break;

  case GPIO18_PIN:
    digitalWrite(GPIO18, enabled ? HIGH : LOW);
    gpioState[4] = enabled;
    break;

  case GPIO19_PIN:
    digitalWrite(GPIO19, enabled ? HIGH : LOW);
    gpioState[5] = enabled;
    break;

  case GPIO20_PIN:
    digitalWrite(GPIO20, enabled ? HIGH : LOW);
    gpioState[6] = enabled;
    break;

  case GPIO21_PIN:
    digitalWrite(GPIO21, enabled ? HIGH : LOW);
    gpioState[7] = enabled;
    break;

  case GPIO22_PIN:  // garagem
    Garagem.write(enabled ? 100 : 15 ); 
    gpioState[8] = enabled ? HIGH : LOW;;
    break;
  
   }

}

/* 
  Função: Default para reconectar
*/

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect(ID_DEVICE, TOKEN, NULL)) {
      Serial.println("connected");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 2 seconds before retrying
      delay(2000);
    }
    Serial.print("Connecting to ThingsBoard node ...");
    // Attempt to connect (clientId, username, password)
    if ( client.connect(ID_DEVICE, TOKEN, NULL) ) {
      Serial.println( "[DONE]" );
      // Subscribing to receive RPC requests
      client.subscribe("v1/devices/me/rpc/request/+");
      // Sending current GPIO status
      Serial.println("Sending current GPIO status ...");
      client.publish("v1/devices/me/attributes", get_gpio_status().c_str());
    } else {
      Serial.print( "[FAILED] [ rc = " );
      Serial.print( client.state() );
      Serial.println( " : retrying in 5 seconds]" );
      // Wait 5 seconds before retrying
      delay( 5000 );
    }
  }
}


void Pisca(){
  digitalWrite(13,HIGH);
  delay(500);
  digitalWrite(13,LOW);
  delay(500);
}

/*
   Mostra Endereço IP na porta Serial Monitor
*/

void printIPAddress()
{
  Serial.print("My IP address: ");
  for (byte thisByte = 0; thisByte < 4; thisByte++) {
    // print the value of each byte of the IP address:
    Serial.print(Ethernet.localIP()[thisByte], DEC);
    Serial.print(".");
  }
  Pisca();
  Pisca();
  Pisca();
  Serial.println();
}


/**
    sensore de LDR e movimento  
    pega os dados e envia pra 
    thingsboard
**/ 


void Pega_Envia_Sensores()
{
  Serial.println("Collecting SENSORS data.");


  float h = analogRead(A0);;
  float t = analogRead(A1);

  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from sensor!");
    return;
  }

  Serial.print("light: ");
  Serial.print(h);
  Serial.print(" \t");
  Serial.print("motion: ");
  Serial.print(t);
  Serial.print(" ");

  String movimento = String(t);
  String luminosidade = String(h);
  
  Serial.print( "Sending movimento and luminosidade : [" );
  Serial.print( movimento ); Serial.print( "," );
  Serial.print( luminosidade );
  Serial.print( "]   -> " );

  String payload = "{";
  payload += "\"light\":"; payload += luminosidade; payload += ",";
  payload += "\"motion\":"; payload += movimento;
  payload += "}";

  // Send payload
  char attributes[100];
  payload.toCharArray( attributes, 100 );
  client.publish( "v1/devices/me/telemetry", attributes );
  Serial.println( attributes );
}
  

