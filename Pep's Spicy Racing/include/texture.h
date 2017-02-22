#ifndef __TEXTURE_H__
#define __TEXTURE_H__

#include <SFML\OpenGL.hpp>
#include <SFML\Graphics.hpp>

class Texture
{
public: 
	/**
	* @brief creates the texture from a given filepath, sets the smoothing and repeating flags, uses the whole image given
	* @param *filepath	path to the image in from our working directory
	* @param repeated	boolean flag to set whether or not to repeat the texture when mapping to our model
	* @param smoothed	boolean flag to set how we handle the drawing of the texture
	*/
	Texture(char *filepath, bool repeated, bool smoothed);

	/**
	* @brief creates the texture from a given filepath, sets the smoothing and repeating flags, uses the image as defined
	* @param *filepath	path to the image in from our working directory
	* @param repeated	boolean flag to set whether or not to repeat the texture when mapping to our model
	* @param smoothed	boolean flag to set how we handle the drawing of the texture
	* @param start_coordinate	vector of the starting point to take from the image file
	* @param size				vector of the dimensions, in pixels, to take from the image file
	*/
	Texture(char * filepath, bool repeated, bool smoothed, sf::Vector2i start_coordinates, sf::Vector2i size);

	/**
	* @brief destructor that handles destroying the texture's members, none now, but in the future there may be some
	*/
	~Texture();

	/**
	* @brief handles binding the texture
	*/
	void bind();

private:
	sf::Texture texture; /**< the data for the texture */


};

#endif
