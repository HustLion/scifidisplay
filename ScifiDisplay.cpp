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

ScifiDisplayBoard* ScifiDisplayBase::get_board(int board) const {
  if(!board_ok(board))
    return 0;
  return boards_[board];
}

static inline const char* next_word(const char* string) {
  while(*string && *string != ' ')
    ++string;
  while(*string == ' ')
    ++string;
  return string;
}

static inline bool in_range(char c, char min, char max) {
  return (c >= min && c <= max);
}

static inline bool is_board_all(char board) {
  return (board == 'a' || board == 'A');
}

static inline int board_as_int(char board) {
  return (is_board_all(board) ? -1 : board - '1');
}

bool ScifiDisplayBase::process_command(const char* command, char* response, unsigned int current_millis) {
  static const int MAX_ARGS = 5;
  const char* argv[MAX_ARGS];
  argv[0] = command;
  for(int i = 1; i < MAX_ARGS; ++i)
    argv[i] = next_word(argv[i - 1]);

  int argc;
  for(argc = 0; argc < MAX_ARGS && *argv[argc]; ++argc)
    ;

  switch(*argv[0]) {
    case 'i': case 'I': // info
      snprintf(response, RESPONSE_SIZE,
"ScifiDisplay v%u.%u\n"
"num_boards: %d\n"
          , (PROTOCOL_VERSION >> 8) & 0xff, PROTOCOL_VERSION & 0xff, num_boards_);
      return true;

    case 'b': case 'B': // brightness
      if(argc != 3 || !board_argv_ok(*argv[1]) || !in_range(*argv[2], '0', '8'))
        goto invalid_args;
      each_board(board_as_int(*argv[1]), &ScifiDisplayBoard::set_brightness, *argv[2] - '0');
      break;

    case 'm': case 'M': // message
      switch(*argv[1]) {
        case 's': case 'S': // set
          if(argc != 5 || !board_argv_ok(*argv[2]) || !in_range(*argv[3], '1', '8'))
            goto invalid_args;
          each_board(board_as_int(*argv[2]), &ScifiDisplayBoard::set_message, *argv[3] - '1', argv[4]);
          break;

        case 'f': case 'F': // flash
          if(argc != 4 || !board_argv_ok(*argv[2]) || !in_range(*argv[3], '1', '8'))
            goto invalid_args;
          each_board(board_as_int(*argv[2]), &ScifiDisplayBoard::flash_message, *argv[3] - '1', current_millis);
          break;

        case 'd': case 'D': // disable
          if(argc != 3 || !board_argv_ok(*argv[2]))
            goto invalid_args;
          each_board(board_as_int(*argv[2]), &ScifiDisplayBoard::disable_message);
          break;

        default:
          goto invalid_args;
      }
      break;

    case 'l': case 'L': // leds
      switch(*argv[1]) {
        case 'b': case 'B': // blink
          if(argc != 4 || !board_argv_ok(*argv[2]) || (*argv[3] != 'r' && *argv[3] != 'R' && *argv[3] != 'g' && *argv[3] != 'G'))
            goto invalid_args;
          each_board(board_as_int(*argv[2]), &ScifiDisplayBoard::blink_leds, (*argv[3] == 'g' || *argv[3] == 'G'), current_millis);
          break;

        case 'f': case 'F': // flash
          if(argc != 4 || !board_argv_ok(*argv[2]) || (*argv[3] != 'r' && *argv[3] != 'R' && *argv[3] != 'g' && *argv[3] != 'G'))
            goto invalid_args;
          each_board(board_as_int(*argv[2]), &ScifiDisplayBoard::flash_leds, (*argv[3] == 'g' || *argv[3] == 'G'), current_millis);
          break;

        case 'd': case 'D': // disable
          if(argc != 3 || !board_argv_ok(*argv[2]))
            goto invalid_args;
          each_board(board_as_int(*argv[2]), &ScifiDisplayBoard::disable_leds);
          break;

        default:
          goto invalid_args;
      }
      break;

    default:
      snprintf(response, RESPONSE_SIZE, "Unknown command %c",
          (*argv[0] >= 0x20 && *argv[0] < 0x7f ? *argv[0] : ' '));
      return false;
  }

  snprintf(response, RESPONSE_SIZE, "ok");
  return true;

invalid_args:
  snprintf(response, RESPONSE_SIZE, "Invalid args for command %c", *argv[0]);
  return false;
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

bool ScifiDisplayBase::board_ok(int board) const {
  return (board >= 0 && board < num_boards_);
}

bool ScifiDisplayBase::board_argv_ok(char board) const {
  return (is_board_all(board) || in_range(board, '1', '0' + num_boards_));
}
