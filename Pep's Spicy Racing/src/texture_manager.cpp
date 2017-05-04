#include <simple_logger.h>
#include "texture_manager.h"

static Texture_Manager *manager = NULL;

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
		if (manager->texture_list[i]->filepath == texture_filepath)
		{
			if (manager->texture_list[i]->reference_count == 0)
			{
				manager->num_textures++;
			}
			manager->texture_list[i]->in_use = true;
			manager->texture_list[i]->reference_count++;
			return manager->texture_list[i];
		}

		//while searching for this texture in the list, save the position of the texture in the list
		if (first_empty == -1 && manager->texture_list[i]->reference_count == 0)
		{
			first_empty = i;
		}
	}

	//makesure we have the room for a new texture
	if (manager->num_textures + 1 > MAX_TEXTURES)
	{
		slog("Maximum Textures Reached.");
		exit(1);
	}
	if (first_empty == -1)
	{
		slog("Error finding open texture");
		return NULL;
	}

	Texture *new_texture = manager->texture_list[first_empty];
	new_texture->load_texture(texture_filepath.c_str(), repeated, smoothed);
	new_texture->in_use = true;
	new_texture->filepath = texture_filepath;

	new_texture->reference_count = 1;
	manager->texture_list[first_empty] = new_texture;
	manager->num_textures++;
	return manager->texture_list[first_empty];
}

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
				manager->texture_list[i]->in_use = false;
				manager->num_textures--;
			}
		}
	}
}

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
