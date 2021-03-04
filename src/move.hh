
#include "color.hh"
#include "piece-type.hh"
#include "position.hh"

namespace board
{
    
    class Move
    {
        public:
            Position from_;
            Position to_;

            PieceType piece_;
            Color color_;

            Move(Position from, Position to, PieceType piece, Color color);
            short board_index() const;

    };

}
