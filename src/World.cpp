#include "World.h"
#include "Platform.h"
#include "Scene.h"


World::World(Scene *scene)
	:	_scene(scene)
{
	CreatePlatforms();
}

World::~World()
{

}


void World::CreatePlatforms()
{
	Game *g = _scene->GetGame();
	_platforms.push_back(new Platform(g, sf::Vector2f(0.f, 450.f)));
	_platforms.push_back(new Platform(g, sf::Vector2f(200.f, 450.f)));
	_platforms.push_back(new Platform(g, sf::Vector2f(400.f, 450.f)));
	_platforms.push_back(new Platform(g, sf::Vector2f(600.f, 450.f)));
	_platforms.push_back(new Platform(g, sf::Vector2f(800.f, 450.f)));
	_platforms.push_back(new Platform(g, sf::Vector2f(1000.f, 450.f)));

	for (auto it=_platforms.begin(); it!=_platforms.end(); it++) {
		_scene->AddGameObject(*it);
	}
}
