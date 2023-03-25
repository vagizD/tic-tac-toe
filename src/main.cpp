#include <boost/dll/import.hpp>
#include <boost/version.hpp>

#if BOOST_VERSION >= 107600
// NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
#define boost_dll_import_symbol ::boost::dll::import_symbol
#else
// NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
#define boost_dll_import_symbol ::boost::dll::import
#endif

#include <iostream>
#include <string>
#include "game.hpp"
#include "view.hpp"

boost::shared_ptr<tictactoe::View> get_view(std::string &lib) {
    return boost_dll_import_symbol<tictactoe::View>(
        lib, "view", boost::dll::load_mode::append_decorations
    );
}

void process_game(std::string lib) {  // NOLINT
    tictactoe::Game game;
    boost::shared_ptr<tictactoe::View> view = get_view(lib);

    view->start_view(game);

    std::string line;

    while (std::getline(std::cin, line) && !std::cin.eof()) {
        if (line.empty() || game.game_finished()) {
            break;
        }
        if (line.substr(0, 5) == "view-") {
            lib = line;
            view = get_view(lib);
            view->continue_view(game);
            continue;
        }
        if (view->process_game(game, line) == -1) {
            break;
        }
    }
}

// NOLINTBEGIN
int main(int argc, char *argv[]) {
    if (argc != 2) {
        return 1;
    } else {
        process_game(std::string(argv[1]));
    }
}

// NOLINTEND
