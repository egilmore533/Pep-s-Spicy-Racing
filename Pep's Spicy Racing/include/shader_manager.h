#ifndef __SHADER_MANAGER_H__
#define __SHADER_MANAGER_H__

#include <array>
#include "shader.h"

#define MAX_SHADERS		1000			//If there is ever more than a thousand shaders at once the lack of space isn't the only problem

class Shader_Manager
{
public:
	/**
	* @brief check shader_list to see if the shader has already been loaded if so 
	*		increment the reference count and return that shader else build the new 
	*		shader in the first available shader position, also check if room 
	*		available in the shader_list, if not exit program
	* @param shader_def_file path to the shader definition file
	* @return a pointer to the shader program that is defined by the given def file
	*/
	static Shader *create_shader(std::string shader_def_file);

	/**
	* @brief decrement the reference count of the given shader, enabling it 
	*		(if the reference count has reached 0) to be replaced by the creation 
	*		of another shader
	* @param shader_def_file the name of the shader to be dereferenced
	*/
	static void dereference_shader(std::string shader_def_file);

	/**
	* @brief initialize the shader manager by filling the shader_list with new shaders and setting the shader_count
	*/
	void initialize();

	/**
	* @brief clears the shader list by setting all reference counts equal to zero
	*/
	void clear();

private:
	std::array<class Shader *, MAX_SHADERS> shader_list;	/**< the list of all shaders in the program, allocates space for the number of shaders defined by the macro MAX_SHADERS */
	int num_shaders;										/**< the number of shaders currently in the shader list (shaders with reference counts of more than one)*/
};

#endif