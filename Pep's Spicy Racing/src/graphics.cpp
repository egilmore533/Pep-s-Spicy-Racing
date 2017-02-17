#include <GL\glew.h>

#include "graphics.h"
#include "shader.h"


Graphics::Graphics()
{
	sf::ContextSettings settings;
	int error;

	settings.depthBits = 24;
	settings.stencilBits = 8;
	settings.antialiasingLevel = 4;
	settings.majorVersion = 3;
	settings.minorVersion = 0;

	game_window = new sf::RenderWindow(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Pep's Spicy Adventure", sf::Style::Default, settings);
	if ((error = glewInit()) != GLEW_OK)
	{
		printf("Error: %s\n", glewGetErrorString(error));
		exit(1);
	}

	game_window->setMouseCursorGrabbed(true);
	game_window->setVerticalSyncEnabled(true);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	graphics_shader_program = glCreateProgram();

	graphics_shader_program = BuildShaderProgram("shaders/vs1.glsl", "shaders/fs1.glsl");
	if (graphics_shader_program == -1)
	{
		exit(0);
	}

	printf("Using program %d", graphics_shader_program);

	time.restart();
	delta_time.restart();
}

Graphics::~Graphics()
{

}

GLuint Graphics::get_shader_program()
{
	return graphics_shader_program;
}

sf::RenderWindow* Graphics::get_game_window()
{
	return game_window;

}

void Graphics::graphics_frame_begin()
{
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Graphics::graphics_next_frame()
{
	game_window->display();
}

sf::Time Graphics::get_game_time()
{
	return time.getElapsedTime();
}
