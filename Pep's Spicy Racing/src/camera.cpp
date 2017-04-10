#include <GL/glew.h>

#include <glm\gtx\transform.hpp>
#include <SFML/Window.hpp>

#include <simple_logger.h>

#include "graphics.h"
#include "camera.h"

Camera::Camera(glm::vec2 screen_size, glm::vec3 pos)
{
	screen_dimensions = screen_size;
	position = pos;
	target = glm::vec3(0.0f, 0.0f, 0.0f);
	forward = glm::vec3(1.0f);
	up = glm::vec3(0.0f, 1.0f, 0.0f);
	right = glm::normalize(glm::cross(forward, up));

	view_matrix = glm::lookAt(position, position + forward, up);

	projection_matrix = glm::perspective(glm::radians(45.0f), screen_size.x / screen_size.y, NEAR_CLIPPING_PLANE, FAR_CLIPPING_PLANE);
	ortho_projection_matrix = glm::ortho(0.0f, screen_size.x, screen_size.y, 0.0f, -1.0f, 1.0f);

	horizontal_angle = 3.14f;
	vertical_angle = 0.0f;

	//set mouse to center at the start before taking input 
	sf::Mouse::setPosition(sf::Vector2i(screen_dimensions.x / 2, screen_dimensions.y / 2), *Graphics::get_game_window());
}

glm::mat4 Camera::get_view_matrix()
{
	return view_matrix;
}

glm::mat4 Camera::get_projection_matrix()
{
	return projection_matrix;
}

glm::mat4 Camera::get_ortho_projection_matrix()
{
	return ortho_projection_matrix;
}

void Camera::get_keyboard_input()
{
	float camera_speed = 128.0f;
	float delta_time = Graphics::get_delta_time().asSeconds();

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
	{
		position += camera_speed * delta_time * forward;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
	{
		position -= camera_speed * delta_time * forward;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
	{
		position += glm::normalize(glm::cross(forward, up)) * delta_time * camera_speed;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
	{
		position -= glm::normalize(glm::cross(forward, up)) * delta_time * camera_speed;
	}
	else
	{
		return;
	}
}

void Camera::get_mouse_input()
{
	float sensitivity = 1.0f;
	float delta_time = Graphics::get_delta_time().asSeconds();

	sf::Vector2i mouse_position = sf::Mouse::getPosition(*Graphics::get_game_window());
	sf::Mouse::setPosition(sf::Vector2i(screen_dimensions.x/2, screen_dimensions.y/2), *Graphics::get_game_window());

	horizontal_angle += sensitivity * delta_time * float(screen_dimensions.x / 2 - mouse_position.x);
	vertical_angle += sensitivity * delta_time * float(screen_dimensions.y / 2 - mouse_position.y);
	
	forward = glm::vec3(
		cos(vertical_angle) * sin(horizontal_angle),
		sin(vertical_angle),
		cos(vertical_angle) * cos(horizontal_angle)
	);

	right = glm::vec3(
		sin(horizontal_angle - 3.14f / 2.0f),
		0,
		cos(horizontal_angle - 3.14f / 2.0f)
	);

	up = glm::cross(right, forward);
}

/**
* @brief update the camera's target, position, and up veector based on the given entity
*		follows the entity from a certain away
* @param *entity the entity to follow
*/
void Camera::follow_entity(Entity *entity)
{
	position = entity->world_position + glm::vec3(-5 * cos(glm::radians(entity->rotation_angle)), 1.5f, 5 * sin(glm::radians(entity->rotation_angle)));
	target = entity->world_position;
	up = glm::vec3(0,1,0);
}

/**
* @brief updates the model matrix using the camera's target as the center
*/
void Camera::view_matrix_look_at_target()
{
	view_matrix = glm::lookAt(position, target, up);
}

/**
* @brief update's the camera's view matrix using position + forward as the center
*/
void Camera::view_matrix_look_forward()
{
	view_matrix = glm::lookAt(position, position + forward, up);
}

/**
* @brief sets the view matrix based on the given data, needs to give the calculated forward vector
* @param target_position	the position to look at (calculate the forward vector and give it to this)
* @param up					the direction that is being used as the upward vector
*/
void Camera::set_view_matrix(glm::vec3 target_position, glm::vec3 up)
{
	view_matrix = glm::lookAt(position, target_position, up);
}

/**
* @brief sets the camera's position
* @param cam_position the new camera position
*/
void Camera::set_camera_position(glm::vec3 cam_position)
{
	position = cam_position;
}

/**
* @brief returns the camera's position in world space
* @return the camera's position
*/
glm::vec3 Camera::get_position()
{
	return position;
}
