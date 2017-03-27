#ifndef __GRAPHICS_H__
#define __GRAPHICS_H__

#include <GL/glew.h>

#include <SFML\OpenGL.hpp>
#include <SFML\Graphics.hpp>

#define WINDOW_WIDTH	1024
#define WINDOW_HEIGHT	768

#define FRAME_DELAY		33

class Graphics
{
public:
	/**
	* @brief creates our Graphics context for the window, shader program, and begins our delta_time clock
	*/
	Graphics();

	/**
	* @brief gets the id for our simple shader program that
	*/
	GLuint get_shader_program();

	/**
	* @brief gets the static pointer to our game window
	* @return sf::RenderWindow pointer to the window to be used by the game
	*/
	static sf::RenderWindow *get_game_window();

	/**
	* @brief clears the screen
	*/
	void frame_begin();

	/**
	* @brief displays the next frame to the window, also restarts the delta time for the game
    *		needs further implementation to calculate a fps
	*/
	void next_frame();

	/**
	* @brief gets the time that has passed since the last frame
	* @return an sf::Time that can be used to get the time that has passed since the last frame in seconds, milliseconds, or microseconds
	*/
	static sf::Time get_delta_time();

private:
		GLuint graphics_shader_program;			/**< the index of our simple shader program */
		static sf::RenderWindow *game_window;	/**< a static pointer to our game's window */
		static sf::Clock game_delta_time;		/**< the clokc that tracks time passed since last frame*/
};

#endif 
