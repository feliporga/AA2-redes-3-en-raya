#include "TankPlayer.h"
#include "Spawner.h"
#include <iostream>
#include <sstream>
#include <iomanip>


bool canShootTank = false;

void TankPlayer::MovingState() {
    if (Input.GetEvent(SDLK_w, KeyState::HOLD)) {
        transform->position.y -= PLAYER_SPEED * TIME.GetDeltaTime();
    }
    if (Input.GetEvent(SDLK_s, KeyState::HOLD)) {
       transform->position.y += PLAYER_SPEED * TIME.GetDeltaTime();
    }
    if (Input.GetEvent(SDLK_a, KeyState::HOLD)) {
        transform->position.x -= PLAYER_SPEED * TIME.GetDeltaTime();
    }
    if (Input.GetEvent(SDLK_d, KeyState::HOLD)) {
        transform->position.x += PLAYER_SPEED * TIME.GetDeltaTime();
    }
    if (Input.GetLeftClick() && bulletTimer <= 0.0f && canShootTank) {
        shootingTank = true;
        canShootTank = false;
        bulletTimer = bulletCooldown;
    }
}

void TankPlayer::AddScore(int points) {
    scoreTanks += points;

    // Actualizar el texto del puntaje
    std::ostringstream ssScoreText;
    ssScoreText << std::setw(5) << std::setfill('0') << scoreTanks;
    if (scoreTextTanks) {
        scoreTextTanks->SetText("SCORE " + ssScoreText.str());
    }

    // Opcional: si el puntaje supera el high score, actualízalo
    if (scoreTanks > highScoreTanks) {
        highScoreTanks = scoreTanks;
        std::ostringstream ssHighScoreText;
        ssHighScoreText << std::setw(5) << std::setfill('0') << highScoreTanks;
        if (highScoreTextTanks) {
            highScoreTextTanks->SetText("HIGH SCORE " + ssHighScoreText.str());
        }
    }
}


void TankPlayer::Update()
{
    if (!highScoreTextTanks)
    {
        std::ostringstream ssHighScoreText;
        ssHighScoreText << std::setw(5) << std::setfill('0') << scoreTanks;

        highScoreTextTanks = new TextObject("HIGH SCORE " + ssHighScoreText.str());
        SPAWN.SpawnObject(highScoreTextTanks);

        highScoreTextTanks->GetTransform()->position = Vector2(125, 70);
    }

    if (!scoreTextTanks)
    {
        std::ostringstream ssScoreText;
        ssScoreText << std::setw(5) << std::setfill('0') << scoreTanks;

        scoreTextTanks = new TextObject("SCORE " + ssScoreText.str());
        SPAWN.SpawnObject(scoreTextTanks);

        scoreTextTanks->GetTransform()->position = Vector2(windowWidth - 200, 70);
    }

	ImageObject::Update();
    if (!Input.GetLeftClick()) {
        canShootTank = true;
    }
    bulletTimer -= TIME.GetDeltaTime();
	MovingState();
}

std::string TankPlayer::shootSFXTank = "squirt";

