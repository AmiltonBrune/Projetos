#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include "DHT.h"
#define dht_dpin 0         // PINO ONDE ESTÁ CONECTADO O DHT11( SENSOR DE TEMPERATURA E UMIDADE)
#define DHTTYPE DHT11     // ESTAMOS USANDO O DHT11(EXISTEM OUTROS TIPOS DE DHT)
DHT dht(dht_dpin, DHTTYPE); // INSTANCIA O SENSOR DHT11

//__ Informações do WIFI
const char* ssid =     "DESKTOP-RN52TCV";
const char* password = "05562C9326";

//__variáveis auxiliares

float umidadeAR = 0;

float temperatura = 0;

//__strings para envio dos dados no JSON

String Str2 = "temperatura";

String Str3 = "umidadeAR";


//**************************************

//__ Configurações para o IBM Watson

//**************************************

//__ Informações do dispositivo

#define DEVICE_TYPE  "Device1"

#define DEVICE_ID    "D1"


//__ Informações da conexão com o servidor

#define ORG     "l1n9wo"

#define TOKEN   "@)Tp+0i8kgN3ACKrkk"


//__ Variáveis de conexão com o servidor (Não customizaveis)

char server[]   = ORG ".messaging.internetofthings.ibmcloud.com";

char topic[]    = "iot-2/type/NodeMCU/id/NodeDht11/evt/1-anl/fmt/json";

char authMeth[] = "a-l1n9wo-awe8s5i2so";

char token[]    = TOKEN;

char clientId[] = "a:" ORG ":" DEVICE_ID;


//__ Inicia WiFi

WiFiClient wifiClient;

PubSubClient client(server, 1883, NULL, wifiClient);


//__ Função de setup do arduino

void setup() {

 //__ Inicializa a serial

 Serial.begin(9600);

 Serial.println();

 Serial.print("Conectando-se na rede ");

 Serial.print(ssid);


 //__ Conecta-se na rede WIFI

 WiFi.begin(ssid, password);

 while (WiFi.status() != WL_CONNECTED) {

   delay(500);

   Serial.print(".");

 }

 Serial.println("");


 Serial.print("Conectado, endereço de IP: ");

 Serial.println(WiFi.localIP());


 //inicia DHT

 dht.begin();

}



//__ Envia os dados para a cloud

// JSON:

//  {"d":{"temperatura":2,"umidadeAr":2}}

void enviaDado(float dado2, float dado3){

String payload = "{\"d\":{\"" + Str2 + "\":";

 payload += dado2;

 payload += ", \"" + Str3 + "\":";

 payload += dado3;

 payload += "}}";

 Serial.print("Sending payload: ");

 Serial.println(payload);

 //__ Envia o dado

 if (client.publish(topic, (char*) payload.c_str())) {

   Serial.println("Publish ok");

 } else {

   Serial.println("Publish failed");

 }

}

//__ Função principal

void loop() {

 
 //__ Verifica se está conectada a cloud para envio dos dados

 if (!!!client.connected()) {

   //__ Caso não esteja conectada, tenta a conexão

   Serial.print("Reconectando-se em ");

   Serial.println(server);

   while (!!!client.connect(clientId, authMeth, token)) {

     Serial.print(".");

     delay(500);

   }

   Serial.println();

 }



 //__ Le Sensores

 umidadeAR = LeUmidadeAr();

 temperatura = LeTemperatura();



 //__ Envia um dado para a cloud

 enviaDado(temperatura,umidadeAR);

 

 //__ Faz o envio a cada 10 segundos.

 delay(10000);

}



//__Função que faz a leitura do nível de umidade

float LeTemperatura(){

  float temperatura = dht.readTemperature();



 if (isnan(temperatura))

 {

   Serial.println("Falha na leitura do dht11...");

 }

 else

 {

  return temperatura;

 }

}


float LeUmidadeAr(){

  float umidade = dht.readHumidity();



 // Se a umidade não for valor válido, acusará falha de leitura.

 if (isnan(umidade))

 {

   Serial.println("Falha na leitura do dht11...");

 }

 else

 {

   return umidade;

 }

}
