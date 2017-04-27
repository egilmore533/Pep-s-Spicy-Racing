#ifndef __SPRITE_H__
#define __SPRITE_H__

#include <string>

#include <GL/glew.h>

#include <glm/common.hpp>

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
	* @brief loads the texture from an image file
	* @param filepath	the path to the image file
	*/
	void set_sprite_texture(std::string filepath);

	/**
	* @brief loads the texture from a json object for text
	* @param text_json	the defintion object containing information for a text sprite
	*/
	void set_sprite_texture(json text_object);

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

	//Data
	bool in_use;				/**< flag for manager to understand if sprite is in use */
	int id;						/**< the id of the sprite */

	Texture *texture;			/**< the texture the sprite will use to draw */
	Shader *shader;				/**< the shader used to draw the sprite */

	glm::vec2 screen_position;		/**< the screen position to draw the sprite to */
	glm::vec4 color;				/**< the color to draw the sprite */
	glm::vec2 size;					/**< the size of the sprite */
	float rotation;					/**< how much to rotate the sprite */

	GLuint model_location;			/**< uniform location of the model in the shader */
	GLuint projection_location;		/**< the uniform projection location in the shader */
	GLuint sprite_color_location;	/**< uniform location of the sprite color in the shader */
};

#endif
