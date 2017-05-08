#include <GL\glew.h>

#include <simple_logger.h>

#include "graphics.h"
#include "shader.h"

sf::Clock Graphics::game_delta_time;
sf::Clock Graphics::total_game_time;
sf::RenderWindow Graphics::game_window;
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

	game_window.create(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Pep's Spicy Adventure", sf::Style::Default, settings);
	if ((error = glewInit()) != GLEW_OK)
	{
		slog("Error: %s\n", glewGetErrorString(error));
		exit(1);
	}

	//game_window.setFramerateLimit(30);
	//game_window.setMouseCursorGrabbed(true);
	game_window.setVerticalSyncEnabled(true);
	game_window.setKeyRepeatEnabled(false);
	clear_color = glm::vec4(0.0f, 0.0f, 0.6f, 0.0f);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);


	game_delta_time.restart();
	total_game_time.restart();
}

sf::RenderWindow *Graphics::get_game_window()
{
	return &game_window;

}

void Graphics::frame_begin()
{
	glClearColor(clear_color.r, clear_color.g, clear_color.b, clear_color.a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

/**
* @brief pushes opengl states, used when you want to start drawing text, should only be used once per loop or will become too slow to be used efficiently
*/
void Graphics::begin_draw_text()
{
	game_window.pushGLStates();
}

/**
* @brief pops the opengl states, used once all text has been rendered
*/
void Graphics::end_draw_text()
{
	game_window.popGLStates();
}

void Graphics::draw_text(sf::Text text)
{
	
	game_window.draw(text);
	
}

void Graphics::next_frame()
{
	game_window.display();
	if (game_delta_time.getElapsedTime().asMilliseconds() < 16.0f)
	{
		sf::Time time = sf::milliseconds(game_delta_time.getElapsedTime().asMilliseconds() - 16.0f);
		sf::sleep(time - game_delta_time.getElapsedTime());
	}
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

sf::Time Graphics::get_time()
{
	return total_game_time.getElapsedTime();
}





