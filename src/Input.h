#pragma once

#include <SFML/Window.hpp>

class Input {
public:
	Input();
	~Input();

	void UnfreshKeys();

	void KeyPressed(const sf::Event &evt);	
	void KeyReleased(const sf::Event &evt);	

	bool IsKeyDown(int key) const;
	bool IsKeyFresh(int key) const;
	
private:
	void GetBit(unsigned key, int &index, int &bit) const;

	unsigned _down[16];
	unsigned _fresh[16];
};
