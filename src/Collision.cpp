#include "Collision.h"

CollisionRect::CollisionRect(sf::Vector2f pos, sf::Vector2f dim)
{
	Set(pos, dim);
}

CollisionRect::CollisionRect(sf::Rect<float> rect) 
{
	Set(rect);
}


void CollisionRect::Set(sf::Rect<float> rect)
{
	_rect = rect;
}

void CollisionRect::Set(sf::Vector2f pos, sf::Vector2f dim)
{
	SetPos(pos);
	SetDim(dim);
}

void CollisionRect::SetPos(sf::Vector2f pos)
{
	_rect.left = pos.x;
	_rect.top = pos.y;
}

void CollisionRect::SetDim(sf::Vector2f dim)
{
	_rect.width = dim.x;
	_rect.height = dim.y;
}


sf::Vector2f CollisionRect::GetPos() const
{
	return sf::Vector2f(_rect.left, _rect.top);
}

sf::Vector2f CollisionRect::GetDim() const
{
	return sf::Vector2f(_rect.width, _rect.height);
}


bool CollisionRect::Intersects(const CollisionRect &rect) const
{
	return _rect.intersects(rect._rect);
}

