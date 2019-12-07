# Metronome

A dead-simple ncurses metronome.

build: `g++ -lncurses -o metronome main.cpp`

run: `metronome $TEMPO $BEATS_PER_BAR $BEAT_VALUE` e.g. `./metronome 120 4 4`

Note: The sound files are hard coded defines, change these if you don't have `hydrogen` installed.
