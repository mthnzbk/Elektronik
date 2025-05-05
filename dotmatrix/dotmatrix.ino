#include <MD_Parola.h>
#include <MD_MAX72xx.h>
#include <SPI.h>

#define HARDWARE_TYPE MD_MAX72XX::PAROLA_HW
#define MAX_DEVICES 1

#define CLK_PIN   13
#define DATA_PIN  11
#define CS_PIN    10

MD_Parola P = MD_Parola(HARDWARE_TYPE, CS_PIN, MAX_DEVICES);

void setup() {
  
  P.begin();
  P.print("Web");
}

void loop() {
  P.print("Web");
  //P.text
  /*if (P.displayAnimate())
    P.displayText("V", PA_LEFT, P.getSpeed(), 1000, PA_SCROLL_DOWN, PA_SCROLL_DOWN);*/
}
