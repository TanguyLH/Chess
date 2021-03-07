#pragma once

#include <iostream>
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
            opt_piecetype_t promotion_;

            Move(Position from, Position to, PieceType piece, Color color, opt_piecetype_t promotion);
            void pretty();
            short board_index() const;

    };

}
