#pragma once
#include "Scene.h"
#include "TextObject.h"
#include "Button.h"
#include "PlayerPiece.h"
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
    TextObject* roomCodeText;
    PlayerPiece* cellSprites[6][6];
    Button* backButton;
    TextObject* backButtonText;

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

    bool isMyTurn = false;
    int myPlayerID = 1;
    std::string opponentName = "";

    void ApplyMoveFromServer(int row, int col, int playerMoved, bool nextTurnIsMine);
};