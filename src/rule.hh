#pragma once

#include <vector>
#include <cmath>
#include "move.hh"
#include "chessboard.hh"

namespace board
{
    class Chessboard;
    void generate_knight_moves(Chessboard board, Color color, std::vector<Move> &res);
    uint64_t generate_knight_attacks(Chessboard board, Color color); 
    void print_BitBoard(uint64_t b);
    uint64_t generate_pawn_attacks(Chessboard board, Color color);
    void generate_pawn_moves(Chessboard board, Color color, std::vector<Move> &res);     
}


