#include "fen-parse.hh"
#include <cctype>
#include <string>
#include "chessboard.hh"

namespace fen
{

    static void set_en_passant(board::Chessboard &board, std::string fen)
    {
        size_t i = 0;
        if (fen[i] != '-')
        {
            int file = fen[i] - 'a';
            int rank = fen[i + 1];
            board.en_passant |= 1ULL << (rank * 8 + file);
        }
    }

    static void set_flags(board::Chessboard &board, std::string fen)
    {
        std::cout << '\'' << fen << '\'' << std::endl;
        size_t i = 0;
        board.white_turn_ = (fen[i] == 'w') ? true : false;
        i += 2;
        if (fen[i] == '-')
        {
            board.white_king_castling_ = false;
            board.white_queen_castling_ = false;
            i += 2;
        } 
        else
        {
            int count = 0;
            if (fen.find('q') == std::string::npos)
            {
                board.black_king_castling_ = false;
                ++count;
            }
            if (fen.find('k') == std::string::npos)
            {
                board.black_queen_castling_ = false;
                ++count;
            }
            if (fen.find('Q') == std::string::npos)
            {
                board.white_queen_castling_ = false;
                ++count;
            }
            if (fen.find('K') == std::string::npos)
            {
                board.white_king_castling_ = false;
                ++count;
            }
            i += count + 1;
        }
        std::string new_fen = fen.substr(i, fen.length() - i);
        set_en_passant(board, new_fen);
    }

    /*
    static int bits_set(uint64_t i)
    {
        i = i - ((i >> 1) & 0x55555555);
        i = (i & 0x33333333) + ((i >> 2) & 0x33333333);
        return (((i + (i >> 4)) & 0x0F0F0F0F) * 0x01010101) >> 24;
    }
    */

    board::Chessboard parse_fen_file(const std::string &file)
    {
        std::vector<uint64_t> boards;
        for (auto &i : boards)
            i = 0ULL;
        std::ifstream os(file);
        std::string fen;
        std::getline(os, fen);
        size_t len = 0;
        for (; len < fen.length() && fen[len] != ' '; len++)
            ;
        size_t i = 0;
        for (uint64_t n = (1ULL << 63ULL); n > 0ULL && i < len; n >>= 1)
        {
            char c = fen[i];
            if (std::isdigit(c))
            {
                n >>= c - '0';
                continue;
            }
            bool b = std::islower(c);
            if (c == 'p' || c == 'P')
                boards[4 + b * 6] |= n;
            if (c == 'n' || c == 'N')
                boards[3 + b * 6] |= n;
            if (c == 'r' || c == 'R')
                boards[1 + b * 6] |= n;
            if (c == 'b' || c == 'B')
                boards[2 + b * 6] |= n;
            if (c == 'q' || c == 'Q')
                boards[b * 6] |= n;
            if (c == 'k' || c == 'K')
                boards[5 + b * 6] |= n;
            i++;
        }
        board::Chessboard res(boards);
        i++;
        std::string new_fen = fen.substr(i, fen.length() - i);
        set_flags(res, new_fen);

        return res; 
    }
}
