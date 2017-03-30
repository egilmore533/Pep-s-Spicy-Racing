#include "sprite_manager.h"
#include "level_editor.h"
#include "graphics.h"

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

																											//lets fill the grid positions with the configuration we have
	for (int i = 0; i < max_grid_columns; i++)
	{
		for (int j = 0; j < max_grid_rows; j++)
		{
			grid_positions.push_back(glm::vec2((i * grid_tile_sprite->size.x) + tile_offset.x, j * grid_tile_sprite->size.y + tile_offset.y));
		}

	}
}

/**
* @brief updates the data in the editor's activated positions and also determines whether to draw the highlighted tile or not (should only draw the highlighted sprite if the cursor is on the grid)
*/
void Level_Editor::update_editor()
{
	mouse_position = sf::Mouse::getPosition(*Graphics::get_game_window());

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
	//only draw the highlighted tile, if the cursor is on the grid
	if (draw_highlighted_tile)
	{
		Sprite_Manager::draw(camera, highlighted_tile_sprite->id);
	}

	//draw all activated tiles
	for (int i = 0; i < activated_positions.size(); i < i++)
	{
		activated_tile_sprite->screen_position = activated_positions[i];
		Sprite_Manager::draw(camera, activated_tile_sprite->id);
	}

	//draw all grid tiles
	for (int i = 0; i < max_grid_columns * max_grid_rows; i < i++)
	{
		grid_tile_sprite->screen_position = grid_positions[i];
		Sprite_Manager::draw(camera, grid_tile_sprite->id);
	}
}

/**
* @param exit the editor and write a level definition file to the given filename
* @param filename		the name of the file to be saved
*/
void Level_Editor::save_and_exit(std::string filename)
{

}
