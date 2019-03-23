//
// Created by zhanghuimeng on 19-3-24.
//

#include <stdlib.h>
#include "ncurses/curses.h"

static WINDOW *create_newwin(int height, int width, int starty, int startx);
static void destroy_win(WINDOW *local_win);

static void frontend() {
    // initialize
    initscr();			/* Start curses mode 		  */
    noecho();
    raw();
    int row,col;
    getmaxyx(stdscr,row,col);		/* get the number of rows and columns */

    // stuff
    printw("Hello World !!!\n");	/* Print Hello World		  */
    attron(A_BLINK | A_REVERSE);
    printw("No, thanks");
    attroff(A_BLINK);

    if(has_colors() == FALSE)
    {	endwin();
        printf("Your terminal does not support color\n");
        exit(1);
    }
    start_color();			/* Start color 			*/
    init_pair(1, COLOR_RED, COLOR_GREEN);
    // create a new window
    WINDOW *my_win;
    int height = 10, width = 8;
    int startx = (col - width) / 2, starty = (row - height) / 2;
    refresh();  // 好像必须在它refresh之前进行refresh，否则永远refresh不了了
    my_win = create_newwin(height, width, starty, startx);
    wattron(my_win, COLOR_PAIR(1));
    box(my_win, 0 , 0);
    mvwprintw(my_win, 0, 0, "?");
    wattroff(my_win, COLOR_PAIR(1));

    refresh();			/* Print it on to the real screen */
    wrefresh(my_win);
    refresh();

    getch();

    destroy_win(my_win);

    refresh();

    getch();			/* Wait for user input */
    endwin();			/* End curses mode		  */
}

WINDOW *create_newwin(int height, int width, int starty, int startx)
{	WINDOW *local_win;

    local_win = newwin(height, width, starty, startx);
    box(local_win, 0 , 0);		/* 0, 0 gives default characters
					 * for the vertical and horizontal
					 * lines			*/
    wrefresh(local_win);		/* Show that box 		*/

    return local_win;
}

void destroy_win(WINDOW *local_win)
{
    /* box(local_win, ' ', ' '); : This won't produce the desired
     * result of erasing the window. It will leave it's four corners
     * and so an ugly remnant of window.
     */
    wborder(local_win, ' ', ' ', ' ',' ',' ',' ',' ',' ');
    /* The parameters taken are
     * 1. win: the window on which to operate
     * 2. ls: character to be used for the left side of the window
     * 3. rs: character to be used for the right side of the window
     * 4. ts: character to be used for the top side of the window
     * 5. bs: character to be used for the bottom side of the window
     * 6. tl: character to be used for the top left corner of the window
     * 7. tr: character to be used for the top right corner of the window
     * 8. bl: character to be used for the bottom left corner of the window
     * 9. br: character to be used for the bottom right corner of the window
     */
    wrefresh(local_win);
    delwin(local_win);
}