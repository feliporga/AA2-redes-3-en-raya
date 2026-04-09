#include "TicTacToe.h"
#include "InputManager.h"
#include "RenderManager.h"
#include "Spawner.h"

TicTacToe::TicTacToe() : Scene() {
    startX = 380.0f;
    startY = 84.0f;
    cellSize = 100.0f;

    statusText = nullptr;
    backButton = nullptr;
    backButtonText = nullptr;
}

std::string TicTacToe::GetPlayerName(int player) {
    if (player == 1) return "JUGADOR 1 (CUADRADO)";
    if (player == 2) return "JUGADOR 2 (TRIANGULO)";
    if (player == 3) return "JUGADOR 3 (CIRCULO)";
    if (player == 4) return "JUGADOR 4 (CRUZ)";
    return "";
}

bool TicTacToe::CheckWin(int p) {
    // Horizontales
    for (int r = 0; r < 6; r++) {
        for (int c = 0; c < 4; c++) {
            if (board[r][c] == p && board[r][c + 1] == p && board[r][c + 2] == p) return true;
        }
    }
    // Verticales
    for (int c = 0; c < 6; c++) {
        for (int r = 0; r < 4; r++) {
            if (board[r][c] == p && board[r + 1][c] == p && board[r + 2][c] == p) return true;
        }
    }
    // Diagonales principales (\)
    for (int r = 0; r < 4; r++) {
        for (int c = 0; c < 4; c++) {
            if (board[r][c] == p && board[r + 1][c + 1] == p && board[r + 2][c + 2] == p) return true;
        }
    }
    // Diagonales inversas (/)
    for (int r = 0; r < 4; r++) {
        for (int c = 2; c < 6; c++) {
            if (board[r][c] == p && board[r + 1][c - 1] == p && board[r + 2][c - 2] == p) return true;
        }
    }
    return false;
}

void TicTacToe::HandleInput() {
    if (gameOver) return;

    bool isLeftClick = Input.GetLeftClick();

    if (isLeftClick && !mouseHeld) {
        mouseHeld = true;

        float mx = (float)Input.GetMouseX();
        float my = (float)Input.GetMouseY();

        if (mx >= startX && mx <= startX + 600 && my >= startY && my <= startY + 600) {
            int col = (int)((mx - startX) / cellSize);
            int row = (int)((my - startY) / cellSize);

            if (board[row][col] == 0) {
                board[row][col] = currentPlayer;
                movesCount++;

                Vector2 pos(startX + col * cellSize, startY + row * cellSize);
                PlayerPiece* newSprite = new PlayerPiece(currentPlayer, pos);

                SPAWN.SpawnObject(newSprite);
                cellSprites[row][col] = newSprite;

                if (CheckWin(currentPlayer)) {
                    statusText->SetText(GetPlayerName(currentPlayer) + " GANA!");
                    statusText->SetColor(sf::Color::Yellow);
                    gameOver = true;
                }
                else if (movesCount >= 36) {
                    statusText->SetText("¡EMPATE!");
                    gameOver = true;
                }
                else {
                    currentPlayer++;
                    if (currentPlayer > 4) currentPlayer = 1;
                    statusText->SetText("TURNO: " + GetPlayerName(currentPlayer));
                }
            }
        }
    }
    else if (!isLeftClick) {
        mouseHeld = false;
    }
}

void TicTacToe::OnEnter() {
    currentPlayer = 1;
    gameOver = false;

    mouseHeld = Input.GetLeftClick();

    movesCount = 0;

    for (int r = 0; r < 6; r++) {
        for (int c = 0; c < 6; c++) {
            board[r][c] = 0;
            cellSprites[r][c] = nullptr;
        }
    }

    statusText = new TextObject("TURNO: " + GetPlayerName(currentPlayer));
    statusText->GetTransform()->position = Vector2(RM->WINDOW_WIDTH / 2 - 200, 30);
    SPAWN.SpawnObject(statusText);

    backButton = new Button(Vector2(50, 50), Vector2(150, 50), sf::Color(255, 0, 255, 255), "", Button::ActionType::ChangeScene, "MainMenu");
    SPAWN.SpawnObject(backButton);

    backButtonText = new TextObject("VOLVER");
    backButtonText->GetTransform()->position = Vector2(75.0f, 60.0f);
    SPAWN.SpawnObject(backButtonText);
}

void TicTacToe::OnExit() {
    Scene::OnExit();
}

void TicTacToe::Update() {
    Scene::Update();
    HandleInput();
}

void TicTacToe::Render() {
    Scene::Render();

    for (int i = 0; i <= 6; i++) {
        RM->DrawRect((int)(startX + i * cellSize), (int)startY, 4, 600, 255, 255, 255, 255);
        RM->DrawRect((int)startX, (int)(startY + i * cellSize), 600, 4, 255, 255, 255, 255);
    }
}