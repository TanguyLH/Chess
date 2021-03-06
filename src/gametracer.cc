#include <string>
#include <iostream>

#include "gametracer.hh"

namespace board
{

    GameTracer::GameTracer(std::string pgn_filepath,
               std::vector<std::string> listener_paths)
    {


        pgn_filepath = pgn_filepath + "INUTILE";

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

            void *(*create)() = reinterpret_cast<void *(*)()>(createFunc);
            auto l = static_cast<listener::Listener *>(create());
            this->listeners_.push_back(l);
        }

    }

    GameTracer::~GameTracer()
    {
        std::cerr << "GameTracer: Dlclosing libs" << std::endl;
        for (auto lib : this->libs_)
        {
            if (lib != nullptr)
                dlclose(lib);
        }

        /*std::cerr << "GameTracer: Deleting listeners" << std::endl;
        for (auto it = this->listeners_.begin(); it != this->listeners_.end(); ++it)
        {
            std::cerr << "hih";
            listener::Listener *tmp = *it;
            std::cerr << "hey";
            delete tmp;
            std::cerr << "hoh";
            tmp = nullptr;
        }*/
    }

    void GameTracer::play_game()
    {
        std::cout << "GameTracer: in play_game\n";
        std::cout << "For now nothing to see but let's try listeners.\n\n";
        std::cout << "Game starting:\n";
        int i = 0;
        for (auto l : this->listeners_)
        {
            std::cout << "[" << i << "]" << std::endl;
            l->on_game_started();
            i++;
        }

        std::cout << "\nMoving a piece (rook):" << std::endl;
        i = 0;
        for (auto l : this->listeners_)
        {
            std::cout << "[" << i << "]" << std::endl;
            l->on_piece_moved(PieceType::ROOK,
                              Position(File::G, Rank::TWO),
                              Position(File::G, Rank::SEVEN));
            i++;
        }
        
        std::cout << "\nBlack won !:\n";
        i = 0;
        for (auto l : this->listeners_)
        {
            std::cout << "[" << i << "]" << std::endl;
            l->on_player_mat(Color::WHITE);
            i++;
        }

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
