#pragma once
#ifndef _GRAPHICS_H_
#define _GRAPHICS_H_

#include <SFML\OpenGL.hpp>
#include <SFML\Graphics.hpp>

#define WINDOW_WIDTH	1024
#define WINDOW_HEIGHT	768

class Graphics
{
public:
	Graphics();
	~Graphics();
	GLuint get_shader_program();
	sf::RenderWindow *get_game_window();
	void graphic_loop();

private:
		GLuint graphics_shader_program;
		sf::RenderWindow *game_window;

};

#endif 
