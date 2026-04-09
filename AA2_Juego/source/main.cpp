#include "Game.h"
#include "InputManager.h"
#include "ConstantsManager.h"

#include <exception>
#include <iostream>

#define Input InputManager::Instance()

GameConstantsManager constantsManager;

// En SFML puro (sin SDL), el main vuelve a ser el estándar de C++
int main() {
    // Cargar las constantes desde el archivo XML
    constantsManager.LoadFromXML("resources/valuesGame.xml");

    Game game;

    srand((unsigned int)time(NULL));

    try
    {
        // Esto inicializa el RenderManager y crea tu ventana de verdad
        game.Init();
    }
    catch (std::exception& exception)
    {
        std::cout << "Error: " << exception.what() << std::endl;
        game.Release();
        return -1;
    }

    bool playing = true;
    while (playing)
    {
        TIME.Update();

        if (TIME.ShouldUpdateGame())
        {
            // Input.Listen() devuelve true si el jugador pulsa la 'X' de cerrar ventana
            playing = !Input.Listen();

            game.Update();
            game.Render();

            TIME.ResetDeltaTime();
        }
    }

    game.Release();

    return 0;
}