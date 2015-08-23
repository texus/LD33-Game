#include "Game.hpp"

int main()
{
    try
    {
        Game game;
        game.loadResources();
        game.showMenu();
    }
    catch (tgui::Exception& e)
    {
        std::cerr << "Exception thrown: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
