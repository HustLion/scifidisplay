#include "ScifiDisplay.h"
#include "TM1638.h" // Needed because of shortcomings in the Arduino IDE.

static const int NUM_BOARDS = 2;

static const char* messages[NUM_BOARDS][ScifiDisplayBoard::NUM_DIGITS] = {
  {
    "dANGEr",
    "FAILUrE",
    "CAUtION",
    "ErrOr",
    "EuACUAtE",
    "COdE rEd",
    "dEStrOY",
    "LEuEL 12",
  },
  {
    "CONtAIN",
    "brEACH",
    "dEAtH",
    "/w{K!m_#",
    "rUN",
    "bIOLAb",
    "SHOOt",
    "tHrEAt",
  }
};

ScifiDisplay<NUM_BOARDS> display(8, 7, 6, 5);

void setup() {
  Serial.begin(9600);

  for(int i = 0; i < NUM_BOARDS; ++i) {
    for(int m = 0; m < ScifiDisplayBoard::NUM_DIGITS; ++m)
      display.get_board(i)->set_message(m, messages[i][m]);
    display.get_board(i)->blink_leds(false, (unsigned int)millis());
  }
}

void loop() {
  unsigned int current_millis = millis();

  if(Serial.available() > 0) {
    char command[64];
    int len = Serial.readBytesUntil('\n', command, 63);
    if(len > 0) {
      command[len] = '\0';
      char response[128];
      if(!display.process_command(command, response, current_millis))
        Serial.println("Error processing command");
      else
        Serial.print(response);
    }
  }

  display.update(current_millis);
}
