/*
   Created by BRUNO HENRIQUE DOS SANTOS SILVA


*/


// constants n�o varia
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
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
LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {

  lcd.init();                      // initialize the lcd
  lcd.init();
  // Print a message to the LCD.
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("E    S    % T   ");
  lcd.setCursor(0, 1);
  lcd.print("   C   %Rest   s");
  lcd.setCursor(2, 1);
  lcd.print((char)223);

  pinMode(RELAY_PIN, OUTPUT);
  HT.begin();
}

void loop() {

  do {
    Umidade = HT.readHumidity();
    Temperatura = HT.readTemperature();

    if (isnan(Temperatura) || isnan(Umidade)) {
      lcd.setCursor(4, 1);
      lcd.print("XXX");     // SENSOR UMIDADE
      lcd.setCursor(0, 1);
      lcd.print("XX");     // SENSOR TEMPERATURA
      Umidade = Temperatura = 200;
    } else {
      lcd.setCursor(4, 1);
      lcd.print(Umidade);     // SENSOR UMIDADE
      lcd.setCursor(0, 1);
      lcd.print(Temperatura);     // SENSOR TEMPERATURA

    }
    Execucao = analogRead(POTExecucao_PIN);
    Execucao = map(Execucao, 0, 1023, 0, 200);
    lcd.setCursor(1,0);                          // LCD POSIÇÃO
     lcd.print(Execucao);                        // POT EXECUÇÃO
    EsperaIni = analogRead(POTEsperaIni_PIN);
    EsperaIni = map(EsperaIni, 0, 1023, 0, 600);
     lcd.setCursor(13,0);                          // LCD POSIÇÃO
     lcd.print(EsperaIni);                         // POT TEMPO
    EsperaMult = analogRead(POTEsperaMult_PIN);
    EsperaMult = map(EsperaMult, 0, 1023, 0, 100);
     lcd.setCursor(7,0);                          // LCD POSIÇÃO
  lcd.print(EsperaMult);                          // POT SENSOR

    delay(1000);
    Tempo + 1;
    
    lcd.setCursor(12,1);
    int Restante = EsperaIni-Tempo;
    lcd.print(Restante);     // TEMPO RESTANTE PARA ACIONAR

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

  } while (Tempo > EsperaIni);

  digitalWrite(RELAY_PIN, HIGH);
  delay(100 * Execucao);
  digitalWrite(RELAY_PIN, LOW);
  delay(100 * Execucao);
  Tempo = hum = 0;
}
