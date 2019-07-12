#include <LiquidCrystal.h>
#include <EEPROM.h>
#include "RTClib.h"

RTC_DS1307 rtc;


//////////////>>>>    LM35
const int LM35 = A0; // Define o pino que lera a saída do LM35
float temperatura; // Variável que armazenará a temperatura medida
char daysOfweek[7][12] ={ "Domingo",  "Segunda", "Terça", "Quarta", "Quinta", "Sexta", "Sabado" };

LiquidCrystal lcd(10, 9, 8, 7, 6, 5);

int cooler = 13;
int Buzzer = 12;
int Led = 11;
int beep_intermitente = 0;
int botao = A2;
int Referencia = 25;
int contador = 0;
int tempMAX = 0;
int tempMIN = 1000;


int botaoaumenta = 0;
int botaodiminui = 0;
int minuto = 0; 
int Mediatemp = 0;

void setup() {

  Serial.print("DIGITE * PARA ABRIR O MENU");
  Serial.begin(9600); // inicializa a comunicação serial
  if(! rtc.begin()){
    Serial.println("RTC não encontrado");
    while(1);
    }
  if(! rtc.isrunning()){
    Serial.println("RTC não está rodando");
    
    rtc.adjust(DateTime(2019,7,5,21,2,0));
    
  }
  pinMode(cooler, OUTPUT);
  pinMode(Buzzer, OUTPUT);
  pinMode(Led,OUTPUT);
  pinMode(temperatura,INPUT); 
  lcd.begin(16, 2);
  //rtc.adjust(DateTime(2019,7,5,21,7,0));
  

}
 

void loop() {

  DateTime now = rtc.now();
 
////>>>LM35<<<</////
  temperatura = (float(analogRead(LM35))*5/(1023))/0.01;
  delay(150);
  Serial.print("Temperatura: ");
  Serial.print(temperatura);
  Serial.print("            ");
  Serial.print("Referencia: ");
  Serial.print(Referencia);
  Serial.print("\n");
  Serial.print(now.day(), DEC);
  Serial.print("/");
  Serial.print(now.month(), DEC);
  Serial.print("/");
  Serial.print(now.year(), DEC);
  Serial.print("Hora: ");
  Serial.print(now.hour(), DEC);
  Serial.print(":");
  Serial.print(now.minute(), DEC);
  Serial.print(":");
  Serial.print(now.second(), DEC);
  Serial.print("\n");
  //delay(2000);

   if(analogRead(botao)>= 900){
        Referencia--;
        //delay();
      }
    if(analogRead(botao)>= 500 && analogRead(botao)<= 900){
        Referencia++;
        //delay();
      }
  
    if (temperatura >=(Referencia + 1)){ ///// 
      digitalWrite(cooler, HIGH);
    } 
    else {
      digitalWrite(cooler, LOW);
    }
   
    if(temperatura >= (Referencia + 5) && beep_intermitente >= 4){
      digitalWrite(Buzzer, HIGH);
      digitalWrite(Led, HIGH);
      beep_intermitente=0;             
    }else{
      digitalWrite(Led, LOW);
      digitalWrite(Buzzer, LOW);
    if(temperatura >= (Referencia + 5)) {  // novo codigo
          beep_intermitente++;          
    }
    }
    if(temperatura >= EEPROM.read(35)){
      EEPROM.write(35, temperatura);
      tempMAX = temperatura - EEPROM.read(35);
      tempMAX = tempMAX*100;
      EEPROM.write(36, tempMAX);     
    }  
    if(temperatura <= EEPROM.read(11)){
      EEPROM.write(11,temperatura);
      tempMIN = temperatura - EEPROM.read(11);
      tempMIN = tempMIN*100;
      EEPROM.write(12, tempMIN);
    }     
    if((now.minute, DEC) == (minuto+29)){
      Mediatemp = temperatura + Mediatemp;
    }
    if((now.minute, DEC) == (minuto + 59)){
      Mediatemp = (temperatura + Mediatemp)/2;
      EEPROM.write(55, Mediatemp);
    }

    ////////////////////////////////// MENU SERIAL //////////////////////

    




    /////////////////////////////////////////////////////////////////////
    
    
    
    lcd.setCursor(0,0);
    lcd.print("Ref:");
    lcd.print(Referencia);
    lcd.setCursor(7,0);
    lcd.print("Tp:");
    lcd.print(temperatura);
    lcd.print(char(223)); //0b11011111
    lcd.setCursor(0,1);
    lcd.print(now.day(), DEC);
    lcd.print("/");
    lcd.print(now.month(), DEC);
    lcd.print("/");
    lcd.print(now.year(), DEC);
    lcd.print("  ");
    lcd.print(now.hour(), DEC);
    lcd.print(":");
    lcd.print(now.minute(), DEC);
    
    


  
}
