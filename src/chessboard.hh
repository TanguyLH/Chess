#pragma once

#include <iostream>
#include <vector>

#include "color.hh"
#include "move.hh"
#include "pgn-move.hh"
#include "rule.hh"

namespace board
{
    class GameTracer;

    class Chessboard
    {
    public:
        Chessboard();
        Chessboard(std::string fen_string);
        Chessboard(std::vector<uint64_t> boards);
        std::vector<Move> generate_legal_moves();
        bool is_move_legal(Move move);
        void do_move(Move &move);

        bool is_check();
        bool is_checkmate();
        bool is_draw();
        void print_board(void);
        bool is_check_compatible(Move move, uint64_t piece);
        Move move_from_pgn(PgnMove pgnmove);
        // std::pair<PieceType, Color> operator[](Position position);

        unsigned int turn_;
        unsigned int last_turn_;
        bool white_turn_;
        bool white_king_castling_;
        bool white_queen_castling_;
        bool black_king_castling_;
        bool black_queen_castling_;
        bool in_check = false;
        std::vector<uint64_t> boards_;
        uint64_t pins_;
        uint64_t en_passant;
    };
} // namespace board
