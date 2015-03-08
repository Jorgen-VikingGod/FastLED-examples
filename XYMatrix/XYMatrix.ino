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

#include <FastLED.h>
#include "button.h"
#include "matrix.h"

// define push button digital pins
#define BUTTON_OFF_PIN 0
#define BUTTON_LOW_PIN 1
#define BUTTON_HIGH_PIN 2

// initial CButton instances
CButton buttonOff;
CButton buttonLow;
CButton buttonHigh;

// define LED digital pin, color order, chipset and brightness
#define LED_PIN  9
#define COLOR_ORDER GRB
#define CHIPSET     WS2811
#define BRIGHTNESS 0

// helper variables for brightness and mode
int brightness;
int mode;

// initial matrix layout (to get led strip index by x/y)
CMatrixXY matrix;

// setup demo code
void setup() {
  // set default speed for serial (for debug with serial monitor)
  Serial.begin(9600);  
  // initial matrix layout (6x6 LEDs with zig-zag layout)
  matrix.configureMatrix(6, 6, true);
  // initial brightness and mode
  brightness = BRIGHTNESS;
  mode = BUTTON_OFF_PIN;
  // initial LEDs
  FastLED.addLeds<CHIPSET, LED_PIN, COLOR_ORDER>(matrix.leds, matrix.numberOfLEDs()).setCorrection(TypicalSMD5050);
  FastLED.setBrightness(BRIGHTNESS);
  // initial buttons with digital pins and pin mode
  buttonOff.setup(BUTTON_OFF_PIN, INPUT_PULLUP);
  buttonLow.setup(BUTTON_LOW_PIN, INPUT_PULLUP);
  buttonHigh.setup(BUTTON_HIGH_PIN, INPUT_PULLUP);
}

// execute demo fade code
void loop()
{
  // first check for pressed buttons to change mode and brightness
  if (buttonOff.pressed()) {
    buttonOff_clicked();
  } else if (buttonLow.pressed()) {
    buttonLow_clicked();
  } else if (buttonHigh.pressed()) {
    buttonHigh_clicked();
  }
  // draw animation on each loop frame
  DrawOneFrame();
}

// helper method of buttonOff clicked state
void buttonOff_clicked()
{
  // change mode and brightness and do some debug outputs
  mode = BUTTON_OFF_PIN;
  brightness = 0;
  FastLED.setBrightness(brightness);
  Serial.println("mode 0 = off");
}

// helper method of buttonLow clicked state
void buttonLow_clicked()
{
  // change mode and brightness and do some debug outputs
  mode = BUTTON_LOW_PIN;
  brightness = 36;
  FastLED.setBrightness(brightness);
  Serial.println("mode 1 = low brightness");
}

// helper method of buttonLow clicked state
void buttonHigh_clicked()
{
  // change mode and brightness and do some debug outputs
  mode = BUTTON_HIGH_PIN;
  brightness = 255;
  FastLED.setBrightness(brightness);
  Serial.println("mode 2 = full brightness");
}

// one frame of demo fade animation
void DrawOneFrame()
{
  uint32_t ms = millis();
  int32_t yHueDelta32 = ((int32_t)cos16( ms * (27/1) ) * ( 350 / matrix.width() ));
  int32_t xHueDelta32 = ((int32_t)cos16( ms * (39/1) ) * ( 310 / matrix.height() ));
  byte startHue8 = ms / 65536;
  byte lineStartHue = startHue8;
  int8_t yHueDelta8 = yHueDelta32 / 32768;
  int8_t xHueDelta8 = xHueDelta32 / 32768;
  for( byte y = 0; y < matrix.height(); y++) {
    lineStartHue += yHueDelta8;
    byte pixelHue = lineStartHue;      
    for( byte x = 0; x < matrix.width(); x++) {
      pixelHue += xHueDelta8;
      matrix.leds[ matrix.XY(x, y)]  = CHSV( pixelHue, 255, 255);
    }
  }
  if( ms < 5000 ) {
    FastLED.setBrightness( scale8( brightness, (ms * 256) / 5000));
  } else {
    FastLED.setBrightness(brightness);
  }
  FastLED.show();
}
