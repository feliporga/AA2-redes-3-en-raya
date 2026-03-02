#pragma once
#include "ImageObject.h"
#include "InputManager.h"
#include "TimeManager.h"
#include "AudioManager.h"
#include "Vector2.h"
#include "TextObject.h"
#include "RenderManager.h"
#include "Spawner.h"

class Splat; // Forward declaration

class Swatter : public ImageObject {
private:
    Splat* parentScene;
    enum SwatterState { MOVING, HIT, STUNNED };
    SwatterState currentState = MOVING;
    float stunnedTimer = 0.0f;
    float maxStunTimer = 2.0f;
    float movementFactor = 10000.0f;
    bool hitFly = false;
    float distance;


    int windowWidth = RM->WINDOW_WIDTH;
    int windowHeight = RM->WINDOW_HEIGHT;

    TextObject* livesTextSplat = nullptr;
    TextObject* highScoreTextSplat = nullptr;
    TextObject* scoreTextSplat = nullptr;
    const int maxLives = 3;
    int currentLives = maxLives;
    int scoreSplat = 0;
    int highScoreSplat = scoreSplat;

    static std::string dieSFXFly;
    static std::string shootSFXSwatter;

    void MovingState();
    void HitState();
    void StunnedState();

public:
    int GetLives() const { return currentLives; }
    void LoseLife();

    bool IsGameOver() const { return currentLives <= 0; }

    Swatter(Splat* scene)
        : ImageObject("resources/PlayerSplat.png", Vector2(0.0f, 0.0f), Vector2(120.0f, 30.0f), 60, 30, 2, true),
        parentScene(scene)
    {
        float posInitX = RM->WINDOW_WIDTH / 2;
        float posInitY = RM->WINDOW_HEIGHT / 2;

        transform->position = Vector2(posInitX, posInitY);
        transform->scale = Vector2(0.2f, 0.2f);
        physics->SetLinearDrag(30.0f);

    }

    static void SetShootSFXSwatter(const std::string& sfxName) {
        shootSFXSwatter = sfxName;
    }
    static void SetShootSFXFlyDie(const std::string& sfxName) {
        dieSFXFly = sfxName;
    }

    inline void ShootSound() { AM.PlayClip(shootSFXSwatter, 0); }

    void Update() override;
};
