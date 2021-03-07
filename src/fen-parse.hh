#include <cctype>
#include <fstream>
#include <iostream>
#include <string>

#include "chessboard.hh"

namespace fen
{
    board::Chessboard parse_fen_file(const std::string &file);
}
