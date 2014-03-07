#include "Player.h"
#include "Input.h"
#include "Game.h"
#include "ResourceManager.h"


Player::Player(Game *game) 
	:	 GameObject(game),
		_inputAvailable(false),
		_colrect(sf::Rect<float>(0, 0, 30, 30))
{

}


void Player::Start()
{
	ResourceManager *resmgr = GetGame()->GetResourceManager();
	ImageResource *res = resmgr->GetImageResource("res/dude.png");
	sf::Texture *texture = res->GetTexture();
	SetTexture(texture);

	const sf::Vector2f scale(0.25f, 0.25f);

	// Set the dimension of the collision rectangle
	sf::Vector2u size = texture->getSize();
	_colrect.SetDim(sf::Vector2f(scale.x*float(size.x), 
								 scale.y*float(size.y)));

	SetScale(scale);
}

void Player::Update(float dt)
{
	UpdateVelocity(dt);
	SetPosition(CalculateNextPosition());
}


void Player::SetInputAvailable(bool input) 
{
	_inputAvailable = input;	
}

void Player::SetPosition(sf::Vector2f pos) 
{
	GameObject::SetPosition(pos);
	_colrect.SetPos(pos);
}


/* Private Methods */
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


/* Key handling */
bool Player::IsKeyPressed(sf::Keyboard::Key key) const
{
	const Input *input = GetGame()->GetInput();
	return input->IsKeyDown(key);	
}

bool Player::IsKeyFresh(sf::Keyboard::Key key) const
{
	const Input *input = GetGame()->GetInput();
	return input->IsKeyFresh(key);	
}

bool Player::IsJumpPressed() const
{
	return IsKeyPressed(sf::Keyboard::J);
}

bool Player::IsLeftPressed() const
{
	return IsKeyPressed(sf::Keyboard::H);
}

bool Player::IsRightPressed() const
{
	return IsKeyPressed(sf::Keyboard::L);
}

bool Player::IsBoostPressed() const
{
	return IsKeyPressed(sf::Keyboard::S);
}



/* Movement handling code */
#define FLOAT_EQ(a,b) (abs(a-b) < 0.00001f)

const static float GRAVITY = 15.f;
const static float WALK_SPEED = 10.f;
const static float RUN_SPEED = 15.f;
const static float TURN_MULT = 0.1f;
const static float JUMP_SPEED = 20.f;


void Player::UpdateVelocity(float dt)
{
	_vel.y += GRAVITY * dt; 

	if (_inputAvailable) {
		if (IsGrounded()) {
			GroundControls(dt);
		} else {
			AirControls(dt);
		}

		const float speed = GetWalkSpeed();
		if (_vel.x > speed)
			_vel.x = speed; 
		if (_vel.x < -speed)
			_vel.x = -speed;
	}

	// Assign the new position
	sf::Vector2f pos = GetPosition();
	pos.x += _vel.x * dt;
	pos.y += _vel.y * dt;
	SetPosition(pos);
}

bool Player::IsGrounded() const
{
	return GetPosition().y >= 650.f;
}

float Player::GetWalkSpeed() const
{
	if (IsBoostPressed()) 
		return RUN_SPEED;
	return WALK_SPEED;
}

float Player::GetVelocitySign() const
{
	if (_vel.x < 0.f) {
		return -1.f;
	} else if (_vel.x > 0.f) {
		return 1.f;
	}

	return -1.f;
}

bool Player::Walk(float dt) 
{
	float sign = 0.f;
	sf::Vector2f vel;

	if (IsLeftPressed()) 
		sign = -1.f;
	 else if (IsRightPressed()) 
		sign = 1.f;
	 else 
		return false;
	
	float curSign = GetVelocitySign();
	float v = GetWalkSpeed();
	
	if (!FLOAT_EQ(curSign, 0.f) && !FLOAT_EQ(curSign, sign)) {
		v *= TURN_MULT;
	}
	
	_vel.x += v * sign;
	return true;
}

void Player::AirControls(float dt)
{
	if (_vel.y < 0.f && !IsJumpPressed()) {
		_vel.y = 0.f;
	}

	Walk(dt);
}

void Player::GroundControls(float dt)
{
	if (IsJumpPressed()) {
		_vel.y = -JUMP_SPEED;
		return;
	} else {
		_vel.y = 0.f;
	}

	if (!Walk(dt)) {
		_vel.x *= 0.8f;
	}
}
