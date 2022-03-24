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
#include "lz4.h"
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <Ticker.h>

const char *ssid = "River";
const char *password = "asd123456";
WiFiUDP Udp;
unsigned int localUdpPort = 9527;                     // local port to listen on
char incomingPacket[1024];                            // buffer for incoming packets
char buffer[1024];                            // buffer for incoming packets

Ticker ticker;

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
#define OLED_DC 6
#define OLED_CS 7
#define OLED_RESET 8
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT,
                         &SPI, 5, 3, 4);


#define LOGO_HEIGHT 16
#define LOGO_WIDTH 16
static const unsigned char PROGMEM logo_bmp[] =
    {0b11111111, 0b11000000,
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
     0b11111111, 0b00110000};

static const unsigned char PROGMEM bmp[] = {255, 255, 255, 255, 0, 0, 0, 0, 0, 0, 63, 255, 255, 255, 255, 255, 255, 255, 255, 255, 0, 0, 0, 0, 0, 0, 63, 255, 255, 255, 255, 255, 255, 255, 255, 255, 0, 0, 0, 0, 0, 0, 127, 255, 255, 255, 255, 255, 255, 255, 255, 255, 128, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 252, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 252, 0, 0, 0, 0, 1, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 248, 0, 0, 0, 0, 3, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 240, 0, 0, 0, 0, 7, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 224, 0, 0, 0, 0, 63, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 192, 0, 0, 0, 0, 63, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 224, 0, 0, 0, 0, 63, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 252, 0, 0, 0, 0, 63, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 254, 0, 0, 0, 0, 127, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 254, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 254, 0, 0, 0, 1, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 0, 0, 0, 1, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 0, 0, 0, 0, 127, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 128, 0, 0, 0, 127, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 128, 0, 0, 0, 127, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 0, 0, 0, 0, 127, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 0, 0, 0, 0, 127, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 128, 0, 0, 0, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 254, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 240, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 224, 0, 0, 0, 0, 127, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 192, 0, 0, 0, 0, 127, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 192, 0, 0, 0, 0, 63, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 192, 0, 0, 0, 0, 63, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 192, 0, 0, 0, 0, 63, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 128, 0, 0, 0, 0, 31, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 128, 0, 0, 0, 0, 15, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 128, 0, 0, 0, 0, 1, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 128, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 255, 255, 255, 240, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 255, 255, 255, 240, 0, 0, 0, 0, 0, 1, 255, 255, 255, 255, 255, 255, 255, 255, 255, 248, 0, 0, 0, 0, 0, 1, 255, 255, 255, 255, 255, 255, 255, 255, 255, 248, 0, 0, 0, 0, 0, 1, 255, 255, 255, 255, 255, 255, 255, 255, 255, 252, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 255, 255, 255, 248, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 255, 255, 255, 248, 0, 192, 0, 0, 0, 0, 127, 255, 255, 255, 255, 255, 255, 255, 255, 248, 0, 192, 0, 0, 0, 0, 127, 255, 255, 255, 255, 255, 255, 255, 255, 240, 0, 192, 0, 0, 0, 0, 63, 255, 255, 255, 255, 255, 255, 255, 255, 240, 0, 224, 0, 0, 0, 0, 63, 255, 255, 255, 255, 255, 255, 255, 255, 240, 0, 224, 0, 0, 0, 0, 31, 255, 255, 255, 255, 255, 255, 255, 255, 224, 0, 224, 0, 0, 0, 0, 31, 255, 255, 255, 255, 255, 255, 255, 255, 224, 0, 192, 0, 0, 0, 0, 31, 255, 255, 255, 255, 255, 255, 255, 255, 192, 0, 192, 0, 0, 0, 0, 15, 255, 255, 255, 255, 255, 255, 255, 255, 128, 0, 192, 0, 0, 0, 0, 15, 255, 255, 255, 255, 255, 255, 255, 255, 0, 0, 192, 0, 0, 0, 0, 7, 255, 255, 255, 255, 255, 255, 255, 255, 0, 0, 192, 0, 0, 0, 0, 3, 255, 255, 255, 255, 255, 255, 255, 254, 0, 1, 128, 0, 0, 0, 0, 3, 255, 255, 255, 255, 255, 255, 255, 252, 0, 1, 128, 0, 0, 0, 0, 1, 255, 255, 255, 255, 255, 255, 255, 252, 0, 1, 128, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 255, 248, 0, 1, 128, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 255, 240, 0, 1, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 255, 224, 0, 0, 0, 0, 0, 0, 0, 1, 255, 255, 255, 255, 255, 255, 255, 224, 0, 0, 0, 0, 0, 0, 0, 1, 255, 255, 255, 255, 255, 255, 255, 192, 0, 0, 0, 0, 0, 0, 0, 1, 255, 255, 255, 255, 255, 255, 255, 128, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 255, 128, 0, 0, 0, 0, 0, 0, 0, 0, 127, 255, 255, 255, 255, 255, 255, 192, 0, 0, 0, 0, 0, 0, 0, 0, 127, 255, 255, 255, 255};
static const unsigned char bmp_c[] = {81, 255, 255, 255, 255, 0, 1, 0, 16, 63, 11, 0, 1, 2, 0, 1, 15, 0, 13, 16, 0, 17, 127, 28, 0, 0, 48, 0, 17, 128, 33, 0, 2, 12, 0, 0, 64, 0, 31, 252, 16, 0, 1, 18, 1, 28, 0, 0, 96, 0, 98, 248, 0, 0, 0, 0, 3, 12, 0, 0, 112, 0, 98, 240, 0, 0, 0, 0, 7, 12, 0, 0, 128, 0, 26, 224, 111, 0, 43, 255, 192, 16, 0, 12, 32, 0, 1, 112, 0, 6, 159, 0, 42, 255, 254, 159, 0, 2, 16, 0, 2, 91, 0, 1, 2, 0, 26, 254, 143, 0, 1, 225, 0, 7, 159, 0, 1, 16, 0, 17, 0, 42, 0, 3, 2, 0, 15, 16, 0, 17, 6, 15, 1, 2, 17, 1, 15, 16, 0, 9, 11, 160, 0, 1, 113, 1, 12, 48, 0, 3, 123, 0, 0, 144, 1, 1, 208, 0, 2, 11, 0, 1, 2, 0, 1, 48, 1, 1, 10, 0, 2, 2, 0, 1, 16, 1, 7, 64, 0, 1, 48, 1, 12, 16, 0, 7, 32, 1, 1, 32, 0, 15, 16, 0, 8, 1, 208, 1, 18, 31, 92, 0, 4, 207, 0, 34, 0, 15, 12, 0, 5, 16, 0, 7, 49, 1, 1, 48, 0, 6, 49, 1, 1, 159, 0, 7, 161, 0, 2, 16, 0, 6, 48, 0, 1, 255, 1, 15, 16, 0, 8, 1, 207, 1, 7, 64, 0, 2, 48, 0, 6, 96, 0, 33, 248, 0, 193, 0, 5, 225, 0, 12, 16, 0, 18, 240, 32, 0, 5, 225, 0, 33, 240, 0, 33, 1, 12, 16, 0, 5, 225, 0, 18, 224, 32, 0, 7, 16, 0, 1, 96, 0, 5, 16, 0, 18, 192, 80, 0, 5, 1, 1, 27, 128, 16, 0, 18, 0, 16, 0, 5, 193, 2, 3, 16, 0, 4, 225, 2, 49, 254, 0, 1, 33, 1, 4, 16, 0, 19, 252, 16, 0, 4, 1, 1, 4, 16, 0, 4, 225, 0, 19, 248, 32, 0, 4, 16, 0, 49, 240, 0, 1, 80, 3, 4, 16, 0, 1, 205, 0, 7, 65, 1, 12, 16, 0, 1, 189, 0, 7, 32, 0, 1, 125, 0, 7, 81, 1, 5, 16, 0, 3, 65, 1, 4, 48, 0, 96, 0, 127, 255, 255, 255, 255};

void bordcast(){
    Udp.beginPacketMulticast(IPAddress(255,255,255,255), 12345, WiFi.localIP());
    Udp.write("ready");
    Udp.endPacket();
}

void setup()
{
  Serial.begin(115200);

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if (!display.begin(SSD1306_SWITCHCAPVCC))
  {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;)
      ; // Don't proceed, loop forever
  }

  // Invert and restore display, pausing in-between
  display.invertDisplay(false);
  delay(1000);
  // display.invertDisplay(false);
  // delay(1000);
  display.clearDisplay();
  // testanimate(logo_bmp, LOGO_WIDTH, LOGO_HEIGHT); // Animate bitmaps
  // display.drawBitmap(0, 0, bmp, 128, 64, SSD1306_WHITE);

  char *regen_buffer = (char *)malloc(1024);
  Serial.print(micros());
  const int decompressed_size = LZ4_decompress_safe((char *)bmp_c, regen_buffer, sizeof(bmp_c), 1024);
  Serial.print(micros());
  display.drawBitmap(0, 0, (unsigned char *)regen_buffer, 128, 64, SSD1306_WHITE);

  display.display();

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print('.');
    delay(500);
  }
  Serial.print("Connected! IP address: ");
  Serial.println(WiFi.localIP());
  Serial.printf("UDP server on port %d\n", localUdpPort);
  Udp.begin(localUdpPort);

  ticker.attach(2,bordcast);
}

void loop()
{
  int packetSize = Udp.parsePacket();
  if (packetSize)
  {
    // receive incoming UDP packets
    Serial.printf("Received %d bytes from %s, port %d\n", packetSize, Udp.remoteIP().toString().c_str(), Udp.remotePort());
    int len = Udp.read(incomingPacket, sizeof(incomingPacket));
    if (len > 0)
    {
        unsigned long t = micros();
        LZ4_decompress_safe((char *)incomingPacket, buffer, len, 1024);
        Serial.print(micros()-t);
        Serial.print("    decompress");
        Serial.print("\n");

          display.clearDisplay();
        t = micros();
        //这需要4毫秒，下一步改用copy看看能不能快点。
        display.drawBitmap(0, 0, (unsigned char *)buffer, 128, 64, SSD1306_WHITE);
        Serial.print(micros()-t);
        Serial.print("    drawBitmap");
        Serial.print("\n");
//        memcpy(display.getBuffer(),incomingPacket,1024);
      display.display();
    }
  }
}
