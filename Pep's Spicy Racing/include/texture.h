#ifndef __TEXTURE_H__
#define __TEXTURE_H__

#include <SFML\OpenGL.hpp>
#include <SFML\Graphics.hpp>

class Texture
{
public: 
	/**
	* @brief default texture constructor setting all data to default values
	*/
	Texture();

	/**
	* @brief creates the texture from a given filepath, sets the smoothing and repeating flags, uses the whole image given
	*/
	Texture(std::string filepath, bool repeated, bool smoothed);

	/**
	* @brief loads texture from given file path, sets smoothing and repeating accordingly, uses the whole image given so it can be reused multiple times 
	*			for instance one sprite might only want the texture's blue values to be drawn while another wants all the colors, this should all be 
	*			done on the sprite end, not the texture
	* @param *filepath	path to the image in from our working directory
	* @param repeated	boolean flag to set whether or not to repeat the texture when mapping to our model
	* @param smoothed	boolean flag to set how we handle the drawing of the texture
	*/
	void load_texture(std::string filepath, bool repeated, bool smoothed);

	/**
	 * @brief gets the unique texture ID for use
	 * @return GLuint texture
	 */
	GLuint get_texture();
	
public:
	std::string filepath;		/**< the filepath to this texture, used to identify different textures */

	int reference_count;		/**< the number of times this texture is being referenced */

	bool in_use;

private:
	GLuint texture;				/**< the index for the texture */
};

#endif
