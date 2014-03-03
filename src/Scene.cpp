#include "Scene.h"
#include "Game.h"
#include "Window.h"
#include "GameObject.h"


Scene::Scene(Game *game)
	:	_game(game),
		_hasLoaded(false)
{

}

Scene::~Scene() 
{
	list<GameObject*>::iterator it;
	for (it=_objects.begin(); it!=_objects.end(); it++) {
		delete *it;
	}
}


void Scene::LoadResources()
{
	_hasLoaded = true;

	list<GameObject*>::iterator it;
	for (it = _objects.begin(); it !=_objects.end(); it++) {
		(*it)->Start();
	}
}

void Scene::PrepareToDie()
{
	// ...
}

bool Scene::ReadyToDie() 
{
	return true;
}


void Scene::Update(float dt) 
{
	list<GameObject*>::iterator it;
	for (it=_objects.begin(); it!=_objects.end(); it++) {
		(*it)->Update(dt);
	}
}

void Scene::Draw(Window *window)
{
	list<GameObject*>::iterator it;
	for (it=_objects.begin(); it!=_objects.end(); it++) {
		(*it)->Draw(window);	
	}
}


Game* Scene::GetGame() 
{
	return _game;
}


void Scene::AddGameObject(GameObject *go)
{
	_objects.push_front(go);

	if (_hasLoaded) {
		go->Start();
	}
}

void Scene::RemoveGameObject(GameObject *go)
{
	printf("COUNT: %lu\n", _objects.size());

	list<GameObject*>::iterator it = _objects.begin();
	for (; it!=_objects.end(); it++) {
		if (*it == go) {
			it = _objects.erase(it);
			return;
		}
	}
}


/* Private Methods */
