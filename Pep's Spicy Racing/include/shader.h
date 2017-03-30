#ifndef __SHADER_H__
#define __SHADER_H__

#include <string>
#include <GL/glew.h>

class Shader {
public:

	/**
	* @brief default constructor for Shader
	*/
	Shader();

	/**
	* @brief loads a Shader's files and compiles them, also saves the Shader's def file 
	* @param def_file	path to the file to load from
	*/
	void build_shader(std::string def_file);

	GLuint program;					/**< the shader program itself */
	std::string shader_def_file;	/**< filepath to this shader's def file, used to uniquely identify each shader in the shader list */
	int reference_count;			/**< number of times this shader has been referenced, if it isn't referenced it can be used as the next slot in the shader manager's list */
};

/**
* @brief creates our simple shader program that only handles one vertex and one fragment shader
*		 attaches them to link the program, handles errors by slogging the error returning
*		 invalid shader index
* @param vsPath	 the path to our vertex shader file
* @param fsPath	 the path to our fragment shader file
*/
GLuint build_shader_program(std::string vsPath, std::string fsPath);

/**
* @brief simple compiler for our shader files, determines if vertex, fragment, or geometry
*		shader and handles appropriately on error
* @param eShaderType	the type of shader being compiled
* @param strShaderFile	the path to the shader file being compiled
*/
GLuint create_shader(GLenum eShaderType, std::string strShaderFile);

#endif
