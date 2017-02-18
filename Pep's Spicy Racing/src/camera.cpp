#include <glm\gtx\transform.hpp>
#include <SFML/Window.hpp>

#include "camera.h"

Camera::Camera(glm::vec2 screen_size, glm::vec3 pos)
{
	position = position;
	target = glm::vec3(0.0f, 0.0f, 0.0f);
	forward = glm::vec3(0.0f, 0.0f, -1.0f);
	up = glm::vec3(0.0f, 1.0f, 0.0f);
	right = glm::normalize(glm::cross(forward, up));

	view_matrix = glm::lookAt(position, target, up);

	projection_matrix = glm::perspective(glm::radians(45.0f), screen_size.x / screen_size.y, NEAR_CLIPPING_PLANE, FAR_CLIPPING_PLANE);

	pitch = 0.0f;
	yaw = -90.0f;
	roll = 0.0f;
}

glm::mat4 Camera::get_camera_view_matrix()
{
	return view_matrix;
}

glm::mat4 Camera::get_camera_projection_matrix()
{
	return projection_matrix;
}

void Camera::camera_get_keyboard_input(sf::Keyboard::Key key)
{
	float camera_speed = 0.5f;

	if (key == sf::Keyboard::W)
	{
		position += camera_speed * forward;
	}
	else if (key == sf::Keyboard::S)
	{
		position -= camera_speed * forward;
	}
	else if (key  == sf::Keyboard::D)
	{
		position += glm::normalize(glm::cross(forward, up)) * camera_speed;
	}
	else if (key == sf::Keyboard::A)
	{
		position -= glm::normalize(glm::cross(forward, up)) * camera_speed;
	}
	else
	{
		return;
	}
	camera_update_view_matrix();
}

void Camera::camera_get_mouse_input(sf::Mouse mouse)
{
	float sensitivity = 0.1f;

}

void Camera::camera_update_view_matrix()
{
	view_matrix = glm::lookAt(position, position + forward, up);

}