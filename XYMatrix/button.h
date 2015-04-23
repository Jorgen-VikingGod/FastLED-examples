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

class CButton
{
  private:
    uint8_t _pin;
    uint8_t _pinMode = INPUT_PULLUP; // default pin mode
    uint8_t _reading = HIGH;         // current state
    uint8_t _previous = HIGH;        // previous state 
    uint8_t _normalState = HIGH;     // state of normal button (not pressed)
    uint8_t _pressState = LOW;       // state of pressed button
    unsigned long _time = 0;         // last time the output pin was toggled
    unsigned long _debounce = 200;   // debounce time, increase on flickers
  public:
    CButton() {}
    CButton(uint8_t buttonPin, 
            uint8_t buttonPinMode = INPUT_PULLUP, 
            unsigned long debounceTime = 200)
    {
      setup(buttonPin, buttonPinMode, debounceTime);
    }
    ~CButton() {}
    void setup(uint8_t buttonPin, 
               uint8_t buttonPinMode = INPUT_PULLUP, 
               unsigned long debounceTime = 200) 
    {
        _pin          = buttonPin;
        _pinMode      = buttonPinMode;
        _reading      = _pinMode == INPUT_PULLUP ? HIGH : LOW;
        _previous     = _reading;
        _normalState  = _reading;
        _pressState   = _pinMode == INPUT_PULLUP ? LOW : HIGH;
        _debounce     = debounceTime;
        // assigne pin mode to button pin
        pinMode(_pin, _pinMode);
    }
    // check for button press within a debounce time
    bool pressed() {
      bool bPressed = false;
      _reading = digitalRead(_pin);
      if (millis() - _time > _debounce &&
          _reading == _pressState &&
          _previous == _normalState) 
      {
        bPressed = true;
        _time = millis();
      }
      _previous = _reading;
      return bPressed;
    }
};

