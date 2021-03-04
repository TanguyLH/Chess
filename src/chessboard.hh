#pragma once

#include <vector>

#include "color.hh"
#include "piece-type.hh"
#include "position.hh" 

namespace board
{
    class Chessboard
    {
    public:
        Chessboard();
        Chessboard(std::string fen_string);
        /*
        std::vector<Move> generate_legal_moves();
        bool is_move_legal(Move move);
        void do_move(Move move);
        */
        bool is_check();
        bool is_checkmate();
        bool is_draw();
        void print_board(void);
        // std::pair<PieceType, Color> operator[](Position position);
        
        unsigned int turn_;
        bool white_turn_;
        bool white_king_castling_;
        bool white_queen_castling_;
        bool black_king_castling_;
        bool black_queen_castling_;
        std::vector<uint64_t> boards_;
    };
}
