#pragma once
#include <map>
#include <string>
#include <SFML/Graphics.hpp> // <- Cambio clave: incluimos SFML Graphics

#define RM RenderManager::GetInstance()

class RenderManager {
public:
    static RenderManager* GetInstance() {
        static RenderManager instance;
        return &instance;
    }

private:
    sf::RenderWindow* window; // <- Sustituye a SDL_Window y SDL_Renderer

    // SFML usa Textures y Fonts de forma muy similar a SDL
    std::map<std::string, sf::Texture*> textures;
    std::map<std::string, sf::Font*> fonts;

    sf::Texture* backgroundTexture = nullptr;
    sf::Sprite* backgroundSprite = nullptr; // En SFML necesitas un Sprite para dibujar una Texture

public:
    void Init();
    void Release();
    void ClearScreen();
    void RenderScreen();

    sf::RenderWindow* GetWindow() { return window; } // Ahora devolvemos la ventana de SFML

    const unsigned int WINDOW_WIDTH = 1360;
    const unsigned int WINDOW_HEIGHT = 768;

    void LoadTexture(std::string path);
    sf::Texture* GetTexture(std::string path);

    void LoadFont(std::string path);
    sf::Font* GetFont(std::string path);

    // Dibuja un rectángulo en pantalla
    void DrawRect(int x, int y, int w, int h, std::uint8_t r, std::uint8_t g, std::uint8_t b, std::uint8_t a);

    void SetBackground(const std::string& path);
    void RenderBackground();

private:
    RenderManager() = default;
    RenderManager(RenderManager&) = delete;
    RenderManager& operator= (const RenderManager&) = delete;
    ~RenderManager();

    void CreateWindow();
};