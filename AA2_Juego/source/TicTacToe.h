#pragma once
#include "Scene.h"
#include "TextObject.h"
#include "Button.h"
#include "PlayerPiece.h"
#include <string>
#include <vector>
#define INITIAL_TIMER 20.0f
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

    float turnTimer;

    float returnToLobbyTimer;

    TextObject* timerText;

    TextObject* statusText;
    TextObject* roomCodeText;
    PlayerPiece* cellSprites[6][6];
    Button* backButton;
    TextObject* backButtonText;

    std::string GetPlayerName(int player);
    bool CheckWin(int p);
    void HandleInput();


    // puntuacion
    std::vector<int> podium;     //ids del orden
    bool hasSentResult = false;  

    void CheckAndSendResults();  




public:
    bool isMyTurn = false;
    int myPlayerID = 1;
    std::string opponentName = "";

    TicTacToe();
    ~TicTacToe() = default;

    void OnEnter() override;
    void OnExit() override;
    void Update() override;
    void Render() override;

    
    void SetTimer();
    void ApplyMoveFromServer(int row, int col, int playerMoved, int nextPlayerturn);
    bool IsMyTurn(int nextPlayerTurn);
};