#include <GL\glew.h>

#include <simple_logger.h>

#include "texture.h"

Texture::Texture()
{
	texture = 0;
}

Texture::Texture(const char *filepath, bool repeated, bool smoothed)
{
	sf::Image image;
	std::string file = filepath;
	if (!image.loadFromFile(file))
	{
		slog("Error Loading image: %s", filepath);
		return;
	}

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
}

Texture::~Texture()
{

}

/**
* @brief gets the unique texture ID for use
* @return GLuint texture
*/
GLuint Texture::get_texture()
{
	return texture;
}
