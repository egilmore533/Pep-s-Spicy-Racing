#ifndef __BUTTON_H__
#define __BUTTON_H__

#include "sprite_manager.h"
#include "graphics.h"
#include "camera.h"

class Button {
public:

	/**
	* @brief constructs a button from the given defintion file, which dictates what sprite the button uses and what if any text it has as well as positioning and rotation
	* @param button_def_file		the filepath to a defintion file for a button
	*/
	Button(std::string button_def_file);

	/**
	* @brief updates the button's state and runs the callback on click
	*/
	void update();

	/**
	* @brief draws the button's background sprite
	* @param *camera		the camera used to draw the button
	*/
	void draw_background(Camera *camera);

	/**
	* @brief draws the button's text to the screen, needs to be called after Graphics::begin_draw_text() and needs to be followed by Graphics::end_draw_text();
	*/
	void draw_text();

	Sprite *background;		/**< the background sprite for the button */
	sf::Text text;			/**< the text that will be displayed on top of the button */
	//int state;			/**< the state the button is in */
	bool clicked;			/**< has this button been clicked */

	/**
	* @brief the function that runs when this button is pressed, not actually a callback
	*/
	void (*callback)();
};



#endif
