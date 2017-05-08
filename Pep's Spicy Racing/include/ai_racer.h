#ifndef __AI_RACER_H__
#define __AI_RACER_H__

#include "racer.h"

enum DrivingState
{
	Gas = 1,
	Brakes,
	Stop,
	Reverse
};

class AI_Racer: public Racer
{
public:

	/**
	* @brief constructs the ai racer with a given definition file for a racer and places them in the given position and rotation
	* @param racer_def_file		the filepath to the racer definition file we want this racer to use
	* @param position			the position in the world to place this ai racer
	* @param rotation			the rotation the ai racer will have when it spawns
	*/
	AI_Racer(std::string racer_def_file, glm::vec3 position, float rotation);

	~AI_Racer();

	/**
	* @brief update the ai racer's current rotation and movement to be twoards the node
	*/
	virtual void update();

	void update_driving_state();

	DrivingState state;
	float target_rotation;
};

/**
* @brief the think function for the player
* @param ent	the player's entity component
*/
void think(Entity *ent);

#endif
