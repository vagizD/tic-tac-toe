#ifndef TIC_TAC_TOE_VIEW_HPP
#define TIC_TAC_TOE_VIEW_HPP

#include <boost/config.hpp>
#include <string>
#include "game.hpp"

namespace tictactoe {
struct BOOST_SYMBOL_VISIBLE View {
    virtual void print_board(const Game &) const = 0;
    virtual int process_game(Game &, std::string &) const = 0;
    virtual void print_move(const Game &) const = 0;
    virtual void start_view(const Game &) const = 0;
    virtual void continue_view(const Game &) const = 0;

    View(const View &) = delete;
    View(View &&) = delete;

    View &operator=(const View &) = delete;
    View &operator=(View &&) = delete;

    View() = default;
    virtual ~View() = default;
};
}  // namespace tictactoe

#endif  // TIC_TAC_TOE_VIEW_HPP