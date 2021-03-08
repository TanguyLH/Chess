#include "rule.hh"

#define TOPDOWN                                                                \
    0b\
11111111\
00000000\
00000000\
00000000\
00000000\
00000000\
00000000\
11111111

#define LEFT                                                                   \
    0b\
10000000\
10000000\
10000000\
10000000\
10000000\
10000000\
10000000\
10000000

#define RIGHT                                                                  \
    0b\
00000001\
00000001\
00000001\
00000001\
00000001\
00000001\
00000001\
00000001

#define TOPLEFT                                                                \
    0b\
11111111\
10000000\
10000000\
10000000\
10000000\
10000000\
10000000\
10000000

#define TOPRIGHT                                                               \
    0b\
11111111\
00000001\
00000001\
00000001\
00000001\
00000001\
00000001\
00000001

#define BOTLEFT                                                                \
    0b\
10000000\
10000000\
10000000\
10000000\
10000000\
10000000\
10000000\
11111111

#define BOTRIGHT                                                               \
    0b\
00000001\
00000001\
00000001\
00000001\
00000001\
00000001\
00000001\
11111111

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
        for (uint64_t n = 1L << 63; n > 0; n >>= 1)
        {
            bool found = false;
            std::cout << " | ";
            if (b & n)
            {
                if (found)
                {
                    std::cerr << "\n\nERROR: 2 PIECES ON SAME CELL\n"
                              << std::endl;
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
            uint64_t bitb = color == board::Color::WHITE ? board.boards_[4]
                                                         : board.boards_[10];
            // if ((bit << 1) & )

            bit_pos = bitb << 7;
            bit_pos &=
                0b0111111101111111011111110111111101111111011111110111111101111111;
            res |= bit_pos;
            bit_pos = bitb << 9;
            bit_pos &=
                0b1111111011111110111111101111111011111110111111101111111011111110;
            res |= bit_pos;
        }
        else
        {
            uint64_t bitb = color == board::Color::WHITE ? board.boards_[4]
                                                         : board.boards_[10];
            bit_pos = bitb >> 7;
            bit_pos &=
                0b1111111011111110111111101111111011111110111111101111111011111110;
            res |= bit_pos;
            bit_pos = bitb >> 9;
            bit_pos &=
                0b0111111101111111011111110111111101111111011111110111111101111111;
            res |= bit_pos;
        }
        return res;
    }

    void generate_pawn_moves(Chessboard board, Color color,
                             std::vector<Move> &res)
    {
        uint64_t bitb =
            color == board::Color::WHITE ? board.boards_[4] : board.boards_[10];
        while (bitb)
        {
            uint64_t tmp = log2(bitb);
            uint64_t piece = 1L << tmp;
            int x = 7 - (int)log2(piece) % 8;
            int y = log2(piece) / 8;
            Position pos = Position(static_cast<File>(x), static_cast<Rank>(y));
            uint64_t adv_col_occ =
                get_occupancy(board.boards_, color != board::Color::WHITE);
            uint64_t same_col_occ =
                get_occupancy(board.boards_, color != board::Color::WHITE);
            uint64_t all_occ = adv_col_occ | same_col_occ;
            uint64_t bit_pos = 0;
            if (color == board::Color::WHITE)
            {
                if ((piece << 1) & board.en_passant)
                {
                    bit_pos = (piece << 1) & board.en_passant;
                    if (!((bit_pos << 9) & all_occ))
                    {
                        auto mv = Move(pos, Position(static_cast<File>(x - 1),
                                                     static_cast<Rank>(y + 1)),
                                       board::PieceType::PAWN, std::nullopt);
                        if (board.is_check_compatible(mv, piece))
                            res.push_back(mv);
                    }
                }
                if ((piece >> 1) & board.en_passant)
                {
                    bit_pos = (piece >> 1) & board.en_passant;
                    if (!((bit_pos << 7) & all_occ))
                    {
                        auto mv = Move(pos, Position(static_cast<File>(x + 1),
                                                     static_cast<Rank>(y + 1)),
                                       board::PieceType::PAWN, std::nullopt);
                        if (board.is_check_compatible(mv, piece))
                            res.push_back(mv);
                    }
                }
                // capture move
                if (x < 7)
                {
                    bit_pos = piece << 7;
                    if (bit_pos & adv_col_occ)
                    {
                        auto mv = Move(pos, Position(static_cast<File>(x + 1),
                                                     static_cast<Rank>(y + 1)),
                                       board::PieceType::PAWN, std::nullopt);
                        if (board.is_check_compatible(mv, piece))
                            res.push_back(mv);
                    }
                }
                if (x > 0)
                {
                    bit_pos = piece << 9;
                    if (bit_pos & adv_col_occ)
                    {
                        auto mv = Move(pos, Position(static_cast<File>(x - 1),
                                                     static_cast<Rank>(y + 1)),
                                       board::PieceType::PAWN, std::nullopt);
                        if (board.is_check_compatible(mv, piece))
                            res.push_back(mv);
                    }
                }
                // deplacement move
                bit_pos = piece << 8;
                if (!(bit_pos & all_occ))
                {
                    auto mv = Move(pos, Position(static_cast<File>(x),
                                                 static_cast<Rank>(y + 1)),
                                   board::PieceType::PAWN, std::nullopt);
                    if (board.is_check_compatible(mv, piece))
                        res.push_back(mv);
                    if (y == 1 && !((bit_pos <<= 8) & all_occ))
                    {
                        mv = Move(pos, Position(static_cast<File>(x),
                                                 static_cast<Rank>(y + 2)),
                                    board::PieceType::PAWN, std::nullopt);
                        if (board.is_check_compatible(mv, piece))
                            res.push_back(mv);
                    }
                }
            }
            else
            {
                // capture move
                if (x > 0)
                {
                    bit_pos = piece >> 7;
                    if (bit_pos & adv_col_occ)
                    {
                        auto mv = Move(pos, Position(static_cast<File>(x - 1),
                                                 static_cast<Rank>(y - 1)),
                                   board::PieceType::PAWN, std::nullopt);
                        if (board.is_check_compatible(mv, piece))
                            res.push_back(mv);
                    }
                }
                if (x < 7)
                {
                    bit_pos = piece >> 9;
                    if (bit_pos & adv_col_occ)
                    {
                        auto mv = Move(pos, Position(static_cast<File>(x + 1),
                                                 static_cast<Rank>(y - 1)),
                                   board::PieceType::PAWN, std::nullopt);
                        if (board.is_check_compatible(mv, piece))
                            res.push_back(mv);
                    }
                }
                // deplacement move
                bit_pos = piece >> 8;
                if (!(bit_pos & all_occ))
                {
                    auto mv = Move(pos, Position(static_cast<File>(x),
                                                 static_cast<Rank>(y - 1)),
                                   board::PieceType::PAWN, std::nullopt);
                    if (board.is_check_compatible(mv, piece))
                        res.push_back(mv);
                    if (y == 6 && !((bit_pos >>= 8) & all_occ))
                    {
                        auto mv = Move(pos, Position(static_cast<File>(x),
                                                 static_cast<Rank>(y - 2)),
                                   board::PieceType::PAWN, std::nullopt);
                        if (board.is_check_compatible(mv, piece))
                            res.push_back(mv);
                    }
                }
            }

            bitb -= piece;
        }
    }

    uint64_t generate_knight_attacks(Chessboard board, Color color)
    {
        uint64_t res = 0;
        uint64_t bitb =
            color == board::Color::WHITE ? board.boards_[3] : board.boards_[9];
        uint64_t bit_pos = 0;

        uint64_t mask = 0b011111110111111101111111011111110111111101111111;
        bit_pos = (bitb & mask);
        if (bit_pos)
            res |= bit_pos << 17;

        mask = 0b111111101111111011111110111111101111111011111110;
        bit_pos = (bitb & mask);
        if (bit_pos)
            res |= bit_pos << 15;

        mask = 0b11111100111111001111110011111100111111001111110011111100;
        bit_pos = (bitb & mask);
        if (bit_pos)
            res |= bit_pos << 6;

        mask =
            0b1111110011111100111111001111110011111100111111001111110000000000;
        bit_pos = (bitb & mask);
        if (bit_pos)
            res |= bit_pos >> 10;

        mask =
            0b0011111100111111001111110011111100111111001111110011111100000000;
        bit_pos = (bitb & mask);
        if (bit_pos)
            res |= bit_pos >> 6;

        mask = 0b00111111001111110011111100111111001111110011111100111111;
        bit_pos = (bitb & mask);
        if (bit_pos)
            res |= bit_pos << 10;

        mask =
            0b0111111101111111011111110111111101111111011111110000000000000000;
        bit_pos = (bitb & mask);
        if (bit_pos)
            res |= bit_pos >> 15;

        mask =
            0b1111111011111110111111101111111011111110111111100000000000000000;
        bit_pos = (bitb & mask);
        if (bit_pos)
            res |= bit_pos >> 17;

        return res;
    }

    void generate_knight_moves(Chessboard board, Color color,
                               std::vector<Move> &res)
    {
        uint64_t bitb =
            color == board::Color::WHITE ? board.boards_[3] : board.boards_[9];
        while (bitb)
        {
            uint64_t tmp = log2(bitb);
            uint64_t piece = 1L << tmp;
            int x = 7 - (int)log2(piece) % 8;
            int y = log2(piece) / 8;
            Position pos = Position(static_cast<File>(x), static_cast<Rank>(y));
            uint64_t same_col_occ =
                get_occupancy(board.boards_, color == board::Color::WHITE);
            uint64_t bit_pos = 0;

            if (x < 7 && y < 6)
            {
                bit_pos = (piece) << 15;
                if ((bit_pos & same_col_occ) == 0)
                {
                    auto mv = Move(pos, Position(static_cast<File>(x + 1),
                                                 static_cast<Rank>(y + 2)),
                                   board::PieceType::KNIGHT, std::nullopt);
                    if (board.is_check_compatible(mv, piece))
                        res.push_back(mv);
                }
            }
            if (x < 6 && y < 7)
            {
                bit_pos = (piece) << 6;
                if ((bit_pos & same_col_occ) == 0)
                {
                    auto mv = Move(pos, Position(static_cast<File>(x + 2),
                                                 static_cast<Rank>(y + 1)),
                                   board::PieceType::KNIGHT, std::nullopt);
                    if (board.is_check_compatible(mv, piece))
                        res.push_back(mv);
                }
            }
            if (x < 6 && y > 0)
            {
                bit_pos = (piece) >> 10;
                if ((bit_pos & same_col_occ) == 0)
                {
                    auto mv = Move(pos, Position(static_cast<File>(x + 2),
                                                 static_cast<Rank>(y - 1)),
                                   board::PieceType::KNIGHT, std::nullopt);
                    if (board.is_check_compatible(mv, piece))
                        res.push_back(mv);
                }
            }
            if (x < 6 && y > 1)
            {
                bit_pos = (piece) >> 17;
                if ((bit_pos & same_col_occ) == 0)
                {
                    auto mv = Move(pos, Position(static_cast<File>(x + 1),
                                                 static_cast<Rank>(y - 2)),
                                   board::PieceType::KNIGHT, std::nullopt);
                    if (board.is_check_compatible(mv, piece))
                        res.push_back(mv);
                }
            }
            if (x > 0 && y < 6)
            {
                bit_pos = (piece) << 17;
                if ((bit_pos & same_col_occ) == 0)
                {
                    auto mv = Move(pos, Position(static_cast<File>(x - 1),
                                                 static_cast<Rank>(y + 2)),
                                   board::PieceType::KNIGHT, std::nullopt);
                    if (board.is_check_compatible(mv, piece))
                        res.push_back(mv);
                }
            }
            if (x > 1 && y < 7)
            {
                bit_pos = (piece) << 10;
                if ((bit_pos & same_col_occ) == 0)
                {
                    auto mv = Move(pos, Position(static_cast<File>(x - 2),
                                                 static_cast<Rank>(y + 1)),
                                   board::PieceType::KNIGHT, std::nullopt);
                    if (board.is_check_compatible(mv, piece))
                        res.push_back(mv);
                }
            }
            if (x > 1 && y > 0)
            {
                bit_pos = (piece) >> 6;
                if ((bit_pos & same_col_occ) == 0)
                {
                    auto mv = Move(pos, Position(static_cast<File>(x - 2),
                                                 static_cast<Rank>(y - 1)),
                                   board::PieceType::KNIGHT, std::nullopt);
                    if (board.is_check_compatible(mv, piece))
                        res.push_back(mv);
                }
            }
            if (x > 0 && y > 1)
            {
                bit_pos = (piece) >> 15;
                if ((bit_pos & same_col_occ) == 0)
                {
                    auto mv = Move(pos, Position(static_cast<File>(x - 1),
                                                 static_cast<Rank>(y - 2)),
                                   board::PieceType::KNIGHT, std::nullopt);
                    if (board.is_check_compatible(mv, piece))
                        res.push_back(mv);
                }
            }
            bitb -= piece;
        }
    }

    uint64_t generate_rook_attacks(Chessboard board, Color color,
                                   bool is_queen)
    {
        uint64_t res = 0ULL;

        // Mask des coté du board (plus tard je ferais pareil pour le haut/bas)
        uint64_t right = RIGHT;
        uint64_t left = LEFT;

        // Mask des haut/bas du board
        uint64_t topdown = TOPDOWN;

        uint64_t all = get_occupancy(board.boards_, true)
            | get_occupancy(board.boards_, false);
        /*std::cout << std::endl << "------ ALL: ------------" << std::endl;
        print_board(buildboard(all, 0));*/

        short i = (!is_queen) + (static_cast<bool>(color) ? 6 : 0);
        uint64_t rooks = board.boards_[i];

        /*
         * cur doit être la puissance de 2 la plus proche en dessous du board.
         * Ce qui veut dire que c'est la première tour (en partant d'en haut a
         * gauche).
         */

        while (rooks > 0)
        {
            uint64_t curn = log2(rooks);
            uint64_t cur = 1ULL << curn;
            rooks -= cur;
            /*std::cout << std::endl << "------ NEW ROOK: ------------" <<
            std::endl; print_board(buildboard(cur, i));*/

            // std::cout << std::endl << "ATTACKS:" << std::endl;
            uint64_t mvs = 0;
            uint64_t it = cur;

            // A GAUCHE
            if ((it & left) == 0)
            {
                do
                {
                    it <<= 1;
                    mvs |= it;
                    if ((it & all) > 0)
                        break;
                } while ((it & left) == 0);
            }

            // A DROITE
            it = cur;
            if ((it & right) == 0)
            {
                do
                {
                    it >>= 1;
                    mvs |= it;
                    if ((it & all) > 0)
                        break;
                } while ((it & right) == 0);
            }

            // EN HAUT
            it = cur;
            do
            {
                it <<= 8;
                mvs |= it;
                if ((it & all) > 0)
                    break;
            } while ((it & topdown) == 0 && it);

            // EN BAS
            it = cur;
            do
            {
                it >>= 8;
                mvs |= it;
                if ((it & all) > 0)
                    break;
            } while ((it & topdown) == 0 && it);

            res |= mvs;
        }

        return res;
    }

    void generate_rook_moves(Chessboard board, Color color,
                             std::vector<Move> &res, bool is_queen)
    {
        // Mask des coté du board (plus tard je ferais pareil pour le haut/bas)
        uint64_t right = RIGHT;
        uint64_t left = LEFT;

        // Mask des haut/bas du board
        uint64_t topdown = TOPDOWN;

        uint64_t ami =
            get_occupancy(board.boards_, !(static_cast<bool>(color)));
        uint64_t ennemi =
            get_occupancy(board.boards_, (static_cast<bool>(color)));

        short i = (!is_queen) + (static_cast<bool>(color) ? 6 : 0);
        uint64_t rooks = board.boards_[i];

        PieceType type = is_queen ? PieceType::QUEEN : PieceType::ROOK;

        // cur doit être la puissance de 2 la plus proche en dessous du board.
        // Ce qui veut dire que c'est la première tour (en partant d'en haut a
        // gauche).

        while (rooks > 0)
        {
            uint64_t curn = log2(rooks);
            uint64_t cur = 1ULL << curn;
            rooks -= cur;
            short x = 7 - (curn % 8);
            short y = (curn / 8);
            auto pos = Position(static_cast<File>(x), static_cast<Rank>(y));

            /*std::cout << std::endl << "------ NEW ROOK: ------------" <<
            std::endl; prAint_board(buildboard(cur, i));*/

            // std::cout << std::endl << "ATTACKS:" << std::endl;
            uint64_t it = cur;

            short curx = x;
            short cury = y;

            // A GAUCHE
            if ((it & left) == 0)
            {
                do
                {
                    it <<= 1;
                    curx--;
                    if ((it & ami) > 0)
                        break;
                    auto mv = Move(pos, Position(static_cast<File>(curx),
                                                 static_cast<Rank>(cury)),
                                   type, std::nullopt);
                    if (board.is_check_compatible(mv, cur))
                        res.push_back(mv);
                    if ((it & ennemi) > 0)
                        break;
                } while ((it & right) == 0 || it == 0);
            }
            // A DROITE
            it = cur;
            curx = x;
            if ((it & right) == 0)
            {
                do
                {
                    it >>= 1;
                    curx++;
                    if ((it & ami) > 0)
                        break;
                    auto mv = Move(pos, Position(static_cast<File>(curx),
                                                 static_cast<Rank>(cury)),
                                   type, std::nullopt);
                    if (board.is_check_compatible(mv, cur))
                        res.push_back(mv);
                    if ((it & ennemi) > 0)
                        break;
                } while ((it & right) == 0 && it > 0);
            }

            // EN HAUT
            it = cur;
            curx = x;
            do
            {
                it <<= 8;
                cury++;
                if ((it & ami) > 0)
                    break;
                auto mv = Move(pos, Position(static_cast<File>(curx),
                                             static_cast<Rank>(cury)),
                                type, std::nullopt);
                if (board.is_check_compatible(mv, cur))
                    res.push_back(mv);
                if ((it & ennemi) > 0)
                    break;
            } while ((it & topdown) == 0 && it);

            // EN BAS
            it = cur;
            cury = y;
            do
            {
                it >>= 8;
                cury--;
                if ((it & ami) > 0)
                    break;
                auto mv = Move(pos, Position(static_cast<File>(curx),
                                             static_cast<Rank>(cury)),
                                type, std::nullopt);
                if (board.is_check_compatible(mv, cur))
                    res.push_back(mv);
                if ((it & ennemi) > 0)
                    break;
            } while ((it & topdown) == 0 && it);
        }
    }

    uint64_t generate_bishop_attacks(Chessboard board, Color color,
                                     bool is_queen)
    {
        uint64_t res = 0ULL;

        uint64_t all = get_occupancy(board.boards_, true)
            | get_occupancy(board.boards_, false);
        /*std::cout << std::endl << "------ ALL: ------------" << std::endl;
        print_board(buildboard(all, 0));*/

        short i = 2 * (!is_queen) + (static_cast<bool>(color) ? 6 : 0);
        uint64_t bishops = board.boards_[i];

        // cur doit être la puissance de 2 la plus proche en dessous du board.
        // Ce qui veut dire que c'est la première tour (en partant d'en haut a
        // gauche).

        while (bishops > 0)
        {
            uint64_t curn = log2(bishops);
            uint64_t cur = 1ULL << curn;
            bishops -= cur;
            /*std::cout << std::endl
                      << "------ NEW BISHOP: ------------" << std::endl;
            print_board(buildboard(cur, i));*/

            // std::cout << std::endl << "ATTACKS:" << std::endl;
            uint64_t mvs = 0;
            uint64_t it = cur;

            // VERS HAUT GAUCHE
            if ((it & TOPLEFT) == 0)
            {
                do
                {
                    it <<= 9;
                    mvs |= it;
                    if ((it & all) > 0)
                        break;
                } while ((it & TOPLEFT) == 0 || it == 0);
            }
            // print_board(buildboard(mvs, i));

            // VERS HAUT DROIT
            it = cur;
            if ((it & TOPRIGHT) == 0)
            {
                do
                {
                    it <<= 7;
                    mvs |= it;
                    if ((it & all) > 0)
                        break;
                } while ((it & TOPRIGHT) == 0 && it > 0);
            }
            // print_board(buildboard(mvs, i));

            // EN BAS GAUCHE
            it = cur;
            if ((it & BOTLEFT) == 0)
            {
                do
                {
                    it >>= 7;
                    mvs |= it;
                    if ((it & all) > 0)
                        break;
                } while ((it & BOTLEFT) == 0 && it);
            }
            // print_board(buildboard(mvs, i));

            // VERS BAS DROIT
            it = cur;
            if ((it & BOTRIGHT) == 0)
            {
                do
                {
                    it >>= 9;
                    mvs |= it;
                    if ((it & all) > 0)
                        break;
                } while ((it & BOTRIGHT) == 0 && it);
            }
            // print_board(buildboard(mvs, i));

            res |= mvs;
        }

        return res;
    }

    void generate_bishop_moves(Chessboard board, Color color,
                             std::vector<Move> &res, bool is_queen)
    {
        uint64_t ami =
            get_occupancy(board.boards_, !(static_cast<bool>(color)));
        uint64_t ennemi =
            get_occupancy(board.boards_, (static_cast<bool>(color)));

        short i = 2 * (!is_queen) + (static_cast<bool>(color) ? 6 : 0);
        uint64_t bishops = board.boards_[i];

        PieceType type = is_queen ? PieceType::QUEEN : PieceType::BISHOP;

        // cur doit être la puissance de 2 la plus proche en dessous du board.
        // Ce qui veut dire que c'est la première tour (en partant d'en haut a
        // gauche).

        while (bishops > 0)
        {
            uint64_t curn = log2(bishops);
            uint64_t cur = 1ULL << curn;
            bishops -= cur;
            short x = 7 - (curn % 8);
            short y = (curn / 8);
            auto pos = Position(static_cast<File>(x), static_cast<Rank>(y));
            /*std::cout << std::endl
                      << "------ NEW BISHOP: ------------" << std::endl;
            print_board(buildboard(cur, i));

            std::cout << std::endl << "ATTACKS:" << std::endl;*/
            uint64_t it = cur;

            uint64_t curx = x;
            uint64_t cury = y;

            // VERS HAUT GAUCHE
            if ((it & TOPLEFT) == 0)
            {
                do
                {
                    it <<= 9;
                    curx--;
                    cury++;
                    if ((it & ami) > 0)
                        break;
                    auto mv = Move(pos, Position(static_cast<File>(curx),
                                                 static_cast<Rank>(cury)),
                                   type, std::nullopt);
                    if (board.is_check_compatible(mv, cur))
                        res.push_back(mv);
                    if ((it & ennemi) > 0)
                        break;
                } while ((it & TOPLEFT) == 0 || it == 0);
            }
            // print_board(buildboard(mvs, i));

            // VERS HAUT DROIT
            it = cur;
            curx = x;
            cury = y;

            if ((it & TOPRIGHT) == 0)
            {
                do
                {
                    it <<= 7;
                    curx++;
                    cury++;
                    if ((it & ami) > 0)
                        break;
                    auto mv = Move(pos, Position(static_cast<File>(curx),
                                                 static_cast<Rank>(cury)),
                                   type, std::nullopt);
                    if (board.is_check_compatible(mv, cur))
                        res.push_back(mv);
                    if ((it & ennemi) > 0)
                        break;
                } while ((it & TOPRIGHT) == 0 && it > 0);
            }
            // print_board(buildboard(mvs, i));

            // EN BAS GAUCHE
            it = cur;
            curx = x;
            cury = y;

            if ((it & BOTLEFT) == 0)
            {
                do
                {
                    it >>= 7;
                    curx--;
                    cury--;
                    if ((it & ami) > 0)
                        break;
                    auto mv = Move(pos, Position(static_cast<File>(curx),
                                                 static_cast<Rank>(cury)),
                                   type, std::nullopt);
                    if (board.is_check_compatible(mv, cur))
                        res.push_back(mv);
                    if ((it & ennemi) > 0)
                        break;
                } while ((it & BOTLEFT) == 0 && it);
            }
            // print_board(buildboard(mvs, i));

            // VERS BAS DROIT
            it = cur;
            curx = x;
            cury = y;
            if ((it & BOTRIGHT) == 0)
            {
                do
                {
                    it >>= 9;
                    curx++;
                    cury--;
                    if ((it & ami) > 0)
                        break;
                    auto mv = Move(pos, Position(static_cast<File>(curx),
                                                 static_cast<Rank>(cury)),
                                   type, std::nullopt);
                    if (board.is_check_compatible(mv, cur))
                        res.push_back(mv);
                    if ((it & ennemi) > 0)
                        break;
                } while ((it & BOTRIGHT) == 0 && it);
            }
        }
    }

    uint64_t generate_queen_attacks(Chessboard board, Color color)
    {
        return generate_rook_attacks(board, color, true) |
               generate_bishop_attacks(board, color, true);
    }

    void generate_queen_moves(Chessboard board, Color color,
                             std::vector<Move> &res)
    {
        generate_rook_moves(board, color, res, true);
        generate_bishop_moves(board, color, res, true);
    }

    void generate_king_moves(Chessboard board, Color color,
                             std::vector<Move> &res)
    {
        uint64_t bitb =
            color == board::Color::WHITE ? board.boards_[5] : board.boards_[11];
        uint64_t tmp = log2(bitb);
        uint64_t piece = 1L << tmp;

        int x = 7 - (int)log2(piece) % 8;
        int y = log2(piece) / 8;

        Position pos = Position(static_cast<File>(x), static_cast<Rank>(y));
        uint64_t same_col_occ =
            get_occupancy(board.boards_, color == board::Color::WHITE);

        uint64_t bit_pos = 0;

        Color enemy_col = color == Color::WHITE ? Color::BLACK : Color::WHITE;

        uint64_t attackboard = generate_rook_attacks(board, enemy_col);
        attackboard |= generate_bishop_attacks(board, enemy_col);
        attackboard |= generate_queen_attacks(board, enemy_col);
        attackboard |= generate_pawn_attacks(board, enemy_col);
        attackboard |= generate_knight_attacks(board, enemy_col);
        attackboard |= generate_king_attacks(board, enemy_col);

        if (x > 0 && y < 7)
        {
            bit_pos = (piece) << 9;
            if ((bit_pos & same_col_occ) == 0)
            {
                auto mv = Move(pos, Position(static_cast<File>(x - 1),
                static_cast<Rank>(y + 1)), board::PieceType::KNIGHT, std::nullopt);
                res.push_back(mv);
            }           
        }
        if (y < 7)
        {
            bit_pos = (piece) << 8;
            if ((bit_pos & same_col_occ) == 0)
            {
                auto mv = Move(pos, Position(static_cast<File>(x),
                static_cast<Rank>(y + 1)), board::PieceType::KNIGHT, std::nullopt);
                res.push_back(mv);
            }           
        }
        if (x < 7 && y < 7)
        {
            bit_pos = (piece) << 7;
            if ((bit_pos & same_col_occ) == 0)
            {
                auto mv = Move(pos, Position(static_cast<File>(x + 1),
                static_cast<Rank>(y + 1)), board::PieceType::KNIGHT, std::nullopt);
                res.push_back(mv);
            }           
        }
        if (x > 0)
        {
            bit_pos = (piece) << 1;
            if ((bit_pos & same_col_occ) == 0)
            {
                auto mv = Move(pos, Position(static_cast<File>(x - 1),
                static_cast<Rank>(y)), board::PieceType::KNIGHT, std::nullopt);
                res.push_back(mv);
            }           
        }
        if (x < 7)
        {
            bit_pos = (piece) >> 1;
            if ((bit_pos & same_col_occ) == 0)
            {
                auto mv = Move(pos, Position(static_cast<File>(x + 1),
                static_cast<Rank>(y)), board::PieceType::KNIGHT, std::nullopt);
                res.push_back(mv);
            }           
        }
        if (x > 0 && y > 0)
        {
            bit_pos = (piece) >> 7;
            if ((bit_pos & same_col_occ) == 0)
            {
                auto mv = Move(pos, Position(static_cast<File>(x - 1),
                static_cast<Rank>(y - 1)), board::PieceType::KNIGHT, std::nullopt);
                res.push_back(mv);
            }           
        }
        if (y > 0)
        {
            bit_pos = (piece) >> 8;
            if ((bit_pos & same_col_occ) == 0)
            {
                auto mv = Move(pos, Position(static_cast<File>(x),
                static_cast<Rank>(y - 1)), board::PieceType::KNIGHT, std::nullopt);
                res.push_back(mv);
            }           
        }
        if (x < 7 && y > 0)
        {
            bit_pos = (piece) >> 9;
            if ((bit_pos & same_col_occ) == 0)
            {
                auto mv = Move(pos, Position(static_cast<File>(x + 1),
                static_cast<Rank>(y - 1)), board::PieceType::KNIGHT, std::nullopt);
                res.push_back(mv);
            }           
        }
    }

    uint64_t generate_king_attacks(Chessboard board, Color color)
    {
        uint64_t res = 0;
        uint64_t bitb =
            color == board::Color::WHITE ? board.boards_[5] : board.boards_[11];
        uint64_t bit_pos = 0;
        
        //NW
        uint64_t mask = 0b01111111011111110111111101111111011111110111111101111111;
        bit_pos = (bitb & mask);
        if (bit_pos)
            res |= bit_pos << 9;
        //N
        mask = 0b11111111111111111111111111111111111111111111111111111111;
        bit_pos = (bitb & mask);
        if (bit_pos)
            res |= bit_pos << 8;
        //NE
        mask = 0b11111110111111101111111011111110111111101111111011111110;
        bit_pos = (bitb & mask);
        if (bit_pos)
            res |= bit_pos << 7;
        //W
        mask =
            0b0111111101111111011111110111111101111111011111110111111101111111;
        bit_pos = (bitb & mask);
        if (bit_pos)
            res |= bit_pos << 1;
        //E
        mask =
            0b1111111011111110111111101111111011111110111111101111111011111110;
        bit_pos = (bitb & mask);
        if (bit_pos)
            res |= bit_pos >> 1;
        //SW
        mask = 0b0111111101111111011111110111111101111111011111110111111100000000;
        bit_pos = (bitb & mask);
        if (bit_pos)
            res |= bit_pos >> 7;
        //S
        mask =
            0b1111111111111111111111111111111111111111111111111111111100000000;
        bit_pos = (bitb & mask);
        if (bit_pos)
            res |= bit_pos >> 8;
        //SE
        mask =
            0b1111111011111110111111101111111011111110111111101111111000000000;
        bit_pos = (bitb & mask);
        if (bit_pos)
            res |= bit_pos >> 9;

        return res;
    }
} // namespace board
