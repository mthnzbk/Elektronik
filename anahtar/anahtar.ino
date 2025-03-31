#include <SPI.h>
#include <RF24.h>
#include <nRF24L01.h>

//nrf pins vcc 3,3v
#define CE 9    // bu
#define CSN 10  // ve bu değişebilir diğerleri kullanılmasa da sabit pinler
// #define SCK 13
// #define MOSI 11
// #define MISO 12
//#define IRQ 2 // veya 3 kesme pini

#define BUTTON 2  // 2 mi 3 mü

RF24 anten(CE, CSN);
const byte address[6] = "00001";

bool press = false;
bool prev = false;

void setup() {
  Serial.begin(9600);
  if (!anten.begin()) {
    Serial.println("anten not connect");
    while (1) {}
  } else {
    Serial.println("anten connected");
  }
  pinMode(BUTTON, INPUT_PULLUP);  //5V  - 10k - button - 2pin
  // Harici kesmeyi ayarla (D2 pini, INT0 kesmesi)
  anten.setChannel(100);
  anten.setPALevel(RF24_PA_HIGH);  // Güç seviyesini yükselt
  anten.setDataRate(RF24_250KBPS);
  anten.openWritingPipe(address);
  anten.stopListening();

  //attachInterrupt(0, wakeUp, LOW);
}

void loop() {
  
  if(digitalRead(BUTTON) == 0){
    press = true;
  } else{
    press = false;
    prev = false;
  }
  if(press == true  && prev == false){
    int touch = 1;
    bool c = anten.write(&touch, sizeof(touch));
    //anten.flush_tx();
    //anten.flush_rx();
    delay(10);
    if (c) {
      Serial.println("Veri başarıyla gönderildi!");
    } else {
      Serial.println("Veri gönderilemedi!");
    }
    prev = true;
    delay(500);
  }
}
