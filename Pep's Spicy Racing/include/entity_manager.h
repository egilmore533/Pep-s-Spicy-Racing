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
	* @param shader_json_filepath the filepath to the shader definition file
	* @param default_shader_program	(temporary parameter) default shader program from the graphics class, only used if no shader defined
	* @return a pointer to the next entity to be used
	*/
	Entity *create_entity(char *entity_json_filepath, glm::vec3 position, char *shader_json_filepath, GLuint temp_default_shader);

	/**
	* @brief deletes the given entity from the entity_list
	* @param entity_id the id of the entity to delete
	*/
	void delete_entity(int entity_id);

	//might not need this one
	void initialize();

	//Might not need this one
	void close();
	void clear();

	void think_all();
	void update_all();
	void draw_all();
	void intersect_all();

private:
	std::array<class Entity*, MAX_ENTITIES> entity_list;
	int num_entities;
};


#endif
