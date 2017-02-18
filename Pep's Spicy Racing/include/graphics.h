#pragma once
#ifndef _GRAPHICS_H_
#define _GRAPHICS_H_

#include <SFML\OpenGL.hpp>
#include <SFML\Graphics.hpp>

#define WINDOW_WIDTH	1024
#define WINDOW_HEIGHT	768

#define FRAME_DELAY		33

class Graphics
{
public:
	Graphics();
	GLuint get_shader_program();
	sf::RenderWindow *get_game_window();
	void graphics_frame_begin();
	void graphics_next_frame();
	sf::Time get_game_time();

private:
		GLuint graphics_shader_program;
		sf::RenderWindow *game_window;
		sf::Clock time;
		sf::Clock delta_time;
};

#endif 
