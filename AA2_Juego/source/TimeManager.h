#pragma once
#include <SDL.h>

#define TIME TimeManager::Instance()

class TimeManager {
private:
	TimeManager() {
		previousElapsedTime = (double)SDL_GetTicks() / 1000.0;
		frameTime = 1.0f / (float)FPS;
	}
	~TimeManager() = default;

	TimeManager(const TimeManager& t) = delete;
	TimeManager& operator=(const TimeManager& t) = delete;

	float deltaTime = 0.0f;
	float elapsedTime = 0.0f;
	float previousElapsedTime;

	static const int FPS = 60;
	float frameTime;
public:
	inline float GetDeltaTime() const { return deltaTime; }
	inline float GetElapsedTime() const { return elapsedTime; }

	inline bool ShouldUpdateGame() const { return deltaTime >= frameTime; }
	inline void ResetDeltaTime() { deltaTime = deltaTime - float(floor(deltaTime / frameTime) * deltaTime); }

	static TimeManager& Instance() {
		static TimeManager tm;

		return tm;
	}

	void Update() {
		//SDL time
		elapsedTime = (double)SDL_GetTicks() / 1000.0;
		deltaTime += elapsedTime - previousElapsedTime;
		previousElapsedTime = elapsedTime;
	}
};