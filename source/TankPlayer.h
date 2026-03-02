#pragma once
#include "ImageObject.h"
#include "InputManager.h"
#include "TimeManager.h"
#include "Vector2.h"
#include "BulletTank.h"
#include "TextObject.h"
#include "ConstantsManager.h"
#include "AudioManager.h"

class TankPlayer : public ImageObject {
private:
	int windowWidth = RM->WINDOW_WIDTH;
	int windowHeight = RM->WINDOW_HEIGHT;

	float PLAYER_SPEED = 150.0f;

	bool shootingTank = false;

	float bulletCooldown = 0.5f;
	float bulletTimer = 0.0f;

	int scoreTanks = 0;
	int highScoreTanks = scoreTanks;

	const int maxLives = 3;
	int currentLives = maxLives;

	TextObject* livesTextTanks = nullptr;
	TextObject* highScoreTextTanks = nullptr;
	TextObject* scoreTextTanks = nullptr;

	static std::string shootSFXTank;

	void MovingState();

public:
	TankPlayer() 
		: ImageObject("resources/PlayerTank.png", Vector2(0.0f, 0.0f), Vector2(120.0f, 30.0f), 60, 30, 2, true) {
		transform->position = Vector2(RM->WINDOW_WIDTH / 2 - 30, RM->WINDOW_HEIGHT - 60);
		transform->scale = Vector2(0.5f, 0.5f);

		const auto& tanksConstants = constantsManager.GetGameConstants("Tanks");

		PLAYER_SPEED = tanksConstants.GetValue("PLAYER_SPEED", 150.0f);
	}

	static void SetShootSFXTank(const std::string& sfxName) {
		shootSFXTank = sfxName;
	}

	void AddScore(int points);

	void Update() override;

	inline bool IsShootingTank() { return shootingTank; }

	inline void ResetShootTank() { shootingTank = false; }

	inline void ShootSound() { AM.PlayClip(shootSFXTank, 0); }
};

