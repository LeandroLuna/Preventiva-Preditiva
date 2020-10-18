#include <dht11.h> //adicionamos a biblioteca para o uso do modulo
#define DHT11PIN 4 //definimos a pinagem para o modulo

dht11 DHT11; //aqui estamos atribuindo a função para a biblioteca

void setup()
{
    Serial.begin(9600);
}

void loop()
{
    Serial.println();

    int chk = DHT11.read(DHT11PIN); // pedimos para checar o sinal do pino

    Serial.print("Umidade (%): ");
    Serial.println((float)DHT11.humidity, 2); // printamos o valor da umidade com base no sinal do pino

    Serial.print("Temperatura (C): ");
    Serial.println((float)DHT11.temperature, 2); // printamos o valor da temperatura com base no sinal do pino

    delay(2000);
}
