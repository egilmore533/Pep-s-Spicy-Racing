#ifndef __GRAPHICS_H__
#define __GRAPHICS_H__

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
	static sf::RenderWindow *get_game_window();
	void graphics_frame_begin();
	void graphics_next_frame();
	static sf::Time get_delta_time();

private:
		GLuint graphics_shader_program;
		static sf::RenderWindow *game_window;
		static sf::Clock game_delta_time;
};

#endif 
