#include <GL\glew.h>

#include "texture.h"

Texture::Texture(char *filepath, bool repeated, bool smoothed)
{
	if (!texture.loadFromFile(filepath))
	{
		printf("Error Loading Texture: %s", filepath);
		return;
	}

	texture.setRepeated(repeated);
	texture.setSmooth(smoothed);

	sf::Texture::bind(&texture);
}

Texture::Texture(char *filepath, bool repeated, bool smoothed, sf::Vector2i start_coordinates, sf::Vector2i size)
{
	if (!texture.loadFromFile(filepath, sf::IntRect(start_coordinates.x, start_coordinates.y, size.x, size.y)))
	{
		printf("Error Loading Texture: %s", filepath);
		return;
	}

	texture.setRepeated(repeated);
	texture.setSmooth(smoothed);
}

Texture::~Texture()
{

}

void Texture::texture_draw()
{

}