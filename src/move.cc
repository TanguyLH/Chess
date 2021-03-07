#include "move.hh"

namespace board
{

    Move::Move(Position from, Position to, PieceType piece, Color color, opt_piecetype_t promotion)
        : from_(from), to_(to), piece_(piece), color_(color), promotion_(promotion)
    {}

    /*Move::Move(PgnMove pgn)
    {
        this->from_ = pgn.start_;
        this->to_ = pgn.
    }*/

    short Move::board_index() const
    {
        return static_cast<short>(piece_)
               + (color_ == Color::BLACK ? 6 : 0);
    }

    std::string to_sFile(File file)
    {
        std::string res;
        if (file == board::File::A)
        {
            res = "A";
        }
        if (file == board::File::B)
        {
            res = "B";
        }
        if (file == board::File::C)
        {
            res = "C";
        }
        if (file == board::File::D)
        {
            res = "D";
        }
        if (file == board::File::E)
        {
            res = "E";
        }
        if (file == board::File::F)
        {
            res = "F";
        }
        if (file == board::File::G)
        {
            res = "G";
        }
        if (file == board::File::H)
        {
            res = "H";
        }
        return res;
    }
    
    std::string to_sRank(Rank rank)
    {
        std::string res;
        if (rank == board::Rank::ONE)
        {
            res = "1";
        }
        if (rank == board::Rank::TWO)
        {
            res = "2";
        }
        if (rank == board::Rank::THREE)
        {
            res = "3";
        }
        if (rank == board::Rank::FOUR)
        {
            res = "4";
        }
        if (rank == board::Rank::FIVE)
        {
            res = "5";
        }
        if (rank == board::Rank::SIX)
        {
            res = "6";
        }
        if (rank == board::Rank::SEVEN)
        {
            res = "7";
        }
        if (rank == board::Rank::EIGHT)
        {
            res = "8";
        }
        return res;
    }

    void Move::pretty()
    {
        std::cout << "FROM " << to_sFile(this->from_.file_get()) << to_sRank(this->from_.rank_get()) << "\n";
        std::cout << "TO " << to_sFile(this->to_.file_get()) << to_sRank(this->to_.rank_get()) << "\n";
    }
}
