#include "rule.hh"

namespace board
{
    uint64_t get_occupancy(std::vector<uint64_t> boards, bool is_white)
    {
        uint64_t res = 0;
        short col = is_white == 1 ? 0 : 6;
        for (int i = col; i < col + 6; i++)
        {
            res = res | boards[i];
        }
        return res;
    }
    void print_BitBoard(uint64_t b)
    {
        int count = 1;
        for (uint64_t n = 1L << 63; n > 0 ; n >>= 1)
        {
            bool found = false;
            std::cout << " | ";
            if (b & n)
            {
                if (found)
                {
                    std::cerr << "\n\nERROR: 2 PIECES ON SAME CELL\n" << std::endl;
                    exit(1);
                }
                found = true;
                std::cout << "X";
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
    uint64_t generate_pawn_attacks(Chessboard board, Color color)
    {
        uint64_t res = 0;
        uint64_t bit_pos = 0;
        if (color == board::Color::WHITE)
        {
            uint64_t bitb = color == board::Color::WHITE ? board.boards_[4] : board.boards_[10];
            bit_pos = bitb << 7;
            bit_pos &= 0b0111111101111111011111110111111101111111011111110111111101111111;
            res |= bit_pos;         
            bit_pos = bitb << 9;
            bit_pos &= 0b1111111011111110111111101111111011111110111111101111111011111110;
            res |= bit_pos;
        }
        else
        {
            uint64_t bitb = color == board::Color::WHITE ? board.boards_[4] : board.boards_[10];
            bit_pos = bitb >> 7;
            bit_pos &= 0b1111111011111110111111101111111011111110111111101111111011111110;
            res |= bit_pos;         
            bit_pos = bitb >> 9;
            bit_pos &= 0b0111111101111111011111110111111101111111011111110111111101111111;
            res |= bit_pos;                   
        }
        return res;
    }        
    void generate_pawn_moves(Chessboard board, Color color, std::vector<Move> &res)
    {
        uint64_t bitb = color == board::Color::WHITE ? board.boards_[4] : board.boards_[10];
        while (bitb)
        {
            uint64_t tmp = log2(bitb);
            uint64_t piece = 1L << tmp;
            int x = 7 - (int)log2(piece) % 8;
            int y = log2(piece) / 8;
            Position pos = Position(static_cast<File>(x), static_cast<Rank>(y));
            uint64_t adv_col_occ = get_occupancy(board.boards_, color != board::Color::WHITE);
            uint64_t same_col_occ = get_occupancy(board.boards_, color != board::Color::WHITE);  
            uint64_t all_occ = adv_col_occ | same_col_occ;                 
            uint64_t bit_pos = 0;
            if (color == board::Color::WHITE)
            {
                //capture move
                if (x < 7)
                {
                    bit_pos = piece << 7;
                    if (bit_pos & adv_col_occ)
                        res.push_back(Move(pos, Position(static_cast<File>(x + 1), static_cast<Rank>(y + 1)), board::PieceType::PAWN, color));
                }
                if (x > 0)
                {      
                    bit_pos = piece << 9;
                    if (bit_pos & adv_col_occ)
                        res.push_back(Move(pos, Position(static_cast<File>(x - 1), static_cast<Rank>(y + 1)), board::PieceType::PAWN, color));                    
                }
                //deplacement move
                bit_pos = piece << 8;
                if (!(bit_pos & all_occ))
                {
                    res.push_back(Move(pos, Position(static_cast<File>(x), static_cast<Rank>(y + 1)), board::PieceType::PAWN, color));
                    if (y == 1 && !((bit_pos <<= 8) & all_occ))
                    {
                        res.push_back(Move(pos, Position(static_cast<File>(x), static_cast<Rank>(y + 2)), board::PieceType::PAWN, color));                         
                    }
                }          
                
            }
            else
            {
                //capture move
                if (x > 0)
                {
                    bit_pos = piece >> 7;
                    if (bit_pos & adv_col_occ)
                        res.push_back(Move(pos, Position(static_cast<File>(x - 1), static_cast<Rank>(y - 1)), board::PieceType::PAWN, color));
                }
                if (x < 7)
                {      
                    bit_pos = piece >> 9;
                    if (bit_pos & adv_col_occ)
                        res.push_back(Move(pos, Position(static_cast<File>(x + 1), static_cast<Rank>(y - 1)), board::PieceType::PAWN, color));                    
                }
                //deplacement move
                bit_pos = piece >> 8;
                if (!(bit_pos & all_occ))
                {
                    res.push_back(Move(pos, Position(static_cast<File>(x), static_cast<Rank>(y - 1)), board::PieceType::PAWN, color));
                    if (y == 6 && !((bit_pos >>= 8) & all_occ))
                    {
                        res.push_back(Move(pos, Position(static_cast<File>(x), static_cast<Rank>(y - 2)), board::PieceType::PAWN, color));                         
                    }
                }                
            }
            
            
            bitb -= piece;
        } 
    }
    
    uint64_t generate_knight_attacks(Chessboard board, Color color)
    {
        uint64_t res = 0;
        uint64_t bitb = color == board::Color::WHITE ? board.boards_[3] : board.boards_[9];
        uint64_t bit_pos = 0;
        uint64_t mask = 0b011111110111111101111111011111110111111101111111;
        bit_pos = (bitb & mask);
        if (bit_pos)
        {
            res |= bit_pos << 17;
        }
        mask = 0b111111101111111011111110111111101111111011111110;                                                                                  
        bit_pos = (bitb & mask);
        if (bit_pos)
            res |= bit_pos << 15;

        mask = 0b11111100111111001111110011111100111111001111110011111100;
        bit_pos = (bitb & mask);
        if (bit_pos)
        {
            res |= bit_pos << 6;
        }
        mask = 0b1111110011111100111111001111110011111100111111001111110000000000;
        bit_pos = (bitb & mask) ;
        if (bit_pos)
        {
            res |= bit_pos >> 10;
        }
        mask = 0b0011111100111111001111110011111100111111001111110011111100000000;
        bit_pos = (bitb & mask);
        if (bit_pos)
        {
            res |= bit_pos >> 6;
        }
        mask = 0b00111111001111110011111100111111001111110011111100111111;
        bit_pos = (bitb & mask);
        if (bit_pos)
        {
            res |= bit_pos << 10;
        }
        mask = 0b0111111101111111011111110111111101111111011111110000000000000000;
        bit_pos = (bitb & mask);
        if (bit_pos)
        {
            res |= bit_pos >> 15;
        }
        mask = 0b1111111011111110111111101111111011111110111111100000000000000000;
        bit_pos = (bitb & mask);
        if (bit_pos)
        {
            res |= bit_pos >> 17;
        }
        return res;
    }
    void generate_knight_moves(Chessboard board, Color color, std::vector<Move> &res)
    {
        uint64_t bitb = color == board::Color::WHITE ? board.boards_[3] : board.boards_[9];
        while (bitb)
        {
            uint64_t tmp = log2(bitb);
            uint64_t piece = 1L << tmp;
            int x = 7 - (int)log2(piece) % 8;
            int y = log2(piece) / 8;
            Position pos = Position(static_cast<File>(x), static_cast<Rank>(y));
            uint64_t same_col_occ = get_occupancy(board.boards_, color == board::Color::WHITE);           
            uint64_t bit_pos = 0;

            if (x < 7 && y < 6)
            {
                bit_pos = (piece) << 15;
                if ((bit_pos & same_col_occ) == 0)
                {
                    res.push_back(Move(pos, Position(static_cast<File>(x + 1), static_cast<Rank>(y + 2)), board::PieceType::KNIGHT, color));
                }
            }
            if (x < 6 && y < 7) 
            {
                bit_pos = (piece) << 6;
                if ((bit_pos & same_col_occ) == 0)
                {
                    res.push_back(Move(pos, Position(static_cast<File>(x + 2), static_cast<Rank>(y + 1)), board::PieceType::KNIGHT, color));                  
                }
            }
            if (x < 6 && y > 0)
            {
                bit_pos = (piece) >> 10;
                if ((bit_pos & same_col_occ) == 0)
                {            
                    res.push_back(Move(pos, Position(static_cast<File>(x + 2), static_cast<Rank>(y - 1)), board::PieceType::KNIGHT, color));                
                }
            }
            if (x < 6 && y > 1)
            {
                bit_pos = (piece) >> 17;
                if ((bit_pos & same_col_occ) == 0)
                {
                    res.push_back(Move(pos, Position(static_cast<File>(x + 1), static_cast<Rank>(y - 2)), board::PieceType::KNIGHT, color));                  
                }
            }
            if (x > 0 && y < 6)
            {
                bit_pos = (piece) << 17;
                if ((bit_pos & same_col_occ) == 0)
                {              
                    res.push_back(Move(pos, Position(static_cast<File>(x - 1), static_cast<Rank>(y + 2)), board::PieceType::KNIGHT, color));                  
                }
            }
            if (x > 1 && y < 7)
            {
                bit_pos = (piece) << 10;
                if ((bit_pos & same_col_occ) == 0)
                {
                    res.push_back(Move(pos, Position(static_cast<File>(x - 2), static_cast<Rank>(y + 1)), board::PieceType::KNIGHT, color));                  
                }
            }
            if (x > 1 && y > 0)
            {
                bit_pos = (piece) >> 6;
                if ((bit_pos & same_col_occ) == 0)
                {            
                    res.push_back(Move(pos, Position(static_cast<File>(x - 2), static_cast<Rank>(y - 1)), board::PieceType::KNIGHT, color));                  
                }
            }
            if (x > 0 && y > 1)
            {
                bit_pos = (piece) >> 15;
                if ((bit_pos & same_col_occ) == 0)
                {               
                    res.push_back(Move(pos, Position(static_cast<File>(x - 1), static_cast<Rank>(y - 2)), board::PieceType::KNIGHT, color));                  
                }
            }
            bitb -= piece;
        }
    }

}
