#ifndef __ENTITY_MANAGER_H__
#define __ENTITY_MANAGER_H__

#include "entity.h"

class Entity_Manager
{
public:

	/**
	* @brief gets a pointer to the next free entity that we can use in the entity_list, and defines an entity using the given json file
	* @param *entity_json_filepath definition file for this entity to be created from
	* @param position  the position the entity is in the world space (model matrix)
	* @param *shader_json_filepath the filepath to the shader definition file
	* @param default_shader_program	(temporary parameter) default shader program from the graphics class, only used if no shader defined
	* @return a pointer to the next entity to be used
	*/
	Entity *create_entity(char *entity_json_filepath, glm::vec3 position, char *shader_json_filepath, GLuint temp_default_shader);

	/**
	* @brief deletes the given entity from the entity_list
	* @param entity_id the id of the entity to delete
	*/
	void delete_entity(int entity_id);

	/**
	* @brief initializes the entity manager by filling the entity_list with empty entities and setting the number of current entities to none
	*/
	void initialize();

	/**
	* @brief TODO	should be set up to empty the entity_list of all entities and freeing the data used by them, similar to clear, not implemented because I don't believe its needed
	*/
	void close();

	/**
	* @brief empties the entity_list useful to load a new level
	*/
	void clear();

	/**
	* @brief perform the think function of all the entities that are currently in use
	*/
	void think_all();

	/**
	* @brief perform the updates of all entities that are currently in use 
	*/
	void update_all();

	/**
	* @brief draws all entities that are in use
	*/
	void draw_all();

	/**
	* @brief check collisions for all entities, if collision perform the touch functions of each
	*/
	void intersect_all();

private:
	std::array<class Entity*, MAX_ENTITIES> entity_list;	/**< the list of all entities in the game, allocates space for the number of entities defined by the macro MAX_ENTITIES */
	int num_entities;										/**< the number of entities currently in the game */
};


#endif
