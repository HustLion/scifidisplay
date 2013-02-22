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

/**
 * A collection of ScifiDisplayBoards that you can send commands to.  This is
 * the base class; to instantiate, use ScifiDisplay<>.
 */
class ScifiDisplayBase {
  protected:
    ScifiDisplayBase(int num_boards, ScifiDisplayBoard* board0,
        ScifiDisplayBoard* board1, ScifiDisplayBoard* board2,
        ScifiDisplayBoard* board3);

  public:
    /// Maximum number of boards possible to chain in one ScifiDisplay.
    static const int MAX_BOARDS = 4;

    /// Maximum size of a valid command string.
    static const int MAX_COMMAND_SIZE = 32;

    /// Necessary size of a command response string.
    static const int RESPONSE_SIZE = 64;

    /// Version of the command protocol.
    static const unsigned int PROTOCOL_VERSION = 0x0001; // 0.1

    /**
     * Return a pointer to the given index of ScifiDisplayBoard, or NULL if
     * invalid index.
     */
    ScifiDisplayBoard* get_board(int board) const;

    /**
     * Return a string explaining the commands accepted in the protocol.  This
     * is implemented in the header so if your code doesn't use it, it won't
     * get compiled in, saving space.
     */
    const char* get_help() const {
      return
"Commands:\n"
"i[nfo] - print info\n"
"b[rightness] BOARD 0-8 - set brightness (0 = off; 8 = max)\n"
"m[essage] s[et] BOARD INDEX text - change message text\n"
"m[essage] f[lash] BOARD INDEX - flash message on display\n"
"m[essage] d[isable] BOARD - stop flashing message\n"
"l[eds] b[link] BOARD r[ed]|g[reen] - randomly blink LEDs\n"
"l[eds] f[lash] BOARD r[ed]|g[reen] - flash LEDs\n"
"l[eds] d[isable] BOARD - stop blinking/flashing LEDs\n"
"BOARD is 1-num connected boards, or a[ll]\n"
"INDEX is 1-8 and corresponds to a button\n"
          ;
    }

    /**
     * Run the command string.  See get_help() for what constitutes a command
     * string.  Fill response (must be at least RESPONSE_SIZE bytes) with the
     * response string.  current_millis is the current value of millis()
     * typecast to unsigned int.  Return whether the command succeeded.
     */
    bool process_command(const char* command, char* response, unsigned int current_millis);

    /**
     * Update the state of all boards.  Should be called often.  current_millis
     * is the current value of millis() typecast to unsigned int.
     */
    void update(unsigned int current_millis);

  private:
    // Apologies for this template ugliness.  It makes the command parser much
    // easier to write.
    template <typename R, typename... Args>
    R call_board(ScifiDisplayBoard* board, R (ScifiDisplayBoard::* method)(Args...), Args... args) {
      return (board->*method)(args...);
    }

    template <typename R, typename... Args>
    void each_board(int board, R (ScifiDisplayBoard::* method)(Args...), Args... args) {
      if(board >= 0 && board < num_boards_)
        call_board(boards_[board], method, args...);
      else {
        for(int i = 0; i < num_boards_; ++i)
          call_board(boards_[i], method, args...);
      }
    }

    bool board_ok(int board) const;
    bool board_argv_ok(char board) const;

    int num_boards_;
    ScifiDisplayBoard* boards_[MAX_BOARDS];
};

/**
 * An instantiable collection of ScifiDisplayBoards that you can send commands
 * to.  Specify the number of boards (up to ScifiDisplayBase::MAX_BOARDS, 4) in
 * the template parameter.
 */
template<int NUM_BOARDS>
class ScifiDisplay;

/**
 * A ScifiDisplay<> of 1 TM1638 board connected to the Arduino.
 */
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

/**
 * A ScifiDisplay<> of 2 chained TM1638 boards connected to the Arduino.
 */
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

/**
 * A ScifiDisplay<> of 3 chained TM1638 boards connected to the Arduino.
 */
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

/**
 * A ScifiDisplay<> of 4 chained TM1638 boards connected to the Arduino.
 */
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
