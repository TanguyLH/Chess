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
        while (!(it & sides_lr))
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
        }
        //Droite
        it = cur;
        is_pin = false;
        while (!(it & sides_lr))
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
        } 
        it = cur;
        is_pin = false;
        //Haut
        while (!(it & sides_td))
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
        } 
        it = cur;
        is_pin = false;
        //Bas
        while (!(it & sides_td))
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
        }

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
        while (!(it & sides))
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
        }
        //Droite
        it = cur;
        is_pin = false;
        while (!(it & sides))
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
        }
        it = cur;
        is_pin = false;
        //Haut
        while (!(it & sides))
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
        }
        it = cur;
        is_pin = false;
        //Bas
        while (!(it & sides))
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
        }
        return res;
    }

    uint64_t find_absolute_pins(Chessboard board)
    {
        uint64_t res = 0L;
        bool b = board.white_turn_;
        uint64_t cur = board.boards_[5 + b * 6];

        uint64_t obstacles = 0L;
        for (int i = 0 + b * 6; i < 5 + b * 6; i++)
            obstacles |= board.boards_[i];
         
        int i = (!b) * 6;
        res |= get_pin_tdlr(obstacles, board.boards_[0 + i] 
                | board.boards_[1 + i], cur);
        res |= get_pin_diag(obstacles, board.boards_[0 + i]
                | board.boards_[2 + i], cur);
        board.pins_ = res;
        return res;
    }

    static uint64_t check_tdlr(uint64_t obs, uint64_t tdlr, uint64_t king)
    {
        uint64_t res = 0L;
        uint64_t it = king;
        uint64_t sides_lr = 9331882296111890817U;
        uint64_t sides_td = 255ULL | 1837468647967162368ULL;
        while (!(it & sides_lr))
        {
            it <<= 1;
            if (it & obs)
                break;
            if (it & tdlr)
            {
                res |= it;
                break;
            }
        }
        it = king;
        while (!(it & sides_lr))
        {
            it >>= 1;
            if (it & obs)
                break;
            if (it & tdlr)
            {
                res |= it;
                break;
            }
        } 
        it = king;
        while (!(it & sides_td))
        {
            it <<= 8;
            if (it & obs)
                break;
            if (it & tdlr) 
            {
                res |= it;
                break;
            }
        }
        it = king;
        while (!(it & sides_td))
        {
            it >>= 8;
            if (it & obs)
                break;
            if (it & tdlr)
            {
                res |= it;
                break;
            }
        } 
        return res;
    }

    static uint64_t check_diag(uint64_t obs, uint64_t tdlr, uint64_t king)
    {
        uint64_t res = 0L;
        uint64_t it = king;
        uint64_t sides_lr = 9331882296111890817U;
        uint64_t sides = 255ULL | 1837468647967162368ULL | sides_lr;
        while (!(it & sides))
        {
            it <<= 7;
            if (it & obs)
                break;
            if (it & tdlr)
            {
                res |= it;
                break;
            }
        } 
        it = king;
        while (!(it & sides))
        {
            it >>= 7;
            if (it & obs)
                break;
            if (it & tdlr)
            {
                res |= it;
                break;
            }
        }
        it = king;
        while (!(it & sides))
        {
            it <<= 9;
            if (it & obs)
                break;
            if (it & tdlr) 
            {
                res |= it;
                break;
            }
        }
        it = king;
        while (!(it & sides))
        {
            it >>= 9;
            if (it & obs)
                break;
            if (it & tdlr)
            {
                res |= it;
                break;
            }
        } 
        return res;
    }


    bool check(Chessboard board, Color c)
    {
        bool b = (c == Color::WHITE) ? false : true;
        uint64_t cur = board.boards_[5 + b * 6];
        
        uint64_t obstacles = 0L;
        for (int i = 0 + b * 6; i < 5 + b * 6; i++)
            obstacles |= board.boards_[i];
        
        uint64_t res = 0L;
        int i = (!b) * 6;
        res |= check_tdlr(obstacles, board.boards_[0 + i] 
                | board.boards_[1 + i], cur);

        if (res != 0L)
            return true;
        res |= check_diag(obstacles, board.boards_[0 + i]
                | board.boards_[2 + i], cur);
        if (res == 0L)
            return false;
        return true;
    }
}
