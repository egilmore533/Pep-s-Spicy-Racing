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

	//now that we've loaded all the tiles we will now determine where to place walls
	for (int i = 0; i < tile_positions.size(); i++)
	{
		bool north, south, east, west; //bools to know if we need corresponding walls
		north = south = east = west = true; //assume we need a wall, unless we have a reason to not put a wall

		//for each tile, look through each other tile determining whether or not to keep them
		for (int j = 0; j < tile_positions.size(); j++)
		{
			if (i == j) continue; //don't compare the same tiles

			//get the x and z differences to determine if we need some walls
			float z_diff = tile_positions[i].z - tile_positions[j].z;
			float x_diff = tile_positions[i].x - tile_positions[j].x;

			bool z_aligned = z_diff == 0.0f;
			bool x_aligned = x_diff == 0.0f;
			//if we are on the same x or z axis we might have a connecting tile
			if (z_aligned || x_aligned)
			{
				if (z_aligned && x_diff <= 10.0f && x_diff > 0.0f)
				{
					south = false;
				}

				else if (z_aligned && x_diff < 0.0f && x_diff >= -10.0f)
				{
					north = false;
				}

				else if (x_aligned && z_diff <= 10.0f && z_diff > 0.0f)
				{
					west = false;
				}

				else if (x_aligned && z_diff < 0.0f && z_diff >= -10.0f)
				{
					east = false;
				}
			}
		}

		if (north)
		{
			wall_position_data.push_back(glm::vec4(tile_positions[i].x + 5.0f, tile_positions[i].y, tile_positions[i].z , 90.0f));
		}
		if (south)
		{
			wall_position_data.push_back(glm::vec4(tile_positions[i].x - 5.0f, tile_positions[i].y, tile_positions[i].z, 90.0f));
		}
		if (east)
		{
			wall_position_data.push_back(glm::vec4(tile_positions[i].x, tile_positions[i].y, tile_positions[i].z + 5.0f, 0.0f));
		}
		if (west)
		{
			wall_position_data.push_back(glm::vec4(tile_positions[i].x, tile_positions[i].y, tile_positions[i].z - 5.0f, 0.0f));
		}
	}
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
		
		model = glm::scale(model, glm::vec3(5.0f));

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

	for (int i = 0; i < wall_position_data.size(); i++)
	{
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(wall_position_data[i].x, wall_position_data[i].y, wall_position_data[i].z));
		model = glm::rotate(model, glm::radians(wall_position_data[i].w), glm::vec3(0, 1, 0));
		model = glm::scale(model, glm::vec3(5.0f, 1.0f, 5.0f));

		glUniform4fv(color_location, 1, &wall_color_data[0]);
		glUniformMatrix4fv(model_location, 1, GL_FALSE, &model[0][0]);

		glUniformMatrix4fv(view_location, 1, GL_FALSE, &camera->Camera::get_view_matrix()[0][0]);
		glUniformMatrix4fv(projection_location, 1, GL_FALSE, &camera->Camera::get_projection_matrix()[0][0]);

		glUniform4f(light_color_location, single_light->color_data.x, single_light->color_data.y, single_light->color_data.z, single_light->color_data.w);
		glUniform3f(light_position_location, single_light->world_position.x, single_light->world_position.y, single_light->world_position.z);
		glUniform3f(view_position_location, camera->get_position().x, camera->get_position().y, camera->get_position().z);

		glBindTexture(GL_TEXTURE_2D, tile_texture->get_texture());

		wall_mesh->Mesh::draw(shader->program);
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
	tile_color_data = glm::vec4(theme["tile-color"][0], theme["tile-color"][1], theme["tile-color"][2], theme["tile-color"][3]);
	tile_texture = Texture_Manager::create_texture(tile["texture-filepath"], true, true);

	//next lets handle the wall loading
	json wall_def = load_from_def(wall_def_filepath);
	json wall = get_element_data(wall_def, "Wall");

	wall_mesh = Mesh_Manager::create_mesh(wall["model-filepath"]);
	wall_color_data = glm::vec4(theme["wall-color"][0], theme["wall-color"][1], theme["wall-color"][2], theme["wall-color"][3]);
	wall_texture = Texture_Manager::create_texture(wall["texture-filepath"], true, true);


	//now lets set up the shader program
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