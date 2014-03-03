#pragma once


class Scene;
class Input;
class Window;
class ResourceManager;


class Game {
public:
	Game();
	~Game();
	
	void SetScene(Scene *scene);
	Scene* GetScene() const;

	ResourceManager* GetResourceManager();
	const Input* GetInput() const;

	int Start();

private:
	bool _init;
	bool Init();

	void SceneTransition();

	int MainLoop();
	void HandleEvents();
	void Update();

	Window *_window;	
	ResourceManager *_resmgr;
	Input *_input;

	Scene *_scene;
	Scene *_nextScene;
};
