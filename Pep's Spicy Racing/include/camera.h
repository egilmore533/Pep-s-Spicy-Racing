#ifndef __CAMERA_H__
#define __CAMERA_H__

#include <glm\gtx\transform.hpp>

#include "entity.h"

#define NEAR_CLIPPING_PLANE		0.1f
#define FAR_CLIPPING_PLANE		100.0f

class Camera
{
public:

	/**
	 * @brief constructs a camera by creating a view and perspective projeciton matrix based on the parameters
	 * @param screen_size	dimensions of the camera's respective screen area, used to determine how the projeciton matrix will be defined
	 * @param pos			initial position of the camera in the game world
	 */
	Camera(glm::vec2 screen_size, glm::vec3 pos);

	/**
	* @brief gets the camera's view matrix, used for drawing 3d objects in the model view projection matrix
	* @return glm::mat4 the camera's view matrix member
	*/
	glm::mat4 get_view_matrix();

	/**
	* @brief gets the camera's projection matrix, used for drawing 3d objects in the model view projection matrix
	* @return glm::mat4 the camera's projection matrix member
	*/
	glm::mat4 get_projection_matrix();

	/**
	* @brief gets the camera's projection matrix, used for drawing 2d HUD/UI elements
	* @return glm::mat4 the camera's ortho_projection_matrix member
	*/
	glm::mat4 get_ortho_projection_matrix();

	/**
	* @brief handles keyboard input and updates the camera's position accordingly, uses delta_time from graphics in an 
	*		 attempt to keep movement smooth, it is still janky
	*/
	void get_keyboard_input();

	/**
	* @brief handles mouse positioning to rotate the camera, uses delta_time and some triginometry to 
	*		 calculate how to rotate the camera smoothly, still janky
	*/
	void get_mouse_input();

	/**
	* @brief update the camera's target, position, and up veector based on the given entity
	*		follows the entity from a certain away
	* @param *entity the entity to follow
	*/
	void follow_entity(Entity *entity);

	/**
	* @brief updates the model matrix using the camera's target as the center
	*/
	void view_matrix_look_at_target();

	/**
	* @brief update's the camera's view matrix using position + forward as the center
	*/
	void view_matrix_look_forward();

	/**
	* @brief sets the view matrix based on the given data
	* @param target_position	the position to look at
	* @param up					the direction that is being used as the upward vector
	*/
	void set_view_matrix(glm::vec3 target_position, glm::vec3 up);

	/**
	* @brief sets the camera's position 
	* @param cam_position the new camera position
	*/
	void Camera::set_camera_position(glm::vec3 cam_position);

	/**
	* @brief returns the camera's position in world space
	*/
	glm::vec3 get_position();

private:	
	glm::vec3 position;		/**< position of the camera in the game world */
	glm::vec3 target;		/**< the position the camera is looking to, use if you want to lock camera to some point in space */
	glm::vec3 forward;		/**< the direction the camera is looking */
	glm::vec3 right;		/**< the direction right of the camera */
	glm::vec3 up;			/**< the direction above the camera */

	float horizontal_angle;		/**< (yaw) the angle that the camera will be looking horizontally, used to update the forward and right vectors, and indirectly the up vector */
	float vertical_angle;		/**< (pitch) the angle that the camera will be looking vertically, used to update the forward and right vectors, and indirectly the up vector */

	glm::mat4 view_matrix;				/**< the view matrix that will be used to draw all game obejcts in the world */
	glm::mat4 projection_matrix;		/**< the projection matrix that the world will be draw from */
	glm::mat4 ortho_projection_matrix;	/**< the projection matrix that the UI/HUD will be drawn from */
	glm::vec2 screen_dimensions;		/**< dimensions of the screen, used to handle mouse input in geT_mouse_input */
};

#endif
