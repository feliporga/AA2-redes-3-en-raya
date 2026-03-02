#pragma once
#include "ImageObject.h"
#include "TimeManager.h"
#include "Vector2.h"
#include "RenderManager.h"
#include "BulletTank.h"
#include "ConstantsManager.h"
#include "Button.h"
#include "TankPlayer.h"


#include <vector>
#include <cstdlib>
#include <ctime>
#include <string>

class TankEnemy : public ImageObject {
private:
    Vector2 velocity;

    float ENEMY_SPEED       = 50.0f;
    float HORIZONTAL_SPEED  = 50.0f;
    float STATE_DURATION    = 2.0f;
    float PAUSE_DURATION    = 1.0f;

    float stateTimer        = 0.0f;
    float shotTimer         = 0.0f;
    enum MovementState { MOVING_DIAGONAL_LEFT, MOVING_HORIZONTAL_RIGHT, MOVING_HORIZONTAL_LEFT, PAUSING };
    MovementState currentState = MOVING_DIAGONAL_LEFT;
    MovementState nextState    = MOVING_HORIZONTAL_RIGHT;

    float width  = 80.0f;
    float height = 32.0f;

    static std::string defaultSpriteTank;
    static std::string dieSFXTank;

    float randomShotInterval() {
        float range = 6.0f - 3.0f;
        float randomFactor = static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX);
        return 3.0f + randomFactor * range;
    }

public:
    static Vector2& playerPosition() {
        static Vector2 pos(0.0f, 0.0f);
        return pos;
    }
    static std::vector<BulletTank*> enemyBullets;

    static void SetDefaultSpriteTank(const std::string& spritePath) {
        defaultSpriteTank = spritePath;
    }

    static void SetShootSFXEnemyDie(const std::string& sfxName) {
        dieSFXTank = sfxName;
    }

    TankEnemy()
        : ImageObject(defaultSpriteTank, Vector2(0.0f, 0.0f), Vector2(80.0f, 32.0f), 40, 32, 2, true)
    {
        GetTransform()->scale = Vector2(0.5f, 0.5f);

        const auto& tanksConstants = constantsManager.GetGameConstants("Tanks");
        ENEMY_SPEED       = tanksConstants.GetValue("ENEMY_SPEED",       50.0f);
        HORIZONTAL_SPEED  = tanksConstants.GetValue("HORIZONTAL_SPEED",  50.0f);
        STATE_DURATION    = tanksConstants.GetValue("STATE_DURATION",    2.0f);
        PAUSE_DURATION    = tanksConstants.GetValue("PAUSE_DURATION",    1.0f);

        stateTimer = STATE_DURATION;
        shotTimer = randomShotInterval();  // Inicializar con un intervalo aleatorio
    }

    void SpawnEnemyTank() {
        float randomX = static_cast<float>(std::rand() % RM->WINDOW_WIDTH);
        float randomY = static_cast<float>(std::rand() % RM->WINDOW_HEIGHT);
        transform->position = Vector2(randomX, randomY);

        // Asignar una velocidad aleatoria
        float angle = static_cast<float>(std::rand() % 360) * 3.14159265f / 180.0f;
        velocity = Vector2(cosf(angle), sinf(angle)) * ENEMY_SPEED;
    }


    void Update() override {
        ImageObject::Update();

        // Movimiento aleatorio
        transform->position = transform->position + velocity * TIME.GetDeltaTime();

        // Rebote en los bordes de la pantalla
        if (transform->position.x < 0 || transform->position.x > RM->WINDOW_WIDTH) {
            velocity.x = -velocity.x;
            // Asegurarse de que la posici�n est� dentro de la pantalla
            transform->position.x = std::max(0.0f, std::min(transform->position.x, static_cast<float>(RM->WINDOW_WIDTH)));
        }
        if (transform->position.y < 0 || transform->position.y > RM->WINDOW_HEIGHT) {
            velocity.y = -velocity.y;
            // Asegurarse de que la posici�n est� dentro de la pantalla
            transform->position.y = std::max(0.0f, std::min(transform->position.y, static_cast<float>(RM->WINDOW_HEIGHT)));
        }

        // Continuar con la l�gica de disparo y otras funciones si es necesario
        shotTimer -= TIME.GetDeltaTime();
        if (shotTimer <= 0.0f) {
            ShootAtPlayer();
            shotTimer = randomShotInterval();
        }

        // Destruir si sale de la pantalla por arriba (opcional, seg�n l�gica de tu juego)
        if (transform->position.y + transform->scale.y * height < 0) {
            Destroy();
        }
    }


    static void CheckCollisions(std::vector<BulletTank*>& bullets, std::vector<TankEnemy*>& enemies, TankPlayer* player) {
        for (int i = (int)bullets.size() - 1; i >= 0; i--) {
            BulletTank* bullet = bullets[i];
            for (int j = (int)enemies.size() - 1; j >= 0; j--) {
                TankEnemy* enemy = enemies[j];
                if (bullet->GetRigidbody()->CheckCollision(enemy->GetRigidbody())) {
                    // Incrementar el puntaje del jugador
                    player->AddScore(100); // Llama a un nuevo m�todo para incrementar el puntaje

                    // Eliminar el tanque y la bala
                    bullets.erase(bullets.begin() + i);
                    enemies.erase(enemies.begin() + j);
                    bullet->Destroy();
                    enemy->Destroy();
                    AM.PlayClip(dieSFXTank, 0);
                    break;
                }
            }
        }
    }


private:
    void MoveDiagonally(int horizontalDirection) {
        transform->position.x += horizontalDirection * ENEMY_SPEED * TIME.GetDeltaTime();
        transform->position.y -= ENEMY_SPEED * TIME.GetDeltaTime();
    }

    void MoveHorizontally(int horizontalDirection) {
        transform->position.x += horizontalDirection * HORIZONTAL_SPEED * TIME.GetDeltaTime();
    }

    void TransitionToState(MovementState newState, MovementState next) {
        currentState = newState;
        if (newState == PAUSING) {
            nextState = next;
            stateTimer = PAUSE_DURATION;
        } else {
            stateTimer = STATE_DURATION;
        }
    }

    void ShootAtPlayer() {
        Vector2 ePos = transform->position;
        Vector2 toPlayer = playerPosition() - ePos;
        toPlayer.Normalize();

        BulletTank* bullet = new BulletTank(ePos, toPlayer);
        SPAWN.SpawnObject(bullet);
        enemyBullets.push_back(bullet);
    }
};

// Inicializaci�n del sprite por defecto
std::string TankEnemy::defaultSpriteTank = "resources/enemyTank1.png";
std::string TankEnemy::dieSFXTank = "explosionBug";
std::vector<BulletTank*> TankEnemy::enemyBullets;