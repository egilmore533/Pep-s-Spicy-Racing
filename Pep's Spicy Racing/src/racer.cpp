#include <simple_logger.h>

#include "graphics.h"
#include "json_helper.h"
#include "racer.h"

Racer::Racer(std::string racer_def_file, glm::vec3 position, float rotation)
{
	entity_component = Entity_Manager::create_entity("json/entities/racer.json", position);
	entity_component->current_rotation = rotation;
	entity_component->scale = 0.3f;
	entity_component->draw = &draw;

	entity_component->movement_velocity = glm::vec3(0.0f);
	entity_component->current_speed = 0;

	json def = load_from_def(racer_def_file);
	json racer_def = get_element_data(def, "Racer");

	if (racer_def == NULL)
	{
		slog("this isn't a proper racer definition files");
		return;
	}

	//since this json object we can count on being formated properly for an entity assume the data is there
	entity_component->acceleration_rate = 60.0f + (10.0f * (float)racer_def["acceleration_rate"]);
	entity_component->deacceleration_rate = 60.0f + (10.0f * (float)racer_def["deacceleration_rate"]);
	entity_component->top_speed = 500.0f + (50.0f * (float)racer_def["top_speed"]);
	entity_component->mass = racer_def["mass"];
	entity_component->handling = (float)racer_def["handling"] * 50.0f;
	entity_component->color_data = glm::vec4(racer_def["color"][0], racer_def["color"][1], racer_def["color"][2], racer_def["color"][3]);

	entity_component->add_rigid_body(glm::vec3(0.6f));

	lap_number = 0;
}

Racer::~Racer()
{
	Entity_Manager::delete_entity(entity_component->id);
}

void racer_base_update(Entity *ent)
{
	float delta_time = Graphics::get_delta_time().asSeconds();

	//get gas/brake/reverse
	//change velocity based on that combined with the acceleration and deacceleration rates from the entity
	//update position based on the new velocity

	ent->movement_velocity = glm::vec3((ent->current_speed) * cos(glm::radians(ent->current_rotation)), 0, -(ent->current_speed) * sin(glm::radians(ent->current_rotation))) * delta_time;

	ent->world_position = ent->world_position + ent->movement_velocity;

	ent->body.rb->getWorldTransform().setOrigin(btVector3(ent->world_position.x, ent->world_position.y, ent->world_position.z));
	
	btQuaternion q;
	q.setRotation(btVector3(0, 1, 0), glm::radians(ent->current_rotation));
	ent->body.rb->getWorldTransform().setRotation(q);

	ent->model = glm::mat4(1.0f);
	ent->model = glm::translate(ent->model, ent->world_position);
	ent->model = glm::rotate(ent->model, glm::radians(ent->current_rotation), glm::vec3(0, 1, 0));
	ent->model = glm::scale(ent->model, glm::vec3(ent->scale));
}

void draw(Entity *ent)
{
	ent->mesh->Mesh::draw(ent->shader->program);
}

void Racer::update(float delta_time)
{
	//get gas/brake/reverse
	//change velocity based on that combined with the acceleration and deacceleration rates from the entity
	//update position based on the new velocity

	entity_component->movement_velocity = glm::vec3((entity_component->current_speed) * cos(glm::radians(entity_component->current_rotation)), 0, -(entity_component->current_speed) * sin(glm::radians(entity_component->current_rotation))) * delta_time;

	entity_component->world_position = entity_component->world_position + entity_component->movement_velocity;

	entity_component->body.rb->getWorldTransform().setOrigin(btVector3(entity_component->world_position.x, entity_component->world_position.y, entity_component->world_position.z));

	btQuaternion q;
	q.setRotation(btVector3(0, 1, 0), glm::radians(entity_component->current_rotation));
	entity_component->body.rb->getWorldTransform().setRotation(q);

	entity_component->model = glm::mat4(1.0f);
	entity_component->model = glm::translate(entity_component->model, entity_component->world_position);
	entity_component->model = glm::rotate(entity_component->model, glm::radians(entity_component->current_rotation), glm::vec3(0, 1, 0));
	entity_component->model = glm::scale(entity_component->model, glm::vec3(entity_component->scale));
}