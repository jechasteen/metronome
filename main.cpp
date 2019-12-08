#include <iostream>
#include <ctime>
#include <string>
#include <sstream>
#include <sys/stat.h>

#include "ui.h"

#define ACCENT "/usr/share/hydrogen/data/drumkits/GMRockKit/HatClosed-Hard.wav"
#define NORMAL "/usr/share/hydrogen/data/drumkits/GMRockKit/HatClosed-Soft.wav"

double time_ms()
{
    return ( ((double)std::clock() / CLOCKS_PER_SEC) * 1000 );
}

double tempo_ms(int bpm)
{
    return (60000 / bpm); 
}

void play(std::string filename)
{
    std::string default_sound = "beep.wav";
    std::stringstream command;
    struct stat buf;
    if (stat(filename.c_str(), &buf) == 0)
    {
        command << "aplay " << filename << " > /dev/null 2>&1 &";
    }
    else
    {
        command << "aplay " << default_sound << " > /dev/null 2>&1 &";
    }
    system(command.str().c_str());
}

std::string usage()
{
    return
    "Usage: metronome TEMPO BAR UNIT\n"
    "e.g. 'metronome 120 4 4' or 'metronome' (defaults)\n";
}

// Set state from command line args
state set_state(char* argv[])
{
    state s = {
        .running = false,
        .ref = time_ms(),
        .cur = 0.0,
        .bpm = atoi(argv[1]),
        .tempo_ms = tempo_ms(s.bpm),
        .bar = atoi(argv[2]),
        .unit = atoi(argv[3]),
        .current_beat = 0,
    };
    return s;
}

// set state to defaults;
state set_state()
{
    state s = {
        .running = false,
        .ref = time_ms(),
        .cur = 0.0,
        .bpm = 120,
        .tempo_ms = tempo_ms(s.bpm),
        .bar = 4,
        .unit = 4,
        .current_beat = 0,
    };
    return s;
}

int main(int argc, char* argv[])
{
    state s;
    state original;
    int ch; // keypress

    if ( ((argc < 4) && (argc != 1)) || ((argc > 1) && (argc != 4)) )
    {
        std::cout << usage();
        return 1;
    }

    if (argc == 1)
        s = set_state();
    else if (argc == 4)
        s = set_state(argv);

    original = s;

    curs::setup();
    curs::draw(&s);
    ch = getch();
    
    while ( (ch != 'q') && (ch != 'Q') )
    {
        switch (ch)
        {
        case KEY_UP:
            s.bpm++;
            s.tempo_ms = tempo_ms(s.bpm);
            break;
        case KEY_DOWN:
            s.bpm--;
            s.tempo_ms = tempo_ms(s.bpm);
            break;
        case KEY_LEFT:
            if (s.bar > 1)
            {
                s.bar--;
                s.current_beat = 0;
            }
            break;
        case KEY_RIGHT:
            s.bar++;
            s.current_beat = 0;
            break;
        case KEY_PPAGE:
            if (s.unit < 64)
            {
                s.unit *= 2;
                s.current_beat = 0;
            }
            break;
        case KEY_NPAGE:
            if (s.unit != 1)
            {
                s.unit /= 2;
                s.current_beat = 0;
            }
            break;
        case ' ':
            if (s.running)
                s.running = false;
            else
                s.running = true;
            break;
        }

        // redraw if a key was pressed
        if (ch != ERR)
            curs::draw(&s);

        if (s.running)
        {
            s.cur = time_ms();

            if ( (s.cur - s.ref) >= (s.tempo_ms / (s.unit / 4)) )
            {
                play((s.current_beat == 0) ? ACCENT : NORMAL);
                s.ref = time_ms();
                if (s.current_beat == (s.bar - 1))
                    s.current_beat = 0;
                else
                    s.current_beat++;
            }
        }

        ch = getch();
    }

    curs::teardown();

    return 0;
}
