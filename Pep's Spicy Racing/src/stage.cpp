#include <simple_logger.h>
#include <glm/gtc/matrix_transform.hpp>
#include "json_helper.h"
#include "stage.h"

/**
* @brief loads a stage from the given defintion file
*/
Stage::Stage(std::string stage_def_filepath)
{
	node_id_num = 0;
	tile_dimensions = glm::vec3(10.0f, 10.0f, 10.0f);

	path_type = CenterAndNeighbor;

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
	load_walls();

	//now we will find the course through the track to get a loop in the tiles
	get_track_in_order();

	//now we will find the racing line through the track for keeping track of position and moving the AI
	switch (path_type)
	{
	case CenterAndNeighbor:
		get_center_and_neighbor_path();
		break;
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

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, tile_texture->get_texture());
		glActiveTexture(GL_TEXTURE0);

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

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, tile_texture->get_texture());
		glActiveTexture(GL_TEXTURE0);

		wall_mesh->Mesh::draw(shader->program);
	}

	for (int i = 0; i < node_list.size(); i++)
	{
		glm::vec4 color = glm::vec4(1.0f);
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(node_list[i].position.x, node_list[i].position.y, node_list[i].position.z));
		
		//no rotate or scale
		//model = glm::rotate(model, glm::radians(wall_position_data[i].w), glm::vec3(0, 1, 0));
		model = glm::scale(model, glm::vec3(1.0f));

		glUniform4fv(color_location, 1, &color[0]);
		glUniformMatrix4fv(model_location, 1, GL_FALSE, &model[0][0]);

		glUniformMatrix4fv(view_location, 1, GL_FALSE, &camera->Camera::get_view_matrix()[0][0]);
		glUniformMatrix4fv(projection_location, 1, GL_FALSE, &camera->Camera::get_projection_matrix()[0][0]);

		glUniform4f(light_color_location, single_light->color_data.x, single_light->color_data.y, single_light->color_data.z, single_light->color_data.w);
		glUniform3f(light_position_location, single_light->world_position.x, single_light->world_position.y, single_light->world_position.z);
		glUniform3f(view_position_location, camera->get_position().x, camera->get_position().y, camera->get_position().z);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, tile_texture->get_texture());
		glActiveTexture(GL_TEXTURE0);

		node_mesh->Mesh::draw(shader->program);
	}
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

	//hard coded debug sphere
	node_mesh = Mesh_Manager::create_mesh("models/sphere.obj");

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

void Stage::load_walls()
{
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
				if (z_aligned && x_diff <= tile_dimensions.x && x_diff > 0.0f)
				{
					south = false;
				}

				else if (z_aligned && x_diff < 0.0f && x_diff >= -tile_dimensions.x)
				{
					north = false;
				}

				else if (x_aligned && z_diff <= tile_dimensions.z && z_diff > 0.0f)
				{
					west = false;
				}

				else if (x_aligned && z_diff < 0.0f && z_diff >= -tile_dimensions.z)
				{
					east = false;
				}
			}
		}

		if (north)
		{
			wall_position_data.push_back(glm::vec4(tile_positions[i].x + tile_dimensions.x / 2.0f, tile_positions[i].y, tile_positions[i].z, 90.0f));
		}
		if (south)
		{
			wall_position_data.push_back(glm::vec4(tile_positions[i].x - tile_dimensions.x / 2.0f, tile_positions[i].y, tile_positions[i].z, 90.0f));
		}
		if (east)
		{
			wall_position_data.push_back(glm::vec4(tile_positions[i].x, tile_positions[i].y, tile_positions[i].z + tile_dimensions.z / 2.0f, 0.0f));
		}
		if (west)
		{
			wall_position_data.push_back(glm::vec4(tile_positions[i].x, tile_positions[i].y, tile_positions[i].z - tile_dimensions.z / 2.0f, 0.0f));
		}
	}
}

void Stage::get_track_in_order()
{
	//go through the stage, connecting the nearby tiles to each other until reaching the first tile again
	tile_positions_in_order.push_back(start_position);

	glm::vec3 current_position = find_next_neighbor_tile(tile_positions, tile_positions_in_order, start_position, tile_dimensions);
	while (current_position != start_position)
	{
		tile_positions_in_order.push_back(current_position);
		current_position = find_next_neighbor_tile(tile_positions, tile_positions_in_order, current_position, tile_dimensions);

		if (current_position == glm::vec3(-1, -1, -1))
		{
			printf("dun goofed\n");
			break;
		}
	}
}

void Stage::get_center_and_neighbor_nodes(glm::vec3 current_tile, glm::vec3 next_tile, bool finish_tile)
{
	Node center_point;
	center_point.position = current_tile;
	center_point.node_num = node_id_num++;
	center_point.finish_line_node = finish_tile;
	node_list.push_back(center_point);


	Node neighbor_point;
	neighbor_point.node_num = node_id_num++;
	neighbor_point.finish_line_node = false;

	CardinalDirection direction = direction_to_next_tile(current_tile, next_tile, tile_dimensions);

	if (direction == North)
		neighbor_point.position = glm::vec3(current_tile.x + tile_dimensions.x / 2.0f, current_tile.y, current_tile.z);

	else if (direction == South)
		neighbor_point.position = glm::vec3(current_tile.x - tile_dimensions.x / 2.0f, current_tile.y, current_tile.z);

	else if (direction == East)
		neighbor_point.position = glm::vec3(current_tile.x, current_tile.y, current_tile.z + tile_dimensions.z / 2.0f);

	else if (direction == West)
		neighbor_point.position = glm::vec3(current_tile.x, current_tile.y, current_tile.z - tile_dimensions.z / 2.0f);

	node_list.push_back(neighbor_point);
}

void Stage::get_center_and_neighbor_path()
{
	get_center_and_neighbor_nodes(tile_positions_in_order[0], tile_positions_in_order[1], true);

	for (int i = 1; i < tile_positions_in_order.size(); i++)
	{
		//first get the current and next tiles for use
		glm::vec3 current_tile, next_tile;

		current_tile = tile_positions_in_order[i];

		//if the next tile doesn't exist in the list its the finish line and we need the first tile again
		if (i + 1< tile_positions_in_order.size())
			next_tile = tile_positions_in_order[i + 1];
		else
			next_tile = tile_positions_in_order[0];


		get_center_and_neighbor_nodes(current_tile, next_tile, false);
	}
}

glm::vec3 find_next_neighbor_tile(std::vector<glm::vec3> tile_positions, std::vector<glm::vec3> already_added_positions, glm::vec3 position, glm::vec3 tile_dimensions)
{
	for (int j = 0; j < tile_positions.size(); j++)
	{
		bool already_entered = false;
		for (int i = 0; i < already_added_positions.size(); i++)
		{
			if (already_added_positions[i].x == tile_positions[j].x && already_added_positions[i].y == tile_positions[j].y && already_added_positions[i].z == tile_positions[j].z)
			{
				already_entered = true;
				break;
			}
		}

		if (already_entered)
			continue;

		if (position.x == tile_positions[j].x && position.y == tile_positions[j].y && position.z == tile_positions[j].z) continue; //don't compare the same tiles

		float z_diff = position.z - tile_positions[j].z;
		float x_diff = position.x - tile_positions[j].x;

		bool z_aligned = z_diff == 0.0f;
		bool x_aligned = x_diff == 0.0f;

		bool north, south, east, west;
		//if we are on the same x or z axis we might have a connecting tile
		if (z_aligned || x_aligned)
		{

			if ((z_aligned && x_diff < 0.0f && x_diff >= -tile_dimensions.x)
				|| (z_aligned && x_diff <= tile_dimensions.x && x_diff > 0.0f)
				|| (x_aligned && z_diff < 0.0f && z_diff >= -tile_dimensions.z)
				|| (x_aligned && z_diff <= tile_dimensions.z && z_diff > 0.0f)
				)
			{
				return tile_positions[j];
			}
		}
	}

	return glm::vec3(-1, -1, -1);
}

/**
* @brief test to determine the direction of the next tile (cardinal directions only)
* @param current_tile	the position of the current tile in world space
* @param next_tile		the position of the next tile in world space
* @param dimensions		dimensions of the tile
*/
CardinalDirection direction_to_next_tile(glm::vec3 current_tile, glm::vec3 next_tile, glm::vec3 dimensions)
{
	float x_diff = current_tile.x - next_tile.x;
	float z_diff = current_tile.z - next_tile.z;
	if (x_diff == dimensions.x)
	{
		return South;
	}
	else if (x_diff == -dimensions.x)
	{
		return North;
	}
	else if (z_diff == dimensions.z)
	{
		return West;
	}
	else if (z_diff == -dimensions.z)
	{
		return East;
	}

	slog("error finding cardinal direction");
}


