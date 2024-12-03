/*
  Created by BRUNO HENRIQUE DOS SANTOS SILVA


*/

// Bibliotecas //

  #include <Wire.h>
  #include <LiquidCrystal_I2C.h>
  #include <DHT.h>
  #define Type DHT11

//Variaveis//

  const int POTExecucao_PIN = A3,    // Pino do Arduino ,o pino do potenciometro tempo de execu��o do rele
            POTEsperaIni_PIN = A6,   // Pino do Arduino ,o pino do potenciometro tempo de espera do rele
            POTEsperaMult_PIN = A7,  // Pino do Arduino ,o pino do potenciometro tempo de espera do rele multiplicado com o sensor
            RELAY_PIN = 5,           // Pino do Arduino ,o pino de saida digital para o rele
            SENSOR_PIN = 3;          // Pino do Arduino ,o pino de entrada digital para o sensor
        
        int Umidade, 
            Execucao, 
            EsperaIni, 
            EsperaMult, 
            Temperatura,
            Tempo = 0, 
            hum = 0;
     
DHT HT(SENSOR_PIN, Type);
LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {

 // Config Porta serial//
  
  Serial.begin(9600);

 // Config LCD 16 x 2//
 
  lcd.init();                      // initialize the lcd
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("E    S    % T   ");
  lcd.setCursor(0, 1);
  lcd.print("   C   %Rest   s");
  lcd.setCursor(2, 1);
  lcd.print((char)223);

 // Config porta Rele//
   
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, HIGH);

 // Config Sensor//
   
  HT.begin();
}

void loop() {
  Tempo = 0;
  lcd.setCursor(12, 1);
  lcd.print("   ");
  do {

    Umidade = HT.readHumidity();
    Temperatura = HT.readTemperature();

    if (isnan(Temperatura) || isnan(Umidade) || Temperatura == 0 || Umidade == 0) {
      lcd.setCursor(0, 1);                                     // LCD
      lcd.print("No Sens ");                                  // SENSOR UMIDADE

      }else {
      imprimir(4, 1, Umidade);
      lcd.setCursor(0, 1);                               // LCD POSI��O
      lcd.print(Temperatura);                            // SENSOR TEMPERATURA
    }


    Execucao = analogRead(POTExecucao_PIN);
    Serial.print("POTExecucao : ");
    Serial.println(Execucao);
    Execucao = map(Execucao, 0, 1023, 1, 20);
    imprimir(1, 0, Execucao);
    Execucao =1000*Execucao;

    EsperaIni = analogRead(POTEsperaIni_PIN);
    Serial.print("EsperaIni : ");
    Serial.println(EsperaIni);
    EsperaIni = map(EsperaIni, 0, 1023, -10, 600);
    Serial.print("EsperaIni : ");
    Serial.println(EsperaIni);
    if ( EsperaIni <= 0) {
      lcd.setCursor(13, 0);
      lcd.print("OFF");
      lcd.setCursor(12, 1);
      lcd.print("   ");
      Tempo = 0;
      EsperaIni=4;
    } else {
      imprimir(13, 0, EsperaIni);
      Tempo ++;
      int Restante = EsperaIni - Tempo;
      imprimir(12, 1, Restante);
    }


    EsperaMult = analogRead(POTEsperaMult_PIN);
    Serial.print("EsperaMult : ");
    Serial.println(EsperaMult);
    
    EsperaMult = map(EsperaMult, 0, 1023, -3, 100);
    Serial.print("EsperaMult : ");
    Serial.println(EsperaMult);
    if ( EsperaMult <= 0) {
      lcd.setCursor(7, 0);
      lcd.print("OFF");
    } else {
      imprimir(7, 0, EsperaMult);
      //Serial.print("umi : ");
      //Serial.println(Umidade);
      //Serial.print("EsperaMult : ");
      //Serial.println(EsperaMult);
      if (Umidade < EsperaMult) {
        hum ++;
        //Serial.println(hum);
        if (hum > 10) {

          digitalWrite(RELAY_PIN, LOW);
          delay(Execucao);
          digitalWrite(RELAY_PIN, HIGH);

          lcd.setCursor(12, 1);
          lcd.print("   ");
          hum = 0;
        }
      } else {
        hum = 0;
      }
    }

    delay(1000);

  } while (Tempo < EsperaIni ||EsperaIni ==0);


  digitalWrite(RELAY_PIN, LOW);
  delay( Execucao);
  digitalWrite(RELAY_PIN, HIGH);


}



void imprimir (int a, int b, int c) {
  lcd.setCursor(a, b);
  lcd.print("   ");
  int tamanho = ((String)c).length();
  if (tamanho == 1)lcd.setCursor(a + 2, b);
  if (tamanho == 2)lcd.setCursor(a + 1, b);
  if (tamanho == 3)lcd.setCursor(a, b);

  lcd.print(c);

}
