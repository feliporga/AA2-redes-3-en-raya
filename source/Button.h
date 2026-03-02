#pragma once
#include "Object.h"
#include "RenderManager.h"
#include "InputManager.h"
#include "SceneManager.h"
#include "AudioManager.h"
#include "TankEnemy.h"
#include "TankPlayer.h"
#include "Swatter.h"
#include "Fly.h"
#include "Alien.h"
#include <string>

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
    SDL_Color color;
    std::string text;
    ActionType actionType;
    std::string targetScene;
    TextRenderer* textRenderer = nullptr;

    bool isPressed = false;

    int windowWidth = RM->WINDOW_WIDTH;
    int windowHeight = RM->WINDOW_HEIGHT;

public:
    // Constructor
    Button(const Vector2& position,
        const Vector2& size,
        const SDL_Color& color,
        const std::string& text,
        ActionType actionType,
        const std::string& targetScene = "")
        : size(size), color(color), text(text), actionType(actionType), targetScene(targetScene)
    {
        transform->position = position;
        transform->scale = Vector2(1.0f, 1.0f);
        textRenderer = new TextRenderer(transform, text);
        textRenderer->SetColor({ 255, 255, 255, 255 });
    }

    // Destructor
    ~Button() {
        delete textRenderer;
        textRenderer = nullptr;
    }

    void Update() override {
        float mouseX = (float)Input.GetMouseX();
        float mouseY = (float)Input.GetMouseY();

        bool isMouseOver =
            mouseX >= transform->position.x && mouseX <= transform->position.x + size.x &&
            mouseY >= transform->position.y && mouseY <= transform->position.y + size.y;

        if (isMouseOver && Input.GetLeftClick()) {
            if (isPressed)
                return;
            isPressed = true;
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
            case ActionType::SpritesChange:
                switch(std::stoi(text)) {
                case 11:
                    RM->SetBackground("resources/background1.png");
                    break;
                case 12:
                    RM->SetBackground("resources/background2.png");
                    break;
                case 13:
                    RM->SetBackground("resources/background3.png");
                    break;
                case 21:
                    TankEnemy::SetDefaultSpriteTank("resources/enemyTank1.png");
                    Fly::SetDefaultSpriteFly("resources/enemySplat1.png");
                    Alien::SetDefaultSpriteAlien("resources/enemy.png");
                    break;
                case 22:
                    TankEnemy::SetDefaultSpriteTank("resources/enemyTank2.png");
                    Fly::SetDefaultSpriteFly("resources/enemySplat2.png");
                    Alien::SetDefaultSpriteAlien("resources/enemy2.png");
                    break;
                case 23:
                    TankEnemy::SetDefaultSpriteTank("resources/enemyTank3.png");
                    Fly::SetDefaultSpriteFly("resources/enemySplat3.png");
                    Alien::SetDefaultSpriteAlien("resources/enemy3.png");
                    break;
                case 31:
                    TankPlayer::SetShootSFXTank("squirt");
                    TankEnemy::SetShootSFXEnemyDie("explosionBug");
                    Ship::SetShootSFXShip("squirt");
                    Alien::SetShootSFXAlienDie("explosionBug");
                    Swatter::SetShootSFXSwatter("squirt");
                    Swatter::SetShootSFXFlyDie("explosionBug");
                    break;
                case 32:
                    TankPlayer::SetShootSFXTank("bulletshot");
                    TankEnemy::SetShootSFXEnemyDie("explosionShoot");
                    Ship::SetShootSFXShip("bulletshot");
                    Alien::SetShootSFXAlienDie("explosionShoot");
                    Swatter::SetShootSFXSwatter("bulletshot");
                    Swatter::SetShootSFXFlyDie("explosionShoot");
                    break;
                case 33:
                    TankPlayer::SetShootSFXTank("laser");
                    TankEnemy::SetShootSFXEnemyDie("explosionLaser");
                    Ship::SetShootSFXShip("laser");
                    Alien::SetShootSFXAlienDie("explosionLaser");
                    Swatter::SetShootSFXSwatter("laser");
                    Swatter::SetShootSFXFlyDie("explosionLaser");
                    break;
                }
                
                //cambio sprite
                break;
            case ActionType::ExitGame:
                SceneManager::Instance().ExitGame();
                break;
            }
        }
        else {
            isPressed = false;
        }
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