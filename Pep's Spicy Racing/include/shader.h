#pragma once
#ifndef _SHADER_H_
#define _SHADER_H_

#include <GL/glew.h>

GLuint BuildShaderProgram(const char *vsPath, const char *fsPath);
GLuint CreateShader(GLenum eShaderType, const char *strShaderFile);

#endif
