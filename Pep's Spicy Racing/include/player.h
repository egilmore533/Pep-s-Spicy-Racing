#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "sprite_manager.h"
#include "camera.h"
#include "racer.h"

class Player: public Racer
{
public:

	/**
	* @brief constructs the player with a given definition file for a racer and places them in the given position and rotation
	* @param racer_def_file		the filepath to the racer definition file we want this racer to use
	* @param position			the position in the world to place this player
	* @param rotation			the rotation the player will have when it spawns
	*/
	Player(std::string racer_def_file, glm::vec3 position, float rotation);

	/**
	* @brief deletes the player's entity and HUD sprites
	*/
	~Player();

	/**
	* @brief draws the player's HUD sprites
	*/
	void draw_player_hud_sprites();

	/**
	* @brief draws the player's HUD text
	*/
	void draw_player_hud_text();

	/**
	* @brief the update's the player's position, rotation, speed and updates the model matrix accordingly
	* @param delta_time		the time since the last frame
	*/
	virtual void update(float delta_time);

	/**
	* @brief uses Camera's follow entity to posiition the camera behind the player, also updates the view matrix for the camera
	*/
	void update_player_cam();

	Camera *player_cam;					/**< the camera that follows the player and is used to draw the game */

	bool w, a, s, d;		/**< flags to determine the player's input */

	/////HUD Sprites
	Sprite *leader_1;			/**< image for leader 1 */
	Sprite *leader_2;			/**< image for leader 2 */
	Sprite *item_backdrop;		/**< image for the backdrop of items and racers */

	sf::Font hud_font;		/**< font used for HUD text */
	sf::Text mph;			/**< text that says mph */
	sf::Text speed;			/**< text that says current speeds */

	sf::Text lap_text;		/**< text that displays lap */
	sf::Text lap_num_text;	/**< */
};

/**
* @brief the think function for the player
* @param ent	the player's entity component
*/
void think(Entity *ent);

#endif
