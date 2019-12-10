#include <ncurses.h>
#include <string>
#include <sstream>
#include <vector>

struct tuple
{
    tuple() : a(0.0), b(0.0) {} 
    double a;
    double b;
};

struct state
{
    bool running;
    double ref;
    double cur;
    int bpm;
    double tempo_ms;
    int bar;
    int unit;
    int current_beat;
    tuple taps;
};


namespace curs
{

    namespace // private
    {    
        std::vector<std::string> controls;

        void set_controls()
        {
            controls.push_back("         t - Tap tempo                      ");
            controls.push_back("     Space - Start/Stop metronome           ");
            controls.push_back(" PgUp/PgDn - Increase/Decrease beat value   ");
            controls.push_back("Left/Right - Increase/Decrease beats per bar");
            controls.push_back("   Up/Down - Increase/Decrease Tempo        ");
            controls.push_back("         q - quit                           ");             
        } 
    }

static void setup()
{
    initscr();
    raw();
    keypad(stdscr, TRUE);
    noecho();
    nodelay(stdscr, TRUE);
    set_controls();
}

static void teardown()
{
    endwin();
}

void print_bpm(int y, int x, int bpm)
{
    mvprintw(y, x, std::to_string(bpm).c_str());
}

void print_meter(int y, int x, int bar, int unit)
{
    std::stringstream meter;
    meter << std::to_string(unit) << "/" << std::to_string(bar);
    mvprintw(y, x, meter.str().c_str());
}

void print_controls(int max_y, int max_x, std::vector<std::string> controls)
{
    for (unsigned int i = 0; i < controls.size(); i++)
    {
        mvprintw(max_y - (i+1), 0, controls[i].c_str());
    }
}

void char_line(WINDOW* win, int y, int x, int n_chars, char character)
{
    mvwaddch(win, y, x, character);
    for (int i = 0; i < n_chars; i++)
    {
        addch(character);
    }
}

void print_beat_view(int bar, int beat)
{
    if (beat > bar) return;

    int start_x, end_x;
    int max_y, max_x;
    getmaxyx(stdscr, max_y, max_x);

    // clear the segment area
    for (int y = 3; y < (max_y - controls.size()); y++)
    {
        mvaddch(y, 0, ' ');
        for (int x = 0; x < max_x; x++)
        {
            addch(' ');
        }
    }
    if (bar % 3 == 0)
    {
        int seg = beat % 3;
        if (seg == 0)
        {
            start_x = 0;
            end_x = max_x / 3;
        }
        else if (seg == 1)
        {
            start_x = max_x / 3;
            end_x = start_x * 2;
        }
        else
        {
            start_x = (max_x / 3) * 2;
            end_x = max_x;
        }
    }
    else
    {
        if (beat % 2 != 0)
        {
            start_x = max_x / 2;
            end_x = max_x;
        }
        else
        {
            start_x = 0;
            end_x = max_x / 2;
        }
    }

    for (int y = 3; y < max_y - controls.size(); y++)
    {
        mvaddch(y, start_x, ACS_CKBOARD);
        for (int x = start_x + 1; x < end_x; x++)
        {
            addch(ACS_CKBOARD);
        }
    }
    
    move(0,0);
    refresh();
}

void draw(struct state *s)
{
    int y, x;
    getmaxyx(stdscr, y, x);
    clear();
    print_bpm(0, x / 2, s->bpm);
    print_meter(1, x / 2, s->unit, s->bar);
    print_controls(y, x, controls);
    move(0, 0);
    wrefresh(stdscr);
    refresh();
}

}
