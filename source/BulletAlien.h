#pragma once
#include "ImageObject.h"
#include "Vector2.h"

class BulletAlien : public ImageObject {
private:
    float BULLET_SPEED = 200.0f;

public:
    BulletAlien(const Vector2& startPosition)
        : ImageObject("resources/bullet.png", Vector2(0.0f, 0.0f), Vector2(80.0f, 32.0f), 40, 32, 0, false) {
        transform->scale = Vector2(0.2f, 0.2f);
        GetRigidbody()->SetVelocity(Vector2(0, BULLET_SPEED));
        transform->position = Vector2(startPosition);
    }

    void Update() override {
        transform->position = transform->position + GetRigidbody()->GetVelocity() * TIME.GetDeltaTime();
        ImageObject::Update();
    }

};
