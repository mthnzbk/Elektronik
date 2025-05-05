#include <TM1637Display.h>

#define CLK 7
#define DIO 6
#define DOTS 64

TM1637Display display(CLK, DIO);
int counter = 0;
bool flip;

void setup() {
  display.setBrightness(2);
}

void loop() {
  // put your main code here, to run repeatedly:
  counter++;
  display.showNumberDecEx(934+counter, DOTS*flip);
  flip = !flip;
  delay(1000);

}
