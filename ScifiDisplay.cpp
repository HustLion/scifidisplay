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
#include "ScifiDisplay.h"

ScifiDisplayBase::ScifiDisplayBase(int num_boards, ScifiDisplayBoard* board0,
    ScifiDisplayBoard* board1, ScifiDisplayBoard* board2,
    ScifiDisplayBoard* board3) {
  num_boards_ = num_boards;
  boards_[0] = board0;
  boards_[1] = board1;
  boards_[2] = board2;
  boards_[3] = board3;
}

void ScifiDisplayBase::set_brightness(int brightness) {
  for(int i = 0; i < num_boards_; ++i)
    boards_[i]->set_brightness(brightness);
}

void ScifiDisplayBase::set_message(int board, int index, const char* text) {
  if(board_ok(board))
    boards_[board]->set_message(index, text);
}

void ScifiDisplayBase::flash_message(int board, int index, unsigned int current_millis) {
  if(board_ok(board))
    boards_[board]->flash_message(index, current_millis);
}

void ScifiDisplayBase::disable_message(int board) {
  if(board_ok(board))
    boards_[board]->disable_message();
}

void ScifiDisplayBase::blink_leds(int board, bool green, unsigned int current_millis) {
  if(board_ok(board))
    boards_[board]->blink_leds(green, current_millis);
}

void ScifiDisplayBase::flash_leds(int board, bool green, unsigned int current_millis) {
  if(board_ok(board))
    boards_[board]->flash_leds(green, current_millis);
}

void ScifiDisplayBase::disable_leds(int board) {
  if(board_ok(board))
    boards_[board]->disable_leds();
}

void ScifiDisplayBase::update(unsigned int current_millis) {
  for(int i = 0; i < num_boards_; ++i) {
    ScifiDisplayBoard* board = boards_[i];

    unsigned int buttons = board->update(current_millis);
    for(int b = 0; buttons != 0 && b < ScifiDisplayBoard::NUM_DIGITS; ++b) {
      if((buttons & (1 << (unsigned int)b)) != 0) {
        if(b == board->get_message_index())
          board->disable_message();
        else
          board->flash_message(b, current_millis);
        break;
      }
    }
  }
}

bool ScifiDisplayBase::board_ok(int board) {
  return (board >= 0 && board < num_boards_);
}
