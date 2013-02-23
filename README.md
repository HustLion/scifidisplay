ScifiDisplay
============

ScifiDisplay is an Arduino library for making sci-fi movie blinking displays
out of some [TM1638][TM1638] boards.

![ScifiDisplay in Action](https://raw.github.com/chazomaticus/scifidisplay/master/scifi-display.gif)

<https://github.com/chazomaticus/scifidisplay>

Functions
---------

* Flash custom messages on the 7-segment display
* Flash or randomly blink the LEDs, red or green
* Pressing buttons will switch or disable the flashing message
* Control up to four TM1638 boards simultaneously or individually
* String-based command interface for control over serial (or HTTP, etc.)
* Brightness control to suit your movie's lighting needs

Command Interface
-----------------

Commands are passed as strings to `ScifiDisplay<>::process_command()`.

You can get concise command interface help at run-time with
`ScifiDisplay<>::get_help()`.

See
[ScifiDisplay.h](https://raw.github.com/chazomaticus/scifidisplay/master/ScifiDisplay.h)
for the help text and how to call `process_command()`.  I won't bother
repeating the docs here; it's pretty simple.  Some example commands are below.

Example
-------

See the example program at
[scifi_display_example.pde](https://raw.github.com/chazomaticus/scifidisplay/master/examples/scifi_display_example/scifi_display_example.pde)
for how to hook the command interface up to the serial port (and even add the
help command so you don't have to close the serial monitor to see the docs).

To see it in action, hook up two boards as shown in the following picture:

TODO: pic of the example circuit

Fire up the example program in the Arduino IDE, upload it to the device, and
then break out the serial console (make sure the dropdowns are set to Newline
and 9600 baud).

Try some of these commands:
* `brightness all 0` - turn off all boards
* You can abbreviate commands: `b a 4` - turn all boards back on, to half
  brightness
* `leds flash 1 green` (or `l f 1 g`) - flash the green LEDs on board 1
* `leds blink 2 red` (or `l b 2 r`) - set board 2's LEDs to blink red
* `message set 1 8 run away` (or `m s 1 8 run away`) - set board 1's message
  slot 8 to "run away" (press button 8 or execute the next command to flash it)
* `message flash 1 8` (etc.) - flash the message in slot 8 on board 1 (press
  button 8 again to turn off the message flashing)

Notes
-----

There are [a couple different varieties of TM1638](http://dx.com/s/TM1638)
board.  [This][TM1638] is the one I got.  You may also be able to find them
from other websites.  The full part number is JY-LKM1638.

See [this tronixstuff
post](https://tronixstuff.wordpress.com/2012/03/11/arduino-and-tm1638-led-display-modules/)
for a great tutorial on how to use TM1638 boards with Arduino.

ScifiDisplay depends on Ricardo Batista's excellent [TM1638
library][TM1638 library].

To install, put this code in a subdirectory of your Arduino sketchbook's
libraries directory.  See [the docs](http://arduino.cc/en/Guide/Libraries) for
details.  Make sure you have the [TM1638 library][TM1638 library] installed
too.


Enjoy!


[TM1638]: http://dx.com/p/81873
[TM1638 library]: http://code.google.com/p/tm1638-library/
