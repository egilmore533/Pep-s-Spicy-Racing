#ifndef __TEXTURE_MANAGER_H__
#define __TEXTURE_MANAGER_H__

#define MAX_TEXTURES	1000

#include <array>
#include "texture.h"

class Texture_Manager
{
public:
	/**
	* @brief check texture_list to see if the texture has already been loaded if so
	*		increment the reference count and return that texture else load the new
	*		texture in the first available texture position, also check if room
	*		available in the texture_list, if not exit program
	* @param texture_filepath path to the texture file
	* @return a pointer to the texture from the given files
	*/
	static Texture *create_texture(std::string texture_filepath);

	/**
	* @brief decrement the reference count of the given texture, enabling the manager
	*		(if the reference count has reached 0) to replace the texture by loading
	*		another texture
	* @param texture_filepath the file of the texture to be dereferenced
	*/
	static void dereference_texture(std::string texture_filepath);

	/**
	* @brief allocates and sets default data for the textures in the texture_manager's list
	*			then sets this texture manager to be the global texture manager
	*/
	void initialize();
	
private:
	std::array<class Texture*, MAX_TEXTURES> texture_list;		/**< the list of textures in the program, allocates pace for the number of textures defined by the macro MAX_TEXTURES  */
	int num_textures;											/**< the number of unique textures in the texture_list*/
};

#endif
