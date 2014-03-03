#include "Window.h"


Window::Window()
	:	_window(NULL)
{

}

Window::~Window()
{
	if (_window) {
		_window->close();
		delete _window;
	}
}


void Window::CreateWindow() 
{
	sf::VideoMode vm;
	vm.width = 800;
	vm.height = 600;
	vm.bitsPerPixel = 32;

	if (_window) {
		_window->close();
		delete _window;
	}

	_window = new sf::RenderWindow();
	_window->create(vm, "SMB", sf::Style::Titlebar);
}

sf::RenderWindow* Window::GetRenderWindow() const
{
	if (!_window) {
		throw "Window hasn't been created yet";
	}

	return _window;
}
