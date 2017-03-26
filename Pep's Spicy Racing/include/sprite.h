#ifndef __SPRITE_H__
#define __SPRITE_H__

#include <map>
#include <string>

#include <SFML/Graphics.hpp>

class Sprite {
public:
	Sprite();
	Sprite(std::string filepath);

	
	//DATA 
public:


private:

};


class SpriteManager {
public:


private:
	std::map <std::string, sf::Image*> image_list;

};

#endif
