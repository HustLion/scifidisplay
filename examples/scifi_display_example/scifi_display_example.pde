/*
  ScifiDisplay - Arduino library for sci-fi style blinking TM1638 panels
                 <https://github.com/chazomaticus/scifidisplay>
  Copyright 2013 Charles Lindsay <chaz@chazomatic.us>

  ScifiDisplay is free software: you can redistribute it and/or modify it under
  the terms of the GNU Lesser General Public License as published by the Free
  Software Foundation, either version 3 of the License, or (at your option) any
  later version.

  ScifiDisplay is distributed in the hope that it will be useful, but WITHOUT
  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
  FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for more
  details.

  You should have received a copy of the GNU Lesser General Public License
  along with ScifiDisplay.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <ScifiDisplay.h>
// Even though this library is never referenced directly here, it's used by
// ScifiDisplay, and the Arduino IDE isn't smart enough to add the required
// include directory.  Short version: any top level file that uses ScifiDisplay
// also has to include this file.
#include <TM1638.h>

// Let's control 2 TM1638 boards at once.
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

// The two boards share data pin 8 and clock pin 7.  The first board has strobe
// pin 6 and the second board has strobe pin 5.
ScifiDisplay<NUM_BOARDS> display(8, 7, 6, 5);

static const char PROMPT[] = "ScifiDisplay> ";

void setup() {
  Serial.begin(9600);

  for(int i = 0; i < NUM_BOARDS; ++i) {
    for(int m = 0; m < ScifiDisplayBoard::NUM_DIGITS; ++m)
      display.get_board(i)->set_message(m, messages[i][m]);
    display.get_board(i)->blink_leds(false, (unsigned int)millis());
  }

  Serial.print(PROMPT);
}

void loop() {
  unsigned int current_millis = millis();

  if(Serial.available() > 0) {
    char command[ScifiDisplayBase::MAX_COMMAND_SIZE];
    int len = Serial.readBytesUntil('\n', command, sizeof(command) - 1);
    if(len > 0) {
      command[len] = '\0';
      Serial.println(command);

      if(command[0] == 'h' || command[0] == 'H')
        Serial.println(display.get_help());
      else {
        char response[ScifiDisplayBase::RESPONSE_SIZE];
        display.process_command(command, response, current_millis);
        Serial.println(response);
      }
      Serial.print(PROMPT);
    }
  }

  display.update(current_millis);
}
