#include "Platform.h"
#include "ResourceManager.h"
#include "Game.h"


Platform::Platform(Game *game, sf::Vector2f pos) 
	:	GameObject(game),
		_colrect(pos, sf::Vector2f(100.f, 100.f))
{

}


void Platform::Start()
{
	ResourceManager *resmgr = GetGame()->GetResourceManager();
	ImageResource *res = resmgr->GetImageResource("res/platform.png");
	sf::Texture *texture = res->GetTexture();
	SetTexture(texture);

	SetPosition(_colrect.GetPos());
}
