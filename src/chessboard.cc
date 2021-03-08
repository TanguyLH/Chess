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
        this->white_king_castling_ = true;
        this->white_queen_castling_ = true;
        this->black_king_castling_ = true;
        this->black_queen_castling_ = true;

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
        board_filler(this->boards_, 59, -1);
        this->pins_ = 0ULL;
        this->en_passant = 0ULL;
        /*for (auto it : this->listen)
        on_piece_moved(move.type_, move.from_, move.to_)*/
    }

    void Chessboard::print_board()
    {
        int count = 1;
        auto b = this->boards_;
        uint64_t n = 1;
        n <<= 63;
        for (; n > 0; n >>= 1)
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
                        std::cerr << "\n\nERROR: 2 PIECES ON SAME CELL\n"
                                  << std::endl;
                        exit(1);
                    }
                    found = true;
                    if (i == 0 || i == 6)
                        std::cout << color << "Q"
                                  << "\x1B[0m";
                    else if (i == 1 || i == 7)
                        std::cout << color << "R"
                                  << "\x1B[0m";
                    else if (i == 2 || i == 8)
                        std::cout << color << "B"
                                  << "\x1B[0m";
                    else if (i == 3 || i == 9)
                        std::cout << color << "N"
                                  << "\x1B[0m";
                    else if (i == 4 || i == 10)
                        std::cout << color << "i"
                                  << "\x1B[0m";
                    else if (i == 5 || i == 11)
                        std::cout << color << "K"
                                  << "\x1B[0m";
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
        this->white_king_castling_ = true;
        this->white_queen_castling_ = true;
        this->black_king_castling_ = true;
        this->black_queen_castling_ = true;
        this->boards_ = boards;
        this->pins_ = find_absolute_pins(*this);
        this->en_passant = 0ULL;
    }

    std::vector<Move> Chessboard::generate_legal_moves()
    {
        Color color =
            this->white_turn_ ? board::Color::WHITE : board::Color::BLACK;
        std::vector<Move> res;
        generate_knight_moves(*this, color, res);
        generate_pawn_moves(*this, color, res);
        return res;
    }

    bool Chessboard::is_move_legal(Move move)
    {
        std::vector<Move> leg_moves = generate_legal_moves();
        for (auto i = leg_moves.begin(); i != leg_moves.end(); i++)
            if (move.from_ == i->from_ && move.to_ == i->to_)
                return true;
        return false;
    }

    uint64_t get_occupancy2(std::vector<uint64_t> boards, bool is_white)
    {
        uint64_t res = 0;
        short col = is_white == 1 ? 0 : 6;
        for (int i = col; i < col + 6; i++)
        {
            res = res | boards[i];
        }
        return res;
    }

    void Chessboard::do_move(Move &move)
    {
        if (!is_move_legal(move))
            return;

        int turn_offset = white_turn_ ? 0 : 6;
        int piece_type = static_cast<int>(move.piece_);
        int index = piece_type + turn_offset;
        uint64_t adv_col_occ =
            get_occupancy2((*this).boards_, !(this->white_turn_));

        int from_rank = static_cast<int>(move.from_.rank_get());
        int to_rank = static_cast<int>(move.to_.rank_get());
        int from_file = static_cast<int>(move.from_.file_get());
        int to_file = static_cast<int>(move.to_.file_get());

        uint64_t dep = 1L << (from_rank * 8 + (7 - from_file));
        uint64_t arr = 1L << (to_rank * 8 + (7 - to_file));

        this->en_passant = 0;
        if (((from_rank == 1 && to_rank == 3)
             || (from_rank == 6 && to_rank == 4))
            && piece_type == 4)
            this->en_passant = arr;

        this->boards_[index] |= arr;
        this->boards_[index] &= ~dep;

        if (piece_type == 4 && to_rank == 0 && move.promotion_ != std::nullopt)
        {
            this->boards_[index] &= ~arr;
            this->boards_[static_cast<int>(*(move.promotion_)) + turn_offset] |= arr;
        }

        turn_offset = 6 - turn_offset;
        if (from_file != to_file && !(arr & adv_col_occ))
        {
            int tmp = (to_rank * 8 + (7 - to_file));
            uint64_t tmp2 = (1L << (tmp - 8));
            this->boards_[4 + turn_offset] -= tmp2;
        }

        for (auto i = turn_offset; i < 6 + turn_offset; i++)
        {
            if (this->boards_[i] & arr)
            {
                this->boards_[i] &= ~arr;
                break;
            }
        }
    }

    bool Chessboard::is_check_compatible(Move move, uint64_t piece)
    {
        if (!(this->pins_ & piece) || !this->in_check)
            return true;
        int turn_offset = white_turn_ ? 0 : 6;
        int piece_type = static_cast<int>(move.piece_);
        int index = piece_type + turn_offset;
        uint64_t adv_col_occ =
            get_occupancy2((*this).boards_, !(this->white_turn_));

        int from_rank = static_cast<int>(move.from_.rank_get());
        int to_rank = static_cast<int>(move.to_.rank_get());
        int from_file = static_cast<int>(move.from_.file_get());
        int to_file = static_cast<int>(move.to_.file_get());

        uint64_t dep = 1L << (from_rank * 8 + (7 - from_file));
        uint64_t arr = 1L << (to_rank * 8 + (7 - to_file));

        this->en_passant = 0;
        if (((from_rank == 1 && to_rank == 3)
             || (from_rank == 6 && to_rank == 4))
            && piece_type == 4)
            this->en_passant = arr;

        uint64_t savedboard = this->boards_[index];
        this->boards_[index] |= arr;
        this->boards_[index] &= ~dep;

        turn_offset = 6 - turn_offset;
        if (from_file != to_file && !(arr & adv_col_occ))
        {
            int tmp = (to_rank * 8 + (7 - to_file));
            uint64_t tmp2 = (1L << (tmp - 8));
            this->boards_[4 + turn_offset] -= tmp2;
        }

        short i;
        uint64_t eatenboard = 0;
        for (i = turn_offset; i < 6 + turn_offset; i++)
        {
            if (this->boards_[i] & arr)
            {
                eatenboard = this->boards_[i];
                this->boards_[i] &= ~arr;
                break;
            }
        }
        turn_offset = white_turn_ ? 0 : 6;

        uint64_t attackboard = generate_rook_attacks(*this, static_cast<Color>(white_turn_));
        attackboard |= generate_bishop_attacks(*this, static_cast<Color>(white_turn_));
        attackboard |= generate_queen_attacks(*this, static_cast<Color>(white_turn_));
        attackboard |= generate_pawn_attacks(*this, static_cast<Color>(white_turn_));
        attackboard |= generate_knight_attacks(*this, static_cast<Color>(white_turn_));

        bool res = !(this->boards_[5 + turn_offset] & attackboard);

        this->boards_[index] = savedboard;
        if (eatenboard)
            this->boards_[i] = eatenboard;

        return res;
    }


    Move Chessboard::move_from_pgn(PgnMove pgnmove)
    {
        Position start = pgnmove.start_get();
        Position end = pgnmove.end_get();
        std::optional<PieceType> promotion = pgnmove.promotion_get();
        return Move(start, end, pgnmove.piece_get(), promotion);
    }
}
