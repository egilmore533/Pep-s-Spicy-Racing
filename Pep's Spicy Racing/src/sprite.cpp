#include <simple_logger.h>

#include "graphics.h"
#include "texture.h"
#include "texture_manager.h"
#include "shader_manager.h"
#include "sprite.h"


/**
* @brief the default constructor for a sprite, setting all data to default values, used by sprite manager to create an empty list of sprites
*/
Sprite::Sprite()
{
	texture = 0;
	id = -1;
	model_location = 0;
	sprite_color_location = 0;
	in_use = false;
}

/**
* @brief loads the texture from an image file
* @param filepath	the path to the image file
*/
void Sprite::set_sprite_texture(std::string filepath)
{
	texture = Texture_Manager::create_texture(filepath, false, true);
}

/**
* @brief loads the texture from a json object for text
* @param text_json	the defintion object containing information for a text sprite
*/
void Sprite::set_sprite_texture(json text_object)
{
	texture = Texture_Manager::create_text_texture(text_object["text"], (unsigned int)text_object["font_size"], text_object["font_filepath"]);
}

/**
* @brief defines how to draw the sprite, needs to be reset everytime the sprite is drawn
* @param screen_position the position on the window to draw the sprite
* @param size			the size of the sprite
* @param rotation		the rotation of the sprite in degrees
* @param color			the color the sprite needs to be color modded to become
*/
void Sprite::set_data(glm::vec2 screen_position, glm::vec2 size, float rotation, glm::vec4 color)
{
	this->screen_position = screen_position;
	this->size = size;
	this->rotation = rotation;
	this->color = color;
}

/**
* @brief loads shader and the uniform locations
* @param shader_def the definition file for the shader program
*/
void Sprite::set_shader(std::string shader_def)
{
	shader = Shader_Manager::create_shader(shader_def);
	model_location = glGetUniformLocation(shader->program, "model");
	projection_location = glGetUniformLocation(shader->program, "projection");
	sprite_color_location = glGetUniformLocation(shader->program, "sprite_color");
}
