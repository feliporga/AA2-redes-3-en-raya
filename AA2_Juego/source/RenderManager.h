#pragma once
#include <map>
#include "SDL.h"
#include <string>
#include "SDL_ttf.h"

#define RM RenderManager::GetInstance()

class RenderManager {
public:
	static RenderManager* GetInstance() {
		static RenderManager instance;
		return &instance;
	}
private:
	SDL_Window* window;
	SDL_Renderer* renderer;
	int currentTextureSet;

	std::map<std::string, SDL_Texture*> textures;
	std::map<std::string, TTF_Font*> fonts;

	SDL_Texture* backgroundTexture = nullptr;

public:
	 void Init();
	 void Release();
	 void ClearScreen();
	 void RenderScreen();

	 SDL_Renderer* GetRenderer() { return renderer; }

	 const unsigned int WINDOW_WIDTH = 1360;
	 const unsigned int WINDOW_HEIGHT = 768;

	 void LoadTexture(std::string path);
	 SDL_Texture* GetTexture(std::string path);

	 void LoadFont(std::string path);
	 TTF_Font* GetFont(std::string path);

	 // Nuevo m�todo para dibujar rect�ngulo
	 void DrawRect(int x, int y, int w, int h, Uint8 r, Uint8 g, Uint8 b, Uint8 a);

	 void SetBackground(const std::string& path);
	 void RenderBackground();
private:
	RenderManager() = default;
	RenderManager(RenderManager&) = delete;
	RenderManager& operator= (const RenderManager&) = delete;
	~RenderManager();

	void InitSDL();
	void CreateWindowAndRenderer();

};