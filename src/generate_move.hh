#pragma once

#include <vector>

#include "chessboard.hh"
#include "move.hh"

namespace board
{
    /*
    std::vector<Move> generate_pawn_moves(Chessboard board);
    std::vector<Move> generate_king_moves(Chessboard board);
    std::vector<Move> generate_bishop_moves(Chessboard board);
    std::vector<Move> generate_rook_moves(Chessboard board);
    std::vector<Move> generate_queen_moves(Chessboard board);
    std::vector<Move> generate_knight_moves(Chessboard board);
    */

    uint64_t find_absolute_pins(Chessboard board);

    bool check(Chessboard board, Color c);
} // namespace board
