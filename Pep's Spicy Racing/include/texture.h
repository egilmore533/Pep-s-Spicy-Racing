#ifndef _TEXTURE_H_
#define _TEXTURE_H_

#include <SFML\OpenGL.hpp>
#include <SFML\Graphics.hpp>

class Texture
{
public: 
	Texture(char * filepath, bool repeated, bool smoothed);
	Texture(char * filepath, bool repeated, bool smoothed, sf::Vector2i start_coordinates, sf::Vector2i size);
	~Texture();
	void texture_draw();

private:
	GLuint textureID;
	sf::Texture texture;


};

#endif
