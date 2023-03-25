#include <algorithm>
#include <boost/config.hpp>
#include <iostream>
#include <string>
#include "view.hpp"

namespace tictactoe {
struct ViewPretty : public View {
    void print_board(const tictactoe::Game &game) const final {  // NOLINT
        auto board = game.board();
        auto size = game.size();

        std::string line;
        int counter = size - 1;

        for (int row = 0; row < 3 * size + 1; row++) {
            line.clear();
            if (row % 3 == 0) {
                line = "  ";
                for (int col = 0; col < 5 * size + 1; col++) {
                    if (col % 5 == 0) {
                        line += '+';
                    } else {
                        line += "-";
                    }
                }
            } else if (row % 3 == 1) {
                line = std::to_string(counter) + " ";
                --counter;

                int index = (row - 1) / 3;
                for (int col = 0; col < size; col++) {
                    if (board[index][col] == OptionalPlayer::TIC) {
                        line += "| \\/ ";
                    } else if (board[index][col] == OptionalPlayer::TAC) {
                        line += "| @@ ";
                    } else {
                        line += "|    ";
                    }
                }
                line += "|";
            } else {  // row % 3 == 2
                line = "  ";

                for (int col = 0; col < size; col++) {
                    int index = (row - 2) / 3;
                    if (board[index][col] == OptionalPlayer::TIC) {
                        line += "| /\\ ";
                    } else if (board[index][col] == OptionalPlayer::TAC) {
                        line += "| @@ ";
                    } else {
                        line += "|    ";
                    }
                }
                line += "|";
            }
            std::cout << line << '\n';
        }

        std::string sp = "    ";
        line.clear();
        for (int c = 0; c < size; c++) {
            line += sp + static_cast<char>(c + 97);
        }
        std::cout << line << "   \n";
    };

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

        if (n_args != 1 || parsed_args[0].length() != 2) {
            std::cout << "Bad move!\n";
            print_move(game);
            return 1;
        } else {
            int row =
                game.size() - 1 - static_cast<int>(parsed_args[0][1] - '0');
            int column = static_cast<int>(parsed_args[0][0]) - 97;

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
    }
};
}  // namespace tictactoe

// NOLINTBEGIN
extern "C" [[maybe_unused]] BOOST_SYMBOL_EXPORT tictactoe::ViewPretty view;
[[maybe_unused]] tictactoe::ViewPretty view;
// NOLINTEND