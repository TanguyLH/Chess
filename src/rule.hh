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

    uint64_t generate_rook_attacks(Chessboard board, Color color,
                                   bool is_queen = false);
    void generate_rook_moves(Chessboard board, Color color,
                             std::vector<Move> &res, bool is_queen = false);

    uint64_t generate_bishop_attacks(Chessboard board, Color color,
                                     bool is_queen = false);
    void generate_bishop_moves(Chessboard board, Color color,
                             std::vector<Move> &res, bool is_queen = false);

    uint64_t generate_queen_attacks(Chessboard board, Color color);
    void generate_queen_moves(Chessboard board, Color color,
                             std::vector<Move> &res);

    uint64_t generate_king_attacks(Chessboard board, Color color);
    void generate_king_moves(Chessboard board, Color color,
                             std::vector<Move> &res);
}


