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

#ifndef SCIFIDISPLAY_H
#define SCIFIDISPLAY_H

#include <ScifiDisplayBoard.h>

class ScifiDisplayBase {
  protected:
    ScifiDisplayBase(int num_boards, ScifiDisplayBoard* board0,
        ScifiDisplayBoard* board1, ScifiDisplayBoard* board2,
        ScifiDisplayBoard* board3);

  public:
    static const int MAX_BOARDS = 4;
    static const int COMMAND_RESPONSE_SIZE = 128;

    ScifiDisplayBoard* get_board(int board) const;

    bool process_command(const char* command, char* response, unsigned int current_millis);

    void update(unsigned int current_millis);

  private:
    bool board_ok(int board) const;

    int num_boards_;
    ScifiDisplayBoard* boards_[MAX_BOARDS];
};

template<int NUM_BOARDS>
class ScifiDisplay;

template<>
class ScifiDisplay<1> : public ScifiDisplayBase {
  public:
    ScifiDisplay(int data_pin, int clock_pin, int strobe_pin)
        : ScifiDisplayBase(1, &board0, 0, 0, 0),
        board0(data_pin, clock_pin, strobe_pin) {
    }

  private:
    ScifiDisplayBoard board0;
};

template<>
class ScifiDisplay<2> : public ScifiDisplayBase {
  public:
    ScifiDisplay(int data_pin, int clock_pin, int strobe1_pin, int strobe2_pin)
        : ScifiDisplayBase(2, &board0, &board1, 0, 0),
        board0(data_pin, clock_pin, strobe1_pin),
        board1(data_pin, clock_pin, strobe2_pin) {
    }

  private:
    ScifiDisplayBoard board0;
    ScifiDisplayBoard board1;
};

template<>
class ScifiDisplay<3> : public ScifiDisplayBase {
  public:
    ScifiDisplay(int data_pin, int clock_pin, int strobe1_pin, int strobe2_pin,
        int strobe3_pin)
        : ScifiDisplayBase(3, &board0, &board1, &board2, 0),
        board0(data_pin, clock_pin, strobe1_pin),
        board1(data_pin, clock_pin, strobe2_pin),
        board2(data_pin, clock_pin, strobe3_pin) {
    }

  private:
    ScifiDisplayBoard board0;
    ScifiDisplayBoard board1;
    ScifiDisplayBoard board2;
};

template<>
class ScifiDisplay<4> : public ScifiDisplayBase {
  public:
    ScifiDisplay(int data_pin, int clock_pin, int strobe1_pin, int strobe2_pin,
        int strobe3_pin, int strobe4_pin)
        : ScifiDisplayBase(4, &board0, &board1, &board2, &board3),
        board0(data_pin, clock_pin, strobe1_pin),
        board1(data_pin, clock_pin, strobe2_pin),
        board2(data_pin, clock_pin, strobe3_pin),
        board3(data_pin, clock_pin, strobe4_pin) {
    }

  private:
    ScifiDisplayBoard board0;
    ScifiDisplayBoard board1;
    ScifiDisplayBoard board2;
    ScifiDisplayBoard board3;
};

#endif
