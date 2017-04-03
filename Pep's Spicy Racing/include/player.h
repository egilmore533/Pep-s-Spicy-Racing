#include "entity_manager.h"
#include "camera.h"

class Player
{
public:

	Player(glm::vec3 position);

	void update_player_cam();

	Camera *player_cam;
	Entity *entity_component;
};

/**
* @brief the update function for the player
*/
void update(Entity *ent);

/**
* @brief the think function for the player
*/
void think(Entity *ent);

/**
* @brief draws the player
*/
void draw(Entity *ent);
