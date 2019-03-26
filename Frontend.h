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
#include <cassert>
#include <fstream>

using namespace std;

class Frontend {
public:
    int round, turn;
    Board board;

    WINDOW* users[3];
    WINDOW* status[3];
    WINDOW* cards[3][4];
    WINDOW* pile[3];
    WINDOW* gems;
    WINDOW* nobles;
    int row, col;

    short customColorCnt;
    short customPairCnt;
    short bgd, bgdPair;
    map<string, short> gemPairMap;
    map<string, short> costPairMap;

    map<string, int> color2int;

    static const int userHeight, userWidth;
    static const int frameMargin;  // 最靠边的框
    static const int statusHeight, statusWidth;
    static const int cardHeight, cardWidth;

    Frontend() {
        round = 1;
        turn = 1;
        initscr();
        noecho();
        raw();
        keypad(stdscr, TRUE);
        getmaxyx(stdscr, row, col);
        start_color();
        refresh();
        getch();
        initColors();
        refresh();  // before refreshing small windows
    }

    void initColors() {
        customColorCnt = 9;
        customPairCnt = 1;

        bgd = COLOR_MAGENTA;
//        assert(init_color(gray, 500, 500, 500) == OK);
        bgdPair = customPairCnt++;
        assert(init_pair(bgdPair, COLOR_WHITE, COLOR_MAGENTA) == OK);
        assert(bkgd(COLOR_PAIR(bgdPair)) == OK);

        color2int["red"] = COLOR_RED;
        gemPairMap["red"] = customPairCnt++;
        assert(init_pair(gemPairMap["red"], COLOR_RED, COLOR_RED) == OK);
        costPairMap["red"] = customPairCnt++;
        assert(init_pair(costPairMap["red"], bgd, COLOR_RED) == OK);

        color2int["green"] = COLOR_GREEN;
        gemPairMap["green"] = customPairCnt++;
        assert(init_pair(gemPairMap["green"], COLOR_GREEN, COLOR_GREEN) == OK);
        costPairMap["green"] = customPairCnt++;
        assert(init_pair(costPairMap["green"], bgd, COLOR_GREEN) == OK);

        color2int["blue"] = COLOR_BLUE;
        gemPairMap["blue"] = customPairCnt++;
        assert(init_pair(gemPairMap["blue"], COLOR_BLUE, COLOR_BLUE) == OK);
        costPairMap["blue"] = customPairCnt++;
        assert(init_pair(costPairMap["blue"], bgd, COLOR_BLUE) == OK);

        color2int["gold"] = COLOR_YELLOW;
        gemPairMap["gold"] = customPairCnt++;
        assert(init_pair(gemPairMap["gold"], COLOR_YELLOW, COLOR_YELLOW) == OK);
        costPairMap["gold"] = customPairCnt++;
        assert(init_pair(costPairMap["gold"], bgd, COLOR_YELLOW) == OK);

        color2int["white"] = COLOR_WHITE;
        gemPairMap["white"] = customPairCnt++;
        assert(init_pair(gemPairMap["white"], COLOR_WHITE, COLOR_WHITE) == OK);
        costPairMap["white"] = customPairCnt++;
        assert(init_pair(costPairMap["white"], bgd, COLOR_WHITE) == OK);

        color2int["black"] = COLOR_BLACK;
        gemPairMap["black"] = customPairCnt++;
        assert(init_pair(gemPairMap["black"], COLOR_BLACK, COLOR_BLACK) == OK);
        costPairMap["black"] = customPairCnt++;
        assert(init_pair(costPairMap["black"], bgd, COLOR_BLACK) == OK);
    }

    void work() {
        while (true) {
            string filename = "output/board_round_" + to_string(round) + "_turn_" + to_string(turn) + ".json";
            ifstream f(filename);
            if (!f.good()) break;
            string input, line;
            while (getline(f, line)) input += line;
            Json::Reader reader;
            Json::Value value;
            reader.parse(input, value);
            board = Board(value);
            fill(board);
            int ch;
            if ((ch = getch()) == KEY_F(2)) break;
            turn++;
            if (turn == 4) {
                turn = 1;
                round++;
            }
        }
    }

    void fill(const Board& board) {
        int left = frameMargin;
        // users
        for (int i = 0; i < 3; i++) {
            users[i] = create_newwin(userHeight, userWidth, frameMargin + i * userHeight, left);
            assert(board.players.size() >= 3);
            // name
            mvwprintw(users[i], 1, 1, board.players[i].name.c_str());
            // score
            string score = to_string(board.players[i].score);
            while (score.size() < 3) score = "0" + score;
            mvwprintw(users[i], 1, userWidth - 5, score.c_str());
            /*
            // Gems
            mvwprintw(users[i], userHeight - 3, 1, "G ");
            for (string color: GemCluster::colors) {
                wattron(users[i], COLOR_PAIR(costPairMap[color]));
                wprintw(users[i], to_string(board.players[i].gems.gems.at(color)).c_str());
                wattroff(users[i], COLOR_PAIR(costPairMap[color]));
            }
            // Gain
            GemCluster gain = board.players[i].getGain();
            mvwprintw(users[i], userHeight - 2, 1, "B ");
            for (string color: GemCluster::colors) {
                wattron(users[i], COLOR_PAIR(costPairMap[color]));
                wprintw(users[i], to_string(gain.gems[color]).c_str());
                wattroff(users[i], COLOR_PAIR(costPairMap[color]));
            }*/
            // colors
            if (i + 1 == turn) {
                // blinking
                wattron(users[i], A_BLINK);
                box(users[i], 0, 0);
                wattroff(users[i], A_BLINK);
            }
            wrefresh(users[i]);
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
                wrefresh(cards[i][j]);
                if (idx > cardsVec.size() || cardsVec[idx].level != i + 1) continue;
                // color
                wattron(cards[i][j], COLOR_PAIR(gemPairMap[cardsVec[idx].color_str]));
//                mvwprintw(cards[i][j], 1, 1, to_string(color2int[cardsVec[idx].color_str]).c_str());
                mvwprintw(cards[i][j], 1, 1, cardsVec[idx].color_str.c_str());
                wattroff(cards[i][j], COLOR_PAIR(gemPairMap[cardsVec[idx].color_str]));
                // score
                if (cardsVec[idx].score > 0)
                    mvwprintw(cards[i][j], 1, cardWidth - 3,
                              to_string(cardsVec[idx].score).c_str());
                // cost
                int printedGems = 0;
                for (auto iter: cardsVec[idx].costs.gems) {
                    if (iter.second == 0) continue;
                    int color = color2int[iter.first];
                    init_pair(color + 1, color, color);
                    wattron(cards[i][j], COLOR_PAIR(color + 1));
                    mvwprintw(cards[i][j], cardHeight - 3, printedGems + 1, " ");
                    wattroff(cards[i][j], COLOR_PAIR(color + 1));
                    mvwprintw(cards[i][j], cardHeight - 2, printedGems + 1, to_string(iter.second).c_str());
                    printedGems++;
                }
                idx++;
                wrefresh(cards[i][j]);
            }
        }
        left += 3 * cardWidth;

        // pile
        left += frameMargin;
        for (int i = 0; i < 3; i++) {
            pile[i] = create_newwin(cardHeight, cardWidth,
                                    frameMargin + i * cardHeight,
                                    left, false);
            mvwprintw(pile[i], 1, 1, to_string(i + 1).c_str());
        }
        mvprintw(row-3, 1, "Round: ");
        printw((to_string(round) + "\n").c_str());
        mvprintw(row-2, 1, "Turn: ");
        printw((to_string(turn) + "\n").c_str());
        mvprintw(row-1, 1, "Press F1 for next state, F2 to exit");

        refresh();
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
        wbkgd(local_win, COLOR_PAIR(bgdPair));
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
