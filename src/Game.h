#pragma once

class Scene;
class World;
class Input;
class Window;
class ResourceManager;


class Game {
public:
	Game();
	~Game();
	
	void SetScene(Scene *scene);
	Scene* GetScene() const;
	World* GetWorld() const;

	ResourceManager* GetResourceManager();
	const Input* GetInput() const;

	int Start();

private:
	bool _init;
	bool Init();

	void SceneTransition();
	void InitScene();

	int MainLoop();
	void HandleEvents();
	void Update();

	Window *_window;	
	ResourceManager *_resmgr;
	Input *_input;

	World *_world;
	Scene *_scene;
	Scene *_nextScene;
};
