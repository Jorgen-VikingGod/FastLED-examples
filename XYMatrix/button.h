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

class CButton
{
  private:
    uint8_t pin;
    uint8_t reading = HIGH;   // current state
    uint8_t previous = HIGH;  // previous state 
    long time = 0;            // the last time the output pin was toggled
    long debounce = 200;      // the debounce time, increase if the output flickers
  public:
    CButton() {}
    ~CButton() {}
    void setup(uint8_t buttonPin, uint8_t buttonPinMode, long debounceTime = 200) {
        pin = buttonPin;
        pinMode(buttonPin, buttonPinMode);
        debounce = debounceTime;
    }
    bool pressed() {
      bool bPressed = false;
      reading = digitalRead(pin);
      if (reading == LOW && previous == HIGH && millis() - time > debounce) {
        bPressed = true;
        time = millis();
      }
      previous = reading;
      return bPressed;
    }
};

