#include <SoftwareSerial.h>
#include "EmonLib.h"
#include <dht11.h>

#define CURRENT_CAL 18.40 // Calibração para o modulo
#define DHT11PIN 4
#define RX 2
#define TX 3
#define pino_D0 9
#define analog 5
const int pinoSensor = A2;

String humidade, temperatura;
String AP = "WannaCry";             // AP NOME: SSID do wifi
String PASS = "leandroluna2104";    // AP SENHA: Senha do wifi
String API = "EBAV1C6XVKMRRYWK";    // API KEY: Chave para subir/baixar informação p/ o site
String HOST = "api.thingspeak.com"; // Host da conexão para montagem da URL
String PORT = "80";                 // Porta de conexão

int countTrueCommand;  // Para sabermos quando a informação enviar para o site
int countTimeCommand;  // Tempo em que a informação foi enviada para o site
boolean found = false; // Servira de apoio para sabermos se a informação foi enviada
int valSensor = 1;
int rpm;                            // variavel que recebera o valor do RPM
volatile byte pulsos;               // vai ser modificada por uma thread paralela (função interrupt)
unsigned long timeold;              // para valores de alto tamanho e não negativos. Usaremos para controle do tempo
unsigned int pulsos_por_volta = 20; // varia o pulso ( mas vai de acordo com os furos do disco do encoder)
SoftwareSerial esp8266(RX, TX);
EnergyMonitor emon1; //cria uma instancia
dht11 DHT11;         // variavel que 'recria' as informações da biblioteca

void contador()
{
  pulsos++; //adiciona um contador para que incremente a variavel pulsos
}

void setup()
{
  Serial.begin(9600);
  emon1.current(pinoSensor, CURRENT_CAL); //passa para função os valores do pino e calibragem
  esp8266.begin(9600);
  sendCommand("AT", 5, "OK");                                        // Precaverá que o modulo wifi está a funcionar
  sendCommand("AT+CWMODE=1", 5, "OK");                               // Configura esp8266 como cliente
  sendCommand("AT+CWJAP=\"" + AP + "\",\"" + PASS + "\"", 20, "OK"); // Conecta a rede wifi
  pinMode(pino_D0, INPUT);
  pinMode(analog, INPUT);
  attachInterrupt(1, contador, FALLING); // interrupção externa 1, chamaremos a função contador a cada borda de descida
  pulsos = 0;
  rpm = 0;
  timeold = 0;
}

void loop()
{
  emon1.calcVI(17, 100);          //FUNÇÃO DE CÁLCULO (17 SEMICICLOS / TEMPO LIMITE PARA FAZER A MEDIÇÃO)
  double corrente = emon1.Irms;   //variavel que recebe o valor da corrente obtido
  int chk = DHT11.read(DHT11PIN); // pedimos para checar o sinal do pino
  humidade = DHT11.humidity;
  humidade = humidade.toFloat();
  temperatura = DHT11.temperature;
  temperatura = temperatura.toFloat();

  if (millis() - timeold >= 1000) // Base de tempo de 1 segundo
  {
    detachInterrupt(1);                                                   //paralisa a interrupção para fazer o calculo
    rpm = (60 * 1000 / pulsos_por_volta) / (millis() - timeold) * pulsos; // Função principal para calculo do RPM
    timeold = millis();                                                   // Preservar a base de tempo de 1 segundo
    pulsos = 0;                                                           // Reinicia os pulsos para um novo calculo
    attachInterrupt(1, contador, FALLING);
  }

  String getData1 = "GET /update?api_key=" + API + "&field1=" + String(temperatura) + "&field2=" + String(humidade) + "&field3=" + String(corrente) + "&field4=" + String(rpm); // Escreve os dados mensurados nos topicos referentes
  sendCommand("AT+CIPMUX=1", 5, "OK");                                                                                                                                          // Habilita multipla conexões
  sendCommand("AT+CIPSTART=0,\"TCP\",\"" + HOST + "\"," + PORT, 15, "OK");                                                                                                      // Numero do ID da transmissão, host e porta
  sendCommand("AT+CIPSEND=0," + String(getData1.length() + 4), 4, ">");                                                                                                         // Define o tamanho da extensão dos dados a serem enviados (no caso, multiplas conexões)
  //**Saber mais: comandos AT no esp8266 https://room-15.github.io/blog/2015/03/26/esp8266-at-command-reference/
  esp8266.println(getData1);
  delay(1500);
  countTrueCommand++;                    // Passa informações para controle
  sendCommand("AT+CIPCLOSE=0", 5, "OK"); // Fecha as conexões novamente
  Serial.println("Temperatura: " + temperatura + "\nHumidade: " + humidade + "\nCorrente: " + corrente + "\nRPM: " + rpm);
  delay(15000);                          // Tempo para ser reenviado novos dados para o servidor. A conta de versão estudante possibilita o envio de novas informações para o site a cada 15 segundos, podendo-se chegar a até mesmo a 1 segundo com a versão licensiada premium comercial. **Saber mais: https://thingspeak.com/prices/thingspeak_standard
}
void sendCommand(String command, int maxTime, char readReplay[]) // Toda essa função garantirá que a informação foi enviada corretamente ao servidor
{
  Serial.print(countTrueCommand);
  Serial.print(". at command => ");
  Serial.print(command);
  Serial.print(" ");
  while (countTimeCommand < (maxTime * 1)) // Garante que o tempo de conexão está dentro dos parametros normais
  {
    esp8266.println(command);     //at+cipsend
    if (esp8266.find(readReplay)) //ok
    {
      found = true;
      break;
    }
    countTimeCommand++;
  }
  if (found == true) // envio feito com sucesso
  {
    Serial.println("OK");
    countTrueCommand++;
    countTimeCommand = 0;
  }
  if (found == false) // erro no envio
  {
    Serial.println("Fail");
    countTrueCommand = 0;
    countTimeCommand = 0;
  }
  found = false; // extendeu o limite do tempo de conexão = erro
}
