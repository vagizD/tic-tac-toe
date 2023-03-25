#include "game.hpp"

namespace tictactoe {
Game::Game() {
    for (int i = 0; i < m_size; i++) {
        m_board.emplace_back(m_size, OptionalPlayer::TOE);
    }
}

int Game::make_move(int row, int column) {
    if (row < 0 || row > m_size - 1 || column < 0 || column > m_size - 1 ||
        m_game_finished) {
        return -1;  // bad move: wrong coords || game already finished
    }

    if (m_board[row][column] != OptionalPlayer::TOE) {
        return 0;  // bad move: cell occupied already
    }

    m_board[row][column] = m_current_player;

    m_cells_left--;

    bool over =
        check_game_result(row, column);  // 1 - game continues, 99 - game over

    if (over) {
        m_game_finished = true;  // game is over
        return 99;               // game is over
    }

    switch_player();

    return 1;  // move succeeded
}

void Game::switch_player() {
    if (m_current_player == OptionalPlayer::TIC) {
        m_current_player = OptionalPlayer::TAC;
    } else {
        m_current_player = OptionalPlayer::TIC;
    }
}

bool Game::check_game_result(int row, int col) {  // NOLINT
    int d = size() / 2 - 1;  // left-right and up-bottom check

    int colLeft = std::max(0, col - d);
    int colRight = std::min(col + d, m_size - 1);
    int rowUp = std::max(0, row - d);  // 0 at top, m_size-1 at bottom
    int rowBottom = std::min(row + d, m_size - 1);

    int counter = 0;

    for (int c = colLeft; c <= colRight; c++) {  // horizontal
        if (m_board[row][c] == m_current_player) {
            counter++;
            if (counter == d + 1) {
                return m_game_finished = true;
            }
        } else {
            counter = 0;
        }
    }

    counter = 0;
    for (int r = rowUp; r <= rowBottom; r++) {  // vertical
        if (m_board[r][col] == m_current_player) {
            counter++;
            if (counter == d + 1) {
                return m_game_finished = true;
            }
        } else {
            counter = 0;
        }
    }

    counter = 0;
    for (int c = colLeft, r = rowUp; c <= colRight && r <= rowBottom;
         c++, r++) {  // main diagonal
        if (m_board[r][c] == m_current_player) {
            counter++;
            if (counter == d + 1) {
                return m_game_finished = true;
            }
        } else {
            counter = 0;
        }
    }

    counter = 0;
    for (int c = colLeft, r = rowBottom; c <= colRight && r >= rowUp;
         c++, r--) {  // second diagonal
        if (m_board[r][c] == m_current_player) {
            counter++;
            if (counter == d + 1) {
                return m_game_finished = true;
            }
        } else {
            counter = 0;
        }
    }

    if (m_cells_left == 0) {
        m_current_player = OptionalPlayer::TOE;  // means draw
        return m_game_finished = true;
    }

    return m_game_finished;
}
}  // namespace tictactoe