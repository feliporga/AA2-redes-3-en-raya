#include "Swatter.h"
#include "Splat.h"
#include "Fly.h"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <string>


bool canHit = false;

void Swatter::MovingState()
{
    Vector2 targetVector = Vector2(Input.GetMouseX(), Input.GetMouseY()) - transform->position;
    
    distance = sqrtf(targetVector.x * targetVector.x + targetVector.y * targetVector.y);
    if (distance > 5.0f) {
        targetVector.Normalize();
        physics->AddForce(targetVector * movementFactor);
    }
    else {
        physics->SetVelocity(Vector2(0.0f, 0.0f));
    }


    if (canHit && Input.GetLeftClick()) {
        ShootSound();
        currentState = HIT;
        canHit = false;
    }
}

void Swatter::HitState() {
    auto& flies = parentScene->GetFlies();
    hitFly = false;

    for (int i = 0; i < (int)flies.size(); i++) {
        if (flies[i]->GetRigidbody()->CheckCollision(GetRigidbody())) {
            flies[i]->Destroy();
            flies.erase(flies.begin() + i);
            hitFly = true;
            AM.PlayClip(dieSFXFly, 0);

            scoreSplat += 100;

            parentScene->IncrementKillCount();

            std::ostringstream ssScoreText;
            ssScoreText << std::setw(5) << std::setfill('0') << scoreSplat;
            scoreTextSplat->SetText("SCORE " + ssScoreText.str());

            if (scoreSplat > highScoreSplat) {
                highScoreSplat = scoreSplat;
                std::ostringstream ssHighScoreText;
                ssHighScoreText << std::setw(5) << std::setfill('0') << highScoreSplat;
                if (highScoreTextSplat) {
                    highScoreTextSplat->SetText("HIGH SCORE " + ssHighScoreText.str());
                }
            }

            break;
        }
    }

    if (!hitFly) {
        currentState = STUNNED;
        stunnedTimer = maxStunTimer;
    }
    else {
        currentState = MOVING;
    }
}


void Swatter::StunnedState() {
    stunnedTimer -= TIME.GetDeltaTime();
    if (stunnedTimer <= 0.0f)
        currentState = MOVING;

    auto& flies = parentScene->GetFlies();

    for (int i = 0; i < (int)flies.size(); i++) {
        if (flies[i]->GetRigidbody()->CheckCollision(GetRigidbody())) {
            flies[i]->Destroy();
            flies.erase(flies.begin() + i);
            hitFly = true;

            LoseLife();
            AM.PlayClip("die", 0);
            if (currentLives <= 0) {
                Destroy();
                AM.PlayClip("GameOver", 0);
            }
            break;
        }
    }
}

void Swatter::Update() {
    if (!highScoreTextSplat)
    {
        std::ostringstream ssHighScoreText;
        ssHighScoreText << std::setw(5) << std::setfill('0') << scoreSplat;

        highScoreTextSplat = new TextObject("HIGH SCORE " + ssHighScoreText.str());
        SPAWN.SpawnObject(highScoreTextSplat);

        highScoreTextSplat->GetTransform()->position = Vector2(125, 70);
    }

    if (!livesTextSplat)
    {
        livesTextSplat = new TextObject("LIVES " + std::to_string(currentLives));
        SPAWN.SpawnObject(livesTextSplat);
        livesTextSplat->GetTransform()->position = Vector2(windowWidth / 2, 70);
    }

    if (!scoreTextSplat)
    {
        std::ostringstream ssScoreText;
        ssScoreText << std::setw(5) << std::setfill('0') << scoreSplat;

        scoreTextSplat = new TextObject("SCORE " + ssScoreText.str());
        SPAWN.SpawnObject(scoreTextSplat);

        scoreTextSplat->GetTransform()->position = Vector2(windowWidth - 200, 70);
    }

    ImageObject::Update();

    if (!Input.GetLeftClick()) {
        canHit = true;
    }

    switch (currentState) {
    case MOVING:
        MovingState();
        break;
    case HIT:
        HitState();
        break;
    case STUNNED:
        StunnedState();
        break;
    }
}



void Swatter::LoseLife() {
    if (currentLives > 0) {
        currentLives--;
        AM.PlayClip("die",0);
    }

    transform->position = Vector2(windowWidth / 2, windowHeight / 2);
    physics->SetVelocity(Vector2(0.0f, 0.0f));

    livesTextSplat->SetText("LIVES " + std::to_string(currentLives));

    currentState = MOVING;
    if (currentLives <= 0) {
        SM.SetNextScene("GameOver");
        AM.PlayClip("GameOver", 0);
    }
}

std::string Swatter::dieSFXFly = "explosionBug";
std::string Swatter::shootSFXSwatter = "squirt";
