#include "RenderManager.h"
#include "SDL_image.h"
#include <iostream>
#include <cassert>

void RenderManager::Init()
{
	try {
		InitSDL();
		CreateWindowAndRenderer();

		SetBackground("resources/background1.png");
	}
	catch (std::exception& exception)
	{
		std::cout << exception.what();
		SDL_Quit();
		return;
	}
}

void RenderManager::Release()
{
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

void RenderManager::ClearScreen()
{
	SDL_RenderClear(renderer);
}

void RenderManager::RenderScreen()
{
	SDL_RenderPresent(renderer); //imprime render
}

void RenderManager::LoadTexture(std::string path)
{
	SDL_Surface* surf = IMG_Load(path.c_str());
	assert(surf);

	textures[path] = SDL_CreateTextureFromSurface(renderer, surf);
	assert(textures[path]);

	SDL_FreeSurface(surf);
}

SDL_Texture* RenderManager::GetTexture(std::string path)
{
	//lo busca emtre las texturas, si llega al final es q no lo a encontrado y salta al else
	if (textures.find(path) != textures.end()) 
		return textures[path];
	else
		return nullptr;
}

void RenderManager::LoadFont(std::string path)
{
	if (fonts.find(path) != fonts.end())
		return;

	fonts[path] = TTF_OpenFont(path.c_str(), 24);
}

TTF_Font* RenderManager::GetFont(std::string path)
{
	if (fonts.find(path) != fonts.end())
		return fonts[path];

	return nullptr;
}

RenderManager::~RenderManager()
{
	for (std::map<std::string, SDL_Texture*>::iterator it = textures.begin();
		it != textures.end();
		it++) {
		SDL_DestroyTexture(it->second);
	}
}

void RenderManager::InitSDL() //esta inicializa SDL
{
	int result = SDL_Init(SDL_INIT_VIDEO);

	bool success = result >= 0;
	if (!success)
		throw SDL_GetError();

	if (TTF_Init() == -1)
		throw SDL_GetError();
}

void RenderManager::CreateWindowAndRenderer() //esta crea ventana y renderer
{
	int result = SDL_CreateWindowAndRenderer( //crear ventana
		WINDOW_WIDTH,
		WINDOW_HEIGHT,
		SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE,
		&window,
		&renderer);

	bool success = result >= 0;
	if (!success)
		throw SDL_GetError();

	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); //color de fondo de ventana
}

void RenderManager::DrawRect(int x, int y, int w, int h, Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
	SDL_SetRenderDrawColor(renderer, r, g, b, a);
	SDL_Rect rect = { x, y, w, h };
	SDL_RenderFillRect(renderer, &rect);
	// Opcionalmente, volver a setear el color por defecto
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
}

void RenderManager::SetBackground(const std::string& path) {
	if (backgroundTexture) {
		SDL_DestroyTexture(backgroundTexture);
	}

	SDL_Surface* surf = IMG_Load(path.c_str());
	assert(surf);

	backgroundTexture = SDL_CreateTextureFromSurface(renderer, surf);
	assert(backgroundTexture);

	SDL_FreeSurface(surf);
}

void RenderManager::RenderBackground() {
	if (backgroundTexture) {
		SDL_RenderCopy(renderer, backgroundTexture, nullptr, nullptr);
	}
}
