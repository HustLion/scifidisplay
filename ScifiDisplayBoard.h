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

    ScifiDisplayBoard(int data_pin, int clock_pin, int strobe_pin);
    void set_brightness(int brightness); // [0,8], 0 = off.
    void set_message(int index, const char* text);

    int get_message_index();
    void blink_message(int index, unsigned int current_millis);
    void disable_message();

    void blink_leds(bool green, unsigned int current_millis);
    void flash_leds(bool green, unsigned int current_millis);
    void disable_leds();

    unsigned int update(unsigned int current_millis);

  private:
    void update_led(int index);
    unsigned int get_button_presses();

    TM1638 board_;

    unsigned int reported_buttons_;

    char messages_[NUM_DIGITS][NUM_DIGITS + 1];
    int message_index_;
    int message_state_;
    unsigned int message_state_change_millis_;

    unsigned int leds_value_;
    int leds_color_;
    int leds_state_;
    unsigned int leds_state_change_millis_;
};

#endif
