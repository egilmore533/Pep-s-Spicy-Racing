#include <simple_logger.h>

#include "graphics.h"
#include "json_helper.h"
#include "player.h"

Player::Player(std::string racer_def_file, glm::vec3 position, float rotation)
{
	glm::vec3 cameraPosition = glm::vec3(position.x, position.y, position.z - 1.0f);
	player_cam = new Camera(glm::vec2(WINDOW_WIDTH, WINDOW_HEIGHT), cameraPosition);
	entity_component = Entity_Manager::create_entity("json/entities/racer.json", position);
	entity_component->current_rotation = rotation;
	entity_component->scale = 0.5f;
	entity_component->update = &update;
	entity_component->draw = &draw;
	entity_component->think = &think;

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
	entity_component->acceleration_rate = 60.0f + (10.0f * racer_def["acceleration_rate"]);
	entity_component->deacceleration_rate = 60.0f + (10.0f * racer_def["deacceleration_rate"]);
	entity_component->top_speed = 500.0f + (50.0f * racer_def["top_speed"]);
	entity_component->mass = racer_def["mass"];
	entity_component->handling = racer_def["handling"] * 50.0f;
}

void Player::update_player_cam()
{
	player_cam->follow_entity(entity_component);
	player_cam->view_matrix_look_at_target();
}

void update(Entity *ent)
{
	int forward;
	forward = 0;
	float delta_time = Graphics::get_delta_time().asSeconds();
	bool turn_slow = false;
	float idle_slow = 3.0f;

	//uniform removal of some speed every frame if player is holding forward or backward this shouldn't matter
	if(ent->current_speed > 0)
		ent->current_speed = glm::max(ent->current_speed - ent->deacceleration_rate / idle_slow, 0.0f);
	else if (ent->current_speed < 0)
		ent->current_speed = glm::min(ent->current_speed + ent->acceleration_rate / idle_slow, 0.0f);

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
	{
		forward = 1;
		ent->current_speed += ent->acceleration_rate;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
	{
		forward = -1;
		ent->current_speed -= ent->deacceleration_rate;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
	{
		ent->current_rotation -= ent->handling * 20 * delta_time;
		turn_slow = true;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
	{
		ent->current_rotation += ent->handling * 20 * delta_time;
		turn_slow = true;
	}
	if (turn_slow)
	{
		if (ent->current_speed > ent->top_speed - (ent->top_speed * 1.0f / (1.0f + (ent->handling / 50.0f))))
		{
			ent->current_speed = glm::max(ent->top_speed - (ent->top_speed * 1.0f / (1.0f + (ent->handling / 50.0f))), ent->current_speed - ent->deacceleration_rate);
		}
		if (ent->current_speed < -ent->top_speed * 0.5f -(ent->top_speed * 0.5f * 1.0f / (1.0f + (ent->handling / 50.0f))))
		{
			ent->current_speed = glm::min(-ent->top_speed * 0.5f - (ent->top_speed * 0.5f * 1.0f / (1.0f + (ent->handling / 50.0f))), ent->current_speed + ent->deacceleration_rate);
		}
	}
	else if (!turn_slow)
	{
		if (ent->current_speed > ent->top_speed)
		{
			ent->current_speed = glm::max(ent->top_speed, (ent->current_speed + 50.0f) * delta_time);
		}
		if (ent->current_speed < -ent->top_speed * 0.5f)
		{
			ent->current_speed = glm::min(-ent->top_speed * 0.5f, (ent->current_speed - 50.0f) * delta_time);
		}
	}

	//get gas/brake/reverse
	//change velocity based on that combined with the acceleration and deacceleration rates from the entity
	//update position based on the new velocity

	ent->movement_velocity = glm::vec3((ent->current_speed) * cos(glm::radians(ent->current_rotation)), 0, -(ent->current_speed) * sin(glm::radians(ent->current_rotation))) * delta_time;

	ent->world_position = ent->world_position + ent->movement_velocity;

	ent->model = glm::mat4(1.0f);
	ent->model = glm::translate(ent->model, ent->world_position);
	ent->model = glm::rotate(ent->model, glm::radians(ent->current_rotation), glm::vec3(0, 1, 0));
	ent->model = glm::scale(ent->model, glm::vec3(ent->scale));
}

void think(Entity *ent)
{

}

void draw(Entity *ent)
{
	ent->mesh->Mesh::draw(ent->shader->program);
}
