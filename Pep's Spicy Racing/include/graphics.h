#ifndef __GRAPHICS_H__
#define __GRAPHICS_H__

#include <GL/glew.h>
#include <glm\glm.hpp>
#include <SFML\OpenGL.hpp>
#include <SFML\Graphics.hpp>

#define WINDOW_WIDTH	1024
#define WINDOW_HEIGHT	900

#define FRAME_DELAY		33

class Graphics
{
public:

	/**
	* @brief creates our Graphics context for the window, shader program, and begins our delta_time clock
	*/
	Graphics();

	/**
	* @brief gets the static pointer to our game window
	* @return sf::RenderWindow pointer to the window to be used by the game
	*/
	static sf::RenderWindow *get_game_window();

	/**
	* @brief clears the screen
	*/
	static void frame_begin();

	/**
	* @brief displays the next frame to the window, also restarts the delta time for the game
    *		needs further implementation to calculate a fps
	*/
	static void next_frame();

	/**
	* @brief gets the time that has passed since the last frame
	* @return an sf::Time that can be used to get the time that has passed since the last frame in seconds, milliseconds, or microseconds
	*/
	static sf::Time get_delta_time();

	/**
	* @brief sets the clear color for the window to be the given values
	* @param new_clear_color	the new color the window will clear to be
	*/
	static void set_clear_color(glm::vec4 new_clear_color);

	/**
	* @brief pushes opengl states, used when you want to start drawing text, should only be used once per loop or will become too slow to be used efficiently
	*/
	static void begin_draw_text();

	/**
	* @brief pops the opengl states, used once all text has been rendered
	*/
	static void end_draw_text();

	/**
	* @brief the only way i discovered to use sfml's text interface is to push and pop the glStates inbetween text draw calls. This may turn out to be slow or not.
	* @param text	an sfml text object that will be draw to the screen according to its own settings
	*/
	static void draw_text(sf::Text text);

private:
		static sf::RenderWindow game_window;	/**< our game's window */
		static sf::Clock game_delta_time;		/**< the clock that tracks time passed since last frame*/
		static glm::vec4 clear_color;			/**< the color of the blank screen */

		//testes
		static sf::Font font;
		static sf::Text text;

};

#endif 
