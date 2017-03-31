#include <GL\glew.h>

#include <simple_logger.h>

#include "graphics.h"
#include "shader.h"

sf::Clock Graphics::game_delta_time;
sf::RenderWindow *Graphics::game_window;
glm::vec4 Graphics::clear_color;

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
	clear_color = glm::vec4(0.0f, 0.0f, 0.6f, 0.0f);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	game_delta_time.restart();
}

sf::RenderWindow *Graphics::get_game_window()
{
	return game_window;

}

void Graphics::frame_begin()
{
	glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Graphics::next_frame()
{
	game_window->display();
	game_delta_time.restart();
}

void Graphics::set_clear_color(glm::vec4 new_clear_color)
{
	clear_color = new_clear_color;
}

sf::Time Graphics::get_delta_time()
{
	return game_delta_time.getElapsedTime();
}
