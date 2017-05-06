#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "entity_manager.h"
#include "camera.h"

/**
* @struct defines a point on the track that will be used to determine a racer's progress through the map
*/
typedef struct
{
	glm::vec3 position;			/**< position of this node in world space */
	int node_num;				/**< this node's number in the node list, since I only load this once we don't have to worry about the number changing */
	bool finish_line_node;		/**< flag to know if this node is the finish line */
	float rotation;				/**< rotation of the node's plane to test position */
}Node;

class Player
{
public:

	/**
	* @brief constructs the player with a given definition file for a racer and places them in the given position and rotation
	* @param racer_def_file		the filepath to the racer definition file we want this racer to use
	* @param position			the position in the world to place this player
	* @param rotation			the rotation the player will have when it spawns
	*/
	Player(std::string racer_def_file, glm::vec3 position, float rotation);

	void draw_player_hud();

	/**
	* @brief uses Camera's follow entity to posiition the camera behind the player, also updates the view matrix for the camera
	*/
	void update_player_cam();

	Camera *player_cam;					/**< the camera that follows the player and is used to draw the game */
	Entity *entity_component;			/**< the entity component of the player */

	Node check_this_node;
	int lap_number;

	Sprite *leader_1;
	Sprite *item_backdrop;

	sf::Font hud_font;
	sf::Text mph;
	sf::Text speed;

	sf::Text lap_text;
	sf::Text lap_max;
	sf::Text lap_num_text;
};

/**
* @brief the update's the player's position, rotation, speed and updates the model matrix accordingly
* @param ent	the player's entity component
*/
void update(Entity *ent);

/**
* @brief the think function for the player
* @param ent	the player's entity component
*/
void think(Entity *ent);

/**
* @brief draws the player
* @param ent	the player's entity component
*/
void draw(Entity *ent);

#endif
