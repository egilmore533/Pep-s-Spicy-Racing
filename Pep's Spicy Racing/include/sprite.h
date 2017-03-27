#ifndef __SPRITE_H__
#define __SPRITE_H__

#include <glm/glm.hpp>
#include "texture.h"
#include "shader.h"
#include "json_helper.h"

class Sprite {
public:
	/**
	* @brief the default constructor for a sprite, setting all data to default values, used by sprite manager to create an empty list of sprites
	*/
	Sprite();

	/**
	* @brief constructor to make a Sprite from an image file
	* @param filepath	the path to the image file
	*/
	Sprite(std::string filepath);

	/**
	* @brief configures how to draw the sprite
	* @param screen_position the position on the window to draw the sprite
	* @param color			 the color components the sprite will draw
	* @param size			 the scale the sprite will be drawn using and also used to define the center of the sprite in rotation
	* @param rotation		 the rotation of the sprite in degrees
	*/
	void set_data(glm::vec2 screen_position, glm::vec2 size, float rotation = 0.0f, glm::vec4 color = glm::vec4(1.0f));

	/**
	* @brief loads shader and the uniform locations
	* @param shader_def the definition file for the shader program
	*/
	void set_shader(std::string shader_def);

	/**
	* @brief draws the sprite to the screen with its current configuration
	*/
	void draw();

private:
	Texture *texture;			/**< the texture the sprite will use to draw */
	Shader *shader;
	int id;						/**< the id of the sprite */
	glm::vec2 screen_position;
	glm::vec4 color;
	glm::vec2 size;
	float rotation;

	GLuint model_location;
	GLuint sprite_color_location;
};

#endif
