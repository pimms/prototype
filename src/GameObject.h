#pragma once

#include <SFML/Graphics.hpp>

class Window;
class Game;

class GameObject {
public:
	GameObject(Game *game);
	virtual ~GameObject();

	void SetTexture(sf::Texture *texture);

	virtual void Start();
	virtual void Update(float dt);
	void Draw(Window *window);

	sf::Vector2f GetPosition() const;
	void SetPosition(sf::Vector2f v);

	sf::Vector2f GetScale() const;
	void SetScale(sf::Vector2f scale);

	Game* GetGame() const;

private:
	Game *_game;
	sf::Sprite *_sprite;
};
