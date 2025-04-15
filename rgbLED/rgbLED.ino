
#define RED 9
#define GREEN 10
#define BLUE 11

//byte counter = 0;

// Renk değerleri
int red   = 255;
int green = 0;
int blue  = 0;

void setup() {
  Serial.begin(9600);
  pinMode(RED, OUTPUT);
  pinMode(GREEN, OUTPUT);
  pinMode(BLUE, OUTPUT);

}



void loop() {
  // Renkten renge yumuşak geçiş
  fadeToColor(255, 0, 0);    // Kırmızı
  fadeToColor(255, 255, 0);  // Sarı
  fadeToColor(0, 255, 0);    // Yeşil
  fadeToColor(0, 255, 255);  // Camgöbeği
  fadeToColor(0, 0, 255);    // Mavi
  fadeToColor(255, 0, 255);  // Mor
  fadeToColor(255, 0, 0);    // Kırmızıya dönüş
}

// Hedef renge doğru yavaş geçiş fonksiyonu
void fadeToColor(int rTarget, int gTarget, int bTarget) {
  for (int i = 0; i <= 255; i++) {
    red   = approach(red, rTarget, 1);
    green = approach(green, gTarget, 1);
    blue  = approach(blue, bTarget, 1);

    analogWrite(RED,   red);   // Ortak anot için tersle
    analogWrite(GREEN, green); // Ortak anot için tersle
    analogWrite(BLUE,  blue);  // Ortak anot için tersle

    delay(10); // Geçiş yumuşaklığı (düşürürsen hızlanır)
  }
}

// Mevcut değeri hedefe yaklaştır
int approach(int current, int target, int step) {
  if (current < target) return min(current + step, target);
  if (current > target) return max(current - step, target);
  return current;
}