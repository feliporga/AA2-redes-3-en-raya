#pragma once
#include "ImageObject.h"
#include "InputManager.h"
#include "TimeManager.h"
#include "Vector2.h"
#include "Ship.h"

class BulletShip : public ImageObject {
private:
    Ship* ship;
    const float BULLET_SPEED = 300.0f;

public:
    BulletShip(const Vector2& startPos)
        : ImageObject("resources/bullet.png", Vector2(0.0f, 0.0f), Vector2(30.0f, 30.0f), 30, 30, 0, false) {
        transform->scale = Vector2(0.1f, 0.1f);
        transform->position = startPos;

    }

    void Update() override {
        ImageObject::Update();

        transform->position.y -= BULLET_SPEED * TIME.GetDeltaTime();
    }

    
};