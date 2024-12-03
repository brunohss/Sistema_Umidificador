/*
  Created by BRUNO HENRIQUE DOS SANTOS SILVA


*/

// Bibliotecas //

#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>
#define Type DHT22

//Variaveis//

const int POTExecucao_PIN = A6,    // Pino do Arduino ,o pino do potenciometro tempo de execu��o do rele
          POTEsperaIni_PIN = A3,   // Pino do Arduino ,o pino do potenciometro tempo de espera do rele
          POTEsperaMult_PIN = A7,  // Pino do Arduino ,o pino do potenciometro tempo de espera do rele multiplicado com o sensor
          RELAY_PIN = 5,           // Pino do Arduino ,o pino de saida digital para o rele
          SENSOR_PIN = 3;          // Pino do Arduino ,o pino de entrada digital para o sensor

bool status=false;
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
  //        "LXXX SXXX%DXMXXS"
  lcd.print("L   S   % D m   s");
  lcd.setCursor(0, 1);
  //        "XXXCXXX%RstXmXXs"
  lcd.print("   C   %Rst m  s");
  lcd.setCursor(2, 1);
  lcd.print((char)223);

  // Config porta Rele//

  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, LOW);

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
      status = false;

    } else {
      lcd.setCursor(0, 1);
  //        "XXXCXXX%RstXmXXs"
  lcd.print("   C   %Rst");
  lcd.setCursor(2, 1);
  lcd.print((char)223);
      
      imprimir(4, 1, Umidade);
      lcd.setCursor(0, 1);                               // LCD POSI��O
      lcd.print(Temperatura);                             // SENSOR TEMPERATURA
      status = true;
    }


    Execucao = analogRead(POTExecucao_PIN);
    //Serial.print("POTExecucao : ");
    //Serial.println(Execucao);
    Execucao = map(Execucao, 50, 970, 1, 20);
    imprimir(1, 0, Execucao);
    Execucao = 1000 * Execucao;

    EsperaIni = analogRead(POTEsperaIni_PIN);
    //Serial.print("EsperaIni : ");
    //Serial.println(EsperaIni);
    EsperaIni = map(EsperaIni, 48, 984, -10, 600);

    if ( EsperaIni <= 0) {
      lcd.setCursor(12, 0);
      lcd.print("OFF ");
      lcd.setCursor(11, 1);
      lcd.print("    ");
      Tempo = 0;
      EsperaIni = 4;
    } else {

      imprimirMi(11, 0, EsperaIni);
      Tempo ++;
      int Restante = EsperaIni - Tempo;
      imprimirMi(11, 1, Restante);
    }


    EsperaMult = analogRead(POTEsperaMult_PIN);
    //Serial.print("EsperaMult : ");
    //Serial.println(EsperaMult);

    EsperaMult = map(EsperaMult, 50, 980, -3, 100);

    if ( EsperaMult <= 0) {
      lcd.setCursor(5, 0);
      lcd.print("OFF");
    } else {
      imprimir(5, 0, EsperaMult);
      //Serial.print("umi : ");
      //Serial.println(Umidade);
      //Serial.print("EsperaMult : ");
      //Serial.println(EsperaMult);
      if (status && Umidade < EsperaMult) {
        hum ++;
        // Serial.println(hum);
        if (hum > 10) {
         digitalWrite(RELAY_PIN, HIGH);
          ligar(Execucao);
         digitalWrite(RELAY_PIN, LOW);

          lcd.setCursor(11, 1);
          lcd.print("    ");
          hum = 0;
        }
      } else {
        hum = 0;
      }
    }

    delay(1000);

  } while (Tempo < EsperaIni || EsperaIni == 0);

  
  digitalWrite(RELAY_PIN, HIGH);
  ligar(Execucao);
 digitalWrite(RELAY_PIN, LOW);


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

//               (linha, coluna, valor(em segundo))
void imprimirMi (int x,  int y,      int z) {
  int q = z;

  if (q > 60) {
    int i = q / 60;
    int s = q % 60;

    //Serial.print("minutos : ");
    // iMss
    lcd.setCursor(x, y);
    if (i >= 10) {
      lcd.print(i);
      lcd.setCursor(x+2, y);
      lcd.print("m");
      int tam = ((String)s).length();
      if (tam == 1){lcd.setCursor(x + 2, y);
      lcd.print("s");
      lcd.setCursor(x + 4, y);
      lcd.print("s");
      }
      if (tam == 2){lcd.setCursor(x + 2, y);
      lcd.print("s");}
      
    } else {
      lcd.print(i);
      lcd.setCursor(x + 1, y);
      lcd.print("m");
      int tam = ((String)s).length();
      if (tam == 1)lcd.setCursor(x + 3, y);
      if (tam == 2)lcd.setCursor(x + 2, y);
      lcd.print(s);
      lcd.setCursor(x + 4, y);
      lcd.print("s");
    }

    //Serial.println(i);
   // Serial.print("segundos : ");
    //Serial.println(s);
   // Serial.println(i , s);


  } else {
    lcd.setCursor(x, y);
    lcd.print("    s");
    imprimir(x, y, z);
    lcd.setCursor(x + 4, y);
    lcd.print("s");
  };
}

void ligar(int a){

     lcd.setCursor(8, 1);
  //        "RstXmXXs"
  lcd.print("Exc    s");

  
  int t= a;
  t=t/1000;
  for(t;t>=0;t--){

    imprimir(11,1,t);
    delay(1000);
    }
     lcd.setCursor(8, 1);
  //        "RstXmXXs"
  lcd.print("Rst");
  
}





void imprimir4 (int a, int b, int c) {
  lcd.setCursor(a, b);
  lcd.print("   ");
  int tamanho = ((String)c).length();
  if (tamanho == 1)lcd.setCursor(a + 3, b);
  if (tamanho == 2)lcd.setCursor(a + 2, b);
  if (tamanho == 3)lcd.setCursor(a + 1, b);
  if (tamanho == 4)lcd.setCursor(a, b);

  lcd.print(c);

}
