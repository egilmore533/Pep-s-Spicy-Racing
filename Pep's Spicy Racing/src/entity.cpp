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

	move_speed = 0.0f;
	rotation_speed = 0.0f;

	think_rate = 0.0f;
	next_think = 0.0f;

	shader = NULL;

	world_position = glm::vec3(0.0f);
	rotation_angle = 0.0f;
	scale = 1.0f;

	model = glm::mat4(1.0f);

	model_location = 0;
	color_location = 0;
}

Entity::~Entity()
{
	//decrement the mesh/model, shader, etc. here
}

void Entity::think()
{

}

/**
 * @brief draws an entity, using its shader program
 */
void Entity::draw()
{
	mesh->Mesh::draw(shader->program);
}

/**
 * @brief temporary update for what will soon become the player entity, updates the players model 
 *			by getting input and using it to determine the entity's position in world space
 */
void Entity::update()
{
	float delta_time = Graphics::get_delta_time().asSeconds();

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
	{
		world_position.z += move_speed * delta_time;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
	{
		world_position.z -= move_speed * delta_time;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
	{
		world_position.x +=  delta_time * move_speed;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
	{
		world_position.x -=  delta_time * move_speed;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::J))
	{
		rotation_angle -= delta_time * rotation_speed;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::L))
	{
		rotation_angle += delta_time * rotation_speed;
	}

	model = glm::mat4(1.0f);
	model = glm::translate(model, world_position);
	model = glm::rotate(model, rotation_angle, glm::vec3(0,1,0));
	model = glm::scale(model, glm::vec3(scale));
}
