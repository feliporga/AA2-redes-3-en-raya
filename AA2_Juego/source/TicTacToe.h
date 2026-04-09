#pragma once
#include "Scene.h"
#include "TextObject.h"
#include "Button.h"
#include "PlayerPiece.h" // <-- Incluimos al nuevo jugador
#include <string>

class TicTacToe : public Scene {
private:
    int board[6][6];
    int currentPlayer;
    bool gameOver;
    bool mouseHeld;
    int movesCount;

    float startX;
    float startY;
    float cellSize;

    TextObject* statusText;
    PlayerPiece* cellSprites[6][6]; // <-- Array de fichas (PlayerPiece)
    Button* backButton;
    TextObject* backButtonText; // <-- Añadimos esto para centrar el texto

    // Lógica interna
    std::string GetPlayerName(int player);
    bool CheckWin(int p);
    void HandleInput();

public:
    TicTacToe();
    ~TicTacToe() = default;

    void OnEnter() override;
    void OnExit() override;
    void Update() override;
    void Render() override;
};