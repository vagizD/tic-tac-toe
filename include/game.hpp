#ifndef TIC_TAC_TOE_GAME_HPP
#define TIC_TAC_TOE_GAME_HPP

#include <vector>
#include "game_fwd.hpp"

namespace tictactoe {
struct Game {
    Game(const Game &) = delete;
    Game(Game &&) = delete;
    Game &operator=(const Game &) = delete;
    Game &operator=(Game &&) = delete;

    ~Game() = default;

    explicit Game();

    int make_move(int row, int column);

    void switch_player();

    bool check_game_result(int row, int col);

    [[nodiscard]] std::vector<std::vector<OptionalPlayer>> board() const {
        return m_board;
    }

    [[nodiscard]] int size() const {
        return m_size;
    }

    [[nodiscard]] OptionalPlayer current_player() const {
        return m_current_player;
    }

    [[nodiscard]] bool game_finished() const {
        return m_game_finished;
    }

private:
    std::vector<std::vector<OptionalPlayer>> m_board;
    OptionalPlayer m_current_player{OptionalPlayer::TAC};  // начинают нолики
    int m_size{10};
    int m_cells_left{10 * 10};
    bool m_game_finished{false};
    std::pair<int, int> m_last_move{-1, -1};
};
}  // namespace tictactoe

#endif  // TIC_TAC_TOE_GAME_HPP