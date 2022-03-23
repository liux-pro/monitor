/**************************************************************************
 This is an example for our Monochrome OLEDs based on SSD1306 drivers

 Pick one up today in the adafruit shop!
 ------> http://www.adafruit.com/category/63_98

 This example is for a 128x64 pixel display using SPI to communicate
 4 or 5 pins are required to interface.

 Adafruit invests time and resources providing this open
 source code, please support Adafruit and open-source
 hardware by purchasing products from Adafruit!

 Written by Limor Fried/Ladyada for Adafruit Industries,
 with contributions from the open source community.
 BSD license, check license.txt for more information
 All text above, and the splash screen below must be
 included in any redistribution.
 **************************************************************************/
#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for SSD1306 display connected using software SPI (default case):
// #define OLED_MOSI   9
// #define OLED_CLK   10
// #define OLED_DC    11
// #define OLED_CS    12
// #define OLED_RESET 13
// Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT,
//   OLED_MOSI, OLED_CLK, OLED_DC, OLED_RESET, OLED_CS);

/* Comment out above, uncomment this block to use hardware SPI */
#define OLED_DC     6
#define OLED_CS     7
#define OLED_RESET  8
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT,
  &SPI, 5, 3, 4);


#define NUMFLAKES     99 // Number of snowflakes in the animation example

#define LOGO_HEIGHT   16
#define LOGO_WIDTH    16
static const unsigned char PROGMEM logo_bmp[] =
{ 0b11111111, 0b11000000,
  0b00000001, 0b11000000,
  0b00000001, 0b11000000,
  0b00000011, 0b11100000,
  0b11110011, 0b11100000,
  0b11111110, 0b11111000,
  0b01111110, 0b11111111,
  0b00110011, 0b10011111,
  0b00011111, 0b11111100,
  0b00001101, 0b01110000,
  0b00011011, 0b10100000,
  0b00111111, 0b11100000,
  0b00111111, 0b11110000,
  0b01111100, 0b11110000,
  0b01110000, 0b01110000,
  0b11111111, 0b00110000 };

static const unsigned char PROGMEM bmp[]={255, 255, 255, 255, 0, 0, 0, 0, 0, 0, 63, 255, 255, 255, 255, 255, 255, 255, 255, 255, 0, 0, 0, 0, 0, 0, 63, 255, 255, 255, 255, 255, 255, 255, 255, 255, 0, 0, 0, 0, 0, 0, 127, 255, 255, 255, 255, 255, 255, 255, 255, 255, 128, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 252, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 252, 0, 0, 0, 0, 1, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 248, 0, 0, 0, 0, 3, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 240, 0, 0, 0, 0, 7, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 224, 0, 0, 0, 0, 63, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 192, 0, 0, 0, 0, 63, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 224, 0, 0, 0, 0, 63, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 252, 0, 0, 0, 0, 63, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 254, 0, 0, 0, 0, 127, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 254, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 254, 0, 0, 0, 1, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 0, 0, 0, 1, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 0, 0, 0, 0, 127, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 128, 0, 0, 0, 127, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 128, 0, 0, 0, 127, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 0, 0, 0, 0, 127, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 0, 0, 0, 0, 127, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 128, 0, 0, 0, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 254, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 240, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 224, 0, 0, 0, 0, 127, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 192, 0, 0, 0, 0, 127, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 192, 0, 0, 0, 0, 63, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 192, 0, 0, 0, 0, 63, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 192, 0, 0, 0, 0, 63, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 128, 0, 0, 0, 0, 31, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 128, 0, 0, 0, 0, 15, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 128, 0, 0, 0, 0, 1, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 128, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 255, 255, 255, 240, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 255, 255, 255, 240, 0, 0, 0, 0, 0, 1, 255, 255, 255, 255, 255, 255, 255, 255, 255, 248, 0, 0, 0, 0, 0, 1, 255, 255, 255, 255, 255, 255, 255, 255, 255, 248, 0, 0, 0, 0, 0, 1, 255, 255, 255, 255, 255, 255, 255, 255, 255, 252, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 255, 255, 255, 248, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 255, 255, 255, 248, 0, 192, 0, 0, 0, 0, 127, 255, 255, 255, 255, 255, 255, 255, 255, 248, 0, 192, 0, 0, 0, 0, 127, 255, 255, 255, 255, 255, 255, 255, 255, 240, 0, 192, 0, 0, 0, 0, 63, 255, 255, 255, 255, 255, 255, 255, 255, 240, 0, 224, 0, 0, 0, 0, 63, 255, 255, 255, 255, 255, 255, 255, 255, 240, 0, 224, 0, 0, 0, 0, 31, 255, 255, 255, 255, 255, 255, 255, 255, 224, 0, 224, 0, 0, 0, 0, 31, 255, 255, 255, 255, 255, 255, 255, 255, 224, 0, 192, 0, 0, 0, 0, 31, 255, 255, 255, 255, 255, 255, 255, 255, 192, 0, 192, 0, 0, 0, 0, 15, 255, 255, 255, 255, 255, 255, 255, 255, 128, 0, 192, 0, 0, 0, 0, 15, 255, 255, 255, 255, 255, 255, 255, 255, 0, 0, 192, 0, 0, 0, 0, 7, 255, 255, 255, 255, 255, 255, 255, 255, 0, 0, 192, 0, 0, 0, 0, 3, 255, 255, 255, 255, 255, 255, 255, 254, 0, 1, 128, 0, 0, 0, 0, 3, 255, 255, 255, 255, 255, 255, 255, 252, 0, 1, 128, 0, 0, 0, 0, 1, 255, 255, 255, 255, 255, 255, 255, 252, 0, 1, 128, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 255, 248, 0, 1, 128, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 255, 240, 0, 1, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 255, 224, 0, 0, 0, 0, 0, 0, 0, 1, 255, 255, 255, 255, 255, 255, 255, 224, 0, 0, 0, 0, 0, 0, 0, 1, 255, 255, 255, 255, 255, 255, 255, 192, 0, 0, 0, 0, 0, 0, 0, 1, 255, 255, 255, 255, 255, 255, 255, 128, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 255, 128, 0, 0, 0, 0, 0, 0, 0, 0, 127, 255, 255, 255, 255, 255, 255, 192, 0, 0, 0, 0, 0, 0, 0, 0, 127, 255, 255, 255, 255};



#define XPOS   0 // Indexes into the 'icons' array in function below
#define YPOS   1
#define DELTAY 2

void testanimate(const uint8_t *bitmap, uint8_t w, uint8_t h) {
  int8_t f, icons[NUMFLAKES][3];

  // Initialize 'snowflake' positions
  for(f=0; f< NUMFLAKES; f++) {
    icons[f][XPOS]   = random(1 - LOGO_WIDTH, display.width());
    icons[f][YPOS]   = -LOGO_HEIGHT;
    icons[f][DELTAY] = random(1, 6);
    Serial.print(F("x: "));
    Serial.print(icons[f][XPOS], DEC);
    Serial.print(F(" y: "));
    Serial.print(icons[f][YPOS], DEC);
    Serial.print(F(" dy: "));
    Serial.println(icons[f][DELTAY], DEC);
  }

  for(;;) { // Loop forever...
    display.clearDisplay(); // Clear the display buffer

    // Draw each snowflake:
    for(f=0; f< NUMFLAKES; f++) {
      display.drawBitmap(icons[f][XPOS], icons[f][YPOS], bitmap, w, h, SSD1306_WHITE);
    }

    display.display(); // Show the display buffer on the screen
    delay(10);        // Pause for 1/10 second

    // Then update coordinates of each flake...
    for(f=0; f< NUMFLAKES; f++) {
      icons[f][YPOS] += icons[f][DELTAY];
      // If snowflake is off the bottom of the screen...
      if (icons[f][YPOS] >= display.height()) {
        // Reinitialize to a random position, just off the top
        icons[f][XPOS]   = random(1 - LOGO_WIDTH, display.width());
        icons[f][YPOS]   = -LOGO_HEIGHT;
        icons[f][DELTAY] = random(1, 6);
      }
    }
  }
}


void setup() {
  Serial.begin(9600);

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }


  // Invert and restore display, pausing in-between
  display.invertDisplay(true);
  delay(1000);
  // display.invertDisplay(false);
  // delay(1000);
  display.clearDisplay();
  // testanimate(logo_bmp, LOGO_WIDTH, LOGO_HEIGHT); // Animate bitmaps
  display.drawBitmap(0,0,bmp,128,64,SSD1306_WHITE);
  display.display();
}

void loop() {
}

