
#include "color.hh"
#include "piece-type.hh"


namespace board
{
    
    Class Move
    {
        public:
            Position from_;
            Position to_;

            PieceType piece_;
            Color color_;

            short board_index() const;

}
