#include <iostream>

#include "chessboard.hh"

namespace board
{
    Chessboard::Chessboard()
    {
        this->turn_ = 0;
        this->white_turn_ = true;
        this->white_king_castling_ = false;
        this->white_queen_castling_ = false;
        this->black_king_castling_ = false;
        /*
        for (uint64_t n = 1 << 14; n > 0; n >>= 1)
            this->boards_.push_back(n);
        */
        uint64_t i = 1;
        i <<= 51;
        uint64_t n = 1;
        n <<= 63;
        for (; n > i; n >>= 1)
            this->boards_.push_back(n);
        /*
        this->black_queen_castling_ = false;
        this->boards_.push_back(1 << 63);
        this->boards_.push_back(1 << 62);
        this->boards_.push_back(1 << 61);
        this->boards_.push_back(1 << 60);
        this->boards_.push_back(1 << 59);
        this->boards_.push_back(1 << 58);
        this->boards_.push_back(1);
        this->boards_.push_back(2);
        this->boards_.push_back(4);
        this->boards_.push_back(8);
        this->boards_.push_back(16);
        this->boards_.push_back(32);
        */
    }

    void Chessboard::print_board()
    {
        int count = 1;
        auto b = this->boards_;
        uint64_t n = 1;
        n <<= 63;
        for (; n > 0 ; n >>= 1)
        {
            int i = 0;
            std::string color = "\x1B[0m";
            std::cout << " | ";
            while (i < 12)
            {
                if (i == 6)
                    color = "\x1B[197m";
                if (b[i] & n)
                {
                    if (i == 0 || i == 6)
                        std::cout << color << "Q" << "\x1B[0m";
                    else if (i == 1 || i == 7)
                        std::cout << color << "R" << "\x1B[0m";
                    else if (i == 2 || i == 8)
                        std::cout << color << "B" << "\x1B[0m";
                    else if (i == 3 || i == 9)
                        std::cout << color << "N" << "\x1B[0m";
                    else if (i == 4 || i == 10)
                        std::cout << color << "i" << "\x1B[0m";
                    else if (i == 5 || i == 11)
                        std::cout << color << "K" << "\x1B[0m";
                    break;
                }
                i++;
            }
            if (i == 12)
                std::cout << " ";
            if (!(count % 8))
                std::cout << std::endl;
            count++;
            // std::cout << " | ";
        }
        std::cout << std::endl;
    }
}

