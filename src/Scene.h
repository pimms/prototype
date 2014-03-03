#pragma once

#include <SFML/Graphics.hpp>
#include <list>
using std::list;


class Game;
class GameObject;
class Window;


/* Whatever the fuck you do, call the superclass' method when
 * overriding any Scene method!
 */
class Scene {
public:
	Scene(Game *game);
	virtual ~Scene();

	virtual void LoadResources();
	virtual void PrepareToDie();
	virtual bool ReadyToDie();

	virtual void Update(float dt);
	virtual void Draw(Window *window);

	Game* GetGame();

	void AddGameObject(GameObject *go);
	void RemoveGameObject(GameObject *go);

private:
	Game *_game;
	list<GameObject*> _objects;

	bool _hasLoaded;
};
