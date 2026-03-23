#include <SFML/Graphics.hpp>

#include "Game.h"
#include "InputManager.h"
#include "ConstantsManager.h"

#include <exception>
#include <iostream>

#define Input InputManager::Instance()

GameConstantsManager constantsManager;

int main(int argc, char* args[]) {
	
	
	sf::RenderWindow window(sf::VideoMode({ 200, 200 }), "SFML works!");
	sf::CircleShape shape(100.f);
	shape.setFillColor(sf::Color::Green);

	while (window.isOpen())
	while (window.isOpen())
	{
		while (const std::optional event = window.pollEvent())
		{
			if (event->is<sf::Event::Closed>())
				window.close();
		}

		window.clear();
		window.draw(shape);
		window.display();
	}
	
	
	
	
	/*
	// Cargar las constantes desde el archivo XML
	constantsManager.LoadFromXML("resources/valuesGame.xml");

	Game game;

	srand(time(NULL));

	try
	{
		game.Init();
	}
	catch (std::exception& exception)
	{
		std::cout << "Error: " << exception.what();
		game.Release();
		return -1;
	}

	bool playing = true;
	while (playing)
	{
		TIME.Update();
		if (TIME.ShouldUpdateGame())
		{
			playing = !Input.Listen();
			game.Update();
			game.Render();
			TIME.ResetDeltaTime();
		}
	}

	game.Release();
	*/
	return 0;
}