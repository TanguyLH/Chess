#include <cctype>
#include <fstream>
#include <iostream>
#include <string>

#include "chessboard.hh"

namespace fen
{
    uint64_t parse_fen_file(const std::string &file);
}