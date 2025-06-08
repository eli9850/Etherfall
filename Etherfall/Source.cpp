#include "GameManager/ResourceManager/ResourceManager.h"
#include "GameManager/GameManager.h"
#include <iostream>
#include <Windows.h>

int main()
{
    Etherfall::g_resource_manager = std::make_unique<Etherfall::ResourceManager>(RESOURCE_DIR);
    Etherfall::GameManager game_manager({800, 600});
    //Etherfall::GameManager game_manager;
    game_manager.run();
    //game_manager.run_test();
}
