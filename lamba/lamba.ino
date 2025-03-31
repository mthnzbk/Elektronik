#include <SPI.h>
#include <RF24.h>
#include <nRF24L01.h>

#include <avr/sleep.h>
#include <avr/power.h>

//nrf pins vcc 3,3v
#define CE 9 // bu
#define CSN 10 // ve bu değişebilir diğerleri kullanılmasa da sabit pinler
// #define SCK 13
// #define MOSI 11
// #define MISO 12
#define IRQ 2 // veya 3 kesme pini

#define RELAY 5 // 2 mi 3 mü

RF24 anten(CE, CSN);
const byte address[6] = "00001";

volatile bool is_open = false;

void setup() {
  Serial.begin(9600);
  if (!anten.begin()) {
    Serial.println("anten not connect");
    while(1) {}
  } else{
    Serial.println("anten connected");
  }


    //pinMode(IRQ, INPUT_PULLUP);
  pinMode(RELAY, OUTPUT);
  anten.openReadingPipe(0, address);
  anten.setChannel(100);           // Kanal ayarı (0-125)
  anten.setPALevel(RF24_PA_LOW);   // Güç seviyesi (RF24_PA_MIN, RF24_PA_LOW, RF24_PA_HIGH, RF24_PA_MAX)
  anten.setDataRate(RF24_250KBPS);
  anten.startListening();

}

void loop() {
  //Serial.println("loop");
  if (anten.available()) {
    int touch;
    anten.read(&touch, sizeof(touch));
    Serial.println("Veri alındı!");

    if (touch == 1) {
      is_open = !is_open;
      digitalWrite(RELAY, is_open);
    } 
  }
}



