#pragma once
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include "RenderManager.h"
#include <queue>
#include <unordered_map>

#define Input InputManager::Instance()

enum KeyState { EMPTY, DOWN, UP, HOLD, RELEASED };

class InputManager {
private:
    int mouseX, mouseY;
    bool leftClick;

    // SFML 3.0 usa sf::Keyboard::Key como identificador
    std::unordered_map<sf::Keyboard::Key, KeyState> keyReference;

    InputManager() {
        mouseX = 0;
        mouseY = 0;
        leftClick = false;
    }

    InputManager(const InputManager& im) = delete;
    InputManager& operator=(const InputManager& val) = delete;

public:
    static InputManager& Instance() {
        static InputManager instance;
        return instance;
    }

    bool Listen() {
        // Actualizamos estados previos
        for (auto& kv : keyReference) {
            if (kv.second == DOWN)
                kv.second = HOLD;
            else if (kv.second == UP)
                kv.second = RELEASED;
        }

        // Obtener posicion del ratón en SFML
        if (RM->GetWindow()) {
            sf::Vector2i mousePos = sf::Mouse::getPosition(*RM->GetWindow());
            mouseX = mousePos.x;
            mouseY = mousePos.y;
        }

        // Bucle de eventos de SFML 3.0 (El que os enseñó el profe)
        if (RM->GetWindow()) {
            while (const std::optional event = RM->GetWindow()->pollEvent()) {

                // Cerrar ventana
                if (event->is<sf::Event::Closed>()) {
                    return true;
                }
                // Pulsar botón del ratón
                else if (const auto* mouseBtn = event->getIf<sf::Event::MouseButtonPressed>()) {
                    if (mouseBtn->button == sf::Mouse::Button::Left) leftClick = true;
                }
                // Soltar botón del ratón
                else if (const auto* mouseBtnRel = event->getIf<sf::Event::MouseButtonReleased>()) {
                    if (mouseBtnRel->button == sf::Mouse::Button::Left) leftClick = false;
                }
                // Pulsar tecla
                else if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
                    if (keyReference[keyPressed->code] != HOLD)
                        keyReference[keyPressed->code] = DOWN;
                }
                // Soltar tecla
                else if (const auto* keyReleased = event->getIf<sf::Event::KeyReleased>()) {
                    keyReference[keyReleased->code] = UP;
                }
            }
        }
        return false;
    }

    inline int GetMouseX() const { return mouseX; }
    inline int GetMouseY() const { return mouseY; }
    inline bool GetLeftClick() const { return leftClick; }

    inline bool GetEvent(sf::Keyboard::Key input, KeyState inputValue) {
        return keyReference[input] == inputValue;
    }
};