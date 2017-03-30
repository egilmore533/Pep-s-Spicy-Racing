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
	* @brief loads a texture that contains the text string given using sfml to load the text from the given font with the given font_size 
	* @param text			the text for the texture will show
	* @param font_size		the size the font will be drawn for
	* @param font_filepath	the path to the font_file
	*/
	void load_text_texture(std::string text, unsigned int font_size, std::string font_filepath);

	/**
	 * @brief gets the unique texture ID for use
	 * @return GLuint texture
	 */
	GLuint get_texture();
	
public:
	std::string filepath;		/**< the filepath to this texture, used to identify different textures */
	
	std::string text_string;	/**< text string that this texture contains, used to identify texture in conjunction with font_size and font if the texture is for text */
	std::string font_name;		/**< font name that this texture used, used to identify texture in conjunction with text_string and font_size if the texture is for text */
	unsigned int font_size;		/**< font size that this texture used, used to identify texture in conjunction with text_string and font_name if the texture is for text */

	int reference_count;		/**< the number of times this texture is being referenced */

private:
	GLuint texture;				/**< the index for the texture */
};

#endif
