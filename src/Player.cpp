#include "Player.h"
#include "Input.h"
#include "Game.h"
#include "ResourceManager.h"


Player::Player(Game *game) 
	:	 GameObject(game),
		_inputAvailable(false)
{

}


void Player::Start()
{
	ResourceManager *resmgr = GetGame()->GetResourceManager();
	ImageResource *res = resmgr->GetImageResource("res/dude.png");
	sf::Texture *texture = res->GetTexture();
	SetTexture(texture);

	SetScale(sf::Vector2f(0.25f, 0.25f));
}

void Player::Update(float dt)
{
	HandleMovement(dt);
}

void Player::SetInputAvailable(bool input) 
{
	_inputAvailable = input;	
}

/* Private Methods */

void Player::HandleMovement(float dt)
{
	if (_inputAvailable) {
		_vel = GetInputVelocity(dt);
		SetPosition(CalculateNextPosition());
	}
}

sf::Vector2f Player::GetInputVelocity(float dt) const
{
	const float speed = 250.f;
	const Input *in = GetGame()->GetInput();
	sf::Vector2f vel;

	if (in->IsKeyDown(sf::Keyboard::H)) {
		vel.x = -speed * dt;
	} else if (in->IsKeyDown(sf::Keyboard::L)) {
		vel.x = speed * dt;
	} else {
		vel.x = 0.f;
	}

	if (in->IsKeyFresh(sf::Keyboard::Space) ||
			in->IsKeyFresh(sf::Keyboard::K)) {
		vel.y = -10.f;
	}

	vel.y += _vel.y + 0.5f;

	return vel;
}

sf::Vector2f Player::CalculateNextPosition()
{
	sf::Vector2f pos = GetPosition();
	pos += _vel;

	if (pos.y > 650.f) {
		_vel.y = 0.f;
		pos.y = 650.f;
	}

	return pos;
}
