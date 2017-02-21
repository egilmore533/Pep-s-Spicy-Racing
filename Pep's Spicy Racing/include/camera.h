#ifndef __CAMERA_H__
#define __CAMERA_H__

#include <glm\gtx\transform.hpp>

#define NEAR_CLIPPING_PLANE		0.1f
#define FAR_CLIPPING_PLANE		100.0f

class Camera
{
public:
	Camera(glm::vec2 screen_size, glm::vec3 pos);
	glm::mat4 get_camera_view_matrix();
	glm::mat4 get_camera_projection_matrix();
	void camera_get_keyboard_input();
	void camera_update_view_matrix();
	void camera_get_mouse_input();

private:
	void camera_translate(Camera *camera, int direction);
	void camera_rotate();
	
	glm::vec3 position;		/**< position of the camera in the game world */
	glm::vec3 target;		/**< the position the camera is looking to */
	glm::vec3 forward;		/**< the direction the camera is looking */
	glm::vec3 right;		/**< the direction right of the camera */
	glm::vec3 up;			/**< the direction above the camera */

	float yaw;		/**< y-axis locked */
	float pitch;	/**< x-axis locked */
	float roll;		/**< z-axis locked probably will go unused*/ 

	float horizontal_angle;
	float vertical_angle;

	glm::mat4 view_matrix;
	glm::mat4 projection_matrix;
	glm::vec2 screen_dimensions;
};

#endif
