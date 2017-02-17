#include <GL\glew.h>

#include "graphics.h"


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
