#pragma once
#include "Object.h"
#include "ImageRenderer.h"
#include "Vector2.h"
#include <string>

class PlayerPiece : public Object {
private:
    int playerID;

    static std::string GetSpritePath(int player);

public:
    PlayerPiece(int player, const Vector2& position);
    ~PlayerPiece() = default;

    int GetPlayerID() const { return playerID; }
};