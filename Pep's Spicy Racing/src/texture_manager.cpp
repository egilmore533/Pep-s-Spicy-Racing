#include <simple_logger.h>
#include "texture_manager.h"

static Texture_Manager *manager = NULL;

/**
* @brief check texture_list to see if the texture has already been loaded if so
*		increment the reference count and return that texture else load the new
*		texture in the first available texture position, also check if room
*		available in the texture_list, if not exit program
* @param texture_filepath path to the texture file
* @param repeated	whether or not the texture should be repeated (doesn't matter if the texture has been loaded)
* @param smoothed	whether or not the texture should be smoothed (doesn't matter if the texture has been loaded)
* @return a pointer to the texture from the given files
*/
Texture *Texture_Manager::create_texture(std::string texture_filepath, bool repeated, bool smoothed)
{
	if (!manager)
	{
		slog("manager doesn't currently exist");
		return NULL;
	}

	int first_empty = -1;
	for (int i = 0; i < MAX_TEXTURES; i++)
	{
		//if the filepaths match then just reuse this texture 
		if (manager->texture_list[i]->filepath == texture_filepath && !manager->texture_list[i]->text)
		{
			if (manager->texture_list[i]->reference_count == 0)
			{
				manager->num_textures++;
			}
			manager->texture_list[i]->image = true;
			manager->texture_list[i]->reference_count++;
			return manager->texture_list[i];
		}

		//while searching for this texture in the list, save the position of the texture in the list
		if (first_empty == -1 && manager->texture_list[i]->reference_count == 0)
		{
			first_empty = i;
		}
	}

	if (first_empty == -1)
	{
		slog("Error finding open texture");
		return NULL;
	}

	//TODO use the texture_list[first_empty] instead of a new texture by making a new texture function that loads data
	//Texture *new_texture = new Texture(texture_filepath.c_str(), repeated, smoothed);

	Texture *new_texture = manager->texture_list[first_empty];
	new_texture->load_texture(texture_filepath.c_str(), repeated, smoothed);
	new_texture->image = true;
	new_texture->filepath = texture_filepath;
	new_texture->reference_count = 1;
	manager->texture_list[first_empty] = new_texture;
	manager->num_textures++;
	return manager->texture_list[first_empty];
}

/**
* @brief loads a text sprite using sf::Text from the given font with the given font size, checks if the text already exists and uses it instead if available and increases the reference_count
*			(TODO load text stuff that is need from a json def file)
* @param text_string the text to create a texture for
* @param font_size the size of the font to use for the text
* @param font_filepath the path to the font to use
* @return a pointer to the loaded texture
*/
Texture *Texture_Manager::create_text_texture(std::string text_string, unsigned int font_size, std::string font_filepath)
{
	if (!manager)
	{
		slog("manager doesn't currently exist");
		return NULL;
	}

	int first_empty = -1;
	for (int i = 0; i < MAX_TEXTURES; i++)
	{
		//if the data matches another texture use that instead
		if (manager->texture_list[i]->font_name == font_filepath && manager->texture_list[i]->font_size == font_size && manager->texture_list[i]->text_string == text_string && !manager->texture_list[i]->image)
		{
			if (manager->texture_list[i]->reference_count == 0)
			{
				manager->num_textures++;
			}
			manager->texture_list[i]->text = true;
			manager->texture_list[i]->reference_count++;
			return manager->texture_list[i];
		}

		//while searching for this texture in the list, save the position of the texture in the list
		if (first_empty == -1 && manager->texture_list[i]->reference_count == 0)
		{
			first_empty = i;
		}
	}

	//TODO use the texture_list[first_empty] instead of a new texture by making a new texture function that loads data
	//Texture *new_texture = new Texture(texture_filepath.c_str(), repeated, smoothed);

	Texture *new_texture = manager->texture_list[first_empty];
	new_texture->load_text_texture(text_string, font_size, font_filepath);
	new_texture->text = true;
	new_texture->text_string = text_string;
	new_texture->font_size = font_size;
	new_texture->font_name = font_filepath;

	new_texture->reference_count = 1;
	manager->texture_list[first_empty] = new_texture;
	manager->num_textures++;
	return manager->texture_list[first_empty];
}

/**
* @brief decrement the reference count of the given texture, enabling the manager
*		(if the reference count has reached 0) to replace the texture by loading
*		another texture
* @param texture_filepath the file of the texture to be dereferenced
*/
void Texture_Manager::dereference_texture(std::string texture_filepath)
{
	if (!manager)
	{
		slog("manager doesn't currently exist");
		return;
	}

	for (int i = 0; i < MAX_TEXTURES; i++)
	{
		if (manager->texture_list[i]->filepath == texture_filepath)
		{
			if(manager->texture_list[i]->reference_count >= 1) manager->texture_list[i]->reference_count--;
			if (manager->texture_list[i]->reference_count == 0)
			{
				manager->texture_list[i]->image = false;
				manager->texture_list[i]->text = false;
				manager->num_textures--;
			}
		}
	}
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
