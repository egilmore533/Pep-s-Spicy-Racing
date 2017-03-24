#include <stdio.h>

#include <simple_logger.h>

#include "json_helper.h"
#include "shader.h"


Shader::Shader()
{
	program = 0;
	shader_def_file = "";
	reference_count = 0;
}

Shader::~Shader()
{
	program = 0;
	shader_def_file = "";
	reference_count = 0;
}

void Shader::Use()
{
	glUseProgram(program);
}

void Shader::build_shader(const char *def_file)
{
	json def = load_from_def((char *)def_file);
	json shader_def = get_element_data(def, "Shader");

	//we cannot count on this def file to contain the proper data
	if (shader_def == NULL)
	{
		slog("%s: is not a Shader Definition JSON file");
		return;
	}

	//get the vertex and fragment filepaths and build the shader program using them
	std::string vsPath = shader_def["vertex-shader-filepath"];
	std::string fsPath = shader_def["fragment-shader-filepath"];
	program = build_shader_program(vsPath.c_str(), fsPath.c_str());

	//get the name and store it to ID the shader
	std::string name = shader_def["name"];
	shader_def_file = def_file;
}

GLuint build_shader_program(const char *vsPath, const char *fsPath)
{
	GLint infoLogLength;
	GLuint vertexShader;
	GLuint fragmentShader;
	GLuint tempProgram;
	GLint status;
	GLchar strInfoLog[4096];

	vertexShader = create_shader(GL_VERTEX_SHADER, vsPath);
	fragmentShader = create_shader(GL_FRAGMENT_SHADER, fsPath);

	tempProgram = glCreateProgram();

	glAttachShader(tempProgram, vertexShader);
	glAttachShader(tempProgram, fragmentShader);

	glLinkProgram(tempProgram);

	glGetProgramiv(tempProgram, GL_LINK_STATUS, &status);
	if (status == GL_FALSE)
	{
		glGetProgramiv(tempProgram, GL_INFO_LOG_LENGTH, &infoLogLength);

		glGetProgramInfoLog(tempProgram, infoLogLength, NULL, strInfoLog);

		slog("Shader linker failure: %s\n", strInfoLog);
		return 0;
	}

	glDetachShader(tempProgram, vertexShader);
	glDetachShader(tempProgram, fragmentShader);

	return tempProgram;
}

GLuint create_shader(GLenum eShaderType, const char *strShaderFile)
{
	char shaderSource[4096];
	char inChar;
	FILE *shaderFile;
	int i = 0;
	GLuint shader;
	GLint status;
	GLint infoLogLength;
	GLchar strInfoLog[4096];
	char strShaderType[16];
	const char *ss;

	shaderFile = fopen(strShaderFile, "r");
	if (!shaderFile)
	{
		slog("failed to open shader file: %s\n", strShaderFile);
		return 0;
	}
	while (fscanf(shaderFile, "%c", &inChar) > 0)
	{
		shaderSource[i++] = inChar; //loading the file's chars into array
	}
	shaderSource[i - 1] = '\0';
	fclose(shaderFile);

	shader = glCreateShader(eShaderType);
	ss = shaderSource;
	glShaderSource(shader, 1, &ss, NULL);

	glCompileShader(shader);

	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
	if (status == GL_FALSE)
	{
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLength);

		glGetShaderInfoLog(shader, infoLogLength, NULL, strInfoLog);

		switch (eShaderType)
		{
		case GL_VERTEX_SHADER:
			sprintf(strShaderType, "vertex");
			break;
		case GL_GEOMETRY_SHADER:
			sprintf(strShaderType, "geometry");
			break;
		case GL_FRAGMENT_SHADER:
			sprintf(strShaderType, "fragment");
			break;
		}

		slog("Compile failure in %s shader:\n%s\n", strShaderType, strInfoLog);
		return 0;
	}

	return shader;
}
