#include <iostream>

#include "generate_move.hh"

namespace board
{
    static uint64_t get_pin_tdlr(uint64_t obstacles, uint64_t slides, uint64_t cur)
    {
        uint64_t sides_lr = 9331882296111890817U;
        uint64_t sides_td = 255ULL | 1837468647967162368ULL;
        uint64_t res = 0L;
        uint64_t it = cur;
        bool is_pin = false;
        uint64_t pinned = 0L;
        //Gauche
        do
        {
            it <<= 1;
            if (it & slides && is_pin)
                res |= pinned;
            if (it & obstacles && is_pin)
                break;
            else if (it & obstacles)
            {
                is_pin = true;
                pinned = it;
            }
        } while (!(it & sides_lr));
        //Droite
        it = cur;
        is_pin = false;
        do
        {
            it >>= 1;
            if (it & slides && is_pin)
                res |= pinned;
            if (it & obstacles && is_pin)
                break;
            else if (it & obstacles)
            {
                pinned = it;
                is_pin = true;
            }
        } while (!(it & sides_lr));
        it = cur;
        is_pin = false;
        //Haut
        do
        {
            it <<= 8;
            if (it & slides && is_pin)
                res |= pinned;
            if (it & obstacles && is_pin)
                break;
            else if (it & obstacles)
            {

                pinned = it;
                is_pin = true;
            }
        } while (!(it & sides_td));
        it = cur;
        is_pin = false;
        //Bas
        do
        {
            it <<= 8;
            if (it & slides && is_pin)
                res |= pinned;
            if (it & obstacles && is_pin)
                break;
            else if (it & obstacles)
            {
                pinned = it;
                is_pin = true;
            }
        } while (!(it & sides_td));

        return res;
    }

    static uint64_t get_pin_diag(uint64_t obstacles, uint64_t slides, uint64_t cur)
    {
        uint64_t sides_lr = 9331882296111890817U;
        uint64_t sides = 255ULL | 1837468647967162368ULL | sides_lr;
        uint64_t res = 0L;
        uint64_t it = cur;
        bool is_pin = false;
        uint64_t pinned = 0L;
        //Gauche
        do
        {
            it <<= 7;
            if (it & slides && is_pin)
                res |= pinned ;
            if (it & obstacles && is_pin)
                break;
            else if (it & obstacles)
            {
                pinned = it;
                is_pin = true;
            }
        } while (!(it & sides));
        //Droite
        it = cur;
        is_pin = false;
        do
        {
            it >>= 7;
            if (it & slides && is_pin)
                res |= pinned;
            if (it & obstacles && is_pin)
                break;
            else if (it & obstacles)
            {
                pinned = it;
                is_pin = true;
            }
        } while (!(it & sides));
        it = cur;
        is_pin = false;
        //Haut
        do
        {
            it <<= 9;
            if (it & slides && is_pin)
                res |= pinned;
            if (it & obstacles && is_pin)
                break;
            else if (it & obstacles)
            {
                pinned = it;
                is_pin = true;
            }
        } while (!(it & sides));
        it = cur;
        is_pin = false;
        //Bas
        do
        {
            it <<= 9;
            if (it & slides && is_pin)
                res |= pinned;
            if (it & obstacles && is_pin)
                break;
            else if (it & obstacles)
            {
                pinned = it;
                is_pin = true;
            }
        } while (!(it & sides));
        return res;
    }

    uint64_t find_absolute_pins(Chessboard board)
    {
        uint64_t res = 0L;
        bool b = board.white_turn_;
        uint64_t cur = board.boards_[5 + b * 6];

        uint64_t slides = 0L;
        for (int i = 0 + (!b) * 6; i < 3 + (!b) * 6; i++)
            slides |= board.boards_[i];

        uint64_t obstacles = 0L;
        for (int i = 0 + b * 6; i < 5 + b * 6; i++)
            obstacles |= board.boards_[i];
         
        res |= get_pin_tdlr(obstacles, slides, cur);
        res |= get_pin_diag(obstacles, slides, cur);
        board.pins_ = res;
        return res;
    }
}
