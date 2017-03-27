#include <GL/glew.h>

#include <glm/gtc/matrix_transform.hpp>

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
}

/**
* @brief constructor to make a Sprite from an image file
* @param filepath	the path to the image file, also used to identify the sprite
*/
Sprite::Sprite(std::string filepath)
{
	texture = Texture_Manager::create_texture(filepath, false, true);
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
	sprite_color_location = glGetUniformLocation(shader->program, "sprite_color");
}

/**
* @brief draws the sprite to the screen with its current configuration
*/
void Sprite::draw()
{
	glUseProgram(shader->program);
	glm::mat4 model;

	model = glm::translate(model, glm::vec3(screen_position, 0.0f));  // First translate (transformations are: scale happens first, then rotation and then finally translation happens; reversed order)

	model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.0f)); // Move origin of rotation to center of quad
	model = glm::rotate(model, rotation, glm::vec3(0.0f, 0.0f, 1.0f)); // Then rotate
	model = glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.0f)); // Move origin back

	model = glm::scale(model, glm::vec3(size, 1.0f)); // Last scale
	
	glUniformMatrix4fv(model_location, 1, GL_FALSE, &model[0][0]);
	glUniform4f(sprite_color_location, color.x, color.y, color.z, color.w);

	//glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture->get_texture());

	/*
	most of this function needs to be in the sprite_manager draw I think

	glBindVertexArray(this->quadVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
	*/
}
