/* Includes */

#include <ncursesw/ncurses.h>

#include <ctime>
#include <cstdlib>

#include "graphics.hpp"
#include "game.hpp"
#include "storing.hpp"

using namespace std; 

/* Vars */

WINDOW *game_win;
WINDOW *info_win;

/* Methods */

/* Main method */

int main() {
    srand(time(0));

    // start ncurses
    initscr();
    curs_set(0); // cursor invisible
    noecho(); // don't print input
    nodelay(stdscr, true); // don't stop the program on getch()
    create_colors(); // create color pairs

    // delete storing files in case user previously left before game over
    delete_files();
    
    // splash screen
    show_splash_screen();

    // start game 
    create_maps();

    game_win = create_game_window();
    info_win = create_info_window(game_win);

    new_game(game_win, info_win);

    start_game_loop(game_win, info_win);

    return 0;
}
