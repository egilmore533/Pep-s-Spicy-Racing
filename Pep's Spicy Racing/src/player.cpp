#include "graphics.h"
#include "player.h"

Player::Player(glm::vec3 position)
{
	glm::vec3 cameraPosition = glm::vec3(position.x, position.y, position.z - 1.0f);
	player_cam = new Camera(glm::vec2(WINDOW_WIDTH, WINDOW_HEIGHT), cameraPosition);
	entity_component = Entity_Manager::create_entity("json/entities/racer.json", position);
	entity_component->scale = 0.5f;
	entity_component->update = &update;
	entity_component->draw = &draw;
	entity_component->think = &think;
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

	//glm::vec3(-5 * cos(glm::radians(entity->rotation_angle)), 1.5f, 5 * sin(glm::radians(entity->rotation_angle)))

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
	{
		forward = 1;
		//ent->movement_vector.x += ent->move_speed * delta_time;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
	{
		forward = -1;
		//ent->world_position.x -= ent->move_speed * delta_time;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
	{
		ent->rotation_angle -= ent->rotation_speed *delta_time;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
	{
		ent->rotation_angle += ent->rotation_speed * delta_time;
	}

	
	ent->movement_vector = ent->move_speed * glm::vec3(forward * cos(glm::radians(ent->rotation_angle)), 0, -forward * sin(glm::radians(ent->rotation_angle))) * delta_time;
	ent->world_position = ent->world_position + ent->movement_vector;

	ent->model = glm::mat4(1.0f);
	ent->model = glm::translate(ent->model, ent->world_position);
	ent->model = glm::rotate(ent->model, glm::radians(ent->rotation_angle), glm::vec3(0, 1, 0));
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
