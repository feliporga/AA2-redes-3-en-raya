#include "Ship.h"
#include <sstream>
#include <iomanip>
#include "SceneManager.h"


void Ship::MovingState(){
    if (Input.GetEvent(SDLK_a, KeyState::HOLD)) {
        transform->position.x -= PLAYER_SPEED_SPACEINVADERS * TIME.GetDeltaTime();
    }

    if (Input.GetEvent(SDLK_d, KeyState::HOLD)) {
        transform->position.x += PLAYER_SPEED_SPACEINVADERS * TIME.GetDeltaTime();
    }


    if (Input.GetEvent(SDLK_SPACE, KeyState::DOWN) && bulletTimer <= 0.0f) {
        shootingShip = true;
        bulletTimer = bulletCooldown;
        ShootSound();
    }
}

void Ship::IncrementScore(int amount) {
    scoreSpaceInvaders += amount;

    // Actualizar el texto del puntaje actual
    if (scoreTextSpaceInvaders) {
        std::ostringstream ssScoreText;
        ssScoreText << std::setw(5) << std::setfill('0') << scoreSpaceInvaders;
        scoreTextSpaceInvaders->SetText("SCORE " + ssScoreText.str());
    }

    // Actualizar el puntaje máximo si es necesario
    if (scoreSpaceInvaders > highScoreSpaceInvaders) {
        highScoreSpaceInvaders = scoreSpaceInvaders;

        if (highScoreTextSpaceInvaders) {
            std::ostringstream ssHighScoreText;
            ssHighScoreText << std::setw(5) << std::setfill('0') << highScoreSpaceInvaders;
            highScoreTextSpaceInvaders->SetText("HIGH SCORE " + ssHighScoreText.str());
        }
    }
}

void Ship::ReduceLives() {
    currentLives--;

    // Actualizar el texto de las vidas
    if (livesTextSpaceInvaders) {
        livesTextSpaceInvaders->SetText("LIVES: " + std::to_string(currentLives));
    }

    // Verificar si el jugador ha perdido todas las vidas
    if (currentLives <= 0) {
        SM.SetNextScene("GameOver");
    }
}


void Ship::Update()
{
    if (!livesTextSpaceInvaders)
    {
        livesTextSpaceInvaders = new TextObject("LIVES: " + std::to_string(currentLives));
        SPAWN.SpawnObject(livesTextSpaceInvaders);
        livesTextSpaceInvaders->GetTransform()->position = Vector2(RM->WINDOW_WIDTH / 2, 70); // Posición inicial del texto

    }
    

    // Inicializar texto de puntaje máximo
    if (!highScoreTextSpaceInvaders) {
        std::ostringstream ssHighScoreText;
        ssHighScoreText << std::setw(5) << std::setfill('0') << highScoreSpaceInvaders;

        highScoreTextSpaceInvaders = new TextObject("HIGH SCORE " + ssHighScoreText.str());
        SPAWN.SpawnObject(highScoreTextSpaceInvaders);

        highScoreTextSpaceInvaders->GetTransform()->position = Vector2(125, 70);
    }

    // Inicializar texto de puntaje actual
    if (!scoreTextSpaceInvaders) {
        std::ostringstream ssScoreText;
        ssScoreText << std::setw(5) << std::setfill('0') << scoreSpaceInvaders;

        scoreTextSpaceInvaders = new TextObject("SCORE " + ssScoreText.str());
        SPAWN.SpawnObject(scoreTextSpaceInvaders);

        scoreTextSpaceInvaders->GetTransform()->position = Vector2(windowWidth - 200, 70);
    }

    ImageObject::Update();

    bulletTimer -= TIME.GetDeltaTime();
    MovingState();
}

std::string Ship::shootSFXShip = "squirt";

    
