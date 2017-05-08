#ifndef __RACER_H__
#define __RACER_H__

#include "entity_manager.h"


enum RacerType
{
	PlayerRacer = 1,
	Bot
};

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

class Racer
{
public:

	/**
	* @brief constructs the racer with a given definition file for a racer and places them in the given position and rotation
	* @param racer_def_file		the filepath to the racer definition file we want this racer to use
	* @param position			the position in the world to place this racer
	* @param rotation			the rotation the racer will have when it spawns
	*/
	Racer(std::string racer_def_file, glm::vec3 position, float rotation);

	~Racer();

	/**
	* @brief update the racer's current state
	*/
	virtual void update();

	RacerType racer_type;
	Entity *entity_component;			/**< the entity component of the racer */

	Node check_this_node;
	int lap_number;
};

/**
* @brief	base update perfromed every frame for each racer, updating their velocity, world position, rigid body and model
* @param	*ent	the entity component to update
*/
void racer_base_update(Entity *ent);

/**
* @brief draws the racer
* @param ent	the racer's entity component
*/
void draw(Entity *ent);

#endif 
