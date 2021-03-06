#include <iostream>

#include "chessboard.hh"
#include "generate_move.hh"

void print_board(uint64_t board)
{
    int count = 1;
    for (uint64_t n = 1L << 63; n > 0 ; n >>= 1)
    {
        std::cout << " | ";
        if (board & n)
            std::cout << "X";
        else
            std::cout << " ";
        if (!(count % 8))
        {
            std::cout << " | ";
            std::cout << std::endl;
        }
        count++;
    }
    std::cout << std::endl;
}


int main(void)
{
    std::vector<uint64_t> boards;
    // WHITE QUEEN e8
    boards.push_back(576460752303423488ULL);
    // ROOK h4 & d1
    boards.push_back(16777232ULL);
    // BISHOP b2
    boards.push_back(64ULL | 512ULL);
    // KNIGHT NONE
    boards.push_back(0ULL);
    // PAWN NONE
    boards.push_back(0ULL);
    // KING NONE
    boards.push_back(1ULL);


    // BLACK QUEEN NONE
    boards.push_back(0ULL);
    // ROOK NONE
    boards.push_back(0ULL);
    // BISHOP c6
    boards.push_back(35184372088832ULL);
    // KNIGHT d3
    boards.push_back(1048576ULL);
    // PAWN e6 & g4
    boards.push_back(8796093022208ULL | 33554432ULL);
    // KING e4
    boards.push_back(134217728ULL);
    board::Chessboard cb(boards);
    std::cout << "Created board ..." << std::endl;
    
    cb.print_board();
    std::cout << "PINNED PIECES : \n";
    print_board(cb.pins_);
    std::cout << "white king is check ?\n";
    std::string phrase = board::check(cb, board::Color::WHITE) ? "true" : "false";
    std::cout << phrase << std::endl;;
    std::cout << "black king is check ?\n";
    phrase = board::check(cb, board::Color::BLACK) ? "true" : "false";
    std::cout << phrase << std::endl;;
    std::cout << "ended !" << std::endl;
    return 0;
}

