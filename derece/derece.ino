
#define LATCH  4
#define CLOCK  3
#define DATA  2
#define TEMP A0
#define D1 6
#define D2 7
#define D3 8
#define D4 9
#define BUZZER 10

byte rakamlar[12] = {
  //abcdefg
  0b01111110,//0
  0b00110000,//1
  0b01101101,//2
  0b01111001,//3
  0b00110011,//4
  0b01011011,//5
  0b01011111,//6
  0b01110000,//7
  0b01111111,//8
  0b01111011,//9
  0b01100011,//°
  0b01001110,//C
};

byte display[4]; // Ekrana yazdırılacak veri

unsigned long zaman;
unsigned long onceki_zaman;
unsigned long buzzer_zaman;
bool buzzer_durum;
int bekle = 2000; //2sn

int temperatureC;
int temp;

void setup() {
  Serial.begin(9600);
  // put your setup code here, to run once:
  pinMode(LATCH, OUTPUT);
  pinMode(DATA, OUTPUT);  
  pinMode(CLOCK, OUTPUT);

  pinMode(D1, OUTPUT);
  pinMode(D2, OUTPUT);
  pinMode(D3, OUTPUT);
  pinMode(D4, OUTPUT);

  pinMode(A0, INPUT);

  pinMode(BUZZER, 10);
  
  temp = analogRead(A0) / 1023.0 * 500;
  display[0] = rakamlar[temp / 10];
  display[1] = rakamlar[temp % 10];
  display[2] = rakamlar[10];
  display[3] = rakamlar[11];
}
//27°C
void loop() {
  zaman = millis();
  if(zaman - onceki_zaman >= bekle){
    onceki_zaman = zaman;    
    temp = analogRead(A0) / 1024.0 * 500;
    Serial.println(analogRead(A0) * 0.48828125);
    Serial.println(temp);
    
    display[0] = rakamlar[temp / 10];
    display[1] = rakamlar[temp % 10];
  }
  
  if(temp > 24) {
    if(zaman - buzzer_zaman >= 1000){
      buzzer_durum = !buzzer_durum;
      buzzer_zaman = zaman;
    }
    if(buzzer_durum){
      tone(BUZZER, 2000);
    } else {
      noTone(BUZZER);
    }

  } else {
    noTone(BUZZER);
  }


  for (int d = 6; d < 10; d++) {

    digitalWrite(D1, HIGH);
    digitalWrite(D2, HIGH);
    digitalWrite(D3, HIGH);
    digitalWrite(D4, HIGH);


    switch (d) {
      case D1:
      displayWrite(0);
      digitalWrite(D1, LOW);
      break;
      case D2:
      displayWrite(1);
      digitalWrite(D2, LOW);
      break;
      case D3:
      displayWrite(2);
      digitalWrite(D3, LOW);
      break;
      case D4:
      displayWrite(3);
      digitalWrite(D4, LOW);
      break;
    }
    delay(1);
  }
}

void displayWrite(int num) {
   digitalWrite(LATCH, LOW);
   shiftOut(DATA, CLOCK, LSBFIRST, display[num]); //MSBFIRST LSBFIRST
   digitalWrite(LATCH, HIGH);
}
