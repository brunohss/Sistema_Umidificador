/*
 * Created by BRUNO HENRIQUE DOS SANTOS SILVA
 *
 * 
 */


// constants n�o varia


#include <DHT.h>
#define Type DHT11


const int POTExecucao_PIN = A3;    // Pino do Arduino ,o pino do potenciometro tempo de execução do rele
const int POTEsperaIni_PIN = A6;   // Pino do Arduino ,o pino do potenciometro tempo de espera do rele
const int POTEsperaMult_PIN = A7;  // Pino do Arduino ,o pino do potenciometro tempo de espera do rele multiplicado com o sensor
const int RELAY_PIN = 5;           // Pino do Arduino ,o pino de saida digital para o rele
const int SENSOR_PIN = 3;          // Pino do Arduino ,o pino de entrada digital para o sensor
float Umidade, Execucao, EsperaIni, EsperaMult, Temperatura;
float del = 0;
int Tempo = 0, hum = 0;
DHT HT(SENSOR_PIN, Type);



void setup() {
  // initialize digital pin as an output.
  Serial.begin(9600);
  pinMode(RELAY_PIN, OUTPUT);
  HT.begin();
}


// the loop function runs over and over again forever
void loop() {

  do {
    Umidade = HT.readHumidity();
    Temperatura = HT.readTemperature();
    if (isnan(Temperatura) || isnan(Umidade)) Umidade = Temperatura = 200;
    Execucao = analogRead(POTExecucao_PIN);
    Execucao = map(Execucao, 0, 1023, 0, 200);
    EsperaIni = analogRead(POTEsperaIni_PIN);
    EsperaIni = map(EsperaIni, 0, 1023, 0, 600);
    EsperaMult = analogRead(POTEsperaMult_PIN);
    EsperaMult = map(EsperaMult, 0, 1023, 0, 100);

    delay(1000);
    Tempo + 1;


    if (Umidade < EsperaMult && Tempo > 10) {
      hum + 1;
      if (hum > 5) {
        
        digitalWrite(RELAY_PIN, HIGH);
        delay(100 * Execucao);
        digitalWrite(RELAY_PIN, LOW);
        delay(100 * Execucao);
        Tempo = hum = 0;
      }
    } else {
      hum = 0;
    }

  } while (Tempo = EsperaIni);
  
  digitalWrite(RELAY_PIN, HIGH);
  delay(100 * Execucao);
  digitalWrite(RELAY_PIN, LOW);
  delay(100 * Execucao);
  Tempo = hum = 0;
}
