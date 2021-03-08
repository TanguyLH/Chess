#pragma once

#include <dlfcn.h>
#include <memory>
#include <algorithm>

#include "chessboard.hh"
#include "listener.hh"

namespace board
{
    class Chessboard;
    class GameTracer
    {
    public:
        GameTracer(std::string pgn_filepath,
                   std::vector<std::string> listener_paths);
        ~GameTracer();

        void play_game();

        Chessboard chessboard_;
        std::vector<listener::Listener *> listeners_;
        std::vector<void *> libs_;
        std::vector<PgnMove> pgnmoves_;
    };
}
