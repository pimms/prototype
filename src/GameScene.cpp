#include "GameScene.h"
#include "Player.h"
#include "Game.h"
#include "ResourceManager.h"
#include "PlayerManager.h"


GameScene::GameScene(Game *game)
	:	Scene(game),
		_playerMgr(NULL)
{

}

GameScene::~GameScene()
{
	if (_playerMgr) {
		delete _playerMgr;
	}
}


void GameScene::LoadResources()
{
	Scene::LoadResources();
	
	_playerMgr = new PlayerManager(this);
}

void GameScene::Update(float dt) 
{
	// The GameObjects must be updated before the playermanager. 
	Scene::Update(dt);
	_playerMgr->Update();
}
