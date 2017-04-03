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
	player_cam->set_camera_position(glm::vec3(entity_component->world_position.x - 5.0f, entity_component->world_position.y + 1, entity_component->world_position.z));
	glm::vec3 cam_position = player_cam->get_position();

	glm::vec3 up = glm::vec3(0, 1, 0);

	glm::vec3 target = glm::vec3(entity_component->world_position.x, entity_component->world_position.y, entity_component->world_position.z);

	player_cam->set_view_matrix(target, up);
}

/**
* @brief the update function for the player
*/
void update(Entity *ent)
{
	float delta_time = Graphics::get_delta_time().asSeconds();

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
	{
		ent->world_position.x += ent->move_speed * delta_time;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
	{
		ent->world_position.x -= ent->move_speed * delta_time;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
	{
		ent->world_position.z += ent->move_speed * delta_time;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
	{
		ent->world_position.z -= ent->move_speed * delta_time;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::J))
	{
		ent->rotation_angle -= ent->rotation_speed *delta_time;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::L))
	{
		ent->rotation_angle += ent->rotation_speed * delta_time;
	}

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
