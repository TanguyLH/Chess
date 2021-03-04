#include "move.hh"


namespace board
{

    Move::Move(Position from, Position to, PieceType piece, Color color)
        : from_(from), to_(to), piece_(piece), color_(color)
    {}

    short Move::board_index() const
    {
        return static_cast<short>(piece_)
               + (color_ == Color::BLACK ? 6 : 0);
    }



}
