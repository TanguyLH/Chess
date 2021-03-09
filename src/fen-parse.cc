#include "fen-parse.hh"

#include <cctype>
#include <string>

#include "chessboard.hh"
#include "depth.hh"
#include "move.hh"

namespace fen
{
    static int set_passant_and_flags(board::Chessboard &board, std::string fen)
    {
        size_t i = 0;
        if (fen[i] != '-')
        {
            int file = fen[i] - 'a';
            int rank = fen[i + 1] - '1';
            // board::print_BitBoard(board.en_passant);
            board.en_passant |= 1ULL << (rank * 8 + (7 - file));
            // board::print_BitBoard(board.en_passant);
            i += 3;
        }
        else
            i++;

        i++;
        int t = fen[i] - '0';
        i++;
        if (fen[i] != ' ')
        {
            t *= 10;
            t += fen[i] - '0';
            i++;
        }

        board.last_turn_ = t;
        i++;

        t = fen[i] - '0';
        i++;
        if (fen[i] != ' ')
        {
            t *= 10;
            t += fen[i] - '0';
            i++;
        }
        board.turn_ = t;
        i++;

        t = fen[i] - '0';
        i++;
        if (fen[i] >= '0' && fen[i] <= '9')
        {
            t *= 10;
            t += fen[i] - '0';
            i++;
        }

        return t;
    }

    static int set_flags(board::Chessboard &board, std::string fen)
    {
        size_t i = 0;
        board.white_turn_ = (fen[i] == 'w') ? true : false;
        i += 2;
        if (fen[i] == '-')
        {
            board.white_king_castling_ = false;
            board.white_queen_castling_ = false;
            board.black_king_castling_ = false;
            board.black_queen_castling_ = false;
            i += 2;
        }
        else
        {
            int count = 4;
            if (fen.find('q') == std::string::npos)
            {
                board.black_king_castling_ = false;
                --count;
            }
            if (fen.find('k') == std::string::npos)
            {
                board.black_queen_castling_ = false;
                --count;
            }
            if (fen.find('Q') == std::string::npos)
            {
                board.white_queen_castling_ = false;
                --count;
            }
            if (fen.find('K') == std::string::npos)
            {
                board.white_king_castling_ = false;
                --count;
            }
            i += count + 1;
        }
        std::string new_fen = fen.substr(i, fen.length() - i);
        return set_passant_and_flags(board, new_fen);
    }

    uint64_t parse_fen_file(const std::string &file)
    {
        std::vector<uint64_t> boards;
        for (int i = 0; i < 12; i++)
            boards.push_back(0ULL);
        std::ifstream os(file);
        if (!os.is_open())
        {
            std::cerr << "couldn't open file";
            return 0ULL;
        }
        std::string fen;
        std::getline(os, fen);
        size_t len = 0;
        for (; len < fen.length() && fen[len] != ' '; len++)
            ;
        size_t i = 0;
        char c = fen[0];
        uint64_t n;
        int r = 7;
        int f = 0;
        for (; i < len; i++)
        {
            c = fen[i];
            if (c == '/')
            {
                r--;
                f = 0;
            }
            else if (c >= '1' && c <= '9')
            {
                f += c - '0';
            }
            else
            {
                n = 1ULL << ((7 - f) + (r * 8));
                if (c == 'Q')
                    boards[0] |= n;
                if (c == 'R')
                    boards[1] |= n;
                if (c == 'B')
                    boards[2] |= n;
                if (c == 'N')
                    boards[3] |= n;
                if (c == 'P')
                    boards[4] |= n;
                if (c == 'K')
                    boards[5] |= n;
                if (c == 'q')
                    boards[6] |= n;
                if (c == 'r')
                    boards[7] |= n;
                if (c == 'b')
                    boards[8] |= n;
                if (c == 'n')
                    boards[9] |= n;
                if (c == 'p')
                    boards[10] |= n;
                if (c == 'k')
                    boards[11] |= n;
                f++;
            }
        }
        board::Chessboard res(boards);

        i++;

        std::cerr << "BOARD : " << std::endl;
        res.print_board();
        board::print_BitBoard(res.pins_);

        std::string new_fen = fen.substr(i, fen.length() - i);
        int depth = set_flags(res, new_fen);
        return generate_all_moves(res, depth);
    }
} // namespace fen
