#define pino_D0 9
#define analog 5

int rpm; // definimos as variaveis para o software
volatile byte pulsos;
unsigned long timeold;

unsigned int pulsos_por_volta = 20; // varia o pulso ( mas vai de acordo com o disco do encorder)

void contador()
{
    //adiciona um contador
    pulsos++;
}

void setup()
{
    Serial.begin(9600);

    //pinMode(vcc, OUTPUT);
    //pinMode(gnd, OUTPUT);
    pinMode(pino_D0, INPUT);
    pinMode(analog, INPUT);

    //digitalWrite(vcc, HIGH);
    //digitalWrite(gnd, LOW);
 
    //Interrupcao  referente ao pino 3
    //Aciona o contador a cada pulso
    attachInterrupt(1, contador, FALLING);
    pulsos = 0;
    rpm = 0;
    timeold = 0;

} //end setup

// --- Loop Infinito ---
void loop()
{
    //Atualiza contador a cada segundo
    if (millis() - timeold >= 1000)
    {
        //paralisa a interrupção para fazer o calculo
        detachInterrupt(1);
        rpm = (60 * 1000 / pulsos_por_volta) / (millis() - timeold) * pulsos;
        timeold = millis();
        pulsos = 0;

        //Mostra o valor de RPM no serial monitor
        Serial.print("RPM = ");
        Serial.println(rpm, DEC);
        //Habilita interrupcao novamente
        attachInterrupt(1, contador, FALLING);
    }
}
