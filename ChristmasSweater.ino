#include <Arduino.h>
#include <FastLED.h>

const int buzzerPin = 8;
const int songLength = 31;
const char notes[] = "eee eee egcde fff ffee eeggfdc " ; 
const int beats[] = {2,2,3,1, 2,2,3,1, 2,2,3,1,4,4, 2,2,3,0, 1,2,2,2,0, 1,1,2,2,2,2,4,4};
const int tempo = 120;

const auto ledPin = 7;
const int numLeds = 9;
CRGB leds[numLeds];
const TProgmemRGBPalette16 ChristmasPalette FL_PROGMEM =
{
    CRGB::DarkGreen, CRGB::DarkGreen,
    CRGB::White, CRGB::White,
    CRGB::DarkRed, CRGB::DarkRed,

    CRGB::DarkGreen,
    CRGB::White, CRGB::White,
    CRGB::DarkRed,

    CRGB::DarkGreen, CRGB::DarkGreen,
    CRGB::White, CRGB::White,
    CRGB::DarkRed, CRGB::DarkRed,
};

void ledSetup();
void updateLeds(int);
void musicSetup();
void musicLoop();
int frequency(char note);

void setup() {
  ledSetup();
  musicSetup();
}

void loop() {    
  musicLoop();
}

void ledSetup() {
  FastLED.addLeds<WS2811, ledPin, GRB>(leds, numLeds).setCorrection(TypicalLEDStrip);
  musicSetup();
}

void updateLeds(int timeLimit) {
  int limit = timeLimit;
  while(limit > 0) {
    static uint8_t startIndex = 0;
    startIndex = startIndex + 1;
    uint8_t colorIndex = startIndex;
    for(int i = 0; i < numLeds; i++) {
      leds[i] = ColorFromPalette(ChristmasPalette, colorIndex, 255, LINEARBLEND);
      colorIndex += 16;
    }
    FastLED.show();
    FastLED.delay(5);
    delay(5);
    limit -= 10;
  }
}

void musicSetup() {
  pinMode(buzzerPin, OUTPUT);
}

void musicLoop() {
  for (int i = 0; i < songLength; i++) {
    int duration = beats[i] * tempo;
    if (notes[i] == ' ') {
      updateLeds(duration);
    } else {
      tone(buzzerPin, frequency(notes[i]), duration);
      updateLeds(duration);
    }
    updateLeds(tempo/10);
  }
}

int frequency(char note) {  
  static constexpr int numNotes = 8;  
  static constexpr char names[] = { 'c', 'd', 'e', 'f', 'g', 'a', 'b', 'C' };
  static constexpr int frequencies[] = {262, 294, 330, 349, 392, 440, 494, 523};
  for (int i = 0; i < numNotes; i++) {
    if (names[i] == note) {
      return frequencies[i];
    }
  }
  return 0;
}

/*
Music playback based on: https://github.com/ammonshepherd/arduino-tuts/blob/master/03-jingle-bells.md

MIT License

Copyright (c) 2021 Ammon Shepherd

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

If you make money off of any of this, send me a some. :)
*/