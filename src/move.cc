#include "move.hh"


namespace board
{


    short Move::board_index() const
    {
        return std::static_cast<short>(piece_)
               + (color_ == Color::BLACK ? 6 : 0);
    }



}
