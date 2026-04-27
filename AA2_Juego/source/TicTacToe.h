#pragma once
#include "Scene.h"
#include "TextObject.h"
#include "Button.h"
#include "PlayerPiece.h"
#include <string>
#include <vector>

#define INITIAL_TIMER 20.0f
#define COLOR_BACK_BUTTON sf::Color(255, 0, 255, 255)
#define BOARD_SIZE 6
#define ROOM_CODE_POSITION Vector2(20.0f, 20.0f)
#define BACK_TEXT_POSITION Vector2(75.0f, 60.0f)
#define STARTX 380.0f
#define STARTY 84.0f
#define CELLSIZE 100.0f
#define BUTTON_POSITION Vector2(50.0f, 50.0f)
#define BUTTON_SIZE Vector2(210.0f, 50.0f)
class TicTacToe : public Scene {
private:
    short board[BOARD_SIZE][BOARD_SIZE];
    short currentPlayer;
    bool gameOver;
    bool mouseHeld;
    short movesCount;

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

    std::string GetPlayerName(short player);
    bool CheckWin(short p);
    void HandleInput();


    // puntuacion
    std::vector<int> podium;     //ids del orden
    bool hasSentResult = false;  

    void CheckAndSendResults();  




public:

    TicTacToe();
    ~TicTacToe() = default;

    void OnEnter() override;
    void OnExit() override;
    void Update() override;
    void Render() override;

    bool isMyTurn = false;
    short myPlayerID = 1;
    std::string opponentName = "";

    void ApplyMoveFromServer(short row, short col, short playerMoved, short nextPlayerturn);
    bool IsMyTurn(short nextPlayerTurn);
};