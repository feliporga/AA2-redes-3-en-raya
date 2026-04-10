#pragma once
#include "Object.h"
#include "RenderManager.h"
#include "InputManager.h"
#include "SceneManager.h"
#include "AudioManager.h"
#include <string>
#include <SFML/Graphics.hpp>

class Button : public Object {
public:
    enum class ActionType {
        ChangeScene,
        ToggleAudio,
        SpritesChange,
        ExitGame
    };

private:
    Vector2 size;
    sf::Color color;
    std::string text;
    ActionType actionType;
    std::string targetScene;
    TextRenderer* textRenderer = nullptr;

    bool prevLeftClick; // <-- NUEVA VARIABLE DE SEGURIDAD

    int windowWidth = RM->WINDOW_WIDTH;
    int windowHeight = RM->WINDOW_HEIGHT;

public:
    Button(const Vector2& position,
        const Vector2& size,
        const sf::Color& color,
        const std::string& text,
        ActionType actionType,
        const std::string& targetScene = "")
        : size(size), color(color), text(text), actionType(actionType), targetScene(targetScene)
    {
        transform->position = position;
        transform->scale = Vector2(1.0f, 1.0f);
        textRenderer = new TextRenderer(transform, text);
        textRenderer->SetColor(sf::Color::White);

        // MAGIA AQUÍ: Al nacer el botón, miramos si el ratón ya estaba pulsado
        // Así evitamos que se pulse solo al cambiar de escena
        prevLeftClick = Input.GetLeftClick();
    }

    ~Button() {
        delete textRenderer;
        textRenderer = nullptr;
    }

    void Update() override {
        float mouseX = (float)Input.GetMouseX();
        float mouseY = (float)Input.GetMouseY();
        bool currentLeftClick = Input.GetLeftClick();

        bool isMouseOver =
            mouseX >= transform->position.x && mouseX <= transform->position.x + size.x &&
            mouseY >= transform->position.y && mouseY <= transform->position.y + size.y;

        // Solo se activa si es el MILISEGUNDO EXACTO en el que pulsas el botón
        if (isMouseOver && currentLeftClick && !prevLeftClick) {
            switch (actionType) {
            case ActionType::ChangeScene:
                SM.SetNextScene(targetScene);
                break;
            case ActionType::ToggleAudio:
            {
                if (AM.GetMuted()) {
                    AM.UnMute();
                    AM.PlaySong("menuMusic");
                }
                else {
                    AM.Mute();
                    AM.StopAudio();
                }
                break;
            }
            case ActionType::ExitGame:
                SceneManager::Instance().ExitGame();
                break;
            }
        }

        // Guardamos el estado para el siguiente frame
        prevLeftClick = currentLeftClick;
    }

    void Render() override {
        RM->DrawRect((int)transform->position.x,
            (int)transform->position.y,
            (int)size.x,
            (int)size.y,
            color.r, color.g, color.b, color.a);

        if (textRenderer) {
            textRenderer->Render();
        }
    }
};