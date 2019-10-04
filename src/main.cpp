// standard c++ lib
#include <iostream>
#include <thread>

// additional libs
#include <X11/Xlib.h>

// project headers
#include "headers/config.hpp"
#include "headers/game.hpp"

int main()
{
    XInitThreads();
    std::cerr << "Pacman launching" << std::endl;
    game();
    return 0;
}

