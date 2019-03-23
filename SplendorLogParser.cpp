//
// Created by zhanghuimeng on 19-3-23.
//

#include <fstream>
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
    Json::Value statsVal;  // 统计数据
    int i = 0;
    vector<string> players;
    int num_players;
    Table table;
    Board board;
    int curRound = -1;
    int curTurn = -1;
    int highScore = 0;
    string player;
    bool ended = false;
    while (i < input.size() && !ended) {
        if (input[i] == "Game starts!") {
            cout << "The game is starting" << endl;
            i++;
            string s = input[i].substr(9, input[i].length() - 9);
            tokenize(s, players, ", ");
            for (string s: players)
                cout << s << endl;
            num_players = players.size();
            i += 2;
            // Read start table
            string table_str = readJson(input, i);
            cout << table_str << endl;
            reader.parse(table_str, value);
            table = Table(value);
            cout << "Test table reading: " << table.gems.toString() << ' ' << table.cards.size() << endl;
            // Create Board
            board.table = table;
            for (string p: players)
                board.players.emplace_back(p);
            ofstream fout("temp_board.json");
            Json::StyledStreamWriter writer;
            writer.write(fout, board.toJson());
            fout.close();
            continue;
        }
        // 一轮开始
        if (input[i].substr(0, 6) == "Round ") {
            if (curRound != -1)
                cout << "Round " << curRound << " end" << endl;
            curRound = stoi(input[i].substr(6, input[i].length() - 6));
            curTurn = 1;
            i++;
            highScore = stoi(input[i].substr(15, input[i].length() - 15));
            i++;
            cout << "========= Round " << curRound << " begin ========" << endl;
            continue;
        }
        // "Player player_example's turn"
        if (input[i].substr(0, 7) == "Player " && input[i].substr(input[i].length() - 7, 7) == "'s turn") {
            player = input[i].substr(7, input[i].length() - 7);
            player = player.substr(0, player.length() - 7);
            // write to log
            board.player_name = player;
            ofstream fout("output/board_round_" + to_string(curRound) +
                "_turn_" + to_string(curTurn) + ".json");
            Json::StyledStreamWriter writer;
            writer.write(fout, board.toJson());
            fout.close();

            cout << "Turn " << curTurn << ' ' << player << endl;
            i++;
            curTurn++;
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
                move.card = card;
                board.execute(move);
                i++;  // "Successfully purchased the reserved card."
                cout << "After buying card: " << board.getPlayerGembyName(player).toString() << endl;
                cout << "-------end of turn--------" << endl;
                continue;
            }
        }
        if (input[i] == "Final status of players.") {
            cout << "The contest has ended." << endl;
            i++;
            Json::Value playersStatList = Json::arrayValue;
            for (int j = 0; j < players.size(); j++) {
                string player_str = readJson(input, i);
                reader.parse(player, value);
                playersStatList.append(value);
            }
            statsVal["playerFinalStats"] = playersStatList;
            continue;
        }
        if (input[i] == "Final results.") {
            i++;
            // 输出统计信息
            ofstream fout("output/result.json");
            for (int j = 0; j < board.players.size(); j++)
                cout << "Player " << board.players[j].name << ": " << board.players[j].score << endl;
            ended = true;
            // player_example_3 got 16 score and purchased 21 cards
            // player_example_1 got 15 score and purchased 20 cards
            // player_example_2 got 11 score and purchased 17 cards
            statsVal["numPlayers"] = num_players;
            statsVal["round"] = curRound;
            Json::Value playersVal = Json::arrayValue;
            while (i < input.size() &&
                    input[i].find(" score and purchased ") != std::string::npos &&
                    input[i].find(" cards") != std::string::npos) {
                string::size_type pos1 = input[i].find(" got ");
                string::size_type pos2 = input[i].find(" score and purchased ");
                string::size_type pos3 = input[i].find(" cards");
                Json::Value playerVal;
                playerVal["name"] = input[i].substr(0, pos1);
                playerVal["score"] = stoi(input[i].substr(pos1 + 5, pos2 - pos1 - 5));
                playerVal["cards"] = stoi(input[i].substr(pos2 + 21, pos3 - pos2 - 21));
                playersVal.append(playerVal);
                i++;
            }
            statsVal["players"] = playersVal;
            Json::StyledStreamWriter writer;
            writer.write(fout, statsVal);
            fout.close();
            break;
        }
    }
}
