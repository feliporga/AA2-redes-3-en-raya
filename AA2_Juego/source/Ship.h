#pragma once
#include "ImageObject.h"
#include "InputManager.h"
#include "TimeManager.h"
#include "Vector2.h"
#include "TextObject.h"
#include "Spawner.h"
#include "ConstantsManager.h"
#include "AudioManager.h"

class Ship : public ImageObject {
private:
    int windowWidth = RM->WINDOW_WIDTH;
    int windowHeight = RM->WINDOW_HEIGHT;
    float PLAYER_SPEED_SPACEINVADERS;
    
    bool shootingShip = false;

    float bulletCooldown = 1.0f;
    float bulletTimer = 0.0f;
    
    int scoreSpaceInvaders = 0;
    int highScoreSpaceInvaders = highScoreSpaceInvaders;

    const int maxLives = 3;
    int currentLives = maxLives;

    TextObject* livesTextSpaceInvaders = nullptr;
    TextObject* highScoreTextSpaceInvaders = nullptr;
    TextObject* scoreTextSpaceInvaders = nullptr;

    static std::string shootSFXShip;

    void MovingState();

public:
    Ship()
        : ImageObject("resources/PlayerSpace.png", Vector2(0.0f, 0.0f), Vector2(120.0f, 30.0f), 60, 30, 0, false) {
        transform->position = Vector2(600, 700);
        transform->scale = Vector2(0.5f, 0.5f);

        const auto& spaceInvadersConstants = constantsManager.GetGameConstants("SpaceInvaders");

        PLAYER_SPEED_SPACEINVADERS = spaceInvadersConstants.GetValue("PLAYER_SPEED_SPACEINVADERS", 200.0f);
    }

    static void SetShootSFXShip(const std::string& sfxName) {
        shootSFXShip = sfxName;
    }

    void ReduceLives();


    void IncrementScore(int amount);

    void Update() override;
    
    bool IsShooting() { return shootingShip; }

    inline void ResetShoot() { shootingShip = false; }

    inline void ShootSound() { AM.PlayClip(shootSFXShip, 0); }
};