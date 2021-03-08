#include <boost/program_options.hpp>
#include <iostream>

#include "fen-parse.hh"
#include "move.hh"
#include "rule.hh"
using namespace boost::program_options;

void to_cout(const std::vector<std::string> &v)
{
    std::copy(v.begin(), v.end(),
              std::ostream_iterator<std::string>{ std::cout, "\n" });
}

int main(int argc, char *argv[])
{
    options_description desc{ "Allowed Options" };
    desc.add_options()("help,h", "show usage")("pgn", value<std::string>(),
                                               "path to the PGN game file")(
        "listeners,l", value<std::vector<std::string>>()->multitoken(),
        "list of paths to listener plugins")("perft", value<std::string>(),
                                             "path to a perft file");

    variables_map vm;
    store(parse_command_line(argc, argv, desc), vm);
    notify(vm);

    if (vm.count("help"))
        std::cout << desc << '\n';

    if (vm.count("pgn"))
        std::cout << "\npath to the PGN game file: "
                  << vm["pgn"].as<std::string>() << "\n\n";

    if (vm.count("listeners"))
    {
        std::cout << "\nlisteners\n-----------------------\n";
        for (auto it = vm["listeners"].as<std::vector<std::string>>().begin();
             it != vm["listeners"].as<std::vector<std::string>>().end(); it++)
            std::cout << "listener: " << *it << std::endl;
        std::cout << "\n";
    }

    if (vm.count("perft"))
    {
        std::cout << "\npath to a perft file: " << vm["perft"].as<std::string>()
                  << "\n\n";
        fen::parse_fen_file(vm["perft"].as<std::string>());
    }

    /*
    board::Chessboard cb;
    cb.print_board();
    uint64_t knight_attacks = generate_pawn_attacks(cb, board::Color::BLACK);
    board::print_BitBoard(knight_attacks);
    std::vector<board::Move> moves;
    generate_pawn_moves(cb, board::Color::WHITE, moves);
    for (auto i = moves.begin(); i != moves.end(); i++)
        (*i).pretty();
    cb.do_move(moves[moves.size() - 3]);
    cb.print_board();
    */
    return 0;
}
