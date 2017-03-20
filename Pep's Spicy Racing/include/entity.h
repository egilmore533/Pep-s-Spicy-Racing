#ifndef __ENTITY_H__
#define __ENTITY_H__

#include "mesh.h"

class Entity
{
public:
	/**
	 * @brief allocates space for this new entity
	 */
	Entity();

	/**
	 * @brief frees the data for this entity
	 */
	~Entity();

	/**
	 * @brief the update function for entities
	 */
	void update();

	/**
	 * @brief the think function for entities, used to process information regarding the entity
	 */
	void think();

	/**
	 * @brief draws the entity to the screen using basic drawing stuff
	 */
	void draw();

	bool in_use;				/**< in use flag to determine if this entity is in use */
	Mesh *mesh;					/**< mesh that belongs to this entity */
	
	int think_rate;				/**< the rate at which the entity will think */
	float move_speed;			/**< the movement speed of the entity */
	//Soundpack *soundpack;		/**< the soundpak that belongs to this entity */
	GLuint shader_program;		/**< this entity's shader (might need to move this to be somewhere else, I really don't know what I'm getting involved in here) */
	glm::vec3 world_position;	/**< entity's position in world space */

	float temp_tracker;	
	glm::mat4 model;			/**< the model matrix of this entity, used to draw the entity in world position */
};

/**
 * @brief frees the entity from the entity list (does not deallocate the memeory in the entity_list)
 * @param entity	double pointer to the entity in the entity_list to be freed
 */
void entity_free(Entity **entity);

/**
* @brief gets a pointer to the next free entity that we can use in the entity_list, and defines an entity using the given json file
* @param *json_file definition file for this entity to be created from
* @param position  the position the entity is in the world space (model matrix)
* @param default_shader_program	(temporary parameter) default shader program from the graphics class, only used if no shader defined
* @return a pointer to the next entity to be used
*/
Entity *entity_new(char *json_file, glm::vec3 position, GLuint default_shader_program);

/**
 * @brief intializes the entity system to be used as a resource manager, sets all pointers to NULL
 * @param num_entities the number of entities the game will allocated memory for
 */
void entity_initialize_system(int num_entities);

/**
 * @brief draws every entity that is in use in the entity_list by running their draw function pointers
 */
void entitiy_draw_all();

/**
 * @brief updates every entity that is in use in the entity_list by running their update function pointers
 */
void entity_update_all();

/**
 * @brief closes the entity system by freeing all entities in the entity_list and then resetting the entity_num and entity_max
 */
void entity_close_system();


#endif
