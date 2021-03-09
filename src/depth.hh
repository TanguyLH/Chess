#pragma once

#include "fen-parse.hh"

namespace fen
{
    uint64_t generate_all_moves(board::Chessboard &board, int depth);
} // namespace fen