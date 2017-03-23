#include <GL\glew.h>

#include <simple_logger.h>

#include "graphics.h"
#include "shader.h"

sf::Clock Graphics::game_delta_time;
sf::RenderWindow *Graphics::game_window;

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
		slog("Error: %s\n", glewGetErrorString(error));
		exit(1);
	}

	game_window->setMouseCursorGrabbed(true);
	game_window->setVerticalSyncEnabled(true);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	graphics_shader_program = glCreateProgram();

	graphics_shader_program = build_shader_program("shader/vs1.glsl", "shader/fs1.glsl");
	if (graphics_shader_program == -1)
	{
		exit(0);
	}

	slog("Using program %d\n", graphics_shader_program);

	game_delta_time.restart();
}

GLuint Graphics::get_shader_program()
{
	return graphics_shader_program;
}

sf::RenderWindow *Graphics::get_game_window()
{
	return game_window;

}

void Graphics::frame_begin()
{
	glClearColor(0.0, 0.0, 1.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Graphics::next_frame()
{
	game_window->display();
	game_delta_time.restart();
}

sf::Time Graphics::get_delta_time()
{
	return game_delta_time.getElapsedTime();
}
