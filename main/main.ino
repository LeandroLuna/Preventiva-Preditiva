#include <SoftwareSerial.h> // Conexão serial
#include "EmonLib.h"        //adiciona a biblioteca
#include <dht11.h>          //adicionamos a biblioteca para o uso do modulo

#define CURRENT_CAL 18.40 // Calibração para o modulo
#define DHT11PIN 4        //definimos a pinagem para o modulo
#define RX 2              // Pino onde esta o RX
#define TX 3              // Pino onde esta o TX
#define pino_D0 9
#define analog 5

const int pinoSensor = A2; // Pino em que o sensor está conectado
String humidade, temperatura;
String AP = "WannaCry";             // AP NOME
String PASS = "leandroluna2104";    // AP SENHA
String API = "EBAV1C6XVKMRRYWK";    // API KEY
String HOST = "api.thingspeak.com"; // Host da conexão para montagem da URL
String PORT = "80";                 // Porta de conexão

int countTrueCommand; // Para sabermos quando a informação enviar
int countTimeCommand;
boolean found = false;
int valSensor = 1;
int rpm; // definimos as variaveis para o software
volatile byte pulsos;
unsigned long timeold;
unsigned int pulsos_por_volta = 20; // varia o pulso ( mas vai de acordo com o disco do encorder)
SoftwareSerial esp8266(RX, TX);
EnergyMonitor emon1; //cria uma instancia
dht11 DHT11;

void contador()
{
    pulsos++; //adiciona um contador
}

void setup()
{
  Serial.begin(9600);
  emon1.current(pinoSensor, CURRENT_CAL); //passa para função os valores do pino e calibragem)
  esp8266.begin(9600);
  sendCommand("AT", 5, "OK");
  sendCommand("AT+CWMODE=1", 5, "OK");
  sendCommand("AT+CWJAP=\"" + AP + "\",\"" + PASS + "\"", 20, "OK");
  pinMode(pino_D0, INPUT);
  pinMode(analog, INPUT);
  attachInterrupt(1, contador, FALLING);
  pulsos = 0;
  rpm = 0;
  timeold = 0;
}

void loop()
{
  emon1.calcVI(17, 100);//FUNÇÃO DE CÁLCULO (17 SEMICICLOS / TEMPO LIMITE PARA FAZER A MEDIÇÃO)
  double corrente = emon1.Irms;   //variavel que recebe o valor da corrente obtido
  int chk = DHT11.read(DHT11PIN); // pedimos para checar o sinal do pino
  humidade = DHT11.humidity;
  humidade = humidade.toFloat();
  temperatura = DHT11.temperature;
  temperatura = temperatura.toFloat();
  if (millis() - timeold >= 1000){
        detachInterrupt(1); //paralisa a interrupção para fazer o calculo
        rpm = (60 * 1000 / pulsos_por_volta) / (millis() - timeold) * pulsos;
        timeold = millis();
        pulsos = 0;
        attachInterrupt(1, contador, FALLING);
  }

  String getData1 = "GET /update?api_key=" + API + "&field1=" + String(temperatura) + "&field2=" + String(humidade) + "&field3=" + String(corrente) + "&field4=" + String(rpm);
  sendCommand("AT+CIPMUX=1", 5, "OK");
  sendCommand("AT+CIPSTART=0,\"TCP\",\"" + HOST + "\"," + PORT, 15, "OK");
  sendCommand("AT+CIPSEND=0," + String(getData1.length() + 4), 4, ">");
  esp8266.println(getData1);
  delay(1500);
  countTrueCommand++;
  sendCommand("AT+CIPCLOSE=0", 5, "OK");
  //Serial.println(temperatura); // printamos o valor da temperatura com base no sinal do pino
  delay(15000);
}
void sendCommand(String command, int maxTime, char readReplay[])
{
  Serial.print(countTrueCommand);
  Serial.print(". at command => ");
  Serial.print(command);
  Serial.print(" ");
  while (countTimeCommand < (maxTime * 1))
  {
    esp8266.println(command);     //at+cipsend
    if (esp8266.find(readReplay)) //ok
    {
      found = true;
      break;
    }
    countTimeCommand++;
  }
  if (found == true)
  {
    Serial.println("OK");
    countTrueCommand++;
    countTimeCommand = 0;
  }
  if (found == false)
  {
    Serial.println("Fail");
    countTrueCommand = 0;
    countTimeCommand = 0;
  }
  found = false;
}
