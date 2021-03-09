#include "depth.hh"

namespace fen
{
    uint64_t generate_all_moves(board::Chessboard &board, int depth)
    {
        std::vector<board::Move> r = board.generate_legal_moves();
        int res = r.size();
        if (depth == 1)
            return res;
        res = 0;
        for (auto &mv : r)
        {
            board::Chessboard cpy = board;
            cpy.do_move(mv);
            cpy.white_turn_ = !(cpy.white_turn_);
            res += generate_all_moves(cpy, depth - 1);
        }
        return res;
    }
} // namespace fen