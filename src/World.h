#pragma once 

#include <list>
using std::list;


class Scene;
class Platform;


/* The world object keeps track of the environment. 
 */
class World {
public:
	World(Scene *scene);
	~World();

private:
	void CreatePlatforms();

	list<Platform*> _platforms;	
	Scene *_scene;
};
