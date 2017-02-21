#ifndef __SHADER_H__
#define __SHADER_H__

#include <GL/glew.h>

GLuint BuildShaderProgram(const char *vsPath, const char *fsPath);
GLuint CreateShader(GLenum eShaderType, const char *strShaderFile);

#endif
