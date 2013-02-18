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
  board.blink_leds(false, (unsigned int)millis());
}

void loop() {
  unsigned int current_millis = (unsigned int)millis();
  unsigned int buttons = board.update(current_millis);
  if(buttons != 0) {
    for(int i = 0; i < ScifiDisplayBoard::NUM_DIGITS; ++i) {
      if((buttons & (1 << i)) != 0) {
        if(i == board.get_message_index())
          board.disable_message();
        else
          board.flash_message(i, current_millis);
        break;
      }
    }
  }
}
