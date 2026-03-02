#pragma once
#include "ImageObject.h"
#include "TankEnemy.h"
#include "Vector2.h"
#include <cmath>


class EnemyCannon : public ImageObject {
private:
    TankEnemy* enemy;
    float offsetX;
    float offsetY;

public:
    EnemyCannon(TankEnemy* attachedEnemy, float offX = 0.0f, float offY = 0.0f)
        : ImageObject("resources/cannon.png", Vector2(0, 0), Vector2(60, 10), 60, 10, 2, true),
        enemy(attachedEnemy), offsetX(offX), offsetY(offY)
    {
        transform->scale = Vector2(0.4f, 0.1f);
    }

    void Update() override {
        if (!enemy || enemy->IsPendingDestroy()) {
            this->Destroy();
            return;
        }

        ImageObject::Update();

        Vector2 enemyPos = enemy->GetTransform()->position;
        Vector2 playerPos = TankEnemy::playerPosition();
        Vector2 dir = playerPos - enemyPos;
        float angleRad = atan2f(dir.y, dir.x);
        float angleDeg = angleRad * 180.0f / 3.14159265f;
        transform->rotation = angleDeg;

        float halfWidth = -80.0f * transform->scale.x * 0.5f;
        Vector2 basePos = enemyPos + Vector2(offsetX, offsetY);

        Vector2 pivotAdjustment = Vector2(halfWidth * cosf(angleRad), halfWidth * sinf(angleRad));

        transform->position = basePos - pivotAdjustment;
    }

};
