#ifndef __SPRITE_MANAGER_H__
#define __SPRITE_MANAGER_H__

#include "camera.h"
#include "sprite.h"

#define MAX_SPRITES		250

class Sprite_Manager {
public:

	/**
	* @brief gets a pointer to the next free sprite that we can use in the sprite_list, and defines an sprite using the given json file
	* @param *sprite_json_filepath definition file for this entity to be created from
	* @return a pointer to the next entity to be used
	*/
	static Sprite *create_sprite(std::string sprite_json_filepath);

	/**
	* @brief deletes the given sprite from the sprite_list
	* @param sprite_id the id of the sprite to delete
	*/
	static void delete_sprite(int sprite_id);

	/**
	* @brief initializes the sprite manager by filling the sprite_list with empty entities and setting the number of current sprites to 0
	*		also intializes rendering data
	*/
	void initialize();

	/**
	* @brief performs the rendering of the given sprite
	* @param *cam	the camera used to determine the orthographic projection for the sprite shader
	* @param the id of the sprite to draw
	*/
	static void draw(Camera *cam, int sprite_id);

private:
	std::array<Sprite *, MAX_SPRITES> sprite_list;		/**< the list of all sprites */
	int num_sprites;									/**< the number of sprites currently in the list */

	//rendering data
	GLuint quad_vao;									/**< vertex array used to draw the sprites */
	GLuint vbo;											/**< vertex buffer object used for the vao */
};

#endif
