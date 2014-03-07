#include "Game.h"
#include "Scene.h"
#include "Window.h"
#include "ResourceManager.h"
#include "Input.h"
#include "World.h"


Game::Game()
	: 	_init(false),
		_window(NULL),
		_resmgr(NULL),
		_input(NULL),
		_world(NULL),
		_scene(NULL),
		_nextScene(NULL)
{

}

Game::~Game() 
{
	if (_window) {
		delete _window;
	}

	if (_resmgr) {
		delete _resmgr;
	}

	if (_input) {
		delete _input;
	}

	if (_scene) {
		delete _scene;
	}

	if (_world) {
		delete _world;
	}
}


void Game::SetScene(Scene *scene)
{
	if (_nextScene) {
		throw "Multiple scenes are queued - wtf man?";
	}
	
	_nextScene = scene;
}

Scene* Game::GetScene() const 
{
	return _scene;
}

World* Game::GetWorld() const
{
	return _world;
}


ResourceManager* Game::GetResourceManager()
{
	return _resmgr;
}

const Input* Game::GetInput() const
{
	return _input;
}


int Game::Start() 
{
	if (!_nextScene && !_scene) {
		printf("No scene assigned");
		return -1;
	}

	if (!Init()) {
		return -1;
	}
	
	return MainLoop();
}


/* Private Methods */

bool Game::Init()
{
	if (_init) {
		return false;
	}

	_window = new Window();
	_window->CreateWindow();

	_resmgr = new ResourceManager();
	_input = new Input();

	_init = true;
	return true;
}


void Game::SceneTransition()
{
	if ((!_scene && _nextScene) 
			|| (_nextScene && _scene->ReadyToDie())) {
		delete _scene;
		_scene = _nextScene;
		_nextScene = NULL;
		InitScene();
	}
}

void Game::InitScene() 
{
	_scene->LoadResources();

	if (_world) {
		delete _world;
	}

	_world = new World(_scene);
}



int Game::MainLoop() 
{
	while (true) {
		SceneTransition();
		HandleEvents();
		Update();

		_window->GetRenderWindow()->clear();
		_scene->Draw(_window);
		_window->GetRenderWindow()->display();
	}
}

void Game::HandleEvents() 
{
	sf::Event evt;
	sf::RenderWindow *win;

	win = _window->GetRenderWindow();
	while (win->pollEvent(evt)) {
		switch (evt.type) {
			case sf::Event::KeyPressed:
				_input->KeyPressed(evt);
				break;
			case sf::Event::KeyReleased:
				_input->KeyReleased(evt);
				break;

			default:
				break;
		}
	}
}

void Game::Update()
{
	_scene->Update(1.f / 30.f);	
	_input->UnfreshKeys();
}
