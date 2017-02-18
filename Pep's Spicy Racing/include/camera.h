#pragma once
#include <glm\gtx\transform.hpp>

#define NEAR_CLIPPING_PLANE		0.1f
#define FAR_CLIPPING_PLANE		100.0f

class Camera
{
public:
	Camera(glm::vec2 screen_size, glm::vec3 pos);
	glm::mat4 get_camera_view_matrix();
	glm::mat4 get_camera_projection_matrix();
	void camera_get_keyboard_input(sf::Keyboard::Key key);
	void camera_update_view_matrix();
	void camera_get_mouse_input(sf::Mouse mouse);

private:
	void camera_translate(Camera *camera, int direction);
	void camera_rotate();
	
	glm::vec3 position;		/**< position of the camera in the game world */
	glm::vec3 target;		/**< the position the camera is looking to */
	glm::vec3 forward;		/**< the direction the camera is looking */
	glm::vec3 right;		/**< the direction right of the camera */
	glm::vec3 up;			/**< the direction above the camera */

	float yaw;		/**< x rotation */
	float pitch;	/**< y rotation */
	float roll;		/**< z rotation */

	glm::mat4 view_matrix;
	glm::mat4 projection_matrix;

};