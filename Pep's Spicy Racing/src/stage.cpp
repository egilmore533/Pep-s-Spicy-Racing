#include <simple_logger.h>
#include <glm/gtc/matrix_transform.hpp>
#include "json_helper.h"
#include "stage.h"

/**
* @brief loads a stage from the given defintion file
*/
Stage::Stage(std::string stage_def_filepath)
{
	//start by loading up the level data
	json level_def = load_from_def(stage_def_filepath);
	json level = get_element_data(level_def, "Level");

	load_theme_data(level["theme"]);

	start_position = glm::vec3(level["start_tile"][0], 0.0f, level["start_tile"][1]);
	int number_tiles = (int)level["number_tiles"];
	tile_positions.reserve(number_tiles);
	for (int i = 0; i < number_tiles; i++)
	{
		tile_positions.push_back(glm::vec3(level["tiles"][i][0], 0.0f, level["tiles"][i][1]));
	}

	//then determine where to place walls

}

Stage::~Stage()
{
	Mesh_Manager::dereference_mesh(tile_mesh->filepath);
	Texture_Manager::dereference_texture(tile_texture->filepath);
	Shader_Manager::dereference_shader(shader->shader_def_file);
}

/**
* @brief draws the stage (should be one of if not the first thing drawn in the game loop)
*			draws all the tiles first, then the walls, then any extra stage bits we add
* @param *camera		the camera to determine how to draw the stage
* @param *single_light	the only scene light (TODO MAKE THE LIGHT MANAGER AND ADD IT TO THE ENTITY DRAW ALL AND HERE)
*/
void Stage::draw_stage(Camera *camera, Entity *single_light)
{
	glUseProgram(shader->program);

	for (int i = 0; i < tile_positions.size(); i++)
	{
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, tile_positions[i]);
		//model = glm::rotate(model, rotation_angle, glm::vec3(0, 1, 0));

		//the start of one tile should be 10 units away from the start of another tile so scale by 5 to make the tiles touch put it slightly less than that for testing
		model = glm::scale(model, glm::vec3(4.9f));

		glUniform4fv(color_location, 1, &tile_color_data[0]);
		glUniformMatrix4fv(model_location, 1, GL_FALSE, &model[0][0]);

		glUniformMatrix4fv(view_location, 1, GL_FALSE, &camera->Camera::get_view_matrix()[0][0]);
		glUniformMatrix4fv(projection_location, 1, GL_FALSE, &camera->Camera::get_projection_matrix()[0][0]);

		glUniform4f(light_color_location, single_light->color_data.x, single_light->color_data.y, single_light->color_data.z, single_light->color_data.w);
		glUniform3f(light_position_location, single_light->world_position.x, single_light->world_position.y, single_light->world_position.z);
		glUniform3f(view_position_location, camera->get_position().x, camera->get_position().y, camera->get_position().z);

		glBindTexture(GL_TEXTURE_2D, tile_texture->get_texture());

		tile_mesh->Mesh::draw(shader->program);
	}

	//then draw walls

	//then draw extra stage bits
}

/**
* @brief loads the stage's theme into the stage's data
* @param theme_filepath		filepath to the theme definition file
*/
void Stage::load_theme_data(std::string theme_filepath)
{
	json theme_def = load_from_def(theme_filepath);
	json theme = get_element_data(theme_def, "Stage Theme");

	//lets get everything out of the theme first
	std::string tile_def_filepath = theme["tile-def-file"];
	std::string wall_def_filepath = theme["wall-def-file"];

	background_color = glm::vec4(theme["background-color"][0], theme["background-color"][1], theme["background-color"][2], theme["background-color"][3]);

	//next lets handle the tile loading
	json tile_def = load_from_def(tile_def_filepath);
	json tile = get_element_data(tile_def, "Tile");

	tile_mesh = Mesh_Manager::create_mesh(tile["model-filepath"]);
	tile_color_data = glm::vec4(tile["color"][0], tile["color"][1], tile["color"][2], tile["color"][3]);
	tile_texture = Texture_Manager::create_texture(tile["texture-filepath"], true, true);

	shader = Shader_Manager::create_shader(theme["shader-program"]);
	model_location = glGetUniformLocation(shader->program, "model");
	color_location = glGetUniformLocation(shader->program, "object_color");

	view_location = glGetUniformLocation(shader->program, "view");
	projection_location = glGetUniformLocation(shader->program, "projection");

	light_color_location = glGetUniformLocation(shader->program, "light_color");
	light_position_location = glGetUniformLocation(shader->program, "light_position");
	view_position_location = glGetUniformLocation(shader->program, "view_position");

	//then we'll load the wall data
}