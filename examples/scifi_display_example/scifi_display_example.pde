#include "ScifiDisplayBoard.h"

static const char* messages[][ScifiDisplayBoard::NUM_DIGITS] = {
  {
    "dANGEr",
    "FAILUrE",
    "CAUtION",
    "ErrOr",
    "EvACUAtE",
    "COdE rEd",
    "dEStrOY",
    "LEvEL 12",
  },
  {
    "CONtAIN",
    "brEACH",
    "dEAtH",
    "dEAdbEEF",
    "rUN",
    "bIOLAb",
    "SHOOt",
    "tHrEAt",
  }
};


ScifiDisplayBoard board(8, 7, 6);

void setup() {
  for(int i = 0; i < ScifiDisplayBoard::NUM_DIGITS; ++i)
    board.set_message(i, messages[0][i]);

  board.blink_message(0, (unsigned int)millis());
}

void loop() {
  board.update((unsigned int)millis());
}
