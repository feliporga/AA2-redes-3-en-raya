#pragma once
#include "Scene.h"
#include "Spawner.h"
#include "ImageObject.h"
#include "InputManager.h"
#include "TimeManager.h"
#include "TankPlayer.h"
#include "TankEnemy.h"
#include "BulletTank.h"
#include "TankCannon.h"
#include "EnemyCannon.h"
#include "ConstantsManager.h"
#include "TextObject.h"
#include "Vector2.h"

#include <vector>
#include <cstdlib>
#include <ctime>

class Tanks : public Scene {
private:
    // Variables relacionadas con oleadas
    int TOTAL_WAVES_TANKS;
    int initialEnemies = 2;
    int currentWave;
    int count = 0;

    // Objetos y datos del jugador
    TankPlayer* playerTank = nullptr;
    TextObject* playerLivesText = nullptr;
    TextObject* waveText = nullptr;

    // Contenedores de enemigos y balas
    std::vector<TankEnemy*> enemyTanks;
    std::vector<BulletTank*> bullets;
    std::vector<BulletTank*> enemyBullets;

    // Variables de tiempo y vidas
    float ENEMY_SPAWN_INTERVAL = 0.0f;
    float enemySpawnTimer = 0.0f;
    const int maxLives = 3;
    int currentLives = maxLives;

    // --- Métodos privados ---

    // Inicia una nueva oleada
    void StartWave(int wave) {
        count += initialEnemies;
        for (int i = 0; i < count; i++) {
            SpawnEnemy();
        }
    }

    // Lógica de disparo del jugador
    void HandlePlayerShooting() {
        if (playerTank->IsShootingTank()) {
            Vector2 tankPos = playerTank->GetTransform()->position;
            Vector2 mousePos((float)Input.GetMouseX(), (float)Input.GetMouseY());
            Vector2 direction = mousePos - tankPos;
            direction.Normalize();

            BulletTank* bullet = new BulletTank(tankPos, direction);
            bullets.push_back(bullet);
            SPAWN.SpawnObject(bullet);
            playerTank->ShootSound();
            playerTank->ResetShootTank();
        }
    }

    // Maneja colisiones entre balas enemigas y el jugador
    void HandleEnemyBulletCollisions() {
        for (int i = (int)TankEnemy::enemyBullets.size() - 1; i >= 0; i--) {
            BulletTank* enemyBullet = TankEnemy::enemyBullets[i];
            if (enemyBullet->GetRigidbody()->CheckCollision(playerTank->GetRigidbody())) {
                currentLives--;
                playerLivesText->SetText("LIVES: " + std::to_string(currentLives));
                AM.PlayClip("die", 0);

                ResetPlayerPosition();

                if (currentLives <= 0) {
                    currentLives = 3;
                    SM.SetNextScene("GameOver");
                    AM.PlayClip("GameOver", 0);
                }

                enemyBullet->Destroy();
                TankEnemy::enemyBullets.erase(TankEnemy::enemyBullets.begin() + i);
            }
        }
    }

    // Elimina balas fuera de la pantalla
    void RemoveOffscreenBullets() {
        auto removeBullets = [](std::vector<BulletTank*>& bullets) {
            for (int i = (int)bullets.size() - 1; i >= 0; i--) {
                Vector2 pos = bullets[i]->GetTransform()->position;
                if (pos.y < 0 || pos.y > RM->WINDOW_HEIGHT || pos.x < 0 || pos.x > RM->WINDOW_WIDTH) {
                    bullets[i]->Destroy();
                    bullets.erase(bullets.begin() + i);
                }
            }
            };

        removeBullets(bullets);
        removeBullets(TankEnemy::enemyBullets);
    }

    // Actualiza los enemigos y verifica colisiones con el jugador
    void UpdateEnemies() {
        for (int i = (int)enemyTanks.size() - 1; i >= 0; i--) {
            TankEnemy* enemy = enemyTanks[i];
            enemy->Update();

            if (enemy->GetRigidbody()->CheckCollision(playerTank->GetRigidbody())) {
                currentLives--;
                playerLivesText->SetText("LIVES: " + std::to_string(currentLives));
                AM.PlayClip("die", 0);

                ResetPlayerPosition();

                if (currentLives <= 0) {
                    currentLives = 3;
                    SM.SetNextScene("GameOver");
                }
                break;
            }

            if (enemy->IsPendingDestroy()) {
                enemyTanks.erase(enemyTanks.begin() + i);
            }
        }

        TankEnemy::CheckCollisions(bullets, enemyTanks, playerTank);

        for (int i = (int)enemyTanks.size() - 1; i >= 0; i--) {
            if (enemyTanks[i]->IsPendingDestroy()) {
                enemyTanks.erase(enemyTanks.begin() + i);
            }
        }
    }

    // Verifica oleadas
    void CheckWaveCompletion() {
        if (enemyTanks.empty()) {
            if (currentWave < TOTAL_WAVES_TANKS) {
                currentWave++;
                waveText->SetText("Wave: " + std::to_string(currentWave));
                StartWave(currentWave);
            }
            else if (TOTAL_WAVES_TANKS == currentWave) {
                SM.SetNextScene("YouWon");
                count = 0;
            }
        }
    }

    void ResetPlayerPosition() {
        playerTank->GetTransform()->position = Vector2(RM->WINDOW_WIDTH / 2, RM->WINDOW_HEIGHT / 2);
        playerTank->GetRigidbody()->SetVelocity(Vector2(0.0f, 0.0f));
    }

    // Spawn Enemigo
    void SpawnEnemy() {
        TankEnemy* enemy = new TankEnemy();
        enemy->SpawnEnemyTank();
        enemyTanks.push_back(enemy);
        SPAWN.SpawnObject(enemy);

        EnemyCannon* enemyCannon = new EnemyCannon(enemy, 0.0f, 5.0f);
        SPAWN.SpawnObject(enemyCannon);
    }

public:
    Tanks() : Scene() {
        std::srand((unsigned int)std::time(0));
    }

    virtual void OnEnter() override {
        AM.PlaySong("tanksMusic");
        const auto& tanksConstants = constantsManager.GetGameConstants("Tanks");
        ENEMY_SPAWN_INTERVAL = tanksConstants.GetValue("ENEMY_SPAWN_INTERVAL", 2.0f);
        TOTAL_WAVES_TANKS = tanksConstants.GetValue("TOTAL_WAVES_TANKS", 3);

        enemySpawnTimer = ENEMY_SPAWN_INTERVAL;

        playerTank = new TankPlayer();
        SPAWN.SpawnObject(playerTank);

        playerLivesText = new TextObject("LIVES: " + std::to_string(currentLives));
        playerLivesText->GetTransform()->position = Vector2(RM->WINDOW_WIDTH / 2, 70);
        SPAWN.SpawnObject(playerLivesText);

        TankCannon* playerCannon = new TankCannon(playerTank, 0.0f, 5.0f);
        SPAWN.SpawnObject(playerCannon);

        currentWave = 1;
        waveText = new TextObject("Wave: " + std::to_string(currentWave));
        waveText->GetTransform()->position = Vector2(125, 120);
        SPAWN.SpawnObject(waveText);

        StartWave(currentWave);
    }

    virtual void Update() override {
        Scene::Update();

        TankEnemy::playerPosition() = playerTank->GetTransform()->position;

        HandlePlayerShooting();
        HandleEnemyBulletCollisions();
        RemoveOffscreenBullets();
        UpdateEnemies();
        CheckWaveCompletion();

        if (Input.GetEvent(SDLK_ESCAPE, KeyState::DOWN)) {
            SM.SetNextScene("MainMenu");
        }
    }

    virtual void OnExit() override {
        AM.StopAudio();
        for (auto bullet : bullets) {
            if (bullet) bullet->Destroy();
        }
        bullets.clear();

        for (auto enemyBullet : TankEnemy::enemyBullets) {
            if (enemyBullet) enemyBullet->Destroy();
        }
        TankEnemy::enemyBullets.clear();

        for (auto enemy : enemyTanks) {
            if (enemy) enemy->Destroy();
        }
        enemyTanks.clear();

        if (playerTank) {
            playerTank->Destroy();
            playerTank = nullptr;
        }

        if (playerLivesText) {
            playerLivesText->Destroy();
            playerLivesText = nullptr;
        }

        Scene::OnExit();
    }

    virtual void Render() override {
        Scene::Render();
    }
};
