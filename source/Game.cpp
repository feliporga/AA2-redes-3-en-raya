#include "Game.h"
#include "RenderManager.h"
#include "SceneManager.h"
#include "AudioManager.h"
#include "SplashScreen.h"
#include "MainMenu.h"
#include "SpriteMenu.h"
#include "RankingMenu.h"
#include "SpaceInvaders.h"
#include "YouWonScene.h"
#include "GameOverScene.h"
#include "Splat.h"
#include "Tanks.h"
#include <cassert>


void Game::Init() {
    RM->Init();

    RM->LoadTexture("resources/background1.png");
    RM->LoadTexture("resources/background2.png");
    RM->LoadTexture("resources/background3.png");

    RM->LoadTexture("resources/PlayerSpace.png");
    RM->LoadTexture("resources/PlayerSplat.png");
    RM->LoadTexture("resources/PlayerTank.png");

    RM->LoadTexture("resources/enemy.png");
    RM->LoadTexture("resources/enemy2.png");
    RM->LoadTexture("resources/enemy3.png");

    RM->LoadTexture("resources/enemyTank1.png");
    RM->LoadTexture("resources/enemyTank2.png");
    RM->LoadTexture("resources/enemyTank3.png");

    RM->LoadTexture("resources/enemySplat1.png");
    RM->LoadTexture("resources/enemySplat2.png");
    RM->LoadTexture("resources/enemySplat3.png");

    RM->LoadTexture("resources/enemyviejo.png");
    RM->LoadTexture("resources/spashscreen.png");
    RM->LoadTexture("resources/cannon.png");
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
    AM.LoadSong("moscaMusic");
    AM.LoadSong("rankingMenu");
    AM.LoadSong("spaceMusic");
    AM.LoadSong("spriteMenu");
    AM.LoadSong("tanksMusic");

    assert(SM.AddScene("Splash", new SplashScreen()));
    assert(SM.AddScene("MainMenu", new MainMenu()));
    assert(SM.AddScene("SpriteMenu", new SpriteMenu()));
    assert(SM.AddScene("RankingMenu", new RankingMenu()));
    
    assert(SM.AddScene("GameOver", new GameOverScene()));
    assert(SM.AddScene("YouWon", new YouWonScene()));

    assert(SM.AddScene("Space Invaders", new SpaceInvaders()));
    assert(SM.AddScene("Splat", new Splat()));
    assert(SM.AddScene("Tanks", new Tanks()));

    assert(SM.InitFirstScene("Splash"));

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
