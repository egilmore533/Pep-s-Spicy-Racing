#ifndef __STAGE_H__
#define __STAGE_H__

#include "mesh_manager.h"
#include "shader_manager.h"
#include "texture_manager.h"
#include "camera.h"
#include "entity.h"

class Stage {

public:
	/**
	* @brief loads a stage from the given defintion file
	*/
	Stage(std::string stage_def_filepath);

	/**
	* @brief destructor for the stage that should delete the shaders, meshes and textures of the stage from their repsective managers 
	*/
	~Stage();

	/**
	* @brief loads the stage's theme into the stage's data
	* @param theme_filepath		filepath to the theme definition file
	*/
	void load_theme_data(std::string theme_filepath);

	/**
	* @brief draws the stage (should be one of if not the first thing drawn in the game loop)
	*/
	void draw_stage(Camera *camera, Entity *single_light);

	//std::string  name;
	//MusicPak *stage_music;

	Mesh *tile_mesh;						/**< mesh that this stage uses to create its tiles */
	glm::vec4 tile_color_data;
	Texture *tile_texture;					/**< the stage's texture for each tile */

	Mesh *wall_mesh;						/**< the mesh for the stage's walls */
	glm::vec4 wall_color_data;
	Texture *wall_texture;					/**< texture for the walls */

	glm::vec3 background_color;

	std::vector<glm::vec3> tile_positions;		/**< the positions of each tile in the stage */
	std::vector<glm::vec4> wall_position_data;	/**< locations of all the walls the stage will build, fourth value is the rotation of the wall */
	glm::vec3 start_position;					/**< the position of the starting tile */

	//shader uniform data 
	Shader *shader;
	GLuint model_location;		/**< the location of the uniform model */
	GLuint color_location;		/**< location of the uniform color data */

	GLuint view_location;			/**< location of the view matrix uniform */
	GLuint projection_location;		/**< location of the projection matrix uniform */

	GLuint light_color_location;		/**< location of the light color uniform */
	GLuint light_position_location;		/**< location of the light position uniform */
	GLuint view_position_location;		/**< location of the camera position uniform */

};


#endif
