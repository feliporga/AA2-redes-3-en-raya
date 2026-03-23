#pragma once
#include "Scene.h"
#include "Spawner.h"
#include "ImageObject.h"
#include "InputManager.h"
#include "TimeManager.h"
#include "Ship.h"
#include "BulletShip.h"
#include "Alien.h"
#include "BulletAlien.h"
#include "ConstantsManager.h"

#include <iostream>
#include <vector>

class SpaceInvaders : public Scene {
private:
    Ship* ship = nullptr;
    std::vector<Alien*> aliens;
    std::vector<BulletAlien*> bulletsAlien;
    std::vector<BulletShip*> bulletsShip;

    int ENEMY_ROWS_SPACEINVADERS;
    int ENEMY_COLS_SPACEINVADERS;

    int enemieCount = 0;
    int enemiesKilled = 0;

    float alienShootCooldown = 1.0f;
    float alienShootTimer = 0.0f;

    const int maxLives = 3;
    int currentLives = maxLives;

    // Configuraci�n inicial del jugador
    void CreatePlayer() {
        ship = new Ship();
        SPAWN.SpawnObject(ship);
    }

    // Configuraci�n inicial de los enemigos
    void CreateEnemies() {
        for (int row = 0; row < ENEMY_ROWS_SPACEINVADERS; row++) {
            for (int col = 0; col < ENEMY_COLS_SPACEINVADERS; col++) {
                Alien* alien = new Alien(Vector2(100 + col * 60.0f, 100 + row * 50.0f));
                aliens.push_back(alien);
                SPAWN.SpawnObject(alien);
            }
        }
    }

    void HandleEnemyShooting() {
        alienShootTimer -= TIME.GetDeltaTime();
        if (alienShootTimer <= 0.0f) {
            alienShootTimer = alienShootCooldown;

            // Crear un vector para almacenar el enemigo m�s bajo en cada columna
            std::map<int, Alien*> columnFrontAliens;

            // Ajustar la posici�n base (m�nima X) para calcular la columna correctamente
            float minX = 100.0f; // Valor base de X usado en CreateEnemies()
            float columnWidth = 60.0f; // Ancho de cada columna

            // Encontrar el enemigo m�s bajo de cada columna
            for (Alien* alien : aliens) {
                int col = static_cast<int>((alien->GetTransform()->position.x - minX) / columnWidth);
                if (col >= 0) {
                    if (columnFrontAliens.find(col) == columnFrontAliens.end() ||
                        alien->GetTransform()->position.y > columnFrontAliens[col]->GetTransform()->position.y) {
                        columnFrontAliens[col] = alien;
                    }
                }
            }

            // Si no hay columnas v�lidas, no hacer nada
            if (columnFrontAliens.empty()) {
                return;
            }

            // Seleccionar una columna aleatoria que tenga un enemigo v�lido
            std::vector<int> availableColumns;
            for (const auto& pair : columnFrontAliens) {
                availableColumns.push_back(pair.first);
            }

            int randomIndex = std::rand() % availableColumns.size();
            int randomColumn = availableColumns[randomIndex];

            // Disparar desde el enemigo m�s bajo de la columna seleccionada
            if (columnFrontAliens[randomColumn]) {
                Alien* shooter = columnFrontAliens[randomColumn];
                Vector2 bulletPos = shooter->GetTransform()->position + Vector2(20.0f, 30.0f); // Ajustar la posici�n del disparo
                BulletAlien* bullet = new BulletAlien(bulletPos);
                bulletsAlien.push_back(bullet);
                SPAWN.SpawnObject(bullet);

            }
        }
    }


    // Gesti�n del disparo del jugador
    void HandleShooting() {
        if (ship->IsShooting()) {
            Vector2 bulletPos = Vector2(ship->GetTransform()->position.x, ship->GetTransform()->position.y);
            BulletShip* bullet = new BulletShip(bulletPos);
            bulletsShip.push_back(bullet);
            SPAWN.SpawnObject(bullet);

            ship->ResetShoot();
        }
    }

    void UpdateBullets() {
        // Eliminar balas del jugador y alien fuera de pantalla
        auto removeBullets = [](auto& bullets, bool isAlien) {
            for (int i = (int)bullets.size() - 1; i >= 0; i--) {
                auto* bullet = bullets[i];
                Vector2 pos = bullet->GetTransform()->position;
                if (pos.y < 0 || pos.y > RM->WINDOW_HEIGHT) {
                    bullets.erase(bullets.begin() + i);
                    bullet->Destroy();
                }
            }
            };

        removeBullets(bulletsShip, false);
        removeBullets(bulletsAlien, true);
    }

    // Cambiar direcci�n de los enemigos
    void ChangeEnemiesDirection() {
        for (Alien* alien : aliens) {
            alien->ChangeCurrentDirection();
        }
    }

    // Verificar si alg�n enemigo necesita cambiar de direcci�n
    void CheckEnemyDirectionChange() {
        for (Alien* alien : aliens) {
            if (alien->changeDirection) {
                ChangeEnemiesDirection();
                break;
            }
        }
    }

    // Restringir el movimiento del jugador dentro de los l�mites de la pantalla
    void RestrictPlayerMovement() {
        if (ship->GetTransform()->position.x < 0) {
            ship->GetTransform()->position.x = 0;
        }
        else if (ship->GetTransform()->position.x > RM->WINDOW_WIDTH - 50) {
            ship->GetTransform()->position.x = RM->WINDOW_WIDTH - 50;
        }
    }

    void CheckWinCondition() {
        if (enemiesKilled >= enemieCount) {
            enemieCount = 0;
            enemiesKilled = 0;
            SM.SetNextScene("YouWon");
        }
    }

public:
    void CheckBulletAlienCollisions() {
        for (int i = (int)bulletsAlien.size() - 1; i >= 0; i--) {
            BulletAlien* bullet = bulletsAlien[i];
            if (bullet->GetRigidbody()->CheckCollision(ship->GetRigidbody())) {
                // Reducir las vidas del jugador
                ship->ReduceLives();

                // Eliminar la bala alien�gena
                bulletsAlien.erase(bulletsAlien.begin() + i);
                bullet->Destroy();
            }
        }
    }



    SpaceInvaders() : Scene() {}

    void OnEnter() override {
        AM.PlaySong("spaceMusic");
        const auto& spaceInvadersConstants = constantsManager.GetGameConstants("SpaceInvaders");

        ENEMY_ROWS_SPACEINVADERS = spaceInvadersConstants.GetValue("ENEMY_ROWS_SPACEINVADERS", 4);
        ENEMY_COLS_SPACEINVADERS = spaceInvadersConstants.GetValue("ENEMY_COLS_SPACEINVADERS", 8);

        enemieCount = ENEMY_COLS_SPACEINVADERS * ENEMY_ROWS_SPACEINVADERS;

        CreatePlayer();
        CreateEnemies();
    }

    void Update() override {
        Scene::Update();

        HandleShooting();

        CheckEnemyDirectionChange();
        HandleEnemyShooting();
        CheckBulletAlienCollisions();

        Alien::CheckCollisions(bulletsShip, aliens, *ship, enemiesKilled);
        RestrictPlayerMovement();
        UpdateBullets();
        CheckWinCondition();

        if (Input.GetEvent(sf::Keyboard::Key::Escape, KeyState::DOWN)) {
            SM.SetNextScene("MainMenu");
        }
    }

    void OnExit() override {
        AM.StopAudio();
        Scene::OnExit();

        enemieCount = 0;
        enemiesKilled = 0;

        bulletsShip.clear();

        bulletsAlien.clear();

        aliens.clear();
    }

    void Render() override {
        Scene::Render();
    }
};