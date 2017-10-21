#include <SoftwareSerial.h>
#include <TinyGPS.h>
#include <Arduino.h>
#include <U8g2lib.h>
#include <SPI.h>
#include <Wire.h>

U8G2_ST7920_128X64_1_SW_SPI u8g2(U8G2_R2, 13, 11, 10, 8 );
SoftwareSerial serial1(6,7); //RX, TX
TinyGPS gps1;

void setup() {
  Serial.begin(9600); 
  serial1.begin(9600);
  u8g2.begin();
  u8g2.drawFrame(0,0,128,64);
    u8g2.setFont(u8g2_font_baby_tr);
    u8g2.drawStr(2,7,"Esperando sinal GPS");  

  Serial.println("Esperando sinal do GPS");
}

void loop() {

  bool recebido = false;
  static unsigned long delayPrint;

  while(serial1.available()){
    char cIn = serial1.read();
    recebido = (gps1.encode(cIn) || recebido);
  }

  if((recebido) && (millis() - delayPrint) > 1000){

    u8g2.firstPage();
    do{
    draw();
    } while (u8g2.nextPage());
  }

}

void draw(){
    u8g2.drawFrame(0,0,128,64);
    u8g2.setFont(u8g2_font_baby_tr);

    //Latitude e Longitude

    float latitude, longitude;

    unsigned long idadeInfo;
    gps1.f_get_position(&latitude, &longitude, &idadeInfo);

    if(latitude != TinyGPS::GPS_INVALID_F_ANGLE){

    u8g2.drawStr(2,7,"Latitude: ");
    u8g2.setCursor(40,7);
    u8g2.print(latitude, 6);

    Serial.println("Latitude: ");
    Serial.println(latitude, 6);
    }

    if(longitude != TinyGPS::GPS_INVALID_F_ANGLE){
      u8g2.drawStr(2,13,"Longitude: ");
      u8g2.setCursor(45, 13);
      u8g2.print(longitude, 6);

      Serial.println("Latitude: ");
      Serial.println(longitude, 6);
    }   

    //Data e Hora
    int ano;
    byte mes, dia, hora, minuto, segundo, centesimo;
    gps1.crack_datetime(&ano, &mes, &dia, &hora, &minuto, &segundo, &centesimo, &idadeInfo);
   
    u8g2.drawStr(2,19,"Data (GMT): ");
    u8g2.setCursor(48,19);
    u8g2.print(dia);
    u8g2.drawStr(56,19,"/");
    u8g2.setCursor(62,19);
    u8g2.print(mes);
    u8g2.drawStr(68,19,"/");
    u8g2.setCursor(74,19);
    u8g2.print(ano);

    u8g2.drawStr(2, 25, "Hora (GMT): ");
    u8g2.setCursor(48,25);
    u8g2.print(hora);
    u8g2.drawStr(52,25,":");
    u8g2.setCursor(55, 25);
    u8g2.print(minuto);
    u8g2.drawStr(64,25,":");
    u8g2.setCursor(67,25);
    u8g2.print(segundo);

    //Altitude
    float altitudeGPS;
    altitudeGPS = gps1.f_altitude();

    if((altitudeGPS != TinyGPS::GPS_INVALID_ALTITUDE) && (altitudeGPS != 1000000)){
      u8g2.drawStr(2,31,"Altitude (cm): ");
      u8g2.setCursor(48,31);
      u8g2.print(altitudeGPS);
    }

    //Sentido
    unsigned long sentido;
    sentido = gps1.course();

    u8g2.drawStr(2,36,"Sentido (grau): ");
    u8g2.setCursor(63,36);
    u8g2.print(float(sentido)/100,2);

    //Velocidade
    float velocidade;
    velocidade = gps1.f_speed_kmph();

    u8g2.drawStr(2,42,"Velocidade (km/h): ");
    u8g2.setCursor(74,42);
    u8g2.print(velocidade, 2);

    //Satélites e Precisão
    unsigned short satelites;
    unsigned long precisao;
    satelites = gps1.satellites();
    precisao = gps1.hdop();

    if(satelites != TinyGPS::GPS_INVALID_SATELLITES){
      u8g2.drawStr(2,48,"Satelites: ");
      u8g2.setCursor(43, 48);
      u8g2.print(satelites);
    }

    if(precisao != TinyGPS::GPS_INVALID_HDOP){
      u8g2.drawStr(2,54,"Precisao: ");
      u8g2.setCursor(40, 54);
      u8g2.print(precisao);
    }
    
  
}
 
