#pragma once

#include <SFML/Graphics.hpp>


class Window {
public:
	Window();
	~Window();
	
	void CreateWindow();
	sf::RenderWindow *GetRenderWindow() const;

private:
	sf::RenderWindow *_window;
};
