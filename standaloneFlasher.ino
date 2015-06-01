#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_TFTLCD.h> // Hardware-specific library
#include <stdint.h>
#include <TouchScreen.h>
#include <SdFat.h>
#include <SPI.h>
#include "main.h"
#include "colors.h"


void setup(void) {
  #if DEBUG == 1
    Serial.begin(9600);
    Serial.println("Debugging enabled!");
  #endif
    initTFT();
    initSD();
    setBit(DDRB, SCK); //pinMode(13, OUTPUT);
}

void loop(void) {
  mainMenu();
}















