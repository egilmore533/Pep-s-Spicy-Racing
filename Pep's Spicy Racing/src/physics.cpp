#include <simple_logger.h>
#include "physics.h"

btDynamicsWorld* Physics::world;
btCollisionDispatcher* Physics::collision_dispatcher;			/**< used to filter collsions (using Masks) and determine what algorithm is used to detect collision */
btCollisionConfiguration* Physics::collision_configuration;		/**< used to allow you to fine tune the algorithms used for the full collision detection (the more precise check) */
btBroadphaseInterface* Physics::broadphase_interface;			/**< used to determine if two objects might collide, faster than just checking every single object, basically determines if there is no chance of two objects colliding so we don't have to run expensive checks to find out */
btConstraintSolver* Physics::constraint_solver;					/**< what causes the objects to interact properly, taking into account gravity, game logic supplied forces, collisions, and hinge constraints (where the performance bottlenecks usually happen in big simulations) */

std::vector<rigid_body> Physics::rigid_body_list;
unsigned int Physics::num_rigid_bodies;

Physics::Physics()
{
	///-----initialization_start-----

	///collision configuration contains default setup for memory, collision setup. Advanced users can create their own configuration.
	collision_configuration = new btDefaultCollisionConfiguration();

	///use the default collision dispatcher. For parallel processing you can use a diffent dispatcher (see Extras/BulletMultiThreaded)
	collision_dispatcher = new btCollisionDispatcher(collision_configuration);

	///btDbvtBroadphase is a good general purpose broadphase. You can also try out btAxis3Sweep.
	broadphase_interface = new btDbvtBroadphase();

	///the default constraint solver. For parallel processing you can use a different solver (see Extras/BulletMultiThreaded)
	constraint_solver = new btSequentialImpulseConstraintSolver();

	///now we can finally set up the world
	world = new btDiscreteDynamicsWorld(collision_dispatcher, broadphase_interface, constraint_solver, collision_configuration);
	world->setGravity(btVector3(0, 0, 0));

	rigid_body_list.resize(MAX_RIGID_BODIES);

	for (int i = 0; i < MAX_RIGID_BODIES; i++)
	{
		btTransform t;
		t.setIdentity();
		t.setOrigin(btVector3(0, -8, 0));
		btStaticPlaneShape* plane = new btStaticPlaneShape(btVector3(0, 1, 0), 0);
		btMotionState* motion = new btDefaultMotionState(t);
		btRigidBody::btRigidBodyConstructionInfo info(0.0, motion, plane);
		btRigidBody* body = new btRigidBody(info);

		rigid_body temp_rigid_body;
		temp_rigid_body.rb = body;
		temp_rigid_body.in_use = false;
		temp_rigid_body.id_num = i;

		rigid_body_list.push_back(temp_rigid_body);
	}
}

Physics::~Physics()
{
	delete_all_rigid_body();

	delete world;
	delete constraint_solver;
	delete broadphase_interface;
	delete collision_dispatcher;
	delete collision_configuration;
}

rigid_body Physics::create_cube_body_trigger(glm::vec3 position, glm::vec3 dimensions, float rotation, float mass)
{
	if (num_rigid_bodies + 1 >= MAX_RIGID_BODIES)
	{
		slog("too many rigid bodies");
		exit(0);
	}
	for (int i = 0; i < MAX_RIGID_BODIES; i++)
	{
		if (!rigid_body_list[i].in_use)
		{
			rigid_body new_rigid_body = rigid_body_list[i];

			btTransform t;
			t.setIdentity();

			t.setOrigin(btVector3(position.x, position.y, position.z));

			btQuaternion q;
			q.setEuler(glm::radians(rotation), 0, 0);

			t.setRotation(q);

			btBoxShape* cube = new btBoxShape(btVector3(dimensions.x / 2.0, dimensions.y / 2.0, dimensions.z / 2.0));
			btVector3 inertia(0, 0, 0);

			if (mass != 0.0)
			{
				cube->calculateLocalInertia(mass, inertia);
			}

			btMotionState* motion = new btDefaultMotionState(t);
			btRigidBody::btRigidBodyConstructionInfo info(mass, motion, cube, inertia);
			btRigidBody* body = new btRigidBody(info);

			body->setCollisionFlags(body->getCollisionFlags() | btCollisionObject::CF_NO_CONTACT_RESPONSE);

			new_rigid_body.rb = body;
			new_rigid_body.id_num = i;
			new_rigid_body.in_use = true;

			world->addRigidBody(body);
			num_rigid_bodies++;

			return new_rigid_body;
		}
	}
}

rigid_body Physics::create_plane_body_trigger(glm::vec3 position, glm::vec3 dimensions, float rotation)
{
	if (num_rigid_bodies + 1 >= MAX_RIGID_BODIES)
	{
		slog("too many rigid bodies");
		exit(0);
	}
	for (int i = 0; i < MAX_RIGID_BODIES; i++)
	{
		if (!rigid_body_list[i].in_use)
		{
			rigid_body new_rigid_body = rigid_body_list[i];

			btTransform t;
			t.setIdentity();
			
			t.setOrigin(btVector3(position.x, position.y, position.z));
			
			btQuaternion q;
			q.setEuler(glm::radians(270.0f + rotation), glm::radians(90.0f), 0);

			t.setRotation(q);

			btStaticPlaneShape* plane = new btStaticPlaneShape(btVector3(0, 1, 0), 0);
			btMotionState* motion = new btDefaultMotionState(t);
			btRigidBody::btRigidBodyConstructionInfo info(0.0, motion, plane);
			btRigidBody* body = new btRigidBody(info);
			body->setCollisionFlags(body->getCollisionFlags() | btCollisionObject::CF_NO_CONTACT_RESPONSE);

			new_rigid_body.rb = body;
			new_rigid_body.id_num = i;
			new_rigid_body.in_use = true;

			world->addRigidBody(body);
			num_rigid_bodies++;

			return new_rigid_body;
		}
	}
}

rigid_body Physics::create_cube_body(glm::vec3 dimensions, glm::vec3 position, float mass)
{
	if (num_rigid_bodies + 1 >= MAX_RIGID_BODIES)
	{
		slog("too many rigid bodies");
		exit(0);
	}
	for (int i = 0; i < MAX_RIGID_BODIES; i++)
	{
		if (!rigid_body_list[i].in_use)
		{
			rigid_body new_rigid_body = rigid_body_list[i];
			
			btTransform t;
			t.setIdentity();
			t.setOrigin(btVector3(position.x, position.y, position.z));
			btBoxShape* cube = new btBoxShape(btVector3(dimensions.x / 2.0, dimensions.y / 2.0, dimensions.z / 2.0));
			btVector3 inertia(0, 0, 0);

			if (mass != 0.0)
			{
				cube->calculateLocalInertia(mass, inertia);
			}

			btMotionState* motion = new btDefaultMotionState(t);
			btRigidBody::btRigidBodyConstructionInfo info(mass, motion, cube, inertia);
			btRigidBody* body = new btRigidBody(info);

			new_rigid_body.rb = body;
			new_rigid_body.id_num = i;
			new_rigid_body.in_use = true;

			world->addRigidBody(body);
			num_rigid_bodies++;

			return new_rigid_body;
		}
	}
}

void Physics::delete_all_rigid_body()
{
	for (int i = 0; i < MAX_RIGID_BODIES; i++)
	{
		if (!rigid_body_list[i].rb)
		{
			continue;
		}
		btMotionState* motionState = rigid_body_list[i].rb->getMotionState();
		btCollisionShape* shape = rigid_body_list[i].rb->getCollisionShape();
		world->removeCollisionObject(rigid_body_list[i].rb);
		rigid_body_list[i].in_use = false;

		delete rigid_body_list[i].rb;
		delete shape;
		delete motionState;

		num_rigid_bodies--;
	}
}

void Physics::delete_rigid_body(unsigned int id)
{
	for (int i = 0; i < MAX_RIGID_BODIES; i++)
	{
		if (rigid_body_list[i].in_use && rigid_body_list[i].id_num == id)
		{
			btMotionState* motionState = rigid_body_list[i].rb->getMotionState();
			btCollisionShape* shape = rigid_body_list[i].rb->getCollisionShape();
			world->removeCollisionObject(rigid_body_list[i].rb);

			delete rigid_body_list[i].rb;
			delete shape;
			delete motionState;

			btTransform t;
			t.setIdentity();
			t.setOrigin(btVector3(0, -8, 0));
			btStaticPlaneShape* plane = new btStaticPlaneShape(btVector3(0, 1, 0), 0);
			btMotionState* motion = new btDefaultMotionState(t);
			btRigidBody::btRigidBodyConstructionInfo info(0.0, motion, plane);
			btRigidBody* body = new btRigidBody(info);
			
			rigid_body_list[i].rb = body;
			rigid_body_list[i].in_use = false;
			rigid_body_list[i].id_num = i;

			num_rigid_bodies--;
		}
	}
}

void Physics::physics_step(float time)
{
	world->stepSimulation(time);
}
