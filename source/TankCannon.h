#pragma once
#include "ImageObject.h"
#include "TankPlayer.h"
#include "Vector2.h"
#include "InputManager.h"
#include <cmath>

class TankCannon : public ImageObject {
private:
    TankPlayer* player;
    float offsetX;
    float offsetY;

public:
    TankCannon(TankPlayer* attachedPlayer, float offX = 0.0f, float offY = 0.0f)
        : ImageObject("resources/cannon.png", Vector2(0, 0), Vector2(60, 10), 60, 10, 2, true),
        player(attachedPlayer), offsetX(offX), offsetY(offY)
    {
        transform->scale = Vector2(0.5f, 0.1f);
    }

    void Update() override {
        ImageObject::Update();

        Vector2 playerPos = player->GetTransform()->position;
        Vector2 mousePos(static_cast<float>(Input.GetMouseX()), static_cast<float>(Input.GetMouseY()));
        Vector2 dir = mousePos - playerPos;
        float angleRad = atan2f(dir.y, dir.x);
        float angleDeg = angleRad * 180.0f / 3.14159265f;
        transform->rotation = angleDeg;

        float realWidth = 100.0f * transform->scale.x;
        float pivotCompX = realWidth * 0.5f;

        transform->position = playerPos + Vector2(offsetX, offsetY);

        Vector2 rotateOffset(
            pivotCompX * cosf(angleRad),
            pivotCompX * sinf(angleRad)
        );

        transform->position = transform->position + rotateOffset;

    }
};
