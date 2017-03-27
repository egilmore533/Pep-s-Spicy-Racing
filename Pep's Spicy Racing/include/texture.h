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
	* @param *filepath	path to the image in from our working directory
	* @param repeated	boolean flag to set whether or not to repeat the texture when mapping to our model
	* @param smoothed	boolean flag to set how we handle the drawing of the texture
	*/
	Texture(const char *filepath, bool repeated, bool smoothed);

	/**
	* @brief destructor that handles destroying the texture's members, none now, but in the future there may be some
	*/
	~Texture();

	/**
	 * @brief gets the unique texture ID for use
	 * @return GLuint texture
	 */
	GLuint get_texture();
	
public:
	std::string filepath;		/**< the filepath to this texture, used to identify different textures */
	int reference_count;		/**< the number of times this texture is being referenced */

private:
	GLuint texture;				/**< the index for the texture */
};

#endif
