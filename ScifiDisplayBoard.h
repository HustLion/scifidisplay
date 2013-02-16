/*
  ScifiDisplay - Arduino library for sci-fi style blinking TM1638 panels
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

#ifndef SCIFIDISPLAYBOARD_H
#define SCIFIDISPLAYBOARD_H

#include <TM1638.h>

class ScifiDisplayBoard {
  public:
    static const int NUM_DIGITS = 8;

    ScifiDisplayBoard(byte data_pin, byte clock_pin, byte strobe_pin);
    void set_message(int message, const char* text);

    void blink_message(int message, unsigned int current_millis);

    byte update(unsigned int current_millis);

  private:
    TM1638 board_;
    char messages_[NUM_DIGITS][NUM_DIGITS + 1];
    int message_;
    int message_state_;
    unsigned int message_state_change_millis_;
};

#endif
