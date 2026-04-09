#include "Game.h"
#include "RenderManager.h"
#include "SceneManager.h"
#include "AudioManager.h"
#include "MainMenu.h"
#include "TicTacToe.h"
#include "YouWonScene.h"
#include "GameOverScene.h"
#include <cassert>


void Game::Init() {
    RM->Init();

    RM->LoadTexture("resources/background1.png");

    RM->LoadTexture("resources/enemy.png");
    RM->LoadTexture("resources/PlayerSpace.png");
    RM->LoadTexture("resources/bullet.png");

    AM.LoadClip("laser");
    AM.LoadClip("squirt");
    AM.LoadClip("bulletshot");
    AM.LoadClip("explosionBug");
    AM.LoadClip("explosionLaser");
    AM.LoadClip("explosionShoot");
    AM.LoadClip("die");
    AM.LoadClip("GameOver");

    AM.LoadSong("menuMusic");
    AM.LoadSong("spaceMusic");

    AM.LoadSong("menuMusic");
    AM.LoadSong("spaceMusic");

    // ¡FUERA ASSERTS! Metemos las escenas directamente a la fuerza
    SM.AddScene("MainMenu", new MainMenu());
    SM.AddScene("GameOver", new GameOverScene());
    SM.AddScene("YouWon", new YouWonScene());
    SM.AddScene("TicTacToe", new TicTacToe());

    SM.InitFirstScene("MainMenu");

}

void Game::Update()
{
    SM.UpdateCurrentScene();
}

void Game::Render()
{
    RM->ClearScreen();

    RM->RenderBackground();

    SM.GetCurrentScene()->Render();

    RM->RenderScreen();
}

void Game::Release()
{
    SM.GetCurrentScene()->OnExit();
    RM->Release();
}
