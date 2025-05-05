#include <SoftwareSerial.h>

#define VOLUME 9

String msg;
String msg2;

SoftwareSerial Bt(10, 11); //rx, tx

void setup() {
  // put your setup code here, to run once:
  pinMode(VOLUME, OUTPUT);
  
  Serial.begin(9600);
  Serial.println("Komut gir:");
  Bt.begin(9600);

}

void loop() {
  
  if (Bt.available()) {
    char data = Bt.read();
    if (data != '\n') {
      msg += (char)data;
    }
    else {
      Serial.println(msg);
      Serial.println(map(msg.toInt(), 0, 100, 0, 255));
      analogWrite(VOLUME, map(msg.toInt(), 0, 100, 0, 255));
      msg = "";
    }
    /*if (msg.length() > 0) {
      Serial.println(msg);
      analogWrite(VOLUME, map(msg.toInt(), 0, 100, 0, 255));
    }*/
    //Serial.write(data);
    //Serial.write((char)Bt.read());
    //analogWrite(VOLUME, map(Bt.read(), 0, 100, 0, 255));
  }
  //if (msg != "") Serial.println(msg);
  
  //if (Serial.available()) {
    //char data = Serial.read();
    //Bt.write(data);
    //Bt.write((char)Serial.read());
  //}
  //Serial.println(msg);
}
