#ifndef __SHADER_H__
#define __SHADER_H__

#include <GL/glew.h>

class Shader {
public:
	Shader();
	~Shader();
	void Use();
	void build_shader(char *def_file);

	GLuint shader;					/**< the shader program itself */
	char *shader_def_file;	/**< filepath to this shader's def file, used to uniquely identify each shader in the shader list */
	int reference_count;			/**< number of times this shader has been referenced, if it isn't referenced it can be used as the next slot in the shader manager's list */
};


//TODO REMOVE THESE AND DO SOMETHING A BIT MORE ELEGANT
/**
* @brief creates our simple shader program that only handles one vertex and one fragment shader
*		 attaches them to link the program, handles errors by slogging the error returning
*		 invalid shader index
* @param *vsPath	a character pointer containing the path to our vertex shader file
* @param *fsPath	a character pointer containing the path to our fragment shader file
*/
GLuint build_shader_program(const char *vsPath, const char *fsPath);

/**
* @brief simple compiler for our shader files, determines if vertex, fragment, or geometry
*		shader and handles appropriately on error
* @param eShaderType	the type of shader being compiled
* @param *strShaderFile	the path to the shader file being compiled
*/
GLuint create_shader(GLenum eShaderType, const char *strShaderFile);

#endif
