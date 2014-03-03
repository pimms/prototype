#include "ResourceManager.h"


/* Resource Implementations */

Resource::Resource() 
	: 	_ref(0)
{

}

Resource::~Resource()
{

}


ImageResource::ImageResource()
	:	_tex(NULL)
{

}

bool ImageResource::Load(string file)
{
	if (_tex) {
		Free();
	}

	_tex = new sf::Texture();
	return _tex->loadFromFile(file);
}

void ImageResource::Free()
{
	if (_tex) {
		delete _tex;
		_tex = NULL;
	}
}

sf::Texture* ImageResource::GetTexture()
{
	return _tex;
}



/* ResourceManager */

ResourceManager::ResourceManager()
{ 
	
}

ResourceManager::~ResourceManager()
{
	FreeResources();
}


Resource* ResourceManager::GetResource(string file)
{
	if (_resources.count(file)) {
		return _resources[file];
	}

	Resource *res = LoadResource(file);
	if (res) {
		_resources[file] = res;
	}

	return res;
}

ImageResource* ResourceManager::GetImageResource(string file)
{
	Resource *res = GetResource(file);
	return dynamic_cast<ImageResource*>(res);
}


void ResourceManager::FreeResource(string file)
{
	Resource *res = NULL;
	
	if (!_resources.count(file)) {
		return;
	}
	
	res = _resources[file];
	_resources.erase(file);
	res->Free();
	delete res;
}

void ResourceManager::FreeResources()
{
	map<string,Resource*>::iterator it;
	
	for (it=_resources.begin(); it!=_resources.end(); it++) {
		it->second->Free();
		delete it->second;
	}

	_resources.clear();
}


/* Private Methods */
Resource* ResourceManager::LoadResource(string file)
{
	Resource *res = NULL;
	string ext;
	
	ext = file.substr(file.find_last_of(".") + 1);

	if (ext == "png" || ext == "jpg") {
		res = new ImageResource();
	}

	if (res) {
		res->Load(file);
	}

	return res;
}
