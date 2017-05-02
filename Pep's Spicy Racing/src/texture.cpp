#include <GL\glew.h>

#include <simple_logger.h>

#include "texture.h"

/**
* @brief default texture constructor setting all data to default values
*/
Texture::Texture()
{
	texture = 0;
	filepath = "";
	reference_count = 0;
	in_use = false;
}

/**
* @brief loads texture from given file path, sets smoothing and repeating accordingly, uses the whole image given so it can be reused multiple times
*			for instance one sprite might only want the texture's blue values to be drawn while another wants all the colors, this should all be
*			done on the sprite end, not the texture
* @param filepath	path to the image in from our working directory
* @param repeated	boolean flag to set whether or not to repeat the texture when mapping to our model
* @param smoothed	boolean flag to set how we handle the drawing of the texture
*/
void Texture::load_texture(std::string filepath, bool repeated, bool smoothed)
{
	sf::Image image;
	std::string file = filepath;
	if (!image.loadFromFile(file))
	{
		slog("Error Loading image: %s", filepath);
		return;
	}

	glActiveTexture(GL_TEXTURE0);
	glEnable(GL_TEXTURE_2D);

	glGenTextures(1, &texture);
	// "Bind" the newly created texture : all future texture functions will modify this texture
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image.getSize().x, image.getSize().y, 0, GL_RGBA, GL_UNSIGNED_BYTE, image.getPixelsPtr());

	if (repeated)
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	}

	if (smoothed)
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	}
	else
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	}
	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);
	glActiveTexture(GL_TEXTURE0);
}

/**
* @brief gets the unique texture ID for use
* @return GLuint texture
*/
GLuint Texture::get_texture()
{
	return texture;
}
