#include <fstream>

#include <simple_logger.h>

#include "sprite_manager.h"
#include "level_editor.h"
#include "graphics.h"

int current_theme_index;
int theme_list_size;

/**
* @brief configures the code for the editor, positioning the grid, and loading the sprites
* @param rows		the number of rows the grid will have
* @param columns	the number of columns in the grid
* @param offsets	the offset of the tiles in the grid
*/
void Level_Editor::configure_editor(unsigned int rows, unsigned int columns, glm::vec2 offsets)
{
	max_grid_columns = columns;
	max_grid_rows = rows;
	tile_offset = offsets;

	grid_tile_sprite = Sprite_Manager::create_sprite("json/sprites/grid_sprite.json");						//empty tile sprite
	activated_tile_sprite = Sprite_Manager::create_sprite("json/sprites/activated_grid_sprite.json");		//tile that has been clicked on and added to the list of activated tiles (added to the stage)
	highlighted_tile_sprite = Sprite_Manager::create_sprite("json/sprites/highlighted_grid_sprite.json");	//the tile that has the cursor over it
	start_tile = Sprite_Manager::create_sprite("json/sprites/start_sprite.json");							//the sprite for the start position in the race
	start_tile_set = false;
	
	//lets fill the grid positions with the configuration we have
	for (int i = 0; i < max_grid_columns; i++)
	{
		for (int j = 0; j < max_grid_rows; j++)
		{
			grid_positions.push_back(glm::vec2((i * grid_tile_sprite->size.x) + tile_offset.x, j * grid_tile_sprite->size.y + tile_offset.y));
		}

	}

	theme_list_size = 0;

	this->add_theme_to_list("json/stage_themes/spicy.json", "json/sprites/spicy_theme_sprite.json");
	this->add_theme_to_list("json/stage_themes/water.json", "json/sprites/water_theme_sprite.json");

	current_theme_index = 0;

	theme_left_arrow_button = new Button("json/buttons/theme_left_arrow.json");
	theme_left_arrow_button->callback = &theme_left_cycle;

	theme_right_arrow_button = new Button("json/buttons/theme_right_arrow.json");
	theme_right_arrow_button->callback = &theme_right_cycle;
}

void Level_Editor::add_theme_to_list(std::string theme_filepath, std::string theme_sprite_filepath)
{
	theme_sprite_map.insert(std::pair<std::string, Sprite *>(theme_filepath, Sprite_Manager::create_sprite(theme_sprite_filepath)));
	theme_list_size++;
	theme_key_list.push_back(theme_filepath);
}

/**
* @brief updates the data in the editor's activated positions and also determines whether to draw the highlighted tile or not (should only draw the highlighted sprite if the cursor is on the grid)
*/
void Level_Editor::update_editor()
{
	mouse_position = sf::Mouse::getPosition(*Graphics::get_game_window());

	theme_left_arrow_button->update();
	theme_right_arrow_button->update();

	bool cursor_on_grid = false;
	for (int i = 0; i < max_grid_columns * max_grid_rows; i++)
	{
		if (mouse_position.x >= grid_positions[i].x && mouse_position.x < grid_positions[i].x + 100)
		{
			if (mouse_position.y >= grid_positions[i].y && mouse_position.y < grid_positions[i].y + 100)
			{
				draw_highlighted_tile = true;
				cursor_on_grid = true;
				//leaving the highlighted tile draw in here makes it only be drawn if its on screen so thats fine
				highlighted_tile_sprite->screen_position = grid_positions[i];

				//since we already have the tile here lets get input and add this tile to the activated tiles if the player left clicks
				if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
				{
					if (!clicked)
					{
						bool found_tile = false;
						for (int j = 0; j < activated_positions.size(); j++)
						{
							if (activated_positions[j] == grid_positions[i])
							{
								activated_positions.erase(activated_positions.begin() + j);
								if (start_tile->screen_position == grid_positions[i])
								{
									start_tile_set = false;
								}
								found_tile = true;
								break;
							}
						}
						if (!found_tile)
						{
							activated_positions.push_back(grid_positions[i]);
						}
						clicked = true;
					}
				}
				else if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
				{
					if (!clicked)
					{
						bool found_tile = false;
						for (int j = 0; j < activated_positions.size(); j++)
						{
							if (activated_positions[j] == grid_positions[i])
							{
								found_tile = true;
								break;
							}
						}
						if (!found_tile)
						{
							activated_positions.push_back(grid_positions[i]);
						}
						start_tile->screen_position = grid_positions[i];
						start_tile_set = true;
						clicked = true;
					}
				}
				else
				{
					clicked = false;
				}
			}
		}
	}
	if (!cursor_on_grid)
	{
		draw_highlighted_tile = false;
	}
}

/**
* @brief draw the tile's onto the screen, then any other UI elements
* @param *camera		the camera used by the sprite manager to draw the sprite
*/
void Level_Editor::draw_editor(Camera *camera)
{
	//draw all grid tiles
	for (int i = 0; i < max_grid_columns * max_grid_rows; i < i++)
	{
		grid_tile_sprite->screen_position = grid_positions[i];
		Sprite_Manager::draw(camera, grid_tile_sprite->id);
	}

	//draw all activated tiles
	for (int i = 0; i < activated_positions.size(); i < i++)
	{
		activated_tile_sprite->screen_position = activated_positions[i];
		Sprite_Manager::draw(camera, activated_tile_sprite->id);
	}

	if (start_tile_set)
	{
		Sprite_Manager::draw(camera, start_tile->id);
	}

	//only draw the highlighted tile, if the cursor is on the grid
	if (draw_highlighted_tile)
	{
		Sprite_Manager::draw(camera, highlighted_tile_sprite->id);
	}



	Sprite_Manager::draw(camera, theme_sprite_map[theme_key_list[current_theme_index]]->id);
	theme_left_arrow_button->draw_background(camera);
	theme_right_arrow_button->draw_background(camera);
}

/**
* @param exit the editor and write a level definition file to the given filename
* @param filename		the name of the file to be saved
*/
void Level_Editor::save_and_exit(std::string filename)
{
	json def = {
		{ "Level",{
			{ "theme", theme_key_list[current_theme_index] },
			{ "start_tile", { start_tile->screen_position.x / 10, start_tile->screen_position.y / 10} },
			{ "number_tiles", activated_positions.size() }
		} }
	};

	for (int i = 0; i < activated_positions.size(); i++)
	{
		def["Level"]["tiles"].push_back({ activated_positions[i].x / 10, activated_positions[i].y / 10 });
	}


	std::ofstream o(filename);
	o << std::setw(4) << def << std::endl;
	o.close();
}

void theme_left_cycle()
{
	current_theme_index--;
	if (current_theme_index < 0)
	{
		current_theme_index = theme_list_size - 1;
	}
	printf("%d\n", current_theme_index);
}

void theme_right_cycle()
{
	current_theme_index++;
	if (current_theme_index >= theme_list_size)
	{
		current_theme_index = 0;
	}
	printf("%d\n", current_theme_index);
}
