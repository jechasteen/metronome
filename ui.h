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
}

}
