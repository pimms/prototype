#include "Input.h"
#include <string.h>


Input::Input() 
{
	memset(_down, 0, sizeof(unsigned)*16);
	memset(_fresh, 0, sizeof(unsigned)*16);
}

Input::~Input()
{

}


void Input::KeyPressed(const sf::Event &evt)
{
	int index, bit;
	GetBit(evt.key.code, index, bit);

	if ((_down[index]&(1<<bit)) == 0) {
		_fresh[index] |= (1<<bit);
	}
	
	_down[index] |= (1<<bit);
}

void Input::KeyReleased(const sf::Event &evt)
{
	int index, bit;
	GetBit(evt.key.code, index, bit);

	_down[index] &= ~(1<<bit);
	_fresh[index] &= ~(1<<bit);
}


bool Input::IsKeyDown(int key) const 
{
	int index, bit;
	GetBit(key, index, bit);

	return _down[index] & (1<<bit);
}

bool Input::IsKeyFresh(int key) const 
{
	int index, bit;
	GetBit(key, index, bit);

	return _fresh[index] & (1<<bit);
}


/* Private Methods */

void Input::GetBit(unsigned key, int &index, int &bit) const
{
	index = key / 32;
	bit = key % 32;
}

void Input::UnfreshKeys()
{
	memset(_fresh, 0, sizeof(unsigned)*16);
}
