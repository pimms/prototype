#pragma once

#include <SFML/Graphics.hpp>

#include <string>
using std::string;

#include <map>
using std::map;


class Resource {
protected:
	friend class ResourceManager;
	Resource();
	virtual ~Resource();
	virtual bool Load(string file) = 0;
	virtual void Free() = 0;
	int _ref;
};

class ImageResource : public Resource {
public:
	sf::Texture* GetTexture();

private:
	friend class ResourceManager;
	ImageResource();
	bool Load(string file);
	void Free();
	sf::Texture *_tex;
};


class ResourceManager {
public:
	ResourceManager();
	~ResourceManager();

	Resource* GetResource(string file);
	ImageResource* GetImageResource(string file);

	void FreeResource(string file);
	void FreeResources();

private:
	Resource* LoadResource(string file);

	map<string,Resource*> _resources;
};


