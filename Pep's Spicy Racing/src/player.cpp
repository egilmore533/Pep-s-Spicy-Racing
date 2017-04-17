#include <simple_logger.h>

#include "graphics.h"
#include "json_helper.h"
#include "player.h"

Player::Player(std::string racer_def_file, glm::vec3 position)
{
	glm::vec3 cameraPosition = glm::vec3(position.x, position.y, position.z - 1.0f);
	player_cam = new Camera(glm::vec2(WINDOW_WIDTH, WINDOW_HEIGHT), cameraPosition);
	entity_component = Entity_Manager::create_entity("json/entities/racer.json", position);
	entity_component->scale = 0.5f;
	entity_component->update = &update;
	entity_component->draw = &draw;
	entity_component->think = &think;

	entity_component->movement_velocity = glm::vec3(0.0f);

	json def = load_from_def(racer_def_file);
	json racer_def = get_element_data(def, "Racer");

	if (racer_def == NULL)
	{
		slog("this isn't a proper racer definition files");
		return;
	}

	//since this json object we can count on being formated properly for an entity assume the data is there
	entity_component->acceleration_rate = racer_def["acceleration_rate"];
	entity_component->deacceleration_rate = racer_def["deacceleration_rate"];
	entity_component->top_speed = racer_def["top_speed"];
}

void Player::update_player_cam()
{
	player_cam->follow_entity(entity_component);
	player_cam->view_matrix_look_at_target();
}

/**
* @brief the update function for the player
*/
void update(Entity *ent)
{
	int forward;
	forward = 0;
	float delta_time = Graphics::get_delta_time().asSeconds();

	//glm::vec3(-5 * cos(glm::radians(entity->current_rotation)), 1.5f, 5 * sin(glm::radians(entity->current_rotation)))

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
		ent->current_rotation -= ent->rotation_rate *delta_time;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
	{
		ent->current_rotation += ent->rotation_rate * delta_time;
	}
	if (ent->current_speed > ent->top_speed)
	{
		ent->current_speed = ent->top_speed;
	}
	else
	{
		ent->current_speed--; //drop speed by one every frame just to slow without gas
	}



	//get gas/brake/reverse
	//change velocity based on that combined with the acceleration and deacceleration rates from the entity
	//update position based on the new velocity

	
	if(forward == 1)
	{
		//handle turning and forward acceleration
		ent->movement_velocity = (ent->movement_velocity / 2.0f) + ((ent->current_speed / 100.0f) * glm::vec3(forward * cos(glm::radians(ent->current_rotation)), 0, -forward * sin(glm::radians(ent->current_rotation))) * delta_time);
	}
	
	else if (forward == 0)
	{
		ent->movement_velocity = (ent->movement_velocity / 2.0f) + ((ent->current_speed / 100.0f) * glm::vec3(forward * cos(glm::radians(ent->current_rotation)), 0, -forward * sin(glm::radians(ent->current_rotation))) * delta_time);
	}
	/*
	else 
	{
		//handle acceleration backwards and turning, turning should be the most effective
		ent->movement_velocity -= ent->movement_velocity / (ent->deacceleration_rate / 200.0f) ;
	}
	*/

	ent->world_position = ent->world_position + ent->movement_velocity;

	ent->model = glm::mat4(1.0f);
	ent->model = glm::translate(ent->model, ent->world_position);
	ent->model = glm::rotate(ent->model, glm::radians(ent->current_rotation), glm::vec3(0, 1, 0));
	ent->model = glm::scale(ent->model, glm::vec3(ent->scale));
}

/**
* @brief the think function for the player
*/
void think(Entity *ent)
{

}

/**
* @brief draws the player
*/
void draw(Entity *ent)
{
	ent->mesh->Mesh::draw(ent->shader->program);
}
