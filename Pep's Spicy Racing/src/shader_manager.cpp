#include <simple_logger.h>

#include "shader_manager.h"

static Shader_Manager *manager = NULL;

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
			if (manager->shader_list[i]->reference_count >= 1) manager->shader_list[i]->reference_count--;
			if (manager->shader_list[i]->reference_count == 0)
			{
				manager->num_shaders--;
			}
		}
	}
}

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
