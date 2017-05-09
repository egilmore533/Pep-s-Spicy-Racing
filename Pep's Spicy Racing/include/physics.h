#ifndef __PHYSICS_H__
#define __PHYSICS_H__

#include <vector>
#include <glm\glm.hpp>
#include <btBulletDynamicsCommon.h>

#define MAX_RIGID_BODIES		500

/**
* @struct quick rigid body struct to track the bullet data type along with an id and an in use flag
*/
typedef struct
{
	btRigidBody *rb;		/**< bullet rigid body object that is used to  */
	bool in_use;			/**< in use flag to determine if this rigid body struct is being used in the list */
	unsigned int id_num;	/**< unique id for the rigid body for deletion */
}rigid_body;


class Physics {

public:

	/**
	* @brief initializes the physics world and configures the world, solver, dispatcher and other elemements required to run bullet collision detection
	*/
	Physics();

	/**
	* @brief deletes each piece in the physics class in the reverse order they were created, including the rigid bodies
	*/
	~Physics();

	/**
	* @brief create a cube rigid body that is a trigger with the given position, dimension, rotation, and mass and returns it (used for the nodes in stage)
	* @param position	the position in the world the rigid body will be placed in the world
	* @param dimensions	the dimensions of the rigid body cube
	* @param rotation	the rotation of the rigid body in the world
	* @param mass		the mass of the rigid body
	* @return the rigid body
	*/
	static rigid_body Physics::create_cube_body_trigger(glm::vec3 position, glm::vec3 dimensions, float rotation, float mass);

	/**
	* @brief creates a rigid body plane that is a trigger (didn't work quickly so switched to cubes for the node)
	* @param position	the position in the world the rigid body will be placed in the world
	* @param dimensions	the dimensions of the rigid body cube
	* @param rotation	the rotation of the rigid body in the world
	* @param mass		the mass of the rigid body
	* @return the rigid body created and kept in the rigid_body list
	*/
	static rigid_body create_plane_body_trigger(glm::vec3 position, glm::vec3 dimensions, float rotation);

	/**
	* @brief create a cube rigid body with the given position, dimension, rotation, and mass and returns it 
	* @param position	the position in the world the rigid body will be placed in the world
	* @param dimensions	the dimensions of the rigid body cube
	* @param rotation	the rotation of the rigid body in the world
	* @param mass		the mass of the rigid body
	* @return the rigid body
	*/
	static rigid_body create_cube_body(glm::vec3 dimensions, glm::vec3 position, float mass);

	/**
	* @brief deletes all rigid bodies in the rigid body list
	*/
	void delete_all_rigid_body();

	/**
	* @brief deletes the given rigid body from the rigid body list
	* @param id		the id associated with the rigid body that needs to be deleted
	*/
	static void delete_rigid_body(unsigned int id);

	/**
	* @brief steps through time, needed to run collisions
	*/
	static void physics_step(float time);

	static btDynamicsWorld* world;								/**< the physics world where all rigid bodies must be in to be used */
	static btCollisionDispatcher* collision_dispatcher;			/**< used to filter collsions (using Masks) and determine what algorithm is used to detect collision */
	static btCollisionConfiguration* collision_configuration;		/**< used to allow you to fine tune the algorithms used for the full collision detection (the more precise check) */
	static btBroadphaseInterface* broadphase_interface;			/**< used to determine if two objects might collide, faster than just checking every single object, basically determines if there is no chance of two objects colliding so we don't have to run expensive checks to find out */
	static btConstraintSolver* constraint_solver;					/**< what causes the objects to interact properly, taking into account gravity, game logic supplied forces, collisions, and hinge constraints (where the performance bottlenecks usually happen in big simulations) */

	static std::vector<rigid_body> rigid_body_list;		/**< list of rigid bodies */
	static unsigned int num_rigid_bodies;				/**< numebr of rigid bodies in the list*/
};

#endif
