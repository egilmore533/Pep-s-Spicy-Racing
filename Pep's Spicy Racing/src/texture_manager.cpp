
#include "texture_manager.h"

static Texture_Manager *manager = NULL;

/**
* @brief check texture_list to see if the texture has already been loaded if so
*		increment the reference count and return that texture else load the new
*		texture in the first available texture position, also check if room
*		available in the texture_list, if not exit program
* @param texture_filepath path to the texture file
* @return a pointer to the texture from the given files
*/
Texture *Texture_Manager::create_texture(std::string texture_filepath)
{

}

/**
* @brief decrement the reference count of the given texture, enabling the manager
*		(if the reference count has reached 0) to replace the texture by loading
*		another texture
* @param texture_filepath the file of the texture to be dereferenced
*/
void Texture_Manager::dereference_texture(std::string texture_filepath)
{

}


/**
* @brief allocates and sets default data for the textures in the texture_manager's list
*			then sets this texture manager to be the global texture manager
*/
void Texture_Manager::initialize()
{
	for (int i = 0; i < MAX_TEXTURES; i++)
	{
		//memset
		Texture *texture = new Texture();
		this->texture_list[i] = texture;
	}
	num_textures = 0;
	manager = this;
}
