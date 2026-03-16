#pragma once
#include "ImageObject.h"
#include "InputManager.h"
#include "TimeManager.h"
#include "Vector2.h"
#include "BulletShip.h"
#include <vector>
#include <string>

class Alien : public ImageObject {
public:
    bool changeDirection = false;

private:
    float ENEMY_MOVE_SPEED_SPACEINVADERS;
    float ENEMY_DROP_AMOUNT_SPACEINVADERS;
    float ENEMY_MOVE_COOLDOWN_SPACEINVADERS;

    float enemyDirection = 1.0f;
    float enemyMoveTimer = 0.5f;

    static std::string defaultSpriteAlien;
    static std::string dieSFXAlien;

    void MovingState() {
        if (enemyMoveTimer <= 0.0f) {
            transform->position.x += ENEMY_MOVE_SPEED_SPACEINVADERS * enemyDirection;

            if (transform->position.x < 100 || transform->position.x > RM->WINDOW_WIDTH - 100) {
                changeDirection = true;
            }

            enemyMoveTimer = ENEMY_MOVE_COOLDOWN_SPACEINVADERS;
        }
    }

public:
    static void SetDefaultSpriteAlien(const std::string& spritePath) {
        defaultSpriteAlien = spritePath;
    }
    static void SetShootSFXAlienDie(const std::string& sfxName) {
        dieSFXAlien = sfxName;
    }

    Alien(const Vector2& startPos)
        : ImageObject(defaultSpriteAlien, Vector2(0.0f, 0.0f), Vector2(80.0f, 32.0f), 40, 32, 2, true) {
        transform->position = startPos;
        transform->scale = Vector2(0.3f, 0.3f);

        const auto& spaceInvadersConstants = constantsManager.GetGameConstants("SpaceInvaders");

        ENEMY_MOVE_SPEED_SPACEINVADERS = spaceInvadersConstants.GetValue("ENEMY_MOVE_SPEED_SPACEINVADERS", 20.0f);
        ENEMY_DROP_AMOUNT_SPACEINVADERS = spaceInvadersConstants.GetValue("ENEMY_DROP_AMOUNT_SPACEINVADERS", 20.0f);
        ENEMY_MOVE_COOLDOWN_SPACEINVADERS = spaceInvadersConstants.GetValue("ENEMY_MOVE_COOLDOWN_SPACEINVADERS", 20.0f);
    }

    void Update() override {
        ImageObject::Update();

        enemyMoveTimer -= TIME.GetDeltaTime();

        MovingState();
    }

    static void CheckCollisions(std::vector<BulletShip*>& bulletsShip, std::vector<Alien*>& aliens, Ship& ship, int& enemiesKilled) {
        for (int i = (int)bulletsShip.size() - 1; i >= 0; i--) {
            BulletShip* bullet = bulletsShip[i];
            for (int j = (int)aliens.size() - 1; j >= 0; j--) {
                Alien* alien = aliens[j];
                if (bullet->GetRigidbody()->CheckCollision(alien->GetRigidbody())) {
                    // Incrementar el contador de enemigos eliminados
                    enemiesKilled++;

                    // Incrementar el puntaje del jugador
                    ship.IncrementScore(100);

                    // Eliminar el alien y la bala
                    bulletsShip.erase(bulletsShip.begin() + i);
                    aliens.erase(aliens.begin() + j);

                    bullet->Destroy();
                    alien->Destroy();
                    AM.PlayClip(dieSFXAlien, 0);
                    break;
                }
            }
        }
    }



    void ChangeCurrentDirection() {
        enemyDirection *= -1;
        transform->position.y += ENEMY_DROP_AMOUNT_SPACEINVADERS;
        changeDirection = false;

    }
};

// Inicializaci�n del sprite por defecto
std::string Alien::defaultSpriteAlien = "resources/enemy.png";
std::string Alien::dieSFXAlien = "explosionBug";
