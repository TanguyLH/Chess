#include "chessboard.hh"
#include "generate_move.hh"

namespace board
{

    void board_filler(std::vector<uint64_t> &boards, int i_offset, int j_offset)
    {
        (j_offset != -1) ? boards.push_back(1L << i_offset | 1L << j_offset) 
            : boards.push_back(1L << i_offset);
    }

    Chessboard::Chessboard()
    {
        this->turn_ = 0;
        this->white_turn_ = true;
        this->white_king_castling_ = false;
        this->white_queen_castling_ = false;
        this->black_king_castling_ = false;

        this->boards_.push_back(8);
        this->boards_.push_back(129);
        this->boards_.push_back(36);
        this->boards_.push_back(66);
        this->boards_.push_back(65280);
        this->boards_.push_back(16);

        board_filler(this->boards_, 60, -1);
        board_filler(this->boards_, 63, 56);
        board_filler(this->boards_, 61, 58);
        board_filler(this->boards_, 62, 57);
        
        uint64_t res = 0;
        for (uint64_t j = 1L << 55; j > 1L << 47; j >>= 1)
            res |= j;
        this->boards_.push_back(res);
        this->boards_[10] += (1L << 17);
        board_filler(this->boards_, 59, -1);
        this->pins_ = 0ULL;
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
            bool found = false;
            while (i < 12)
            {
                if (i == 6)
                    color = "\x1B[31m";
                if (b[i] & n)
                {
                    if (found)
                    {
                        std::cerr << "\n\nERROR: 2 PIECES ON SAME CELL\n" << std::endl;
                        exit(1);
                    }
                    found = true;
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
                }
                i++;
            }
            if (!found)
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

    Chessboard::Chessboard(std::vector<uint64_t> boards)
    {
        this->turn_ = 0;
        this->white_turn_ = true;
        this->white_king_castling_ = false;
        this->white_queen_castling_ = false;
        this->black_king_castling_ = false;

        this->boards_ = boards;        
        this->pins_ = find_absolute_pins(*this);
    }
  
    std::vector<Move> Chessboard::generate_legal_moves()
    {
        Color color = this->white_turn_ ? board::Color::WHITE : board::Color::BLACK;
        std::vector<Move> res;
        generate_knight_moves(*this, color, res);
        generate_pawn_moves(*this, color, res);
        return res;  
    }
  
    bool Chessboard::is_move_legal(Move move)
    {
        std::vector<Move> leg_moves = generate_legal_moves();
        for (auto i = leg_moves.begin(); i != leg_moves.end(); i++)
            if (move.from_ == (*i).from_ && move.to_ == (*i).to_)
                return true;
        return false;
    }
  
    void Chessboard::do_move(Move move)
    {
        if (!is_move_legal(move))
            return;

        int turn_offset = white_turn_ ? 0 : 6;
        int index = static_cast<int>(move.piece_) + turn_offset;
        uint64_t dep = 1L << (static_cast<int>(move.from_.rank_get()) * 8 + (7 - static_cast<int>((move.from_.file_get()))));
        uint64_t arr = 1L << (static_cast<int>(move.to_.rank_get()) * 8 + (7 - static_cast<int>((move.to_.file_get()))));
        this->boards_[index] &= ~dep;
        this->boards_[index] |= arr;

        turn_offset = 6 - turn_offset;           
        for (auto i = 0 + turn_offset; i < 6 + turn_offset; i++)
        {
            if (this->boards_[i] & arr)
            {
                this->boards_[i] &= ~arr;
                break;
            }
        }           
    }
}
