#include <algorithm>
#include <boost/config.hpp>
#include <iostream>
#include <string>
#include <vector>
#include "view.hpp"

namespace tictactoe {
struct ViewCompact : public View {
    void print_board(const Game &game) const final {
        auto board = game.board();
        auto size = game.size();

        std::string line;
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                switch (board[i][j]) {
                    case OptionalPlayer::TOE:  // .
                        line += '.';
                        break;
                    case OptionalPlayer::TAC:  // O
                        line += 'O';
                        break;
                    case OptionalPlayer::TIC:  // X
                        line += 'X';
                        break;
                }
            }
            std::cout << line << '\n';
            line.clear();
        }
    }

    static int
    split(std::string &txt, std::vector<std::string> &strs, char ch) {
        txt.erase(std::remove(txt.begin(), txt.end(), '\t'), txt.end());
        size_t initialPos = 0;
        while (initialPos < txt.length() && txt[initialPos] == ' ') {
            initialPos++;
        }

        size_t pos = txt.find(ch, initialPos);
        strs.clear();

        while (pos != std::string::npos) {  // Decompose statement
            if (initialPos == txt.length()) {
                return static_cast<int>(strs.size());
            }
            strs.push_back(txt.substr(initialPos, pos - initialPos));
            initialPos = pos + 1;

            while (initialPos < txt.length() && txt[initialPos] == ' ') {
                initialPos++;
            }

            pos = txt.find(ch, initialPos);
        }

        if (txt.substr(
                   initialPos, std::min(pos, txt.size()) - initialPos + 1
            )  // Add the last one
                .find_first_not_of(' ') != std::string::npos) {
            strs.push_back(txt.substr(
                initialPos, std::min(pos, txt.size()) - initialPos + 1
            ));
        }

        return static_cast<int>(strs.size());
    }

    void print_move(const tictactoe::Game &game) const final {
        switch (game.current_player()) {
            case tictactoe::OptionalPlayer::TAC:
                std::cout << "O move: ";
                break;
            case tictactoe::OptionalPlayer::TIC:
                std::cout << "X move: ";
            case tictactoe::OptionalPlayer::TOE:
                break;
        }
    }

    static bool is_number(const std::string &s) {
        int pos = (s[0] == '-') ? 1 : 0;
        return !s.empty() &&
               std::find_if(s.begin() + pos, s.end(), [](unsigned char c) {
                   return !static_cast<bool>(std::isdigit(c));
               }) == s.end();
    }

    void start_view(const tictactoe::Game &game) const final {
        std::cout << '\n';
        continue_view(game);
    }

    void continue_view(const tictactoe::Game &game) const final {
        print_board(game);
        print_move(game);
    }

    int process_game(Game &game, std::string &line) const final {
        std::vector<std::string> parsed_args;

        int n_args = split(line, parsed_args, ' ');

        if (n_args != 2 || !is_number(parsed_args[0]) ||
            !is_number(parsed_args[1])) {  // handle spaces
            std::cout << "Bad move!\n";
            print_move(game);
            return 1;
        }

        int row = std::stoi(parsed_args[0]);
        int column = std::stoi(parsed_args[1]);

        if (row == -1 && column == -1) {  // exit
            return -1;
        }

        int result = game.make_move(row, column);

        if (result == 0 || result == -1) {  // handle result
            std::cout << "Bad move!\n";
            print_move(game);
            return 1;
        } else if (result == 1) {
            if (line.substr(0, 5) != "view-") {
                std::cout << '\n';
            }
            print_board(game);
            print_move(game);
            return 0;
        } else {  // result == 99
            if (line.substr(0, 5) != "view-") {
                std::cout << '\n';
            }
            print_board(game);
            if (game.current_player() == tictactoe::OptionalPlayer::TIC) {
                std::cout << "X wins!\n";
            } else if (game.current_player() == tictactoe::OptionalPlayer::TAC) {
                std::cout << "O wins!\n";
            } else {
                std::cout << "Draw.\n";
            }
            return -1;
        }
    }
};
}  // namespace tictactoe

// NOLINTBEGIN
extern "C" [[maybe_unused]] BOOST_SYMBOL_EXPORT tictactoe::ViewCompact view;
[[maybe_unused]] tictactoe::ViewCompact view;
// NOLINTEND
