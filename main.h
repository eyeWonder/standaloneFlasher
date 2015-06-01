#ifndef _MAIN_H
#define _MAIN_H

#define DEBUG 0

//Definitions for TFT
#define LCD_CS A3     // Chip Select goes to Analog 3
#define LCD_CD A2     // Command/Data goes to Analog 2
#define LCD_WR A1     // LCD Write goes to Analog 1
#define LCD_RD A0     // LCD Read goes to Analog 0
#define LCD_RESET A4  // Can alternately just connect to Arduino's reset pin

Adafruit_TFTLCD tft(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);

//Definitions for Touchscreen
#define YP A2  // must be an analog pin, use "An" notation!
#define XM A1  // must be an analog pin, use "An" notation!
#define YM 6   // can be a digital pin
#define XP 7   // can be a digital pin

TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);

//Definitions for Programmer
#define SCK PB5
#define MISO PB4
#define MOSI PB3
#define RESET PC5

//Object for MCU
struct mcu{
  uint32_t signature = 0;
  uint8_t  defFuseLow;
  uint8_t  defFuseHigh;
  uint8_t  defFuseExt;
  uint8_t  fuseLow;
  uint8_t  fuseHigh;
  uint8_t  fuseExt;
  uint8_t  flashPageSize;
  uint16_t flashPages;
  uint8_t  eepromPageSize;
  uint16_t eepromPages;
  uint8_t  fuseDelay;
  uint8_t  flashDelay;
  uint8_t  eepromDelay;
  String     Name;
} myMCU;

char lang[3] = {"en"};
char tmpText[30];

SdFat SD;
File myFile;


const uint8_t chipSelect = 10;
uint8_t clockSpeed = SPI_CLOCK_DIV128;

#define setBit(reg, Bit) asm volatile("sbi %0,%1" :: "I" (_SFR_IO_ADDR(reg)) , "I" (Bit));
#define clrBit(reg, Bit) asm volatile("cbi %0,%1" :: "I" (_SFR_IO_ADDR(reg)) , "I" (Bit));

void start_pmode ();

#endif
