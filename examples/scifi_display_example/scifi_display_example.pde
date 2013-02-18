#include "ScifiDisplay.h"
#include "TM1638.h" // Needed because of shortcomings in the Arduino IDE.

static const int NUM_BOARDS = 2;

static const char* messages[NUM_BOARDS][ScifiDisplayBoard::NUM_DIGITS] = {
  {
    "dAnGEr",
    "FAILUrE",
    "CAUtIOn",
    "ErrOr",
    "EuACUAtE",
    "COdE rEd",
    "dEStrOY",
    "LEuEL 12",
  },
  {
    "CONtAIn",
    "brEACH",
    "dEAtH",
    "/w{K!m_#",
    "rUn",
    "bIOLAb",
    "SHOOt",
    "tHrEAt",
  }
};

ScifiDisplay<NUM_BOARDS> display(8, 7, 6, 5);

void setup() {
  for(int i = 0; i < NUM_BOARDS; ++i) {
    for(int m = 0; m < ScifiDisplayBoard::NUM_DIGITS; ++m)
      display.set_message(i, m, messages[i][m]);
    display.blink_leds(i, false, (unsigned int)millis());
  }
}

void loop() {
  display.update((unsigned int)millis());
}
