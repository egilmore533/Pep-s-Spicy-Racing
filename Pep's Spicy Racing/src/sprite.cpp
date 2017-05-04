#include <simple_logger.h>

#include "graphics.h"
#include "texture.h"
#include "texture_manager.h"
#include "shader_manager.h"
#include "sprite.h"

Sprite::Sprite()
{
	texture = 0;
	id = -1;
	model_location = 0;
	sprite_color_location = 0;
	in_use = false;
}

void Sprite::set_sprite_texture(std::string filepath)
{
	texture = Texture_Manager::create_texture(filepath, false, true);
}

void Sprite::set_data(glm::vec2 screen_position, glm::vec2 size, float rotation, glm::vec4 color)
{
	this->screen_position = screen_position;
	this->size = size;
	this->rotation = rotation;
	this->color = color;
}

void Sprite::set_shader(std::string shader_def)
{
	shader = Shader_Manager::create_shader(shader_def);
	model_location = glGetUniformLocation(shader->program, "model");
	projection_location = glGetUniformLocation(shader->program, "projection");
	sprite_color_location = glGetUniformLocation(shader->program, "sprite_color");
}
