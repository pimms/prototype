#pragma once

#include "GameObject.h"
#include "Collision.h"

class Platform : public GameObject {
public:
	Platform(Game *game, sf::Vector2f pos);
	
	void Start();

private:
	CollisionRect _colrect;
};
