#include <string>
#include <iostream>

#include "pgn-parser.hh"
#include "gametracer.hh"

namespace board
{

    GameTracer::GameTracer(std::string pgn_filepath,
               std::vector<std::string> listener_paths)
    {


        auto pgnmoves = pgn_parser::parse_pgn(pgn_filepath);
        for (auto it : pgnmoves)
            this->pgnmoves_.push_back(it);

        for (auto path : listener_paths)
        {
            auto lib = dlopen(path.c_str(), RTLD_NOW);
            if (0 == lib)
            {
                std::cerr << "GameTracer: couldn't dlopen lib " << path << '\n';
                std::cerr << "GameTracer: " << dlerror() << std::endl;
                continue;
            }
            this->libs_.push_back(lib);

            dlerror(); // Clear errors

            void *createFunc = dlsym(lib, "listener_create");
            if (0 == createFunc)
            {
                std::cerr << "GameTracer: couldn't dlsym create() " << path;
                std::cerr << "\nGameTracer: " << dlerror() << std::endl;
                continue;
            }

            //listener::Listener *(*create)() = reinterpret_cast<listener::Listener *(*)()>(createFunc)();
            listener::Listener *l = reinterpret_cast<listener::Listener *(*)()>(createFunc)();
            this->listeners_.push_back(l);
        }

    }

    GameTracer::~GameTracer()
    {
        std::cerr << "GameTracer: Deleting listeners" << std::endl;
        for (auto it : this->listeners_)
        {
            std::cout << "tst" << std::endl;
            delete it;
        }

        std::cerr << "GameTracer: Dlclosing libs" << std::endl;
        for (auto lib : this->libs_)
        {
            if (lib != nullptr)
            {
                dlclose(lib);
            }
        }
        /*for (auto it = this->listeners_.begin(); it != this->listeners_.end(); ++it)
        {
            std::cerr << "hih";
            //listener::Listener *tmp = *it;
            std::cerr << "hey";
            (*it)->~Listener();
            //delete tmp;
            std::cerr << "hoh";
            //tmp = nullptr;
        }*/
    }

    void GameTracer::play_game()
    {
        std::cout << "GameTracer: in play_game\n";
        std::cout << "Game starting:\n";
        int i = 0;
        for (auto l : this->listeners_)
        {
            std::cout << "[" << i << "]" << std::endl;
            l->on_game_started();
            i++;
        }

        std::cout << "\nDo moves:\n" << std::endl;
        for (auto pmv : this->pgnmoves_)
        {
            auto mv = this->chessboard_.move_from_pgn(pmv);
            if (!this->chessboard_.is_move_legal(mv))
            {
                std::cout << "GameTracer: MOVE NOT LEGAL" << std::endl;
                mv.prettyout();
                continue;
            }
            this->chessboard_.do_move(mv);
            auto col = this->chessboard_.white_turn_ ? Color::WHITE : Color::BLACK;
            for (auto l : this->listeners_)
            {
                l->on_piece_moved(pmv.piece_get(), pmv.start_get(),
                                  pmv.end_get());
                if (pmv.capture_get())
                {
                    PieceType took = PieceType::KING;
                    uint64_t pos = 1L << ((7 - static_cast<int>(mv.to_.file_get())) +
                                   (8 * static_cast<int>(mv.to_.rank_get())));
                    for (int i = 0; i < 6; i++)
                    {
                        if ((this->chessboard_.boards_[i] & pos) ||
                            (this->chessboard_.boards_[i + 6] & pos))
                            took = static_cast<PieceType>(i);
                    }
                    if (took == PieceType::KING)
                        throw std::runtime_error("Nothing to take or King taken.");
                    l->on_piece_taken(took, pmv.end_get());
                }
                if (pmv.promotion_get() != std::nullopt)
                {
                    PieceType prom = pmv.promotion_get().value();
                    if (prom == PieceType::KING || prom == PieceType::PAWN)
                        throw std::runtime_error("Promotion in pawn or King.");
                    l->on_piece_promoted(prom, pmv.end_get());
                }
                if (pmv.piece_get() == PieceType::KING)
                {
                    short file_diff = static_cast<int>(mv.to_.file_get()) -
                                    static_cast<int>(mv.from_.file_get());
                    if (file_diff == 2)
                        l->on_kingside_castling(col);
                    else if (file_diff == -2)
                        l->on_queenside_castling(col);
                }
            }
            this->chessboard_.white_turn_ = !this->chessboard_.white_turn_;
            col = this->chessboard_.white_turn_ ? Color::WHITE : Color::BLACK;
            for (auto l : this->listeners_)
            {
                if (this->chessboard_.is_check())
                {
                    l->on_player_check(col);
                    if (this->chessboard_.generate_legal_moves().size() == 0)
                        l->on_player_mat(col);
                }
                else if (this->chessboard_.generate_legal_moves().size() == 0)
                    l->on_player_pat(col);
            }
        }
        std::cout << "\nEnd of moves\n";
        
        std::cout << "\nGame finished:" << std::endl;
        i = 0;
        for (auto l : this->listeners_)
        {
            std::cout << "[" << i << "]" << std::endl;
            l->on_game_finished();
            i++;
        }
    }
}
