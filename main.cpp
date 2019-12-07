#include <iostream>
#include <ctime>
#include <string>
#include <sstream>
#include <ncurses.h>

#define ACCENT "/usr/share/hydrogen/data/drumkits/GMRockKit/HatClosed-Hard.wav"
#define NORMAL "/usr/share/hydrogen/data/drumkits/GMRockKit/HatClosed-Soft.wav"

double get_ms()
{
    return (((double)std::clock()) / CLOCKS_PER_SEC) * 1000;
}

double get_tempo_ms(int bpm)
{
    return 60000 / bpm;
}

void beep(std::string filename)
{
    std::stringstream command;
    command << "aplay " << filename << " > /dev/null 2>&1 &";
    system(command.str().c_str());
    flash();
}

void setup_ncurses()
{
    initscr();
    raw();
    keypad(stdscr, TRUE);
    noecho();
    nodelay(stdscr, true);
}

void stop_ncurses()
{
    endwin();
}

void print_bpm(int bpm, int y, int x)
{
    mvprintw(0, x / 2, std::to_string(bpm).c_str());
}

void print_meter(int bar, int beats, int y, int x)
{
    std::stringstream meter;
    meter << std::to_string(beats) << "/" << std::to_string(bar);
    mvprintw(1, x / 2, meter.str().c_str()); 
}

void print_controls(int y, int x)
{
    std::string controls[3] {
        "Left/Right - Increase/Decrease beats per bar",
        "   Up/Down - Increase/Decrease Tempo",
        "         q - quit"
    };
    for (int i = 0; i < 3; i++)
    {
        mvprintw(y - (i+1), 0, controls[i].c_str());
    }
}

void draw(int bpm, int beats, int bar)
{
    int y, x;
    getmaxyx(stdscr, y, x);
    clear();
    print_bpm(bpm, y, x);
    print_meter(beats, bar, y, x);
    print_controls(y, x);
    wrefresh(stdscr);
}

int main(int argc, char* argv[])
{
    // TODO: Handle command line args better
    if (argc < 4) return 1;
    if (argc > 1 && argc != 4) return 1;

    setup_ncurses();

    double ref = get_ms();
    int tempo_bpm = atoi(argv[1]);
    double tempo_ms = get_tempo_ms(tempo_bpm);
    int bar = atoi(argv[2]);
    int beat = atoi(argv[3]);
    int current_beat = 1;
    int ch = 0;
   
    beep(ACCENT);   // We beep once to begin with so we don't have to wait a whole beat
    draw(tempo_bpm, beat, bar); 

    while ((ch = getch()) != 'q')
    {
        if (ch == KEY_UP)
        {
            tempo_bpm++;
            tempo_ms = get_tempo_ms(tempo_bpm);
        }
        if (ch == KEY_DOWN)
        {
            tempo_bpm--;
            tempo_ms = get_tempo_ms(tempo_bpm);
        }
        if (ch == KEY_LEFT)
        {
            bar--;
            current_beat = 0;
        }
        if (ch == KEY_RIGHT)
        {
            bar++;
            current_beat = 0;
        }
        // If a key was pressed, refresh the screen
        if (ch != ERR )
        {
            draw(tempo_bpm, beat, bar);
        }

        double cur = get_ms(); 
        if (cur - ref >= tempo_ms)
        {
            //std::cout << "current: " << cur << " reference: " << ref << " tempo: " << tempo_ms << std::endl;
            beep(current_beat == 0 ? ACCENT : NORMAL);
            ref = get_ms();
            if (current_beat == bar - 1) current_beat = 0;
            else current_beat++;
        }
    }

    stop_ncurses();
    return 0;
}
