#pragma once

#include <SFML/Graphics.hpp>



class CollisionRect {
public:
	CollisionRect(sf::Vector2f pos, sf::Vector2f dim);
	CollisionRect(sf::Rect<float> rect);

	void Set(sf::Rect<float> rect);
	void Set(sf::Vector2f pos, sf::Vector2f dim);
	void SetPos(sf::Vector2f pos);
	void SetDim(sf::Vector2f dim);

	sf::Vector2f GetPos() const;
	sf::Vector2f GetDim() const;
	
	bool Intersects(const CollisionRect &rect) const;

private:
	sf::Rect<float> _rect;
};  
