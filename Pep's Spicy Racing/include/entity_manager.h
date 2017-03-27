#ifndef __ENTITY_MANAGER_H__
#define __ENTITY_MANAGER_H__

#include "camera.h"
#include "entity.h"

class Entity_Manager
{
public:

	/**
	* @brief gets a pointer to the next free entity that we can use in the entity_list, and defines an entity using the given json file
	* @param *entity_json_filepath definition file for this entity to be created from
	* @param position  the position the entity is in the world space (model matrix)
	* @return a pointer to the next entity to be used
	*/
	static Entity *create_entity(char *entity_json_filepath, glm::vec3 position);

	/**
	* @brief deletes the given entity from the entity_list
	* @param entity_id the id of the entity to delete
	*/
	static void delete_entity(int entity_id);

	/**
	* @brief initializes the entity manager by filling the entity_list with empty entities and setting the number of current entities to none
	*/
	void initialize();

	/**
	* @brief empties the entity_list useful to load a new level, not static because this shouldn't be done in any file beside one that has access to the actual entity_manager object
	*/
	void clear();

	/**
	* @brief perform the think function of all the entities that are currently in use
	*/
	static void think_all();

	/**
	* @brief perform the updates of all entities that are currently in use 
	*/
	static void update_all();

	/**
	* @brief draws all entities that are in use from the perspective of the given camera
	* @param *camera the camera that will determine the perspective these entities will be drawn from
	* @param
	*/
	static void draw_all(Camera *camera, Entity *single_light);

	/**
	* @brief check collisions for all entities, if collision perform the touch functions of each
	*/
	static void intersect_all();

private:
	std::array<class Entity*, MAX_ENTITIES> entity_list;	/**< the list of all entities in the game, allocates space for the number of entities defined by the macro MAX_ENTITIES */
	int num_entities;										/**< the number of entities currently in the game */
};


#endif
