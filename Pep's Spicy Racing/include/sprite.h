#ifndef __SPRITE_H__
#define __SPRITE_H__

#include <map>
#include <string>

#include <SFML/Graphics.hpp>

#include <glm\common.hpp>

#include "json_helper.h"

class Sprite {
public:
	/**
	* @brief the default constructor for a sprite, setting all data to default values, used by sprite manager to create an empty list of sprites
	*/
	Sprite();

	/**
	* @brief constructor to make a Sprite from an image file
	* @param filepath	the path to the image file, also used to identify the sprite
	*/
	Sprite(std::string filepath);

	/**
	* @brief configures how to draw the sprite, needs to be set everytime the sprite is drawn
	* @param screen_position the position on the window to draw the sprite
	* @param color			 the color the sprite needs to be color modded to become
	* @param scale			 the scale the sprite will be drawn using
	* @param rotation		 the rotation of the sprite in degrees
	* @param rectangle		 the rectangle the sprite will use from the texture
	* @param smoothed		 should the sprite be scaled and drawn smoothly and a little blurry or just upscaled and pixelated
	*/
	void set_data(glm::vec2 screen_position, glm::u32vec4 color, glm::vec2 scale, float rotation, glm::u32vec4 rectangle, bool smoothed);

	/**
	* @brief draws the sprite to the screen with its current configuration
	*/
	void draw();
	
	//DATA
	std::string image_filepath;

private:
	sf::Sprite sprite;
	sf::Texture texture;
};

#endif
