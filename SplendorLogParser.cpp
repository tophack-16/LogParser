//
// Created by zhanghuimeng on 19-3-23.
//

#include "Board.h"
#include "json/json.h"
#include "Move.h"

static void tokenize(const string& str, vector<string>& tokens, const string& delimiters = ",")
{
    // Skip delimiters at beginning.
    string::size_type lastPos = str.find_first_not_of(delimiters, 0);

    // Find first non-delimiter.
    string::size_type pos = str.find_first_of(delimiters, lastPos);

    while (string::npos != pos || string::npos != lastPos) {
        // Found a token, add it to the vector.
        tokens.push_back(str.substr(lastPos, pos - lastPos));

        // Skip delimiters.
        lastPos = str.find_first_not_of(delimiters, pos);

        // Find next non-delimiter.
        pos = str.find_first_of(delimiters, lastPos);
    }
}

static string readJson(const vector<string>& input, int& i) {
    string table_str;
    while (i < input.size() && input[i] != "}") {
        table_str += input[i];
        i++;
    }
    if (i < input.size()) {
        table_str += input[i];
        i++;
    }
    return table_str;
}

static void logParser(const vector<string>& input) {
    Json::Reader reader;
    Json::Value value;
    int i = 0;
    vector<string> players;
    Table table;
    Board board;
    int curRound = -1;
    int highScore = 0;
    string player;
    while (i < input.size()) {
        if (input[i] == "Game starts!") {
            cout << "The game is starting" << endl;
            i++;
            string s = input[i].substr(9, input[i].length() - 9);
            tokenize(s, players, ", ");
            for (string s: players)
                cout << s << endl;
            i += 2;
            // Read start table
            string table_str = readJson(input, i);
//            cout << table_str << endl;
            if (reader.parse(table_str, value))
                table = Table(value);
            cout << "Test table reading: " << table.gems.toString() << endl;
            // Create Board
            board.table = table;
            for (string p: players)
                board.players.emplace_back(p);
            continue;
        }
        // 一轮开始
        if (input[i].substr(0, 6) == "Round ") {
            curRound = stoi(input[i].substr(6, input[i].length() - 6));
            i++;
            highScore = stoi(input[i].substr(15, input[i].length() - 15));
            i++;
            cout << curRound << ' ' << highScore << ' ' << endl;
            continue;
        }
        // "Player player_example's turn"
        if (input[i].substr(0, 7) == "Player " && input[i].substr(input[i].length() - 7, 7) == "'s turn") {
            player = input[i].substr(7, input[i].length() - 7);
            player = player.substr(0, player.length() - 7);
            cout << "Turn: " << player << endl;
            i++;
            // RES1
            if (input[i].size() > 23 &&
                input[i].substr(input[i].length() - 23, 23) == " is reserving the card.") {
                i++;
                // read card
                string card_str = readJson(input, i);
                NormalCard card;
                reader.parse(card_str, value);
                card = NormalCard(value);
                Move move(player, Move::Type::RES1);
                move.card = card;
                cout << "The card to reserve: " << card.toString() << endl;
                board.execute(move);
                i++;  // "A new Card on the table."
                card_str = readJson(input, i);
                reader.parse(card_str, value);
                card = NormalCard(value);
                board.table.cards.push_back(card);
                // TODO: 把黄金加上
                i++;  // "Successfully reserved the card."
                cout << "The card added: " << card.toString() << endl;
                cout << "-------end of turn--------" << endl;
                continue;
            }
            // DIFF_COL
            if (input[i].size() > 33 &&
                input[i].substr(input[i].length() - 33, 33) == " is getting different color gems.") {
                // read colors
                i++;
                // "Successfully got red, green gems."
                string s = input[i].substr(17, input[i].length() - 17);
                s = s.substr(0, s.length() - 6);
                cout << "gems: " << s << endl;
                i++;
                vector<string> colors;
                tokenize(s, colors, ", ");
                GemCluster acqGems(colors);
                Move move(player, Move::Type::DIFF_COL);
                move.acqGem = acqGems;
                board.execute(move);
                cout << "After getting gems: " << board.getPlayerGembyName(player).toString() << endl;
                cout << "-------end of turn--------" << endl;
                continue;
            }
            // BUY_RES
            if (input[i].size() > 33 &&
                input[i].substr(input[i].length() - 33, 33) == " is purchasing the reserved card.") {
                i++;
                // read card
                NormalCard card;
                string card_str = readJson(input, i);
                reader.parse(card_str, value);
                card = NormalCard(value);
                Move move(player, Move::Type::BUY_RES);
            }
        }
    }
}
