#ifndef __ENTITY_H__
#define __ENTITY_H__

#define MAX_ENTITIES	100

#include <array>

#include "physics.h"
#include "shader.h"
#include "texture.h"
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
	 * @brief the entity's update function, should handle all the model matrix updates as well
	 */
	void (*update)(Entity *ent);

	/**
	 * @brief the think function for entities, used to process information regarding the entity periodically
	 */
	void (*think)(Entity *ent);

	/**
	* @brief the entity's draw function
	*/
	void (*draw)(Entity *ent);

	void add_rigid_body(glm::vec3 dimensions);

	bool in_use;				/**< in use flag to determine if this entity is in use */
	int id;						/**< the id of the entity */

	//Soundpack *soundpack;		/**< the soundpak that belongs to this entity */
	Mesh *mesh;					/**< mesh that belongs to this entity */
	Texture *texture;			/**< the entity's texture */


	float current_speed;			/**< the movement speed of the entity */
	float current_rotation;		/**< the model's rotation (about the y axis, in )*/
	float scale;				/**< the scale of the entity's model */

	float think_rate;			/**< the rate at which the entity will think (in seconds) */
	float next_think;			/**< the next time this entity will perform its think function (sf::Time asSeconds)*/

	glm::vec3 world_position;	/**< entity's position in world space */
	glm::vec3 movement_velocity;	/**< the current velocity of the entity */

	float acceleration_rate;	/**< the rat at which this entity will accerlate */
	float top_speed;			/**< the fastest speed this entity can move */
	float deacceleration_rate;	/**< the rate at which this entity will deaccelerate */
	float rotation_rate;		/**< the speed at which the entity rotates */
	float mass;					/**< currently unused but will effect how long the racer is stunned when he is hit by an item, and how far he will bounce back and bounce other racers away */
	float handling;				/**< defines how well a racer turns */

	rigid_body body;

	glm::mat4 model;			/**< the model matrix of this entity, used to draw the entity in world position */
	glm::vec4 color_data;		/**< the colors this entity will be drawn in, unique to entity's now not mesh's that way I can set up a mesh resource manager */

	Shader *shader;				/**< the shader program for this entity */
	GLuint model_location;		/**< the location of the uniform model */
	GLuint color_location;		/**< location of the uniform color data */

	GLuint view_location;			/**< location of the view matrix uniform */
	GLuint projection_location;		/**< location of the projection matrix uniform */

	GLuint light_color_location;		/**< location of the light color uniform */
	GLuint light_position_location;		/**< location of the light position uniform */
	GLuint view_position_location;		/**< location of the camera position uniform */
};

/**
* @brief deadult draw for entities
* @param the entity to draw
*/
void default_draw(Entity *ent);

/**
* @brief default update for entities
*/
void default_update(Entity *ent);

#endif
