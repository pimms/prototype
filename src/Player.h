#pragma once

#include "GameObject.h"


class Player : public GameObject {
public:
	Player(Game *game);

	void Start();
	void Update(float dt);

	void SetInputAvailable(bool input);

private:
	void HandleMovement(float dt);
	sf::Vector2f GetInputVelocity(float dt) const;
	sf::Vector2f CalculateNextPosition();


	bool _inputAvailable;
	sf::Vector2f _vel;
};
