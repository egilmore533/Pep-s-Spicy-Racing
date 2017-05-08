#include <string>
#include <glm/gtc/matrix_transform.hpp>
#include <simple_logger.h>

#include "json_helper.h"
#include "entity.h"
#include "entity_manager.h"
#include "graphics.h"

Entity::Entity()
{
	in_use = false;
	id = -1;

	mesh = NULL;
	texture = NULL;

	update = NULL;
	draw = NULL;
	think = NULL;

	current_speed = 0.0f;
	rotation_rate = 0.0f;

	think_rate = 0.0f;
	next_think = 0.0f;

	shader = NULL;

	world_position = glm::vec3(0.0f);
	current_rotation = 0.0f;
	scale = 1.0f;

	model = glm::mat4(1.0f);

	model_location = 0;
	color_location = 0;
}

Entity::~Entity()
{
	//decrement the mesh/model, shader, etc. here
	slog("delete");
}

void Entity::add_rigid_body(glm::vec3 dimensions)
{
	body = Physics::create_cube_body(dimensions, world_position, mass);
}

/*
void update()
{
	model = glm::mat4(1.0f);
	model = glm::translate(model, world_position);
	model = glm::rotate(model, glm::radians(current_rotation), glm::vec3(0,1,0));
	model = glm::scale(model, glm::vec3(scale));
}
*/

void default_draw(Entity *ent)
{
	ent->mesh->draw(ent->shader->program);
}

void default_update(Entity *ent)
{
	ent->model = glm::mat4(1.0f);
	ent->model = glm::translate(ent->model, ent->world_position);
	ent->model = glm::rotate(ent->model, glm::radians(ent->current_rotation), glm::vec3(0, 1, 0));
	ent->model = glm::scale(ent->model, glm::vec3(ent->scale));
}

