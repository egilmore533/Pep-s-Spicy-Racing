#ifndef __SHADER_H__
#define __SHADER_H__

#include <GL/glew.h>

/**
* @brief creates our simple shader program that only handles one vertex and one fragment shader
*		 attaches them to link the program, handles errors by slogging the error returning 
*		 invalid shader index
* @param *vsPath	a character pointer containing the path to our vertex shader file
* @param *fsPath	a character pointer containing the path to our fragment shader file
*/
GLuint build_shader_program(const char *vsPath, const char *fsPath);

/**
* @brief cimple compiler for our shader files, determines if vertex, fragment, or geometry 
*		shader and handles appropriately on error
* @param eShaderType	the type of shader being compiled
* @param *strShaderFile	the path to the shader file being compiled
*/
GLuint create_shader(GLenum eShaderType, const char *strShaderFile);

#endif
