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
	forward = glm::normalize(position - target);
	up = glm::vec3(0.0f, 1.0f, 0.0f);
	right = glm::normalize(glm::cross(forward, up));

	view_matrix = glm::lookAt(position, target, up);

	projection_matrix = glm::perspective(glm::radians(45.0f), screen_size.x / screen_size.y, NEAR_CLIPPING_PLANE, FAR_CLIPPING_PLANE);

	pitch = 0.0f;
	yaw = -90.0f;
	roll = 0.0f;

	horizontal_angle = 3.14f;
	vertical_angle = 0.0f;
}

glm::mat4 Camera::get_camera_view_matrix()
{
	return view_matrix;
}

glm::mat4 Camera::get_camera_projection_matrix()
{
	return projection_matrix;
}

void Camera::camera_get_keyboard_input(sf::Event event)
{
	float camera_speed = 100.0f;
	sf::Keyboard::Key key = event.key.code;
	float delta_time = Graphics::get_delta_time().asSeconds();

	if (key == sf::Keyboard::W)
	{
		position += camera_speed * delta_time * forward;
	}
	else if (key == sf::Keyboard::S)
	{
		position -= camera_speed * delta_time * forward;
	}
	else if (key  == sf::Keyboard::D)
	{
		position += glm::normalize(glm::cross(forward, up)) * delta_time * camera_speed;
	}
	else if (key == sf::Keyboard::A)
	{
		position -= glm::normalize(glm::cross(forward, up)) * delta_time * camera_speed;
	}
	else
	{
		return;
	}

	//camera_update_view_matrix();
}

void Camera::camera_get_mouse_input(sf::Event event)
{
	float sensitivity = 10;
	float delta_time = Graphics::get_delta_time().asSeconds();

	sf::Vector2i mouse_position = sf::Mouse::getPosition();
	sf::Mouse::setPosition(sf::Vector2i(screen_dimensions.x/2, screen_dimensions.y/2), *Graphics::get_game_window());

	slog("mouse Position: %d, %d", mouse_position.x, mouse_position.y);
	slog("set positon: %d, %d", screen_dimensions.x / 2, screen_dimensions.y / 2);
	slog("mouse Position: %d, %d", sf::Mouse::getPosition().x, sf::Mouse::getPosition().y);

	float initial_fov = 45.0f;

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

	//camera_update_view_matrix();
}

void Camera::camera_update_view_matrix()
{
	view_matrix = glm::lookAt(position, target, up);
}