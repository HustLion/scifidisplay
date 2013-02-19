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

/**
 * An individual TM1638 display board.  We store 8 messages that can be flashed
 * on the display, and the LEDs can be set to flash or blink randomly.  We
 * report new button presses (not the current button state).
 */
class ScifiDisplayBoard {
  public:
    /// Number of buttons, LEDs, and digits on the board.
    static const int NUM_DIGITS = 8;

    /**
     * Parameters are passed to the TM1638 constructor; see it.
     */
    ScifiDisplayBoard(int data_pin, int clock_pin, int strobe_pin);

    /**
     * Set the board's brightness, in the range [0,8].  If you pass 0, the
     * board's lights will be turned off; 8 is maximum brightness.
     */
    void set_brightness(int brightness);

    /**
     * Set the text of the message at the given index, which must be in the
     * range [0,NUM_DIGITS).
     */
    void set_message(int index, const char* text);

    /**
     * Return the text of the message at the given index, or NULL if index not
     * in the range [0,NUM_DIGITS).
     */
    const char* get_message(int index);

    /**
     * Return the currently flashing message index, or -1 if the message is
     * disabled.
     */
    int get_message_index();

    /**
     * Flash the message at the given index on the display.  index must be in
     * the range [0,NUM_DIGITS); the text at that index is defined with
     * set_message().  current_millis is the value of millis() typecast to
     * unsigned int.
     */
    void flash_message(int index, unsigned int current_millis);

    /**
     * Disables the message display.
     */
    void disable_message();

    /**
     * Return whether the LEDs are blinking or flashing, false if disabled.  If
     * blinking_out is non-NULL, fill it with true if blinking, else false.
     */
    bool get_leds_state(bool* blinking_out);

    /**
     * Randomly blink the LEDs, red if green is false.  current_millis is the
     * value of millis() typecast to unsigned int.
     */
    void blink_leds(bool green, unsigned int current_millis);

    /**
     * Flash the LEDs, red if green is false.  current_millis is the value of
     * millis() typecast to unsigned int.
     */
    void flash_leds(bool green, unsigned int current_millis);

    /**
     * Disables the LEDs.
     */
    void disable_leds();

    /**
     * Update the state of the board.  current_millis is the value of millis()
     * typecast to unsigned int.  Return button presses: if the LSB (bit 0) is
     * set, the first button was pressed; if bit 1 is set, the second button;
     * etc.  Must be called often inside loop().
     */
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
