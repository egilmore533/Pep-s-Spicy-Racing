#include <simple_logger.h>

#include "shader_manager.h"

static Shader_Manager *manager = NULL;

/**
* @brief initialize the shader manager by filling the shader_list with new shaders and setting the shader_count
*/
void Shader_Manager::initialize()
{
	if (manager)
	{
		slog("manager already exists");
		return;
	}

	for (int i = 0; i < MAX_SHADERS; i++)
	{
		Shader *shader = new Shader();
		this->shader_list[i] = shader;
	}
	num_shaders = 0;
	manager = this;
}

/**
* @brief check shader_list to see if the shader has already been loaded if so
*		increment the reference count and return that shader else build the new
*		shader in the first available shader position, also check if room
*		available in the shader_list, if not exit program
* @param shader_def_file path to the shader definition file
* @return a pointer to the shader program that is defined by the given def file
*/
Shader *Shader_Manager::create_shader(std::string shader_def_file)
{
	if (!manager)
	{
		slog("manager doesn't currently exist");
		return NULL;
	}

	int first_empty = -1;
	for (int i = 0; i < MAX_SHADERS; i++)
	{
		if (manager->shader_list[i]->shader_def_file == shader_def_file)
		{
			if (manager->shader_list[i]->reference_count == 0)
			{
				manager->num_shaders++;
			}
			manager->shader_list[i]->reference_count++;
			return manager->shader_list[i];
		}
		//while searching for this shader in the list, save the position of the first unused shader in the list
		if (first_empty == -1 && manager->shader_list[i]->reference_count == 0)
		{
			first_empty = i;
		}
	}
	if (manager->num_shaders + 1 > MAX_SHADERS)
	{
		slog("Maximum Shaders Reached.");
		exit(1);
	}

	if (first_empty == -1)
	{
		slog("error: couldn't find an empty slot and there is apparently still space");
		return NULL;
	}

	Shader *new_shader = manager->shader_list[first_empty];
	new_shader->build_shader(shader_def_file.c_str());
	new_shader->reference_count = 1;
	manager->shader_list[first_empty] = new_shader;
	manager->num_shaders++;
	return manager->shader_list[first_empty];
}

/**
* @brief decrement the reference count of the given shader, enabling it
*		(if the reference count has reached 0) to be replaced by the creation
*		of another shader
* @param *shader_def_file the name of the shader to be dereferenced
*/
void Shader_Manager::dereference_shader(std::string shader_def_file)
{
	if (!manager)
	{
		slog("manager doesn't currently exist");
		return;
	}

	for (int i = 0; i < MAX_SHADERS; i++)
	{
		if (manager->shader_list[i]->shader_def_file == shader_def_file)
		{
			manager->shader_list[i]->reference_count--;
			if (manager->shader_list[i]->reference_count == 0)
			{
				manager->num_shaders--;
			}
		}
	}
}

/**
* @brief clears the shader list by setting all reference counts equal to zero
*/
void Shader_Manager::clear()
{
	if (!manager)
	{
		slog("manager doesn't currently exist");
		return;
	}

	for (int i = 0; i < MAX_SHADERS; i++)
	{
		manager->shader_list[i]->reference_count = 0;
	}
	num_shaders = 0;
}
