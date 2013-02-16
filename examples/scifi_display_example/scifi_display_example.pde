#include "ScifiDisplayBoard.h"

static const char* messages[][ScifiDisplayBoard::NUM_DIGITS] = {
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
    "dEAdbEEF",
    "rUn",
    "bIOLAb",
    "SHOOt",
    "tHrEAt",
  }
};

ScifiDisplayBoard board(8, 7, 6);

void setup() {
  for(int i = 0; i < ScifiDisplayBoard::NUM_DIGITS; ++i)
    board.set_message(i, messages[0][i]);
}

void loop() {
  unsigned int current_millis = (unsigned int)millis();
  byte buttons = board.update(current_millis);
  if(buttons != 0) {
    for(int i = 0; i < 8; ++i) {
      if((buttons & (1 << i)) != 0) {
        board.blink_message(i, current_millis);
        break;
      }
    }
  }
}
