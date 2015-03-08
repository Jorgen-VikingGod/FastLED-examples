/*
The MIT License (MIT)

Copyright (c) 2015 Juergen Skrotzky alias Jorgen (JorgenVikingGod@gmail.com)

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/

class CMatrixXY
{
  private:
    uint8_t kMatrixWidth;
    uint8_t kMatrixHeight;
    uint8_t num_leds;
  public:
    bool    serpentineLayout;
    CRGB*   leds;
  public:
    CMatrixXY(){}
    CMatrixXY(uint8_t uiWidth, uint8_t uiHeight, bool bSerpentine) {
      configureMatrix(uiWidth, uiHeight, bSerpentine);
    }
    ~CMatrixXY() {
      free(leds);
    }
    void configureMatrix(uint8_t uiWidth, uint8_t uiHeight, bool bSerpentine) {
      kMatrixWidth      = uiWidth;
      kMatrixHeight     = uiHeight;
      serpentineLayout  = bSerpentine;
      num_leds = kMatrixWidth * kMatrixHeight;
      if (leds)
        free(leds);
      leds = (CRGB*) malloc(num_leds * sizeof(CRGB));
    }
    uint8_t width() { return kMatrixWidth; }
    void setWidth(uint8_t uiWidth) { configureMatrix(uiWidth, height(), serpentineLayout); }
    uint8_t height() { return kMatrixWidth; }
    void setHeight(uint8_t uiHeight) { configureMatrix(width(), uiHeight, serpentineLayout); }
    uint8_t numberOfLEDs() { return num_leds; }
    uint16_t XY(uint8_t x, uint8_t y) {
      // get index by matrix (x,y)
      uint16_t i; 
      if( serpentineLayout == false) {
        i = (y * kMatrixWidth) + x;
      }
      if( serpentineLayout == true) {
        if( y & 0x01) { // Odd rows run backwards
          uint8_t reverseX = (kMatrixWidth - 1) - x;
          i = (y * kMatrixWidth) + reverseX;
        } else { // Even rows run forwards
          i = (y * kMatrixWidth) + x;
        }
      }
      return i;
    }
};

