#pragma once
#include "ImageObject.h"
#include "InputManager.h"
#include "TimeManager.h"
#include "Vector2.h"
#include "RenderManager.h"
#include "ConstantsManager.h"

#include <iostream>

class BulletTank : public ImageObject {
private:
    float BULLET_SPEED;
    Vector2 direction;
    float timerBullet;

public:
    BulletTank(const Vector2& startPos, const Vector2& direction)
        : ImageObject("resources/bullet.png", Vector2(0.0f, 0.0f), Vector2(30.0f, 30.0f), 30, 30, 1, false), direction(direction) {
        transform->position = startPos;
        GetRigidbody()->SetVelocity(direction);
        transform->scale = Vector2(0.2f, 0.2f);

        const auto& tanksConstants = constantsManager.GetGameConstants("Tanks");

        BULLET_SPEED = tanksConstants.GetValue("BULLET_SPEED", 150.0f);

    }

    void Update() override {
        
        ImageObject::Update();

        transform->position = transform->position + (GetRigidbody()->GetVelocity() * BULLET_SPEED * TIME.GetDeltaTime());

    }


};