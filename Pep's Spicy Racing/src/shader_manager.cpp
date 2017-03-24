#include <simple_logger.h>

#include "shader_manager.h"


void Shader_Manager::initialize()
{
	for (int i = 0; i < MAX_SHADERS; i++)
	{
		Shader *shader = new Shader();
		this->shader_list[i] = shader;
	}
	num_shaders = 0;
}

/**
* @brief check shader_list to see if the shader has already been loaded if so
*		increment the reference count and return that shader else build the new
*		shader in the first available shader position, also check if room
*		available in the shader_list, if not exit program
* @param *shader_def_file
* @return a pointer to the shader program that is defined by the given def file
*/
Shader *Shader_Manager::create_shader(char *shader_def_file)
{
	int first_empty = -1;
	for (int i = 0; i < MAX_SHADERS; i++)
	{
		if (this->shader_list[i]->shader_def_file == shader_def_file)
		{
			this->shader_list[i]->reference_count++;
			return this->shader_list[i];
		}
		//while searching for this shader in the list, save the position of the first unused shader in the list
		if (first_empty == -1 && this->shader_list[i]->reference_count == 0)
		{
			first_empty = i;
		}
	}
	if (num_shaders + 1 > MAX_SHADERS)
	{
		slog("Maximum Shaders Reached.");
		exit(1);
	}

	Shader *new_shader = new Shader();
	new_shader->build_shader(shader_def_file);
	new_shader->reference_count = 1;
	this->shader_list[first_empty] = new_shader;
	num_shaders++;
	return this->shader_list[first_empty];

}