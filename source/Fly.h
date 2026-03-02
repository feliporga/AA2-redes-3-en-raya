#pragma once
#include "ImageObject.h"
#include "Vector2.h"
#include "TimeManager.h"
#include "RenderManager.h"
#include "ConstantsManager.h"

#include <cstdlib>
#include <ctime>
#include <string>
#include <cmath>

class Fly : public ImageObject {
private:
    float FLY_SPEED;
    Vector2 velocity;

    static std::string defaultSpriteFly;

public:
    static void SetDefaultSpriteFly(const std::string& spritePath) {
        defaultSpriteFly = spritePath;
    }

    Fly()
        : ImageObject(defaultSpriteFly, Vector2(0.0f, 0.0f), Vector2(80.0f, 32.0f), 40, 32, 2, true)
    {
        transform->scale = Vector2(0.2f, 0.2f);

        SetFlySpeed();
    }

    void SetFlySpeed() {
        const auto& splatConstants = constantsManager.GetGameConstants("Splat");

        FLY_SPEED = splatConstants.GetValue("FLY_SPEED", 300.0f);
    }

    void SpawnFly() {
        int edge = std::rand() % 4;
        float x = 0.0f, y = 0.0f;

        float dirX = 0.0f, dirY = 0.0f;

        switch (edge) {
        case 0: // Arriba
            x = static_cast<float>(std::rand() % RM->WINDOW_WIDTH);
            y = 0.0f;
            dirY = 1.0f; // Hacia abajo
            dirX = ((std::rand() % 200) - 100) / 100.0f;
            break;
        case 1: // Abajo
            x = static_cast<float>(std::rand() % RM->WINDOW_WIDTH);
            y = static_cast<float>(RM->WINDOW_HEIGHT);
            dirY = -1.0f; // Hacia arriba
            dirX = ((std::rand() % 200) - 100) / 100.0f;
            break;
        case 2: // Izquierda
            x = 0.0f;
            y = static_cast<float>(std::rand() % RM->WINDOW_HEIGHT);
            dirX = 1.0f; // Hacia derecha
            dirY = ((std::rand() % 200) - 100) / 100.0f;
            break;
        case 3: // Derecha
            x = static_cast<float>(RM->WINDOW_WIDTH);
            y = static_cast<float>(std::rand() % RM->WINDOW_HEIGHT);
            dirX = -1.0f; // Hacia izquierda
            dirY = ((std::rand() % 200) - 100) / 100.0f;
            break;
        }
        transform->position = Vector2(x, y);

        Vector2 direction = Vector2(dirX, dirY);
        direction.Normalize();

        velocity = direction * FLY_SPEED;
    }


    void Update() override {
        ImageObject::Update();
        transform->position = transform->position + (velocity * TIME.GetDeltaTime());

        float margin = 10.0f;

        if (transform->position.x < margin) {
            transform->position.x = margin;
            velocity.x = std::abs(velocity.x);
        }
        else if (transform->position.x > RM->WINDOW_WIDTH - margin) {
            transform->position.x = RM->WINDOW_WIDTH - margin;
            velocity.x = -std::abs(velocity.x);
        }

        if (transform->position.y < margin) {
            transform->position.y = margin;
            velocity.y = std::abs(velocity.y);
        }
        else if (transform->position.y > RM->WINDOW_HEIGHT - margin) {
            transform->position.y = RM->WINDOW_HEIGHT - margin;
            velocity.y = -std::abs(velocity.y);
        }
    }

};
