#include <fstream>

#include <simple_logger.h>

#include "sprite_manager.h"
#include "level_editor.h"
#include "graphics.h"

int current_theme_index;
int theme_list_size;

int current_special_index;
int special_list_size;

Level_Editor::Level_Editor(unsigned int rows, unsigned int columns, glm::vec2 offsets)
{
	max_grid_columns = columns;
	max_grid_rows = rows;
	tile_offset = offsets;

	grid_tile_sprite = Sprite_Manager::create_sprite("json/GUI/sprites/grid_sprite.json");						//empty tile sprite
	activated_tile_sprite = Sprite_Manager::create_sprite("json/GUI/sprites/activated_grid_sprite.json");		//tile that has been clicked on and added to the list of activated tiles (added to the stage)
	highlighted_tile_sprite = Sprite_Manager::create_sprite("json/GUI/sprites/highlighted_grid_sprite.json");	//the tile that has the cursor over it
	start_tile = Sprite_Manager::create_sprite("json/GUI/sprites/start_sprite.json");							//the sprite for the start position in the race
	
	speed_pad = Sprite_Manager::create_sprite("json/GUI/sprites/speed_pad_sprite.json");
	item_box = Sprite_Manager::create_sprite("json/GUI/sprites/item_box_sprite.json");

	start_tile_set = false;
	
	//lets fill the grid positions with the configuration we have
	for (int i = 0; i < max_grid_columns; i++)
	{
		for (int j = 0; j < max_grid_rows; j++)
		{
			grid_positions.push_back(glm::vec2((i * grid_tile_sprite->size.x) + tile_offset.x, j * grid_tile_sprite->size.y + tile_offset.y));
		}

	}

	if (!editor_font.loadFromFile("fonts/Spicy.ttf"))
	{
		printf("dun goofed\n");
	}

	//theme settings stuff
	theme_list_size = 0;

	this->add_theme_to_list("json/stage_themes/spicy.json", "json/GUI/sprites/spicy_theme_sprite.json");
	this->add_theme_to_list("json/stage_themes/water.json", "json/GUI/sprites/water_theme_sprite.json");

	current_theme_index = 0;

	theme_left_arrow_button = new Button("json/GUI/buttons/level_editor/theme_left_arrow.json");
	theme_left_arrow_button->callback = &theme_left_cycle;

	theme_right_arrow_button = new Button("json/GUI/buttons/level_editor/theme_right_arrow.json");
	theme_right_arrow_button->callback = &theme_right_cycle;

	theme_text.setFont(editor_font);
	theme_text.setString("Theme");
	theme_text.setFillColor(sf::Color::Red);
	theme_text.setOutlineColor(sf::Color::Black);
	theme_text.setOutlineThickness(4.0f);
	theme_text.setCharacterSize(64);
	theme_text.setPosition((WINDOW_WIDTH - theme_text.getLocalBounds().width - 75.0f) - (theme_text.getLocalBounds().width / 2.0f), 20);
	


	special_text.setFont(editor_font);
	special_text.setString("Right Click Placement");
	special_text.setFillColor(sf::Color::Red);
	special_text.setOutlineColor(sf::Color::Black);
	special_text.setOutlineThickness(2.0f);
	special_text.setCharacterSize(24);
	special_text.setPosition((WINDOW_WIDTH - special_text.getLocalBounds().width - 75.0f) - (special_text.getLocalBounds().width / 2.0f), 320);

	special_item_left_arrow = new Button("json/GUI/buttons/level_editor/special_left_arrow.json");
	special_item_left_arrow->callback = &special_left_cycle;

	special_item_right_arrow = new Button("json/GUI/buttons/level_editor/special_right_arrow.json");
	special_item_right_arrow->callback = &special_right_cycle;

	finish_line_text.setFont(editor_font);
	finish_line_text.setString("Finish Line");
	finish_line_text.setFillColor(sf::Color::Red);
	finish_line_text.setOutlineColor(sf::Color::Black);
	finish_line_text.setOutlineThickness(2.0f);
	finish_line_text.setCharacterSize(32);
	finish_line_text.setPosition(special_item_left_arrow->background->screen_position.x + special_item_left_arrow->background->size.x + 40.0f, 440);

	speed_pad_text.setFont(editor_font);
	speed_pad_text.setString("Speed Pad");
	speed_pad_text.setFillColor(sf::Color::Red);
	speed_pad_text.setOutlineColor(sf::Color::Black);
	speed_pad_text.setOutlineThickness(2.0f);
	speed_pad_text.setCharacterSize(32);
	speed_pad_text.setPosition(special_item_left_arrow->background->screen_position.x + special_item_left_arrow->background->size.x + 40.0f, 440);

	
	item_box_text.setFont(editor_font);
	item_box_text.setString("Item Box");
	item_box_text.setFillColor(sf::Color::Red);
	item_box_text.setOutlineColor(sf::Color::Black);
	item_box_text.setOutlineThickness(2.0f);
	item_box_text.setCharacterSize(32);
	item_box_text.setPosition(special_item_left_arrow->background->screen_position.x + special_item_left_arrow->background->size.x + 40.0f, 440);


	//special settings stuff
	special_list_size = 0;

	this->add_special_to_list("json/entities/finish_line.json", finish_line_text);
	this->add_special_to_list("json/entities/speed_pad.json", speed_pad_text);
	this->add_special_to_list("json/entities/item_box.json", item_box_text);

	current_special_index = 0;
}

Level_Editor::~Level_Editor()
{
	Sprite_Manager::delete_sprite(grid_tile_sprite->id);
	Sprite_Manager::delete_sprite(activated_tile_sprite->id);
	Sprite_Manager::delete_sprite(highlighted_tile_sprite->id);
	Sprite_Manager::delete_sprite(start_tile->id);
	Sprite_Manager::delete_sprite(speed_pad->id);
	Sprite_Manager::delete_sprite(item_box->id);

	delete theme_left_arrow_button;
	delete theme_right_arrow_button;

	delete special_item_left_arrow;
	delete special_item_right_arrow;
}

void Level_Editor::add_theme_to_list(std::string theme_filepath, std::string theme_sprite_filepath)
{
	theme_sprite_map.insert(std::pair<std::string, Sprite *>(theme_filepath, Sprite_Manager::create_sprite(theme_sprite_filepath)));
	theme_list_size++;
	theme_key_list.push_back(theme_filepath);
}

void Level_Editor::add_special_to_list(std::string special_def, sf::Text special_text)
{
	special_text_map.insert(std::pair<std::string, sf::Text>(special_def, special_text));
	special_list_size++;
	special_list.push_back(special_def);
}

void Level_Editor::update_editor()
{
	mouse_position = sf::Mouse::getPosition(*Graphics::get_game_window());

	theme_left_arrow_button->update();
	theme_right_arrow_button->update();

	special_item_left_arrow->update();
	special_item_right_arrow->update();

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

	special_item_left_arrow->draw_background(camera);
	special_item_right_arrow->draw_background(camera);

	Graphics::begin_draw_text();
	Graphics::draw_text(theme_text);
	for (int i = 0; i < special_text_map.size(); i++)
	{
		Graphics::draw_text(special_text_map[special_list[current_special_index]]);
	}
	Graphics::draw_text(special_text);
	Graphics::end_draw_text();
}

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
}

void theme_right_cycle()
{
	current_theme_index++;
	if (current_theme_index >= theme_list_size)
	{
		current_theme_index = 0;
	}
}

void special_left_cycle()
{
	current_special_index--;
	if (current_special_index < 0)
	{
		current_special_index = special_list_size - 1;
	}
}

void special_right_cycle()
{
	current_special_index++;
	if (current_special_index >= special_list_size)
	{
		current_special_index = 0;
	}
}
