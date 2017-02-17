#pragma once
#include <glm\gtx\transform.hpp>

class Camera
{
public:
	Camera(glm::vec2 screen_size, glm::vec3 position);
	~Camera();
	glm::mat4 get_camera_view_matrix();
	glm::mat4 get_camera_projection_matrix();
	void camera_get_input();

private:
	void camera_translate(Camera *camera, int direction);
	void camera_rotate();


};