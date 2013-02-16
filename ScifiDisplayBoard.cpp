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

ScifiDisplayBoard::ScifiDisplayBoard(byte data_pin, byte clock_pin, byte strobe_pin)
    : board_(data_pin, clock_pin, strobe_pin) {
  for(int i = 0; i < NUM_DIGITS; ++i)
    messages_[i][0] = '\0';
  message_state_ = -1;
}

static inline bool message_ok(int message) {
  return (message >= 0 && message < ScifiDisplayBoard::NUM_DIGITS);
}

void ScifiDisplayBoard::set_message(int message, const char* text) {
  if(!message_ok(message))
    return;

  int len;
  for(len = 0; len < NUM_DIGITS && text[len]; ++len)
    ;
  int padding = (NUM_DIGITS - len) >> 1;

  for(int i = 0; i < padding; ++i)
    messages_[message][i] = ' ';
  for(int i = 0; i < len; ++i)
    messages_[message][i + padding] = text[i];
  messages_[message][len + padding] = '\0';
}

void ScifiDisplayBoard::blink_message(int message, unsigned int current_millis) {
  if(!message_ok(message))
    return;

  message_ = message;
  message_state_ = 0;
  message_state_change_millis_ = current_millis;
  board_.clearDisplay();
}

static const unsigned int MESSAGE_STATE_DURATION[2] = {
  200u, 400u,
};

byte ScifiDisplayBoard::update(unsigned int current_millis) {
  if(message_state_ >= 0
  && current_millis - message_state_change_millis_ >= MESSAGE_STATE_DURATION[message_state_]) {
    message_state_change_millis_ += MESSAGE_STATE_DURATION[message_state_];
    message_state_ = !message_state_;

    if(message_state_)
      board_.setDisplayToString(messages_[message_]);
    else
      board_.clearDisplay();
  }

  return board_.getButtons();
}
