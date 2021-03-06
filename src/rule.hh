#pragma once

#include <vector>
#include <cmath>
#include "move.hh"
#include "chessboard.hh"

namespace board
{
    std::vector<Move> generate_knight_moves(Chessboard board, Color color);
    uint64_t generate_knight_attacks(Chessboard board, Color color); 
    void print_BitBoard(uint64_t b);
    uint64_t generate_pawn_attacks(Chessboard board, Color color);
    std::vector<Move> generate_pawn_moves(Chessboard board, Color color);     
}

/*std::vector<Move> generate_pawn_moves(Chessboard board);
std::vector<Move> generate_king_moves(Chessboard board);
std::vector<Move> generate_bishop_moves(Chessboard board);
std::vector<Move> generate_rook_moves(Chessboard board);
std::vector<Move> generate_queen_moves(Chessboard board);*/
