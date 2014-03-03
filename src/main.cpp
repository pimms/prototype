#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include "Game.h"
#include "GameScene.h"


int main() 
{
	Game g;
	GameScene gs(&g);
	g.SetScene(&gs);

	return g.Start();
}
