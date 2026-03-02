#pragma once
#include "Scene.h"
#include "Spawner.h"
#include "Swatter.h"
#include "Fly.h"
#include "InputManager.h"
#include "TimeManager.h"
#include "SceneManager.h"
#include "ConstantsManager.h"
#include "RenderManager.h"

#include <vector>
#include <cstdlib>
#include <ctime>

class Splat : public Scene {
private:
    Swatter* swatter = nullptr;
    std::vector<Fly*> flies;

    int killCount = 0;
    int maxKills = 10;
    TextObject* killCountText = nullptr;

    // constantes
    
    float SPAWN_INTERVAL;
    float SWATTER_COOLDOWN;

    float spawnTimer = 0.0f;
    float swatterCooldownTimer = 0.0f;
    bool swatterStunned = false;

public:
    Splat() : Scene() {
        std::srand((unsigned int)std::time(0));
    }

    void OnEnter() override {
        AM.PlaySong("moscaMusic");
        const auto& splatConstants = constantsManager.GetGameConstants("Splat");
        SPAWN_INTERVAL = splatConstants.GetValue("SPAWN_INTERVAL", 2.0f);
        SWATTER_COOLDOWN = splatConstants.GetValue("SWATTER_COOLDOWN", 2.0f);

        swatter = new Swatter(this);
        SPAWN.SpawnObject(swatter);
        spawnTimer = SPAWN_INTERVAL;

        // Inicializar y posicionar el texto del contador de muertes
        killCountText = new TextObject("KILLS: "+ std::to_string(killCount) + "/" + std::to_string(maxKills));
        killCountText->GetTransform()->position = Vector2(125, 120);
        SPAWN.SpawnObject(killCountText);
    }


    void Update() override {
        Scene::Update();
        
        // Spawn de moscas
        spawnTimer -= TIME.GetDeltaTime();
        if (spawnTimer <= 0.0f) {
            SpawnFly();
            spawnTimer = SPAWN_INTERVAL;
        }

        for (int i = (int)flies.size() - 1; i >= 0; i--) {
            Object* fly = flies[i];
            if ((fly->GetTransform()->position.y > RM->WINDOW_HEIGHT)) {
                flies.erase(flies.begin() + i);
                fly->Destroy();
            }
        }

        if (Input.GetEvent(SDLK_ESCAPE, KeyState::DOWN))
        {
            SM.SetNextScene("MainMenu");
        }
    }
    
    void IncrementKillCount() {
        killCount++;
        killCountText->SetText("KILLS: " + std::to_string(killCount) + "/" + std::to_string(maxKills));
        std::cout << "funciona";

        if (killCount >= maxKills) {
            SM.SetNextScene("YouWon");  // O la escena de victoria deseada
        }
    }

    std::vector<Fly*>& GetFlies() {
        return flies;
    }

    void SpawnFly() {
        Fly* fly = new Fly();
        fly->SpawnFly();
        flies.push_back(fly);
        SPAWN.SpawnObject(fly);
    }

    void OnExit() override {
        AM.StopAudio();
        Scene::OnExit();
        flies.clear();
    }

    void Render() override {
        Scene::Render();
    }
};
