#include "GameObject.h"
#include "Window.h"
#include "Game.h"

	
GameObject::GameObject(Game *game) 
	:	_sprite(new sf::Sprite),
		_game(game)
{

}

GameObject::~GameObject()
{
	delete _sprite;
}


void GameObject::SetTexture(sf::Texture *texture)
{
	_sprite->setTexture(*texture, true);
}


void GameObject::Start()
{

}

void GameObject::Update(float dt) 
{
	// ...
}

void GameObject::Draw(Window *window)
{
	if (_sprite) {
		window->GetRenderWindow()->draw(*_sprite);
	}
}


sf::Vector2f GameObject::GetPosition() const
{
	return _sprite->getPosition();
}

void GameObject::SetPosition(sf::Vector2f v)
{
	_sprite->setPosition(v);
}

sf::Vector2f GameObject::GetScale() const 
{
	return _sprite->getScale();
}

void GameObject::SetScale(sf::Vector2f scale) 
{
	_sprite->setScale(scale);
}


Game* GameObject::GetGame() const
{
	return _game;
}
