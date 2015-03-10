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
    uint8_t _width;
    uint8_t _height;
    uint8_t _num_leds;
  public:
    bool    serpentineLayout;
    CRGB*   leds;
  public:
    CMatrixXY(){}
    CMatrixXY(uint8_t uiWidth, uint8_t uiHeight, bool bSerpentine = true) {
      configureMatrix(uiWidth, uiHeight, bSerpentine);
    }
    ~CMatrixXY() {
      free(leds);
    }
    void configureMatrix(uint8_t uiWidth, uint8_t uiHeight, bool bSerpentine = true) {
      _width            = uiWidth;
      _height           = uiHeight;
      _num_leds         = _width * _height;
      serpentineLayout  = bSerpentine;
      if (leds)
        free(leds);
      leds = (CRGB*) malloc(_num_leds * sizeof(CRGB));
    }
    uint8_t width() { return _width; }
    void setWidth(uint8_t uiWidth) { configureMatrix(uiWidth, _height, serpentineLayout); }
    uint8_t height() { return _width; }
    void setHeight(uint8_t uiHeight) { configureMatrix(_width, uiHeight, serpentineLayout); }
    uint8_t numberOfLEDs() { return _num_leds; }
    // get pixel index of matrix by x/y >> code base by Mark Kriegsman, simplified by Juergen Skrotzky
    uint16_t XY(uint8_t x, uint8_t y) {
      // Odd rows run backwards : Even rows run forwards
      uint8_t deltaX = (y & 0x01) ? (_width - 1) - x : x;
      // for none serpentine layouts always run forwards
      if (serpentineLayout == false)
        deltaX = x;
      return (y * _width) + deltaX;
    }
};

