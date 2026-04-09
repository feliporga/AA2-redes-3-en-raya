#include "PlayerPiece.h"
#include "RenderManager.h"

std::string PlayerPiece::GetSpritePath(int player) {
    if (player == 1) return "resources/player1.png";
    if (player == 2) return "resources/player2.png";
    if (player == 3) return "resources/player3.png";
    if (player == 4) return "resources/player4.png";
    return "";
}

PlayerPiece::PlayerPiece(int player, const Vector2& position)
    : Object(), playerID(player)
{
    // Tu ImageRenderer dibuja las cosas desde el centro. 
    // Al sumarle 50 a la X y a la Y, clavamos la ficha en el centro exacto de la celda.
    transform->position = position + Vector2(50.0f, 50.0f);

    std::string path = GetSpritePath(player);
    RM->LoadTexture(path);

    // 1. Le decimos que la imagen original a leer es de 293x293
    renderer = new ImageRenderer(transform, path, Vector2(0.0f, 0.0f), Vector2(293.0f, 293.0f));

    // 2. Establecemos el tamaño real y calculamos la escala
    transform->size = Vector2(293.0f, 293.0f);

    // 3. Magia matemática: Encogemos los 293px para que ocupen exactamente 100px en pantalla
    transform->scale = Vector2(100.0f / 293.0f, 100.0f / 293.0f);
}