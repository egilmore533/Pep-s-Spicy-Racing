#include <GL\glew.h>

#include <simple_logger.h>

#include "texture.h"

Texture::Texture(char *filepath, bool repeated, bool smoothed)
{
	sf::Image *img_data = new sf::Image();

	if (!img_data->loadFromFile(filepath))
	{
		slog("Error Loading Texture: %s", filepath);
		return;
	}

	glEnable(GL_TEXTURE_2D);

	texture.setRepeated(repeated);
	texture.setSmooth(smoothed);

	//sf::Texture::bind(&texture);
}

Texture::Texture(char *filepath, bool repeated, bool smoothed, sf::Vector2i start_coordinates, sf::Vector2i size)
{
	if (!texture.loadFromFile(filepath, sf::IntRect(start_coordinates.x, start_coordinates.y, size.x, size.y)))
	{
		slog("Error Loading Texture: %s", filepath);
		return;
	}

	texture.setRepeated(repeated);
	texture.setSmooth(smoothed);

	glEnable(GL_TEXTURE_2D);

	texture.setRepeated(repeated);
	texture.setSmooth(smoothed);

	sf::Texture::bind(&texture);
}

Texture::~Texture()
{

}

void Texture::bind()
{
	sf::Texture::bind(&texture);
}