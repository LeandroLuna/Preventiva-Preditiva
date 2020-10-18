#include "EmonLib.h" //adiciona a biblioteca

#define CURRENT_CAL 18.40  // Calibração para o modulo
const int pinoSensor = A2; // Pino em que o sensor está conectado

EnergyMonitor emon1; //cria uma instancia

void setup()
{
    Serial.begin(9600);
    emon1.current(pinoSensor, CURRENT_CAL); //passa para função os valores do pino e calibragem)
}

void loop()
{
    emon1.calcVI(17, 100);           //FUNÇÃO DE CÁLCULO (17 SEMICICLOS / TEMPO LIMITE PARA FAZER A MEDIÇÃO)
    double currentDraw = emon1.Irms; //variavel que recebe o valor da corrente obtido

    Serial.print("Corrente medida: ");
    Serial.print(currentDraw);
    Serial.println("A");
}