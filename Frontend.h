//
// Created by zhanghuimeng on 19-3-24.
//

#ifndef PLAYER_FRONTEND_H
#define PLAYER_FRONTEND_H

#include <stdlib.h>
#include "ncurses/curses.h"
#include <vector>
#include "Board.h"
#include "GemCluster.h"
#include <algorithm>

using namespace std;

class Frontend {
public:
    Board board;
    WINDOW* users[3];
    WINDOW* status[3];
    WINDOW* cards[3][4];
    WINDOW* pile[3];
    WINDOW* gems;
    WINDOW* nobles;
    int row, col;

    map<string, short> color2int;

    static const int userHeight, userWidth;
    static const int frameMargin;  // 最靠边的框
    static const int statusHeight, statusWidth;
    static const int cardHeight, cardWidth;

    Frontend(const Board& board): board(board) {
        initscr();
        noecho();
        raw();
        getmaxyx(stdscr, row, col);
        start_color();
        refresh();  // before refreshing windows
        fill(board);

        for (int i = 0; i < GemCluster::colors.size(); i++) {
            color2int[GemCluster::colors[i]] = i + 1;
            if (GemCluster::colors[i] == "red")
                init_pair(i + 1, COLOR_RED, COLOR_RED);
            else if (GemCluster::colors[i] == "blue")
                init_pair(i + 1, COLOR_BLUE, COLOR_BLUE);
        }
    }

    void fill(const Board& board) {
        int left = frameMargin;
        // users
        for (int i = 0; i < 3; i++) {
            users[i] = create_newwin(userHeight, userWidth, frameMargin + i * userHeight, left);
        }
        left += userWidth + frameMargin;
        // status
        for (int i = 0; i < 3; i++) {
            status[i] = create_newwin(statusHeight, statusWidth,
                    frameMargin + i * statusHeight,
                    left, false);
            mvwprintw(status[i], 1, 1, "CARDS\n");
            wprintw(status[i], "P-ed: %d\n", board.players[i].purchased_cards.size());
            wprintw(status[i], "Re-d: %d\n", board.players[i].reserved_cards.size());
            wrefresh(status[i]);
        }
        left += statusWidth + frameMargin;

        // cards
        vector<NormalCard> cardsVec = board.table.cards;
        sort(cardsVec.begin(), cardsVec.end());
        int idx = 0;
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 4; j++) {
                cards[i][j] = create_newwin(cardHeight, cardWidth,
                        frameMargin + i * cardHeight,
                        left + j * cardWidth, true);
                if (idx > cardsVec.size() || cardsVec[idx].level != i + 1) continue;
                wattron(cards[i][j], color2int[cardsVec[idx].color_str]);
                mvwprintw(cards[i][j], 1, 1, cardsVec[idx].color_str.c_str());
                wattroff(cards[i][j], COLOR_PAIR(color2int[cardsVec[idx].color_str]));
                if (cardsVec[idx].score > 0)
                    mvwprintw(cards[i][j], 1, cardWidth - 3,
                              to_string(cardsVec[idx].score).c_str());
                idx++;
                wrefresh(cards[i][j]);
            }
        }

        getch();
    }

    ~Frontend() {
        endwin();
    }

    WINDOW *create_newwin(int height, int width, int starty, int startx, bool do_box=true)
    {	WINDOW *local_win;

        local_win = newwin(height, width, starty, startx);
        if (do_box)
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
};


#endif //PLAYER_FRONTEND_H
