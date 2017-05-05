#ifndef __PHYSICS_H__
#define __PHYSICS_H__

#include <vector>
#include <glm\glm.hpp>
#include <btBulletDynamicsCommon.h>
//#include <btBulletCollisionCommon.h>

#define MAX_RIGID_BODIES		500

typedef struct
{
	btRigidBody *rb;
	bool in_use;
	unsigned int id_num;
}rigid_body;


class Physics {

public:
	Physics();
	~Physics();

	static rigid_body create_plane_body_trigger(glm::vec3 position, glm::vec3 dimensions, float rotation);

	static rigid_body create_cube_body(glm::vec3 dimensions, glm::vec3 position, float mass);

	void delete_all_rigid_body();

	static void delete_rigid_body(unsigned int id);

	static void physics_step(float time);

	static btDynamicsWorld* world;
	static btCollisionDispatcher* collision_dispatcher;			/**< used to filter collsions (using Masks) and determine what algorithm is used to detect collision */
	static btCollisionConfiguration* collision_configuration;		/**< used to allow you to fine tune the algorithms used for the full collision detection (the more precise check) */
	static btBroadphaseInterface* broadphase_interface;			/**< used to determine if two objects might collide, faster than just checking every single object, basically determines if there is no chance of two objects colliding so we don't have to run expensive checks to find out */
	static btConstraintSolver* constraint_solver;					/**< what causes the objects to interact properly, taking into account gravity, game logic supplied forces, collisions, and hinge constraints (where the performance bottlenecks usually happen in big simulations) */

	static std::vector<rigid_body> rigid_body_list;		/**< list of rigid bodies */
	static unsigned int num_rigid_bodies;
};

#endif
