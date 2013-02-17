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

#include "Arduino.h"
#include "ScifiDisplayBoard.h"
#include <stdlib.h> // For random().

// These would be the TM1638_COLOR_* constants in TM1638.h, but they're defined
// backwards there so I use the correct numerical values here instead.
static const int COLOR_GREEN = 1;
static const int COLOR_RED = 2;

ScifiDisplayBoard::ScifiDisplayBoard(int data_pin, int clock_pin, int strobe_pin)
    : board_((byte)data_pin, (byte)clock_pin, (byte)strobe_pin) {
  reported_buttons_ = 0u;

  for(int i = 0; i < NUM_DIGITS; ++i)
    messages_[i][0] = '\0';
  message_state_ = -1;

  leds_state_ = -1;

  board_.clearDisplay();
  board_.setLEDs((word)0);
}

void ScifiDisplayBoard::set_brightness(int brightness) {
  board_.setupDisplay(brightness > 0, (byte)(brightness - 1));
}

static inline bool message_index_ok(int index) {
  return (index >= 0 && index < ScifiDisplayBoard::NUM_DIGITS);
}

void ScifiDisplayBoard::set_message(int index, const char* text) {
  if(!message_index_ok(index))
    return;

  int len;
  for(len = 0; len < NUM_DIGITS && text[len]; ++len)
    ;
  int padding = (NUM_DIGITS - len) >> 1;

  for(int i = 0; i < padding; ++i)
    messages_[index][i] = ' ';
  for(int i = 0; i < len; ++i)
    messages_[index][i + padding] = text[i];
  messages_[index][len + padding] = '\0';
}

int ScifiDisplayBoard::get_message_index() {
  return (message_state_ < 0 ? -1 : message_index_);
}

void ScifiDisplayBoard::blink_message(int index, unsigned int current_millis) {
  if(!message_index_ok(index))
    return;

  message_index_ = index;
  message_state_ = 0;
  message_state_change_millis_ = current_millis;

  board_.clearDisplay();
}

void ScifiDisplayBoard::disable_message() {
  message_state_ = -1;

  board_.clearDisplay();
}

void ScifiDisplayBoard::blink_leds(bool green, unsigned int current_millis) {
  leds_value_ = (unsigned int)(random() & 0xff);
  leds_color_ = (green ? COLOR_GREEN : COLOR_RED);
  leds_state_ = 2;
  leds_state_change_millis_ = current_millis;

  for(int i = 0; i < NUM_DIGITS; ++i)
    update_led(i);
}

void ScifiDisplayBoard::flash_leds(bool green, unsigned int current_millis) {
  leds_value_ = 0u;
  leds_color_ = (green ? COLOR_GREEN : COLOR_RED);
  leds_state_ = 0;
  leds_state_change_millis_ = current_millis;

  board_.setLEDs((word)0);
}

void ScifiDisplayBoard::disable_leds() {
  leds_state_ = -1;

  board_.setLEDs((word)0);
}

unsigned int ScifiDisplayBoard::update(unsigned int current_millis) {
  static const unsigned int MESSAGE_STATE_DURATION[2] = {
    200u, 400u,
  };
  static const unsigned int LEDS_STATE_DURATION[3] = {
    100u, 200u,
    300u,
  };

  if(message_state_ >= 0
  && current_millis - message_state_change_millis_ >= MESSAGE_STATE_DURATION[message_state_]) {
    message_state_change_millis_ += MESSAGE_STATE_DURATION[message_state_];
    message_state_ = !message_state_;

    if(message_state_)
      board_.setDisplayToString(messages_[message_index_]);
    else
      board_.clearDisplay();
  }

  if(leds_state_ >= 0
  && current_millis - leds_state_change_millis_ >= LEDS_STATE_DURATION[leds_state_]) {
    leds_state_change_millis_ += LEDS_STATE_DURATION[leds_state_];

    if(leds_state_ == 2) {
      int flip = (int)(random() & (NUM_DIGITS - 1));
      leds_value_ ^= (1 << (unsigned int)flip);

      update_led(flip);
    }
    else {
      leds_state_ = !leds_state_;

      if(leds_state_)
        board_.setLEDs(leds_color_ == COLOR_GREEN ? (word)0xff : (word)0xff00);
      else
        board_.setLEDs(0);
    }
  }

  return get_button_presses();
}

void ScifiDisplayBoard::update_led(int index) {
  board_.setLED(
    ((leds_value_ & (1 << (unsigned int)index)) ? (byte)leds_color_ : 0),
    (byte)index
  );
}

unsigned int ScifiDisplayBoard::get_button_presses() {
  unsigned int buttons = (unsigned int)board_.getButtons();
  unsigned int new_buttons = buttons & ~reported_buttons_;
  reported_buttons_ = buttons;
  return new_buttons;
}
