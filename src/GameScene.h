#pragma once

#include "Scene.h"

class Player;
class PlayerManager;


class GameScene : public Scene {
public:
	GameScene(Game *game); 
	virtual ~GameScene();

	virtual void LoadResources();
	virtual void Update(float dt);

private:
	PlayerManager *_playerMgr;
};
