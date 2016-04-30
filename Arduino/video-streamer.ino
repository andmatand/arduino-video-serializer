#include <stdio.h>
#include <math.h>

#include <Adafruit_GFX.h>   // Core graphics library
#include <RGBmatrixPanel.h> // Hardware-specific library

#define CLK 8  // MUST be on PORTB! (Use pin 11 on Mega)
#define LAT A3
#define OE  9
#define A   A0
#define B   A1
#define C   A2
RGBmatrixPanel matrix(A, B, C, CLK, LAT, OE, false);

void setup() {
  //Serial.begin(9600);
  Serial.begin(115200);
  matrix.begin();

  // clear screen with red
  matrix.fillScreen(matrix.Color333(1, 0, 0));
}

uint16_t read_color() {
  uint8_t input;

  while (true) {
    if (Serial.available() > 0) {
      input = Serial.read();
  
      uint8_t r = (input & 0b11100000) >> 5;
      uint8_t g = (input & 0b00011100) >> 2;
      uint8_t b = input & 0b00000011;

      b *= 2; // Roughly upscale the blue to the full range

      return matrix.Color333(r, g, b);
    }
  }
}


void loop() {
  int i = 0;
  uint16_t color;
  for (uint8_t y = 0; y < 16; y++) {
    for (uint8_t x = 0; x < 32; x++) {
      matrix.drawPixel(x, y, read_color());
    }
  }
}
