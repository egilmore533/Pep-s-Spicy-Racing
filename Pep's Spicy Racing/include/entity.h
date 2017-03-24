#ifndef __ENTITY_H__
#define __ENTITY_H__

#define MAX_ENTITIES	100

#include <array>

#include "shader.h"
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
	int id;						/**< the id of the entity */

	//Soundpack *soundpack;		/**< the soundpak that belongs to this entity */
	Mesh *mesh;					/**< mesh that belongs to this entity */

	float move_speed;			/**< the movement speed of the entity */
	float rotation_speed;		/**< the speed at which the entity rotates */

	float think_rate;			/**< the rate at which the entity will think (in seconds) */
	float next_think;			/**< the next time this entity will perform its think function (sf::Time asSeconds)*/

	glm::vec3 world_position;	/**< entity's position in world space */
	float rotation_angle;		/**< the model's rotation (about the y axis, in )*/
	float scale;				/**< the scale of the entity's model */

	glm::mat4 model;			/**< the model matrix of this entity, used to draw the entity in world position */
	glm::vec4 color_data;		/**< the colors this entity will be drawn in, unique to entity's now not mesh's that way I can set up a mesh resource manager */

	Shader *shader;			/**< the shader program for this entity */
	GLuint model_location;		/**< the location of the uniform model */
	GLuint color_location;		/**< location of the uniform color data */

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
