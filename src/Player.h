#pragma once

#include "GameObject.h"
#include "Collision.h"
 

class Player : public GameObject {
public:
	Player(Game *game);

	void Start();
	void Update(float dt);

	void SetInputAvailable(bool input);
	void SetPosition(sf::Vector2f v);

private:
	sf::Vector2f GetInputVelocity(float dt) const;
	sf::Vector2f CalculateNextPosition();

	bool IsKeyPressed(sf::Keyboard::Key key) const;
	bool IsKeyFresh(sf::Keyboard::Key key) const;
	bool IsJumpPressed() const;
	bool IsLeftPressed() const;
	bool IsRightPressed() const;
	bool IsBoostPressed() const;

	void UpdateVelocity(float dt);
	bool IsGrounded() const;
	float GetWalkSpeed() const;
	float GetVelocitySign() const;
	bool Walk(float dt);
	void AirControls(float dt);
	void GroundControls(float dt);

	bool _inputAvailable;
	sf::Vector2f _vel;

	CollisionRect _colrect;
};
