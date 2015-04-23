/******************************************************************************
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
******************************************************************************/

// define, to tell FastLED 3.1 to work as stable version
#define FASTLED_INTERNAL

#include <FastLED.h>
#include "button.h"
#include "matrix.h"

// define push button digital pins
#define BUTTON_STATE_PIN        0
#define BUTTON_MODE_PIN         1
#define BUTTON_BRIGHTNESS_PIN   2
#define BUTTON_TEMPERATURE_PIN  3

// initial CButton instances
CButton buttonState;
CButton buttonMode;
CButton buttonBrightness;
CButton buttonTemperature;

// define LED digital pin, color order, chipset and brightness
#define LED_PIN     7
#define COLOR_ORDER GRB
#define CHIPSET     WS2811
#define BRIGHTNESS  255

// helper variables for brightness and mode
int state;
int mode;
int brightness;
int lightList[] = {36, 90, 120, 190, 255};
int lightCount  = sizeof(lightList) / sizeof(int);
int lightIndex;

int temperature;
int tempList[]  = {Candle, Tungsten40W, Tungsten100W, Halogen, CarbonArc, HighNoonSun, DirectSunlight, OvercastSky, ClearBlueSky};
int tempCount   = sizeof(tempList) / sizeof(int);
int tempIndex;

// initial matrix layout (to get led strip index by x/y)
CMatrixXY matrix;

// setup demo code
void setup() 
{
  // set default speed for serial (for debug with serial monitor)
  Serial.begin(9600);  
  
  // initial matrix layout (6x6 LEDs with zig-zag layout = default)
  matrix.configureMatrix(6, 6);
  
  // initial brightness and mode
  brightness  = BRIGHTNESS;
  lightIndex  = 4;
  temperature = OvercastSky;
  tempIndex   = 7;
  mode        = 0;
  state       = 0;
  
  // initial LEDs
  FastLED.addLeds<CHIPSET, LED_PIN, COLOR_ORDER>(matrix.leds, 
                                                 matrix.numberOfLEDs());
  FastLED.setCorrection(TypicalSMD5050);
  FastLED.setBrightness(BRIGHTNESS);
  FastLED.setTemperature(OvercastSky);
  
  // initial buttons with digital pins with default pin mode = INPUT_PULLUP
  buttonState.setup(BUTTON_STATE_PIN);
  buttonMode.setup(BUTTON_MODE_PIN);
  buttonBrightness.setup(BUTTON_BRIGHTNESS_PIN);
  buttonTemperature.setup(BUTTON_TEMPERATURE_PIN);
}

// execute demo fade code
void loop()
{
  // first check for pressed buttons to change mode and brightness
  if (buttonState.pressed()) {
    if (state == 0) {
      state = 1;
      Serial.println("state On");
    } else {
      state = 0;   
      stateOff();
    }   
  } else if (buttonMode.pressed()) {
    if (mode == 0) {
      mode = 1;
      Serial.println("mode colorful");
    } else {
      mode = 0;
      Serial.println("mode full white");
    }
  } else if (buttonBrightness.pressed()) {
    lightIndex = (lightIndex >= lightCount-1) ? 0 : lightIndex+1;
    brightness = lightList[lightIndex];
    Serial.print("brightness ");
    Serial.println(brightness);
  } else if (buttonTemperature.pressed()) {
    tempIndex = (tempIndex >= tempCount-1) ? 0 : tempIndex+1;
    temperature = tempList[tempIndex];
    FastLED.setTemperature(temperature);
    Serial.print("temperature: ");
    Serial.println(temperature);
  }
  
  if (state == 1)
    DrawOneFrame();
}

void stateOff()
{
  FastLED.setBrightness(255);
  int fade = 255;
  for (int i=0;i<3; i++) {
    fade = min(max(0,fade),255);
    FastLED.setBrightness(fade);
    delay(50);
    fill_solid(matrix.leds, matrix.numberOfLEDs(), CRGB::White);
    FastLED.show();
    delay(50);
    fill_solid(matrix.leds, matrix.numberOfLEDs(), CRGB::Black);
    FastLED.show();
    fade -= 85;
  }
  // change mode and brightness and do some debug outputs
  fill_solid(matrix.leds, matrix.numberOfLEDs(), CRGB::Black);
  FastLED.setBrightness(0);
  FastLED.show();
  Serial.println("state Off");
}

// one frame of demo fade animation
void DrawOneFrame()
{
  uint32_t ms = millis();
  if (mode == 0) {
    fill_solid(matrix.leds, matrix.numberOfLEDs(), CRGB::White);
  } else {
    uint8_t width = matrix.width();
    uint8_t height = matrix.height();
    int32_t yHueDelta32 = ( (int32_t)cos16( ms * (27/1) ) * (350 / width) );
    int32_t xHueDelta32 = ( (int32_t)cos16( ms * (39/1) ) * (310 / height) );
    byte startHue8 = ms / 65536;
    byte lineStartHue = startHue8;
    int8_t yHueDelta8 = yHueDelta32 / 32768;
    int8_t xHueDelta8 = xHueDelta32 / 32768;
    for( byte y = 0; y < height; y++) {
      lineStartHue += yHueDelta8;
      byte pixelHue = lineStartHue;      
      for( byte x = 0; x < width; x++) {
        pixelHue += xHueDelta8;
        matrix.leds[ matrix.XY(x, y) ]  = CHSV(pixelHue, 255, 255);
      }
    }
  }
  if( ms < 5000 ) {
    FastLED.setBrightness( scale8(brightness, (ms * 256) / 5000) );
  } else {
    FastLED.setBrightness(brightness);
  }
  FastLED.show();
}

